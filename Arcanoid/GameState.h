#pragma once
#include "Ball.h"
#include "BlockSet.h"
#include "ContactListener.h"
#include "EndGame.h"
#include "GraphicsSettings.h"
#include "Paddle.h"
#include "PausedMenu.h"
#include "State.h"
#include "Timer.h"
#include "stdafx.h"

class GameState : public State
{
  private:
    std::unique_ptr<ContactListener> contactListener;
    std::unique_ptr<b2World> world;
    float physicsAccumulator = 0.f;
    sf::Font font;

    std::unique_ptr<PausedMenu> pausedMenu;
    std::unique_ptr<EndGame> lose;
    std::unique_ptr<EndGame> win;
    std::unique_ptr<Paddle> paddle;

    std::vector<std::unique_ptr<Ball>> balls;
    std::unique_ptr<Timer> timer;
    std::unique_ptr<BlockSet> blockSet;
    sf::VideoMode vm;

    bool startGame;

    unsigned livesRemaining;
    sf::Texture lifeTexture;
    sf::Sprite lifeSprite;
    std::vector<sf::Sprite> lives;

    sf::Texture bgTexture;
    sf::RectangleShape background;
    sf::IntRect textureRect;

    sf::SoundBuffer buffer[3];
    sf::Sound collisionSound;
    sf::Music music;
    sf::Sound loseSound;
    sf::Sound winSound;
    bool playMusic;

    std::unique_ptr<gui::Bar> inversePaddleBar;

    void initVariables();
    void initB2World();

    void initContactListener();
    void initPaddle();
    void initBalls();
    void initBackground();
    void initPausedMenu();
    void initBar();
    void initTimer();
    void initLives();
    void initBlockSet(std::string filename);
    void initEndGame();
    void initSound();

    void initKeyBinds();
    void initFont();

  public:
    GameState(StateData* stateData, std::string filename);
    ~GameState() override;

    const std::vector<std::unique_ptr<Ball>>& getBalls() const;

    void updateInput(float dt) override;
    void updatePausedButtons();
    void updateEndGameButtons();
    void updateEndGame(float dt);
    void updateTimer(float dt);
    void updateBall(float dt);
    void updateBlockSet(float dt);
    void updateInverseTimer(float dt);
    void update(float dt) override;

    void renderEndGame(sf::RenderTarget* target);
    void renderBalls(sf::RenderTarget* target);
    void renderLives(sf::RenderTarget* target);

    void render(sf::RenderTarget* target) override;
};
