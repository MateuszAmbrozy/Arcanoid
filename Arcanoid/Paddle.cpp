#include "stdafx.h"
#include "Paddle.h"

//PRIVATE FUNCTIONS
void Paddle::initTexture(StateData* stateData)
{
	this->texture.loadFromFile("Resources/Sprites/paddle.png");
	this->shape.setTexture(&this->texture);

	//this->shape.setScale(
	//	gui::p2pX(6.667f, stateData->gfxSettings->resolution) / shape.getLocalBounds().width,
	//	gui::p2pY(2.779f, stateData->gfxSettings->resolution) / shape.getLocalBounds().height
	//);
	this->shape.setSize(sf::Vector2f(this->width, this->height));

	this->textureRect = sf::IntRect(57, 0, 90, 30);

	if (stateData->paddleTopRect != -1)
	{
		this->textureRect.top = stateData->paddleTopRect;
		std::cout << "TOP: " << stateData->paddleTopRect << std::endl;
	}
	
	this->shape.setTextureRect(this->textureRect);
		
}
void Paddle::initBox2D(b2World* world)
{
	m_bodyDef = new b2BodyDef();
	m_bodyDef->position = b2Vec2(this->position.x / SCALE, this->position.y / SCALE);
	m_bodyDef->type = b2_kinematicBody;
	m_body = world->CreateBody(m_bodyDef);


	m_shape = new b2PolygonShape();
	b2Vec2 vertices[4];
	vertices[0].Set(0.f, 0.f);
	vertices[1].Set(this->width / SCALE, 0.f);
	vertices[2].Set(this->width / SCALE, this->height / SCALE);
	vertices[3].Set(0.f, this->height / SCALE);
	static_cast<b2PolygonShape*>(m_shape)->Set(vertices, 4);

	m_fixtureDef = new b2FixtureDef();
	m_fixtureDef->density = 0.1f;
	m_fixtureDef->friction = 0.01f;
	m_fixtureDef->restitution = 1.f;
	m_fixtureDef->shape = m_shape;

	////////////////////////
	userData = new UserData();
	userData->name = "paddle";

	m_fixtureDef->userData.pointer = (uintptr_t)userData;

	m_body->CreateFixture(m_fixtureDef);
}

//CONSTRUCTOR AND DESTRUCTOR
Paddle::Paddle(StateData* stateData, b2World* world)
	:vm(stateData->gfxSettings->resolution)
{
	this->width = gui::p2pX(20.7f, stateData->gfxSettings->resolution);
	this->height = gui::p2pY(2.7f, stateData->gfxSettings->resolution);
	this->initTexture(stateData);

	this->position = sf::Vector2f(static_cast<float>(stateData->gfxSettings->resolution.width / 2.f), static_cast<float>(stateData->gfxSettings->resolution.height - gui::p2pY(9.25f, stateData->gfxSettings->resolution)));
	this->shape.setPosition(this->position);
	this->initBox2D(world);

	this->maxVelocity = static_cast<float>(gui::callCharSize(stateData->gfxSettings->resolution, 1.f));
}
Paddle::~Paddle()
{
	delete this->m_bodyDef;
	delete this->m_shape;
	delete this->m_fixtureDef;
	delete this->userData;
}

//ACCESSORS
const sf::Vector2f Paddle::getPosition() const
{
	return this->shape.getPosition();
}
const sf::FloatRect Paddle::getGlobalBounds() const
{
	return this->shape.getGlobalBounds();
}
const sf::Vector2f Paddle::getSize() const
{
	return sf::Vector2f(width, height);
}

//box2d accessors
b2BodyDef* Paddle::GetBodyDef()
{
	return this->m_bodyDef;
}
b2Body* Paddle::GetBody()
{
	return this->m_body;
}
b2FixtureDef* Paddle::GetFixtureDef()
{
	return this->m_fixtureDef;
}

//MODIFIERS
void Paddle::setPosition(sf::Vector2f position)
{
	this->shape.setPosition(position);
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
		this->width = width * 1.4f;//1.428571428571429f
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
		this->width = width * 0.7f;
		this->shape.setSize(sf::Vector2f(this->width, this->height));
		this->shape.setTextureRect(this->textureRect);
		this->setBodySize();
		break;
	case 147:
		this->textureRect.left = 57;
		this->textureRect.width = 90;
		this->width = width * 0.7f;
		this->shape.setSize(sf::Vector2f(this->width, this->height));
		this->shape.setTextureRect(this->textureRect);
		this->setBodySize();
		break;
	case 57:
		this->textureRect.left = 0;
		this->textureRect.width = 57;
		this->width = width * 0.7f;
		this->shape.setSize(sf::Vector2f(this->width, this->height));
		this->shape.setTextureRect(this->textureRect);
		this->setBodySize();
		break;
	default:
		break;
	}
}

//box2d modifiers
void Paddle::setBodySize()
{
	// Pobierz istniej¹c¹ fixture i ustawienia jej w³aœciwoœci
	b2Fixture* fixture = m_body->GetFixtureList();
	b2FixtureDef fixtureDef;
	fixtureDef.density = fixture->GetDensity();
	fixtureDef.friction = fixture->GetFriction();
	fixtureDef.restitution = fixture->GetRestitution();
	fixtureDef.filter = fixture->GetFilterData();

	// Stwórz nowy kszta³t i ustaw jego wierzcho³ki
	b2PolygonShape shape;
	b2Vec2 vertices[4];
	vertices[0].Set(0.f, 0.f);
	vertices[1].Set(this->width / SCALE, 0.f);
	vertices[2].Set(this->width / SCALE, this->height / SCALE);
	vertices[3].Set(0.f, this->height / SCALE);
	shape.Set(vertices, 4);

	// Zastosuj nowy kszta³t do istniej¹cej fixture
	fixtureDef.shape = &shape;
	m_body->DestroyFixture(fixture);
	m_body->CreateFixture(&fixtureDef);
}

//FUNCTIONS
void Paddle::move(const float dirX, const float dirY, const float& dt)
{	
	this->m_body->SetLinearVelocity(b2Vec2(dirX * this->maxVelocity * dt * s, dirY * this->maxVelocity * dt * s));
}
void Paddle::updateWorldBoundsCollision()
{
	if (m_body->GetPosition().x * SCALE < 0)
	{
		m_body->SetLinearVelocity(b2Vec2(0, 0));
		m_body->SetTransform(b2Vec2(0.f, m_bodyDef->position.y), 0.f);
	}
	else if (m_body->GetPosition().x * SCALE + this->width /** SCALE*/ > vm.width)
	{
		m_body->SetLinearVelocity(b2Vec2(0, 0));
		m_body->SetTransform(b2Vec2((vm.width - width) / SCALE, m_bodyDef->position.y), 0.f);
	}
}
void Paddle::update(const float& dt)
{
	this->shape.setPosition(SCALE * m_body->GetPosition().x, SCALE * m_body->GetPosition().y);
	this->updateWorldBoundsCollision();
}
void Paddle::render(sf::RenderTarget* target)
{
	target->draw(this->shape);
}
