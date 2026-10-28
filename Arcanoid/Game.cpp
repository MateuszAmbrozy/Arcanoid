#include "Game.h"
#include "LevelCatalog.h"
#include "stdafx.h"

void Game::initGraphicsSettings()
{
    try
    {
        gfxSettings.loadFromFile("Config/graphics.ini");
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << "; using defaults\n";
    }
}
void Game::initVariables()
{
    this->window = nullptr;

    this->dt = 0.f;
}
void Game::initWindow()
{
    if (this->gfxSettings.fullScreen)
        this->window =
            std::make_unique<sf::RenderWindow>(this->gfxSettings.resolution, this->gfxSettings.title,
                                               sf::Style::Fullscreen, gfxSettings.contextSettings);
    else
        this->window = std::make_unique<sf::RenderWindow>(
            this->gfxSettings.resolution, this->gfxSettings.title, sf::Style::Titlebar | sf::Style::Close,
            gfxSettings.contextSettings);
    this->window->setFramerateLimit(this->gfxSettings.frameRateLimit);
    this->window->setVerticalSyncEnabled(this->gfxSettings.verticalSync);
    window->setMouseCursorGrabbed(false);
}
void Game::initKeys()
{
    std::ifstream input("Config/supported_keys.ini");
    if (!input)
        throw std::runtime_error("Cannot open Config/supported_keys.ini");
    std::string name;
    int code = 0;
    while (input >> name >> code)
    {
        if (code < 0 || code >= sf::Keyboard::KeyCount)
            throw std::runtime_error("Invalid key code: " + name);
        supportedKeys.emplace(name, code);
    }
    if (!input.eof())
        throw std::runtime_error("Invalid supported key configuration");
}
void Game::initStateData()
{
    this->stateData.gridHeight =
        this->gfxSettings.resolution.height / static_cast<float>(gameConfig::gridRows);
    this->stateData.gridWidth =
        this->gfxSettings.resolution.width / static_cast<float>(gameConfig::gridColumns);
    this->stateData.window = this->window.get();
    this->stateData.gfxSettings = &this->gfxSettings;
    this->stateData.supportedKeys = &this->supportedKeys;
    this->stateData.states = &this->states;
    this->stateData.reset = 0;
    this->stateData.paddleTopRect = 0;
    this->stateData.bgLeftRect = 0;

    for (int i = 1; i < 16; i++)
        this->stateData.levels[i] = 0;
    const int first = levels::next(0);
    if (first != 0)
        stateData.levels[first - 1] = 1;

    this->stateData.collisionVolume = 50.f;
    this->stateData.endVolume = 50.f;
    this->stateData.musicVolume = 50.f;
    this->stateData.inversePaddle = false;
}
void Game::initStates()
{
    this->states.push(std::make_unique<MainMenuState>(&this->stateData));
}

Game::Game()
{
    this->initVariables();
    this->initGraphicsSettings();

    this->initWindow();
    this->initKeys();
    this->initStateData();
    this->initStates();
    prevSize = window->getSize();
}
Game::~Game()
{
    while (!states.empty())
        states.pop();
}

void Game::run()
{
    while (this->window->isOpen())
    {
        this->pollEvents();
        if (!window->isOpen())
            break;
        this->update();
        if (window->isOpen())
            this->render();
    }
}
void Game::pollEvents()
{
    while (window->pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window->close();
            break;
        default:
            break;
        }
    }
    sf::Vector2u currSize = window->getSize();
    if (currSize != prevSize)
    {
        window->setMouseCursorGrabbed(false);
        prevSize = currSize;
    }
}
void Game::updateDT()
{
    this->dt = std::min(this->clock.restart().asSeconds(), physics::maxFrameTime);
}
void Game::update()
{
    this->updateDT();
    if (!window->hasFocus())
    {
        stateData.keysDown.fill(false);
        stateData.keysPressed.fill(false);
        stateData.mouseDown = false;
        stateData.mouseClicked = false;
        stateData.rightMouseDown = false;
        return;
    }
    stateData.mousePosition = sf::Mouse::getPosition(*window);
    stateData.rightMouseDown = sf::Mouse::isButtonPressed(sf::Mouse::Right);
    for (int key = 0; key < sf::Keyboard::KeyCount; ++key)
    {
        const bool down = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key));
        stateData.keysPressed[key] = down && !stateData.keysDown[key];
        stateData.keysDown[key] = down;
    }
    const bool mouseDown = window->hasFocus() && sf::Mouse::isButtonPressed(sf::Mouse::Left);
    stateData.mouseClicked = mouseDown && !stateData.mouseDown;
    stateData.mouseDown = mouseDown;
    if (!this->states.empty())
    {
        State* activeState = states.top().get();
        activeState->update(this->dt);
        if (states.top().get() == activeState && activeState->getQuit())
        {
            this->states.top()->endState();

            this->states.pop();
        }
    }
    else
    {
        this->window->close();
    }
}
void Game::render()
{
    this->window->clear();

    if (!this->states.empty())
        this->states.top()->render(this->window.get());

    this->window->display();
}
