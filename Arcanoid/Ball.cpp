#include "Ball.h"
#include "Assets.h"
#include "Random.h"
#include "stdafx.h"

void Ball::initTexture(StateData* stateData, const float radius)
{
    this->shape.setRadius(radius);
    assets::load(this->texture, "Resources/Sprites/ball.png");
    this->shape.setTexture(&this->texture);
    if (stateData->ballTexture.getSize().x > 0)
        this->shape.setTexture(&stateData->ballTexture);
    this->shape.setOrigin(radius, radius);
}
void Ball::initBox2D(StateData*, b2World* world, sf::Vector2f position, const float radius)
{
    m_bodyDef.position = b2Vec2(position.x / physics::pixelsPerMeter, position.y / physics::pixelsPerMeter);
    m_bodyDef.type = b2_dynamicBody;
    m_bodyDef.bullet = true;
    ownerWorld = world;
    m_body = world->CreateBody(&m_bodyDef);
    m_shape.m_radius = radius / physics::pixelsPerMeter;
    m_fixtureDef.density = 5.f;
    m_fixtureDef.friction = 0.01f;
    m_fixtureDef.restitution = 1.f;
    m_fixtureDef.shape = &m_shape;

    userData.type = ObjectType::Ball;

    m_fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(&userData);

    m_body->CreateFixture(&m_fixtureDef);
}

Ball::Ball(StateData* stateData, b2World* world, const Paddle* paddle, const float radius)
    : vm(stateData->gfxSettings->resolution)
{
    position = sf::Vector2f(paddle->getPosition().x + paddle->getGlobalBounds().width / 2.f,
                            paddle->getPosition().y - radius - 1.f);

    this->initTexture(stateData, radius);
    this->initBox2D(stateData, world, this->position, radius);

    this->shape.setPosition(position);

    this->velocity = sf::Vector2f(gameRandom::launchDirection(), -1.f);
    this->maxSpeed = static_cast<float>(gui::callCharSize(this->vm, 2.5f));
}
Ball::Ball(StateData* stateData, b2World* world, const float radius, sf::Vector2f position)
    : vm(stateData->gfxSettings->resolution)
{
    this->initTexture(stateData, radius);
    this->initBox2D(stateData, world, position, radius);

    this->shape.setPosition(position);

    this->velocity = sf::Vector2f(gameRandom::launchDirection(), -1.f);
    this->maxSpeed = static_cast<float>(gui::callCharSize(vm, 2.5f));
    this->m_body->SetLinearVelocity(b2Vec2(velocity.x * maxSpeed * 0.5f / physics::pixelsPerMeter,
                                           velocity.y * maxSpeed * 0.5f / physics::pixelsPerMeter));
}
Ball::~Ball()
{
    if (ownerWorld && m_body)
        ownerWorld->DestroyBody(m_body);
}

sf::Vector2i Ball::getGridPosition(const int gridWidth, const int gridHeight) const
{
    return sf::Vector2i(static_cast<int>(this->shape.getPosition().x) / gridWidth,
                        static_cast<int>(this->shape.getPosition().y) / gridHeight);
}
sf::Vector2f Ball::getPosition() const
{
    return this->shape.getPosition();
}
sf::FloatRect Ball::getGlobalBounds() const
{
    return this->shape.getGlobalBounds();
}

b2BodyDef* Ball::getBodyDefinition()
{
    return &m_bodyDef;
}
b2Body* Ball::getBody()
{
    return m_body;
}
b2FixtureDef* Ball::getFixtureDefinition()
{
    return &m_fixtureDef;
}

void Ball::setPosition(sf::Vector2f position)
{
    this->shape.setPosition(position);
    m_body->SetTransform(b2Vec2(position.x / physics::pixelsPerMeter, position.y / physics::pixelsPerMeter),
                         m_body->GetAngle());
}
void Ball::setTexture(sf::Texture texture)
{
    this->texture = texture;
    this->shape.setTexture(&this->texture);
}

void Ball::setRotation(float rotation)
{
    this->shape.setRotation(rotation);
}

void Ball::move(const float dirX, const float dirY, float)
{
    this->m_body->SetLinearVelocity(b2Vec2(dirX * this->maxSpeed * 0.5f / physics::pixelsPerMeter,
                                           dirY * this->maxSpeed * 0.5f / physics::pixelsPerMeter));
}
void Ball::move(sf::Vector2f velocity, float)
{
    this->m_body->SetLinearVelocity(b2Vec2(velocity.x * this->maxSpeed * 0.5f / physics::pixelsPerMeter,
                                           velocity.y * this->maxSpeed * 0.5f / physics::pixelsPerMeter));
}

void Ball::update(float)
{
    auto velocity = m_body->GetLinearVelocity();
    const float limit = maxSpeed * 0.5f / physics::pixelsPerMeter * 2.f;
    const float speed = velocity.Length();
    if (speed > limit && speed > 0.f)
        velocity *= limit / speed;
    if (speed > 0.f && std::abs(velocity.y) < limit * 0.1f)
        velocity.y = velocity.y <= 0.f ? -limit * 0.1f : limit * 0.1f;
    m_body->SetLinearVelocity(velocity);
    shape.setRotation(m_body->GetAngle() * 180.f / b2_pi);
    shape.setPosition(physics::pixelsPerMeter * m_body->GetPosition().x,
                      physics::pixelsPerMeter * m_body->GetPosition().y);
}
void Ball::render(sf::RenderTarget* target)
{
    target->draw(this->shape);
}
