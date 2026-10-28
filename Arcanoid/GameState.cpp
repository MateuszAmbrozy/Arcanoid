#include "GameState.h"
#include "Assets.h"
#include "LevelCatalog.h"
#include "stdafx.h"

void GameState::initVariables()
{
    this->startGame = false;
    this->vm = this->stateData->gfxSettings->resolution;
}
void GameState::initB2World()
{
    this->world = std::make_unique<b2World>(b2Vec2(0.f, 0.f));
}
void GameState::initContactListener()
{
    this->contactListener = std::make_unique<ContactListener>();
    world->SetContactListener(this->contactListener.get());
}
void GameState::initPaddle()
{
    this->paddle = std::make_unique<Paddle>(this->stateData, this->world.get());
}
void GameState::initBalls()
{
    this->balls.push_back(std::make_unique<Ball>(this->stateData, this->world.get(), this->paddle.get(),
                                                 gui::p2pX(1.4f, this->vm)));
}
void GameState::initBackground()
{
    this->textureRect = sf::IntRect(0, 0, 200, 241);

    assets::load(this->bgTexture, "Resources/Background/bg3.png");

    this->background.setSize(sf::Vector2f(static_cast<float>(vm.width), static_cast<float>(vm.height)));
    if (this->stateData->bgLeftRect != -1)
    {
        this->textureRect.left = stateData->bgLeftRect;
    }

    this->background.setTextureRect(this->textureRect);
    this->background.setTexture(&this->bgTexture);
}
void GameState::initPausedMenu()
{
    this->pausedMenu = std::make_unique<PausedMenu>(this->stateData->gfxSettings->resolution);
    this->pausedMenu->addButton("BACK", gui::p2pX(26.f, this->vm), gui::p2pY(23.8f, this->vm),
                                gui::p2pX(6.6f, this->vm), gui::callCharSize(this->vm, 100), sf::Color::Black,
                                sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
                                sf::Color(200, 100, 100), sf::Color(200, 100, 100, 100),
                                sf::Color(20, 20, 20, 50), "BACK");
}
void GameState::initBar()
{
    this->inversePaddleBar = std::make_unique<gui::Bar>(gui::p2pX(50.f, this->vm), gui::p2pY(5.f, this->vm),
                                                        gui::p2pX(12.f, this->vm), gui::p2pY(4.f, this->vm),
                                                        10, this->vm, this->font, sf::Color::Red);
}
void GameState::initTimer()
{
    this->timer = std::make_unique<Timer>();
    this->timer->setPosition(sf::Vector2f(gui::p2pX(5.2f, this->vm), gui::p2pY(9.25f, this->vm)));
    this->timer->setCharacterSize(gui::callCharSize(this->vm, 80));
}
void GameState::initLives()
{
    this->livesRemaining = gameConfig::initialLives;
    assets::load(this->lifeTexture, "Resources/Sprites/heart.png");
    this->lifeSprite.setTexture(this->lifeTexture);
    this->lifeSprite.setScale(0.03f, 0.0346f);

    for (size_t i = 0; i < livesRemaining; i++)
    {
        this->lives.push_back(this->lifeSprite);
        this->lives[i].setPosition(i * this->lifeSprite.getGlobalBounds().width + gui::p2pX(1.0f, this->vm),
                                   0);
    }
}
void GameState::initKeyBinds()
{
    loadKeyBinds("Config/gamestate_keybinds.ini");
}
void GameState::initFont()
{
    assets::load(this->font, "Fonts/Arialn.ttf");
}
void GameState::initBlockSet(std::string filename)
{
    this->blockSet = std::make_unique<BlockSet>(this->stateData, this->world.get(), filename,
                                                this->stateData->gridWidth, this->stateData->gridHeight);
}
void GameState::initEndGame()
{
    lose = std::make_unique<EndGame>(stateData->gfxSettings->resolution, "BACK", "YOU LOSE!");
    if (levels::next(levels::number(blockSet->getFileName())) != 0)
        win = std::make_unique<EndGame>(stateData->gfxSettings->resolution, "BACK", "NEXT", "YOU WIN!");
    else
        win = std::make_unique<EndGame>(stateData->gfxSettings->resolution, "BACK", "ALL LEVELS COMPLETE!",
                                        EndGameResult::Victory);
}
void GameState::initSound()
{

    this->playMusic = true;

    assets::load(this->buffer[0], "Resources/Sounds/w.wav");
    this->collisionSound.setBuffer(this->buffer[0]);
    this->collisionSound.setVolume(this->stateData->collisionVolume);

    if (!this->music.openFromFile("Resources/Sounds/music.wav"))
        throw std::runtime_error("Cannot load Resources/Sounds/music.wav");
    this->music.setLoop(true);
    this->music.setVolume(this->stateData->musicVolume);
    if (!this->stateData->mute)
        this->music.play();

    assets::load(this->buffer[1], "Resources/Sounds/lose.wav");
    this->loseSound.setBuffer(this->buffer[1]);
    this->loseSound.setVolume(this->stateData->endVolume);

    assets::load(this->buffer[2], "Resources/Sounds/win.wav");
    this->winSound.setBuffer(this->buffer[2]);
    this->winSound.setVolume(this->stateData->endVolume);
}

