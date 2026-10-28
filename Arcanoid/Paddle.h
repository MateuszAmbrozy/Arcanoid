#pragma once
#include "Gui.h"
#include "ObjectData.h"
#include "State.h"
#include "stdafx.h"
class Paddle
{
  private:
    sf::Texture texture;
    sf::IntRect textureRect;
    sf::RectangleShape shape;
    float width;
    float height;
    sf::VideoMode vm;
    sf::Vector2f position;
    float maxVelocity;

    b2World* ownerWorld = nullptr;
    b2Body* m_body = nullptr;
    b2BodyDef m_bodyDef{};
    b2PolygonShape m_shape{};
    b2FixtureDef m_fixtureDef{};
    UserData userData{};

    void initTexture(StateData* stateData);
    void initBox2D(b2World* world);

  public:
    Paddle(StateData* stateData, b2World* world);
    ~Paddle();
    Paddle(const Paddle&) = delete;
    Paddle& operator=(const Paddle&) = delete;

    sf::Vector2f getPosition() const;
    sf::FloatRect getGlobalBounds() const;
    sf::Vector2f getSize() const;

    b2BodyDef* getBodyDefinition();
    b2Body* getBody();
    b2FixtureDef* getFixtureDefinition();

    void setPosition(sf::Vector2f position);
    void biggerPaddle();
    void smallerPaddle();

    void setBodySize();

    void move(const float dirX, const float dirY, float dt);
    void updateWorldBoundsCollision();
    void update(float dt);
    void render(sf::RenderTarget* target);
};
