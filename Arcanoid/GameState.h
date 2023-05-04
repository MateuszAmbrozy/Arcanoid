#pragma once
#include "State.h"
#include "PausedMenu.h"
#include "GraphicsSettings.h"
#include "Paddle.h"
#include "Ball.h"
#include "Timer.h"
#include "BlockSet.h"
#include "EndGame.h"
#include "ContactListener.h"

class GameState :
    public State
{
private:
    //VARIABLES
    sf::Font font;

    PausedMenu* pausedMenu;
    EndGame* lose;
    EndGame* win;
    Paddle* paddle;
    bool paddleIsMoving;

    std::vector<Ball*> balls;
    Timer* timer;
    BlockSet* blockSet;
    sf::VideoMode vm;
    
    bool startGame;
    bool renderEnd;
    sf::Vector2f ballVelocity;

    //LIFES
    unsigned lifeCounter;
    sf::Texture lifeTexture;
    sf::Sprite lifeSprite;
    std::vector<sf::Sprite> lifes;

    //BACKGROUND
    sf::Texture bgTexture;
    sf::RectangleShape background;
    sf::IntRect textureRect;

    //SOUND
    sf::SoundBuffer buffer[3];
    sf::Sound collisionSound;
    sf::Music music;
    sf::Sound loseSound;
    sf::Sound winSound;
    bool playMusic;

    //INVERSE PADDLE TIMER
    sf::Clock clock;
    gui::Bar* inversePaddleBar;

    //Box2d
    b2World* world;
    ContactListener* contactListener;
    std::vector<b2Vec2> velocityBuffer;


    //PRIVATE FUNCTIONS
    void initVariables();
    void initB2World();
    void initB2WorldBounds();
    void initContactListener();
    void initPaddle();
    void initBalls();
    void initBackground();
    void initPausedMenu();
    void initBar();
    void initTimer();
    void initLifes();
    void initBlockSet(std::string filename);
    void initEndGame();
    void initSound();


    void initKeyBinds();
    void initFont();
public:
    //CONSTRUCTOR AND DESTRUCTOR
    GameState(StateData* stateData, std::string filename);
    virtual ~GameState();

    //ACCESSRS
    std::vector<Ball*> getBalls();


    //FUNCSTIONS
    void updateInput(const float& dt);
    void updatePausedButtons();
    void updateEndGameButtons();
    void updateEndGame(const float& dt);
    void updateTimer(const float& dt);
    void updateBall(const float& dt);
    void updateBlockSet(const float& dt);
    void updateInverseTimer();
    void update(const float& dt);

    void renderEndGame(sf::RenderTarget* target);
    void renderBalls(sf::RenderTarget* target);
    void renderLifes(sf::RenderTarget* target);

    void render(sf::RenderTarget* target);
};