GameState::GameState(StateData* stateData, std::string filename) : State(stateData)
{
    this->initVariables();
    this->initB2World();

    this->initContactListener();
    this->initFont();
    this->initKeyBinds();
    this->initPausedMenu();
    this->initBar();
    this->initTimer();
    this->initPaddle();
    this->initBalls();
    this->initBackground();
    this->initLives();
    this->initBlockSet(filename);
    this->initEndGame();
    this->initSound();
}
GameState::~GameState()
{
    stateData->inversePaddle = false;
    stateData->inversePaddleElapsed = 0.f;
}

const std::vector<std::unique_ptr<Ball>>& GameState::getBalls() const
{
    return this->balls;
}

void GameState::updateInput(float dt)
{
    if (stateData->isKeyJustPressed(sf::Keyboard::Key(keyBinds.at("CLOSE"))))
    {
        paused = !paused;
        physicsAccumulator = 0.f;
        if (paused)
        {
            timer->stop();
            music.pause();
        }
        else
        {
            if (startGame)
                timer->resume();
            if (!stateData->mute)
                music.play();
        }
    }
    if (paused)
        return;
    float direction = 0.f;
    if (stateData->isKeyPressed(sf::Keyboard::Key(keyBinds.at("MOVE_LEFT"))))
        direction -= 1.f;
    if (stateData->isKeyPressed(sf::Keyboard::Key(keyBinds.at("MOVE_RIGHT"))))
        direction += 1.f;
    if (direction != 0.f && !startGame)
    {
        startGame = true;
        timer->resume();
    }
    paddle->move(stateData->inversePaddle ? -direction : direction, 0.f, dt);
}
void GameState::updatePausedButtons()
{
    if (this->pausedMenu->isButtonPressed("BACK"))
    {
        this->endState();
    }
}
void GameState::updateEndGameButtons()
{
    if (livesRemaining == 0)
    {
        if (lose->isButtonPressed("BACK"))
            endState();
        return;
    }
    if (win->isButtonPressed("BACK"))
    {
        endState();
        return;
    }
    if (!win->isButtonPressed("NEXT"))
        return;
    const int next = levels::next(levels::number(blockSet->getFileName()));
    if (next == 0)
    {
        endState();
        return;
    }

    // Load the next map before releasing the previous level.
    auto nextBlocks = std::make_unique<BlockSet>(stateData, world.get(), levels::filename(next),
                                                 stateData->gridWidth, stateData->gridHeight);
    balls.clear();
    blockSet = std::move(nextBlocks);
    initPaddle();
    initBalls();
    lives.clear();
    initLives();
    initTimer();
    initEndGame();
    stateData->inversePaddle = false;
    stateData->inversePaddleElapsed = 0.f;
    startGame = false;
    physicsAccumulator = 0.f;
    playMusic = true;
    if (!stateData->mute)
        music.play();
}
void GameState::updateEndGame(float dt)
{
    const bool won = livesRemaining > 0;
    timer->stop();
    if (playMusic)
    {
        music.stop();
        if (!stateData->mute)
        {
            if (won)
                winSound.play();
            else
                loseSound.play();
        }
        playMusic = false;
        if (won)
        {
            const int next = levels::next(levels::number(blockSet->getFileName()));
            if (next != 0)
                stateData->levels[next - 1] = 1;
            stateData->refreshLevelState = true;
            stateData->inversePaddle = false;
            stateData->inversePaddleElapsed = 0.f;
        }
    }
    if (won)
        win->update(stateData, mousePosWindow, dt);
    else
        lose->update(stateData, mousePosWindow, dt);
    updateEndGameButtons();
}
void GameState::updateBlockSet(float dt)
{
    this->blockSet->startSuperPower(this->stateData, this->paddle.get(), &this->balls);
    this->blockSet->collisionBallBlock(this->stateData, dt);

    this->blockSet->updateSuperPower(this->stateData, dt);
    this->blockSet->update();
}
void GameState::updateTimer(float dt)
{
    if (this->startGame)
    {

        if (!this->paused)
            timer->update(dt);
    }
    if (this->livesRemaining == 0)
    {
        this->timer->stop();
    }
}
void GameState::updateBall(float dt)
{
    if (!startGame || livesRemaining == 0)
        return;
    for (auto& ball : balls)
    {
        auto* body = ball->getBody();
        if (body->GetLinearVelocity().LengthSquared() == 0.f)
            ball->move(ball->velocity, dt);
        ball->update(dt);
        auto position = body->GetPosition();
        auto velocity = body->GetLinearVelocity();
        const float radius = ball->getGlobalBounds().width / 2.f / physics::pixelsPerMeter;
        bool hitWall = false;
        if (position.x < radius)
        {
            position.x = radius;
            velocity.x = std::abs(velocity.x);
            hitWall = true;
        }
        if (position.x > vm.width / physics::pixelsPerMeter - radius)
        {
            position.x = vm.width / physics::pixelsPerMeter - radius;
            velocity.x = -std::abs(velocity.x);
            hitWall = true;
        }
        if (position.y < radius)
        {
            position.y = radius;
            velocity.y = std::abs(velocity.y);
            hitWall = true;
        }
        if (hitWall)
        {
            body->SetTransform(position, body->GetAngle());
            body->SetLinearVelocity(velocity);
            if (!stateData->mute)
                collisionSound.play();
        }
        auto* contact = reinterpret_cast<UserData*>(ball->getFixtureDefinition()->userData.pointer);
        if (contact->hit)
        {
            const float halfWidth = paddle->getSize().x / 2.f;
            const float offset =
                (body->GetPosition().x * physics::pixelsPerMeter - paddle->getPosition().x - halfWidth) /
                halfWidth;
            const float angle = std::clamp(offset, -1.f, 1.f) * b2_pi / 3.f;
            const float speed = std::max(body->GetLinearVelocity().Length(), 1.f);
            body->SetLinearVelocity(b2Vec2(std::sin(angle) * speed, -std::cos(angle) * speed));
            contact->hit = false;
            if (!stateData->mute)
                collisionSound.play();
        }
    }
    balls.erase(
        std::remove_if(balls.begin(), balls.end(), [&](const auto& ball)
                       { return ball->getBody()->GetPosition().y * physics::pixelsPerMeter >= vm.height; }),
        balls.end());
    if (balls.empty())
    {
        --livesRemaining;
        if (!lives.empty())
            lives.pop_back();
        startGame = false;
        if (livesRemaining > 0)
            initBalls();
    }
}
void GameState::updateInverseTimer(float dt)
{
    if (stateData->inversePaddle)
    {
        stateData->inversePaddleElapsed += dt;
        if (stateData->inversePaddleElapsed >= gameConfig::inversePaddleDuration)
        {
            stateData->inversePaddle = false;
            stateData->inversePaddleElapsed = 0.f;
        }
    }
    inversePaddleBar->update("INVERSE PADDLE TIME", stateData->inversePaddleElapsed);
}
void GameState::update(float dt)
{
    updateMousePosition();
    if (livesRemaining == 0 || blockSet->checkEndGame())
    {
        updateEndGame(dt);
        return;
    }
    updateInput(dt);
    if (paused)
    {
        pausedMenu->update(stateData, mousePosWindow);
        updatePausedButtons();
        return;
    }
    if (!startGame)
        return;
    physicsAccumulator += std::min(dt, physics::maxFrameTime);
    while (physicsAccumulator + 1e-7f >= physics::fixedTimeStep)
    {
        const float step = physics::fixedTimeStep;
        // Keep the original game speed while decoupling simulation from rendering.
        world->Step(step, 8, 3);
        physicsAccumulator = std::max(0.f, physicsAccumulator - step);
        paddle->update(step);
        updateBall(step);
        updateBlockSet(step);
        updateInverseTimer(step);
        updateTimer(step);
        if (livesRemaining == 0 || blockSet->checkEndGame() || !startGame)
            break;
    }
}
void GameState::renderEndGame(sf::RenderTarget* target)
{
    if (livesRemaining < 1)
    {
        this->timer->stop();
        this->timer->setCharacterSize(70);
        this->lose->render(target);
    }
    else if (this->blockSet->checkEndGame())
    {
        this->timer->stop();
        this->timer->setCharacterSize(70);
        this->win->render(target);
    }
}
void GameState::renderBalls(sf::RenderTarget* target)
{
    for (auto& ball : this->balls)
    {
        ball->render(target);
    }
}
void GameState::renderLives(sf::RenderTarget* target)
{
    for (auto& life : this->lives)
    {
        target->draw(life);
    }
}
void GameState::render(sf::RenderTarget* target)
{

    target->draw(this->background);

    this->blockSet->render(*target);
    if (this->stateData->inversePaddle)
        this->inversePaddleBar->render(*target);
    this->paddle->render(target);
    this->renderBalls(target);
    this->renderEndGame(target);
    this->timer->render(*target);
    this->renderLives(target);

    if (this->paused)
    {
        this->pausedMenu->render(*target);
    }
}
