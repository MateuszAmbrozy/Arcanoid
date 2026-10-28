#pragma once
#include "GraphicsSettings.h"
#include "stdafx.h"

class State;

class StateData
{
  public:
    float gridWidth = 0;
    float gridHeight = 0;
    GraphicsSettings* gfxSettings = nullptr;
    sf::RenderWindow* window = nullptr;
    std::map<std::string, int>* supportedKeys = nullptr;
    std::stack<std::unique_ptr<State>>* states = nullptr;

    bool reset = false;

    sf::Texture ballTexture;
    int paddleTopRect = 0;
    int bgLeftRect = 0;
    std::array<int, gameConfig::levelCount> levels{};
    bool mouseClicked = false;
    bool mouseDown = false;
    bool rightMouseDown = false;
    sf::Vector2i mousePosition;
    std::array<bool, sf::Keyboard::KeyCount> keysDown{};
    std::array<bool, sf::Keyboard::KeyCount> keysPressed{};
    bool isKeyPressed(sf::Keyboard::Key key) const
    {
        return key >= 0 && key < sf::Keyboard::KeyCount && keysDown[static_cast<std::size_t>(key)];
    }
    bool isKeyJustPressed(sf::Keyboard::Key key) const
    {
        return key >= 0 && key < sf::Keyboard::KeyCount && keysPressed[static_cast<std::size_t>(key)];
    }
    int selectedBall = 0;
    bool refreshLevelState = 0;

    bool mute = false;
    float musicVolume = 50.f;
    float collisionVolume = 50.f;
    float endVolume = 50.f;

    bool inversePaddle = false;
    float inversePaddleElapsed = 0.f;
};

class State
{
  protected:
    float gridWidth;
    float gridHeight;
    StateData* stateData;
    std::stack<std::unique_ptr<State>>* states;
    sf::RenderWindow* window;
    std::map<std::string, sf::Texture> textures;
    bool paused;
    bool quit;

    sf::Vector2i mousePosWindow;
    sf::Vector2i mousePosGrid;

    std::map<std::string, int>* supportedKeys;
    std::map<std::string, int> keyBinds;

  public:
    explicit State(StateData* stateData);
    State(const State&) = delete;
    State& operator=(const State&) = delete;
    virtual ~State();

    void pauseState();
    void unpauseState();

    virtual void endState();
    virtual void checkForQuit();
    virtual void updateMousePosition();
    void loadKeyBinds(const std::string& path);

    bool getQuit() const;

    virtual void updateInput(float dt) = 0;

    virtual void update(float dt) = 0;
    virtual void render(sf::RenderTarget* target = nullptr) = 0;
};
