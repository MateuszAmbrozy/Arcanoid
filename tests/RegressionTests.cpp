#include "BlockSet.h"
#include "EditorState.h"
#include "GameState.h"
#include "LevelCatalog.h"
#include "LevelState.h"
#include "MainMenuState.h"
#include "MapFile.h"
#include "SettingsState.h"
#include "Timer.h"

#include <functional>

namespace
{
void require(bool condition, const std::string& message)
{
    if (!condition)
        throw std::runtime_error(message);
}

template <typename Action> void rejects(Action action)
{
    bool rejected = false;
    try
    {
        action();
    }
    catch (const std::exception&)
    {
        rejected = true;
    }
    require(rejected, "Invalid input was accepted");
}

struct TemporaryDirectory
{
    std::filesystem::path path =
        std::filesystem::temp_directory_path() /
        ("arcanoid-test-" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    TemporaryDirectory()
    {
        std::filesystem::create_directory(path);
    }
    ~TemporaryDirectory()
    {
        std::error_code ignored;
        std::filesystem::remove_all(path, ignored);
    }
};

void settingsRoundTrip(const std::filesystem::path& directory)
{
    const auto file = directory / "graphics.ini";
    GraphicsSettings original;
    original.title = "Arcanoid regression test";
    original.resolution = sf::VideoMode(1024, 768);
    original.fullScreen = true;
    original.verticalSync = true;
    original.frameRateLimit = 144;
    original.contextSettings.antialiasingLevel = 4;
    original.saveToFile(file.string());
    GraphicsSettings loaded;
    loaded.loadFromFile(file.string());
    require(loaded.title == original.title && loaded.resolution == original.resolution && loaded.fullScreen &&
                loaded.verticalSync && loaded.frameRateLimit == 144 &&
                loaded.contextSettings.antialiasingLevel == 4,
            "Graphics settings did not round-trip");
    std::ofstream(file) << "bad settings\n0 0\n";
    rejects([&] { loaded.loadFromFile(file.string()); });
    require(loaded.resolution == original.resolution, "Invalid settings replaced valid values");
}

void validateMaps(const std::filesystem::path& directory)
{
    int available = 0;
    for (int number = 1; number <= levels::count; ++number)
        if (levels::exists(number))
        {
            const auto map = maps::load(levels::filename(number));
            require(!map.tiles.empty(), "A shipped level is empty");
            ++available;
        }
    require(available == 7, "Unexpected number of supplied levels");
    require(levels::next(4) == 7 && levels::next(11) == 0, "Level progression ignores gaps or final level");
    const auto file = directory / "invalid.slmp";
    const std::vector<std::string> invalid = {
        "0 36\nResources/Sprites/blocks.png\n",
        "32 36\nResources/Sprites/blocks.png\n-1 0 0 0 0 0 1 0\n",
        "32 36\nResources/Sprites/blocks.png\n32 0 0 0 0 0 1 0\n",
        "32 36\nResources/Sprites/blocks.png\n0 0 0\n",
        "32 36\nResources/Sprites/blocks.png\n0 0 0 0 0 0 1 0\n0 0 0 0 0 0 1 0\n",
        "32 36\n../outside.png\n",
        "32 36\nResources/Sprites/blocks.png\n0 0 0 0 0 0 1 9\n"};
    for (const auto& input : invalid)
    {
        std::ofstream(file) << input;
        rejects([&] { maps::load(file); });
    }
}

struct Context
{
    GraphicsSettings graphics;
    sf::RenderWindow window;
    std::map<std::string, int> keys;
    std::stack<std::unique_ptr<State>> states;
    StateData data;

