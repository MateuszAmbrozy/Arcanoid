#pragma once
#include "Gui.h"
#include "ObjectData.h"
#include "Paddle.h"
#include "stdafx.h"
class Ball
{
  private:
    sf::CircleShape shape;
    float maxSpeed;
    sf::Texture texture;
    sf::VideoMode vm;
    sf::Vector2f position;

    b2World* ownerWorld = nullptr;
    b2Body* m_body = nullptr;
    b2BodyDef m_bodyDef{};
    b2CircleShape m_shape{};
    b2FixtureDef m_fixtureDef{};
    UserData userData{};

    void initTexture(StateData* stateData, const float radius);
    void initBox2D(StateData* stateData, b2World* world, sf::Vector2f position, const float radius);

  public:
    sf::Vector2f velocity;

    Ball(StateData* stateData, b2World* world, const Paddle* paddle, const float radius);
    Ball(StateData* stateData, b2World* world, const float radius, sf::Vector2f position);
    ~Ball();
    Ball(const Ball&) = delete;
    Ball& operator=(const Ball&) = delete;

    sf::Vector2i getGridPosition(const int gridWidth, const int gridHeight) const;
    sf::Vector2f getPosition() const;
    sf::FloatRect getGlobalBounds() const;

    b2BodyDef* getBodyDefinition();
    b2Body* getBody();
    b2FixtureDef* getFixtureDefinition();

    void setPosition(sf::Vector2f position);
    void setTexture(sf::Texture texture);

    void setRotation(float rotation);

    void move(const float dirX, const float dirY, float dt);
    void move(sf::Vector2f velocity, float dt);

    void update(float dt);
    void render(sf::RenderTarget* target);
};
