#pragma once
#include "Ball.h"
#include "Gui.h"
#include "Paddle.h"
#include "stdafx.h"

enum PowerTypes
{
    BIGGERPADDLE = 0,
    SMALLPADDLE,
    NEWBALLS,
    NEWBALLFROMPADDLE,
    FASTBALL,
    SLOWBALL,
    VICEVERSA
};
class Powers
{
  private:
    sf::Texture texture[3];

    sf::CircleShape shape;
    float maxVelocity;
    short unsigned powerType;
    sf::VideoMode vm;

  public:
    Powers(StateData* stateData, sf::Vector2f position);
    ~Powers();

    bool intersects(const sf::FloatRect bounds) const;
    unsigned short getType() const;
    sf::FloatRect getGlobalBounds() const;

    void move(float dt);

    void initNewBalls(StateData* stateData, b2World* world, std::vector<std::unique_ptr<Ball>>* balls);
    void initNewBallsFromPaddle(StateData* stateData, b2World* world, Paddle* paddle,
                                std::vector<std::unique_ptr<Ball>>* balls);
    void initBiggerPaddle(Paddle* paddle);
    void initSmallerPaddle(Paddle* paddle);
    void initFastBalls(std::vector<std::unique_ptr<Ball>>* balls);
    void initSlowBalls(std::vector<std::unique_ptr<Ball>>* balls);

    void update(float dt);
    void render(sf::RenderTarget& target);
};
