#include "stdafx.h"
#include "Ball.h"

//PRIVATE FUNCTIONS
void Ball::initTexture(StateData* stateData, const float radious)
{
	this->shape.setRadius(radious);
	if (!this->texture.loadFromFile("Resources/Sprites/ball.png"))
		std::cout << "ERROR::BALL::Could not load ball.png\n";
	this->shape.setTexture(&this->texture);
	if (stateData->ballTexture.getSize().x > 0)
		this->shape.setTexture(&stateData->ballTexture);
	this->shape.setOrigin(radious, radious);

}
void Ball::initbox2d(StateData* stateData, b2World* world, sf::Vector2f position, const float radious)
{
	m_bodyDef = new b2BodyDef();
	m_bodyDef->position = b2Vec2(position.x / SCALE, position.y / SCALE);
	m_bodyDef->type = b2_dynamicBody;
	m_bodyDef->bullet = true;
	m_body = world->CreateBody(m_bodyDef);

	m_shape = new b2CircleShape();
	m_shape->m_radius = radious / SCALE;

	m_fixtureDef = new b2FixtureDef();
	m_fixtureDef->density = 5.f;
	m_fixtureDef->friction = 0.01f;
	m_fixtureDef->restitution = 1.f;
	m_fixtureDef->shape = m_shape;

	////////////////////////
	userData = new UserData();
	userData->name = "ball";

	m_fixtureDef->userData.pointer = (uintptr_t)userData;

	m_body->CreateFixture(m_fixtureDef);
}

//CONSTRUCTOR AND DESTRUCTOR
Ball::Ball(StateData* stateData, b2World* world,  const Paddle* paddle, const float radious) //constructor for default ball
	:vm(stateData->gfxSettings->resolution)
{
	position = sf::Vector2f(paddle->getPosition().x + paddle->getGlobalBounds().width / 2.f,
		paddle->getPosition().y - radious - 1.f);

	this->initbox2d(stateData, world, this->position, radious);
	this->initTexture(stateData, radious);

	this->shape.setPosition(position);

	this->velocity = sf::Vector2f(static_cast<float>((std::rand() % 200) - 100) / 100.f, -1.f);
	this->maxSpeed = static_cast<float>(gui::callCharSize(this->vm, 2.5f));

}
Ball::Ball(StateData* stateData, b2World* world, const float radious, sf::Vector2f position) //constructor for ball made by superPowers
	:vm(stateData->gfxSettings->resolution)
{
	this->initbox2d(stateData, world, position, radious);
	this->initTexture(stateData, radious);

	this->shape.setPosition(position);

	this->velocity = sf::Vector2f(static_cast<float>((std::rand() % 200) - 100) / 100.f, -1.f);
	this->maxSpeed = static_cast<float>(gui::callCharSize(vm, 2.5f));
	this->m_body->SetLinearVelocity(b2Vec2(velocity.x * maxSpeed * 0.01f * s, velocity.x * maxSpeed * 0.01f * s));
}
Ball::~Ball()
{
	delete this->m_bodyDef;
	delete this->m_fixtureDef;
	delete this->m_shape;
	delete this->userData;
}

//ACCESSORS
const sf::Vector2i Ball::getGridPosition(const int gridWidth, const int gridHeight) const
{
	return sf::Vector2i(
		static_cast<int>(this->shape.getPosition().x) / gridWidth,
		static_cast<int>(this->shape.getPosition().y) / gridHeight
	);
}
const sf::Vector2f Ball::getPosition() const
{
		return this->shape.getPosition();
}
const sf::FloatRect Ball::getGlobalBounds() const
{
	return this->shape.getGlobalBounds();
}

//box2d accessors
b2BodyDef* Ball::GetBodyDef()
{
	return m_bodyDef;
}
b2Body* Ball::GetBody()
{
	return m_body;
}
b2FixtureDef* Ball::GetFixtureDef()
{
	return m_fixtureDef;
}

//MODIFIERS
void Ball::setPosition(sf::Vector2f position)
{
	this->shape.setPosition(position);
}
void Ball::setTexture(sf::Texture texture)
{
	this->texture = texture;
	this->shape.setTexture(&texture);
}

//box2d modifiers
void Ball::setRotation(float rotation)
{
	this->shape.setRotation(rotation);
}

//FUNCTIONS
void Ball::move(const float dirX, const float dirY, const float& dt)
{
	this->m_body->SetLinearVelocity(b2Vec2(dirX * this->maxSpeed * dt  * s, dirY * this->maxSpeed * dt * s));
}
void Ball::move(sf::Vector2f velocity, const float& dt)
{
	this->m_body->SetLinearVelocity(b2Vec2(velocity.x * this->maxSpeed * dt * s, velocity.y * this->maxSpeed * dt * s));
}

void Ball::update(const float& dt)
{
	if (std::abs(this->m_body->GetLinearVelocity().x) >= maxSpeed * dt * s * 2.f)
	{
		this->m_body->SetLinearVelocity(b2Vec2(this->m_body->GetLinearVelocity().x + ((this->m_body->GetLinearVelocity().x > 0.0f) ? -this->maxSpeed * dt : this->maxSpeed * dt), m_body->GetLinearVelocity().y));
	}
	if (std::abs(this->m_body->GetLinearVelocity().y) >= maxSpeed * dt * s * 2.f)
	{
		this->m_body->SetLinearVelocity(b2Vec2(this->m_body->GetLinearVelocity().x, this->m_body->GetLinearVelocity().y + ((this->m_body->GetLinearVelocity().y > 0.0f) ? -this->maxSpeed * dt : this->maxSpeed * dt)));
	}
	
	if (std::abs(m_body->GetLinearVelocity().x) < maxSpeed * dt * 5)
	{
		m_body->SetLinearVelocity(b2Vec2(m_body->GetLinearVelocity().x * 1.2f, m_body->GetLinearVelocity().y));
	}
	if (std::abs(m_body->GetLinearVelocity().y) < maxSpeed * dt * 5)
	{
		m_body->SetLinearVelocity(b2Vec2(m_body->GetLinearVelocity().x, m_body->GetLinearVelocity().y* 1.2f));

	}

	shape.setRotation(m_body->GetAngle() * 180 / b2_pi);
	shape.setPosition(SCALE * m_body->GetPosition().x, SCALE * m_body->GetPosition().y);
}
void Ball::render(sf::RenderTarget* target)
{
	target->draw(this->shape);
}
