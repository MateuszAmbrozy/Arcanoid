#include "Paddle.h"
#include "Assets.h"
#include "stdafx.h"

void Paddle::initTexture(StateData* stateData)
{
    assets::load(this->texture, "Resources/Sprites/paddle.png");
    this->shape.setTexture(&this->texture);

    this->shape.setSize(sf::Vector2f(this->width, this->height));

    this->textureRect = sf::IntRect(57, 0, 90, 30);

    if (stateData->paddleTopRect != -1)
    {
        this->textureRect.top = stateData->paddleTopRect;
    }

    this->shape.setTextureRect(this->textureRect);
}
void Paddle::initBox2D(b2World* world)
{
    m_bodyDef.position =
        b2Vec2(this->position.x / physics::pixelsPerMeter, this->position.y / physics::pixelsPerMeter);
    m_bodyDef.type = b2_kinematicBody;
    ownerWorld = world;
    m_body = world->CreateBody(&m_bodyDef);
    b2Vec2 vertices[4];
    vertices[0].Set(0.f, 0.f);
    vertices[1].Set(this->width / physics::pixelsPerMeter, 0.f);
    vertices[2].Set(this->width / physics::pixelsPerMeter, this->height / physics::pixelsPerMeter);
    vertices[3].Set(0.f, this->height / physics::pixelsPerMeter);
    m_shape.Set(vertices, 4);
    m_fixtureDef.density = 0.1f;
    m_fixtureDef.friction = 0.01f;
    m_fixtureDef.restitution = 1.f;
    m_fixtureDef.shape = &m_shape;

    userData.type = ObjectType::Paddle;

    m_fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(&userData);

    m_body->CreateFixture(&m_fixtureDef);
}

Paddle::Paddle(StateData* stateData, b2World* world) : vm(stateData->gfxSettings->resolution)
{
    this->width = gui::p2pX(20.7f, stateData->gfxSettings->resolution);
    this->height = gui::p2pY(2.7f, stateData->gfxSettings->resolution);
    this->initTexture(stateData);

    this->position = sf::Vector2f(static_cast<float>(stateData->gfxSettings->resolution.width / 2.f),
                                  static_cast<float>(stateData->gfxSettings->resolution.height -
                                                     gui::p2pY(9.25f, stateData->gfxSettings->resolution)));
    this->shape.setPosition(this->position);
    this->initBox2D(world);

    this->maxVelocity = static_cast<float>(gui::callCharSize(stateData->gfxSettings->resolution, 1.f));
}
Paddle::~Paddle()
{
    if (ownerWorld && m_body)
        ownerWorld->DestroyBody(m_body);
}

sf::Vector2f Paddle::getPosition() const
{
    return this->shape.getPosition();
}
sf::FloatRect Paddle::getGlobalBounds() const
{
    return this->shape.getGlobalBounds();
}
sf::Vector2f Paddle::getSize() const
{
    return sf::Vector2f(width, height);
}

b2BodyDef* Paddle::getBodyDefinition()
{
    return &this->m_bodyDef;
}
b2Body* Paddle::getBody()
{
    return this->m_body;
}
b2FixtureDef* Paddle::getFixtureDefinition()
{
    return &this->m_fixtureDef;
}

void Paddle::setPosition(sf::Vector2f position)
{
    this->shape.setPosition(position);
    m_body->SetTransform(b2Vec2(position.x / physics::pixelsPerMeter, position.y / physics::pixelsPerMeter),
                         0.f);
}
void Paddle::biggerPaddle()
{
    switch (this->textureRect.left)
    {
    case 0:
        this->textureRect.left = 57;
        this->textureRect.width = 90;
        this->width = width * 1.4f;
        this->shape.setSize(sf::Vector2f(this->width, this->height));
        this->shape.setTextureRect(this->textureRect);
        this->setBodySize();
        break;
    case 57:
        this->textureRect.left = 147;
        this->textureRect.width = 128;
        this->width = width * 1.4f;
        this->shape.setSize(sf::Vector2f(this->width, this->height));
        this->shape.setTextureRect(this->textureRect);
        this->setBodySize();
        break;
    case 147:
        this->textureRect.left = 275;
        this->width = width * 1.4f;
        this->textureRect.width = 175;
        this->shape.setSize(sf::Vector2f(this->width, this->height));
        this->shape.setTextureRect(this->textureRect);
        this->setBodySize();
        break;
    default:
        break;
    }
}
void Paddle::smallerPaddle()
{

    switch (this->textureRect.left)
    {
    case 275:
        this->textureRect.left = 147;
        this->textureRect.width = 128;
        this->width /= 1.4f;
        this->shape.setSize(sf::Vector2f(this->width, this->height));
        this->shape.setTextureRect(this->textureRect);
        this->setBodySize();
        break;
    case 147:
        this->textureRect.left = 57;
        this->textureRect.width = 90;
        this->width /= 1.4f;
        this->shape.setSize(sf::Vector2f(this->width, this->height));
        this->shape.setTextureRect(this->textureRect);
        this->setBodySize();
        break;
    case 57:
        this->textureRect.left = 0;
        this->textureRect.width = 57;
        this->width /= 1.4f;
        this->shape.setSize(sf::Vector2f(this->width, this->height));
        this->shape.setTextureRect(this->textureRect);
        this->setBodySize();
        break;
    default:
        break;
    }
}

void Paddle::setBodySize()
{
    b2Fixture* fixture = m_body->GetFixtureList();
    b2FixtureDef fixtureDef;
    fixtureDef.density = fixture->GetDensity();
    fixtureDef.friction = fixture->GetFriction();
    fixtureDef.restitution = fixture->GetRestitution();
    fixtureDef.filter = fixture->GetFilterData();
    fixtureDef.userData = fixture->GetUserData();

    b2PolygonShape shape;
    b2Vec2 vertices[4];
    vertices[0].Set(0.f, 0.f);
    vertices[1].Set(this->width / physics::pixelsPerMeter, 0.f);
    vertices[2].Set(this->width / physics::pixelsPerMeter, this->height / physics::pixelsPerMeter);
    vertices[3].Set(0.f, this->height / physics::pixelsPerMeter);
    shape.Set(vertices, 4);

    fixtureDef.shape = &shape;
    m_body->DestroyFixture(fixture);
    m_body->CreateFixture(&fixtureDef);
}

void Paddle::move(const float dirX, const float dirY, float)
{
    this->m_body->SetLinearVelocity(b2Vec2(dirX * this->maxVelocity * 0.5f / physics::pixelsPerMeter,
                                           dirY * this->maxVelocity * 0.5f / physics::pixelsPerMeter));
}
void Paddle::updateWorldBoundsCollision()
{
    if (m_body->GetPosition().x * physics::pixelsPerMeter < 0)
    {
        m_body->SetLinearVelocity(b2Vec2(0, 0));
        m_body->SetTransform(b2Vec2(0.f, m_bodyDef.position.y), 0.f);
    }
    else if (m_body->GetPosition().x * physics::pixelsPerMeter + this->width > vm.width)
    {
        m_body->SetLinearVelocity(b2Vec2(0, 0));
        m_body->SetTransform(b2Vec2((vm.width - width) / physics::pixelsPerMeter, m_bodyDef.position.y), 0.f);
    }
}
void Paddle::update(float)
{
    this->updateWorldBoundsCollision();
    this->shape.setPosition(physics::pixelsPerMeter * m_body->GetPosition().x,
                            physics::pixelsPerMeter * m_body->GetPosition().y);
}
void Paddle::render(sf::RenderTarget* target)
{
    target->draw(this->shape);
}
