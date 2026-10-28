#pragma once
#include "ObjectData.h"
#include "State.h"
#include "stdafx.h"
enum BlockTypes
{
    DEFAULT = 0,
    POWER,
    UNBREAKING
};

class Block
{
  private:
    b2World* ownerWorld = nullptr;
    b2Body* m_body = nullptr;
    b2BodyDef m_bodyDef{};
    b2PolygonShape m_shape{};
    b2FixtureDef m_fixtureDef{};
    UserData userData{};

    void initBox2D(StateData* stateData, b2World* world, sf::Vector2f position);

  protected:
    int lives;
    sf::Sprite shape;
    sf::IntRect sourceRect;
    int left, top;
    short type;
    sf::IntRect textureRect;
    sf::VideoMode vm;

  public:
    Block(StateData* stateData, unsigned grid_x, unsigned grid_y, int left, int top, float gridWidth,
          float gridHeight, int lives, sf::Texture& texture, const sf::IntRect& textureRect, short type,
          b2World* world = nullptr);
    Block(StateData* stateData, unsigned grid_x, unsigned grid_y, float gridWidth, float gridHeight,
          int lives, sf::Texture& texture, const sf::IntRect& textureRect, short type);

    ~Block();
    Block(const Block&) = delete;
    Block& operator=(const Block&) = delete;

    std::string serialize() const;
    const sf::Vector2f& getPosition() const;
    sf::FloatRect getGlobalBounds() const;
    short getType() const;
    int getLives() const;
    sf::IntRect getRect() const;
    sf::Sprite getShape() const;

    b2BodyDef* getBodyDefinition();
    b2Body* getBody();
    b2FixtureDef* getFixtureDefinition();

    void setLives(int lives);
    void setTextureRect(sf::IntRect textureRect);

    void setRotation(float rotation);

    void render(sf::RenderTarget& target);
};