    Context()
    {
        graphics.resolution = sf::VideoMode(960, 720);
        window.create(graphics.resolution, "Arcanoid tests", sf::Style::Titlebar | sf::Style::Close);
        window.setVisible(false);
        std::ifstream input("Config/supported_keys.ini");
        std::string name;
        int code;
        while (input >> name >> code)
            keys.emplace(name, code);
        data.window = &window;
        data.gfxSettings = &graphics;
        data.supportedKeys = &keys;
        data.states = &states;
        data.gridWidth = graphics.resolution.width / 32.f;
        data.gridHeight = graphics.resolution.height / 36.f;
        data.levels[0] = 1;
        data.mute = true;
    }
};

void physicsOwnership(Context& context)
{
    b2World world(b2Vec2(0.f, 0.f));
    {
        Paddle paddle(&context.data, &world);
        Ball ball(&context.data, &world, &paddle, 12.f);
        require(world.GetBodyCount() == 2, "Missing physics bodies");
        ball.move(1.f, -1.f, 1.f / 30.f);
        const auto velocity = ball.getBody()->GetLinearVelocity();
        ball.move(1.f, -1.f, 1.f / 144.f);
        require(ball.getBody()->GetLinearVelocity() == velocity, "Velocity depends on frame duration");
        paddle.biggerPaddle();
        const auto* tag =
            reinterpret_cast<UserData*>(paddle.getBody()->GetFixtureList()->GetUserData().pointer);
        require(tag && tag->type == ObjectType::Paddle, "Resizing lost fixture user data");
        {
            BlockSet map(&context.data, &world, "1.slmp", context.data.gridWidth, context.data.gridHeight);
            require(world.GetBodyCount() > 2, "Map did not create collision bodies");
        }
        require(world.GetBodyCount() == 2, "Map destruction leaked physics bodies");
    }
    require(world.GetBodyCount() == 0, "Ball or paddle destruction leaked a physics body");
}

void editorRoundTrip(Context& context, const std::filesystem::path& directory)
{
    BlockSet map(&context.data, context.data.gridWidth, context.data.gridHeight, 32, 36,
                 "Resources/Sprites/blocks.png");
    const auto file = directory / "editor.slmp";
    map.addBlock(2, 3, 2, sf::IntRect(400, 50, 100, 50), BlockTypes::DEFAULT);
    map.addBlock(-1, 0, 1, sf::IntRect(0, 0, 100, 50), BlockTypes::DEFAULT);
    map.saveToFile(file.string());
    const auto saved = maps::load(file);
    require(saved.tiles.size() == 1 && saved.tiles[0].column == 4 && saved.tiles[0].row == 1,
            "Editor saved incorrect grid/atlas coordinates");
    map.loadFromFile(file.string());
    map.saveToFile(file.string());
    require(maps::load(file).tiles[0].lives == 2, "Loading changed block durability");
    const auto broken = directory / "broken.slmp";
    std::ofstream(broken) << "invalid";
    rejects([&] { map.loadFromFile(broken.string()); });
    require(!map.checkEndGame(), "Failed loading destroyed the open map");
    map.removeBlock(2, 3);
    require(map.checkEndGame(), "Removed editor block is still present");
}

void gameplay(Context& context)
{
    context.data.keysDown.fill(false);
    GameState game(&context.data, "1.slmp");
    context.data.keysDown[sf::Keyboard::A] = true;
    game.update(1.f / 60.f);
    context.data.keysDown.fill(false);
    require(game.getBalls().size() == 1, "Gameplay failed to launch a ball");
    const auto position = game.getBalls().front()->getBody()->GetPosition();
    context.data.keysPressed[sf::Keyboard::Escape] = true;
    game.update(0.f);
    context.data.keysPressed.fill(false);
    context.data.inversePaddle = true;
    context.data.inversePaddleElapsed = 2.f;
    game.update(1.f);
    require(game.getBalls().front()->getBody()->GetPosition() == position &&
                context.data.inversePaddleElapsed == 2.f,
            "Pause advanced physics or effect timer");
    context.data.keysPressed[sf::Keyboard::Escape] = true;
    game.update(0.f);
    context.data.keysPressed.fill(false);
    game.update(1.f / 60.f);
    require(game.getBalls().front()->getBody()->GetPosition() != position, "Unpause did not resume physics");
    for (int life = 0; life < 3; ++life)
    {
        context.data.keysDown[sf::Keyboard::A] = true;
        game.update(1.f / 60.f);
        context.data.keysDown.fill(false);
        for (const auto& ball : game.getBalls())
            ball->setPosition({400.f, 900.f});
        game.update(1.f / 60.f);
        require(game.getBalls().size() == (life < 2 ? 1u : 0u), "Losing a life left invalid balls");
    }
    game.update(1.f / 60.f);
    game.render(&context.window);
}

void fixedStepConsistency(Context& context)
{
    const auto simulate = [&](int fps)
    {
        context.data.keysDown.fill(false);
        GameState game(&context.data, "1.slmp");
        context.data.keysDown[sf::Keyboard::A] = true;
        game.update(0.f);
        context.data.keysDown.fill(false);
        game.getBalls().front()->setPosition({450.f, 500.f});
        game.getBalls().front()->move(0.2f, -1.f, 0.f);
        for (int frame = 0; frame < fps; ++frame)
            game.update(1.f / fps);
        require(game.getBalls().size() == 1, "Fixed-step test unexpectedly lost a ball");
        return game.getBalls().front()->getPosition();
    };
    const auto baseline = simulate(60);
    for (int fps : {30, 75, 144})
    {
        const auto position = simulate(fps);
        require(std::abs(position.x - baseline.x) < 0.01f && std::abs(position.y - baseline.y) < 0.01f,
                "Physics trajectory depends on render FPS");
    }
}

void levelTransitions(Context& context, const std::filesystem::path& directory)
{
    const auto click = [&](GameState& game, int x, int y)
    {
        context.data.mousePosition = {x, y};
        context.data.mouseClicked = true;
        game.update(1.f / 60.f);
        context.data.mouseClicked = false;
    };
    const auto first = directory / "1.slmp";
    std::ofstream(first) << "32 36\nResources/Sprites/blocks.png\n";
    {
        GameState game(&context.data, first.string());
        game.getBalls().front()->setPosition({100.f, 100.f});
        game.update(1.f / 60.f);
        require(context.data.levels[1] == 1, "Winning did not unlock the next existing level");
        click(game, 770, 575);
        require(!game.getQuit() && game.getBalls().front()->getPosition().x > 100.f,
                "NEXT failed to replace the completed level");
    }
    const auto last = directory / "11.slmp";
    std::ofstream(last) << "32 36\nResources/Sprites/blocks.png\n";
    {
        GameState game(&context.data, last.string());
        game.update(1.f / 60.f);
        click(game, 770, 575);
        require(!game.getQuit(), "Final level unexpectedly exposed a NEXT action");
        click(game, 460, 575);
        require(game.getQuit(), "Final victory screen cannot return to the menu");
    }
}

void userInterface(Context& context)
{
    for (int iteration = 0; iteration < 3; ++iteration)
    {
        MainMenuState menu(&context.data);
        LevelState picker(&context.data, 0);
        EditorState editor(&context.data, "1.slmp");
        SettingsState settings(&context.data);
        for (State* state : std::array<State*, 4>{&menu, &picker, &editor, &settings})
        {
            state->update(1.f / 60.f);
            state->render(&context.window);
        }
        // Repeated APPLY rebuilds every owned GUI control and must preserve selection/volumes.
        context.data.selectedBall = 2;
        context.data.paddleTopRect = 90;
        context.data.bgLeftRect = 400;
        settings.resetGui();
        for (int apply = 0; apply < 2; ++apply)
        {
            context.data.mousePosition = {static_cast<int>(context.graphics.resolution.width * 0.6f),
                                          static_cast<int>(context.graphics.resolution.height * 0.84f)};
            context.data.reset = false;
            context.data.mouseClicked = true;
            settings.update(1.f / 60.f);
            context.data.mouseClicked = false;
            require(context.data.reset, "APPLY did not rebuild the settings interface");
            require(context.data.selectedBall == 2 && context.data.paddleTopRect == 90 &&
                        context.data.bgLeftRect == 400,
                    "Applying settings reset appearance selection");
            settings.render(&context.window);
        }
    }
}
} // namespace

int main()
{
    try
    {
        TemporaryDirectory temporary;
        settingsRoundTrip(temporary.path);
        validateMaps(temporary.path);
        Context context;
        physicsOwnership(context);
        editorRoundTrip(context, temporary.path);
        Timer timer;
        timer.start();
        timer.update(2.5f);
        timer.stop();
        timer.update(5.f);
        require(timer.getElapsedTime() == 2, "Paused timer advanced");
        timer.resume();
        timer.update(1.f);
        require(timer.getElapsedTime() == 3, "Timer resume lost elapsed time");
        gameplay(context);
        fixedStepConsistency(context);
        levelTransitions(context, temporary.path);
        userInterface(context);
        std::cout << "All Arcanoid regression checks passed\n";
        return EXIT_SUCCESS;
    }
    catch (const std::exception& error)
    {
        std::cerr << "Regression failed: " << error.what() << '\n';
        return EXIT_FAILURE;
    }
}
