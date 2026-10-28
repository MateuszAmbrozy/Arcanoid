#pragma once
#include "GraphicsSettings.h"
#include "MainMenuState.h"
#include "stdafx.h"

class Game
{
  private:
    GraphicsSettings gfxSettings;
    StateData stateData;

    std::unique_ptr<sf::RenderWindow> window;
    sf::Event event;

    std::stack<std::unique_ptr<State>> states;

    sf::Clock clock;
    float dt;
    sf::Vector2u prevSize;

    std::map<std::string, int> supportedKeys;

    void initVariables();
    void initKeys();
    void initStateData();
    void initStates();
    void initWindow();
    void initGraphicsSettings();

  public:
    Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    ~Game();

    void run();
    void pollEvents();
    void updateDT();
    void update();
    void render();
};
