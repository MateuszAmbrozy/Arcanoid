#include "Block.h"
#include "stdafx.h"

void Block::initBox2D(StateData* stateData, b2World* world, sf::Vector2f position)
{
    m_bodyDef.position = b2Vec2(position.x / physics::pixelsPerMeter, position.y / physics::pixelsPerMeter);
    m_bodyDef.type = b2_staticBody;
    ownerWorld = world;
    m_body = world->CreateBody(&m_bodyDef);
    b2Vec2 vertices[4];
    vertices[0].Set(0.f, 0.f);
    vertices[1].Set(stateData->gridWidth / physics::pixelsPerMeter, 0.f);
    vertices[2].Set(stateData->gridWidth / physics::pixelsPerMeter,
                    stateData->gridHeight / physics::pixelsPerMeter);
    vertices[3].Set(0.f, stateData->gridHeight / physics::pixelsPerMeter);
    m_shape.Set(vertices, 4);
    m_fixtureDef.density = 1.f;
    m_fixtureDef.friction = 0.01f;
    m_fixtureDef.restitution = 1.f;
    m_fixtureDef.shape = &m_shape;

    userData.type = ObjectType::Block;

    m_fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(&userData);

    m_body->CreateFixture(&m_fixtureDef);
}

Block::Block(StateData* stateData, unsigned grid_x, unsigned grid_y, int left, int top, float gridWidth,
             float gridHeight, int lives, sf::Texture& texture, const sf::IntRect& textureRect, short type,
             b2World* world)
{
    this->m_body = nullptr;

    this->left = left;
    this->top = top;
    this->sourceRect = textureRect;
    this->textureRect = textureRect;

    this->textureRect = sf::IntRect(left * 100, top * 50, 100, 50);

    this->shape.setPosition(static_cast<float>(grid_x) * gridWidth, static_cast<float>(grid_y) * gridHeight);

    this->shape.setTexture(texture);

    this->shape.setScale(gridWidth / 100.f, gridHeight / 50.f);

    this->shape.setTextureRect(this->textureRect);

    this->type = type;
    this->lives = lives;

    if (world)
        this->initBox2D(
            stateData, world,
            sf::Vector2f(static_cast<float>(grid_x) * gridWidth, static_cast<float>(grid_y) * gridHeight));
}
Block::Block(StateData* stateData, unsigned grid_x, unsigned grid_y, float gridWidth, float gridHeight,
             int lives, sf::Texture& texture, const sf::IntRect& textureRect, short type)
{
    this->m_body = nullptr;

    this->textureRect = sf::IntRect(textureRect.left, textureRect.top, 100, 50);
    this->sourceRect = this->textureRect;
    this->vm = stateData->gfxSettings->resolution;
    this->left = textureRect.left / 100;
    this->top = textureRect.top / 50;

    this->shape.setPosition(
        sf::Vector2f(static_cast<float>(grid_x) * gridWidth, static_cast<float>(grid_y) * gridHeight));

    this->shape.setTexture(texture);

    this->shape.setScale(gridWidth / 100.f, gridHeight / 50.f);

    this->shape.setTextureRect(this->textureRect);

    this->lives = lives;
    this->type = type;
}
Block::~Block()
{
    if (ownerWorld && m_body)
        ownerWorld->DestroyBody(m_body);
}

std::string Block::serialize() const
{
    std::stringstream ss;
    ss << this->left << " " << this->top << " " << this->sourceRect.left << " " << this->sourceRect.top << " "
       << this->lives << " " << this->type;
    return ss.str();
}
const sf::Vector2f& Block::getPosition() const
{
    return this->shape.getPosition();
}
sf::FloatRect Block::getGlobalBounds() const
{
    return this->shape.getGlobalBounds();
}
short Block::getType() const
{
    return this->type;
}
int Block::getLives() const
{
    return this->lives;
}
sf::IntRect Block::getRect() const
{
    return this->textureRect;
}
sf::Sprite Block::getShape() const
{
    return this->shape;
}

b2BodyDef* Block::getBodyDefinition()
{
    return &m_bodyDef;
}
b2Body* Block::getBody()
{
    return m_body;
}
b2FixtureDef* Block::getFixtureDefinition()
{
    return &m_fixtureDef;
}

void Block::setLives(int lives)
{
    this->lives = lives;
}
void Block::setTextureRect(sf::IntRect textureRect)
{
    this->textureRect = textureRect;
    this->shape.setTextureRect(this->textureRect);
}

void Block::setRotation(float rotation)
{
    this->shape.setRotation(rotation);
}

void Block::render(sf::RenderTarget& target)
{
    target.draw(this->shape);
}
