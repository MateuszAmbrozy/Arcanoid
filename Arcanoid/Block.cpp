#include "stdafx.h"
#include "Block.h"

//PRIVATE FUNCTIONS
void Block::initBox2d(StateData* stateData, b2World* world, sf::Vector2f position)
{
	m_bodyDef = new b2BodyDef();
	m_bodyDef->position = b2Vec2(position.x / SCALE, position.y / SCALE);
	m_bodyDef->type = b2_staticBody;
	m_body = world->CreateBody(m_bodyDef);


	m_shape = new b2PolygonShape();
	b2Vec2 vertices[4];
	vertices[0].Set(0.f, 0.f);
	vertices[1].Set(stateData->gridWidth / SCALE, 0.f);
	vertices[2].Set(stateData->gridWidth / SCALE, stateData->gridHeight / SCALE);
	vertices[3].Set(0.f, stateData->gridHeight / SCALE);
	static_cast<b2PolygonShape*>(m_shape)->Set(vertices, 4);

	m_fixtureDef = new b2FixtureDef();
	m_fixtureDef->density = 1.f;
	m_fixtureDef->friction = 0.01f;
	m_fixtureDef->restitution = 1.f;
	m_fixtureDef->shape = m_shape;

	////////////////////////
	userData = new UserData();
	userData->name = "block";

	m_fixtureDef->userData.pointer = (uintptr_t)userData;

	m_body->CreateFixture(m_fixtureDef);
}

//CONSTRUCTOR AND DESTRUCTOR
Block::Block(StateData* stateData, unsigned grid_x, unsigned grid_y, int left, int top, float gridWidth, float gridHeight, int lifes, sf::Texture& texture,
	const sf::IntRect& textureRect, short type, b2World* world)
{
	this->m_body = nullptr;
	this->m_bodyDef = nullptr;
	this->m_fixtureDef = nullptr;
	this->m_shape = nullptr;
	this->userData = nullptr;

	this->left = left;
	this->top = top;
	this->bufor = textureRect;
	this->textureRect = textureRect;

	this->textureRect.left = static_cast<float>(left * gridWidth * 1920.f / stateData->gfxSettings->resolution.width);
	this->textureRect.top = static_cast<float>(top * gridHeight * 1080.f / stateData->gfxSettings->resolution.height);
	this->textureRect.width = static_cast<float>(textureRect.width * 1920 / stateData->gfxSettings->resolution.width);
	this->textureRect.height = static_cast<float>(textureRect.height * 1080 / stateData->gfxSettings->resolution.height);

	this->shape.setPosition(static_cast<float>(grid_x) * gridWidth, static_cast<float>(grid_y) * gridHeight);


	this->shape.setTexture(texture);

	this->shape.setScale(
		gridWidth * 9 / shape.getLocalBounds().width,
		gridHeight * 6 / shape.getLocalBounds().height
	);

	this->shape.setTextureRect(this->textureRect);

	this->type = type;
	this->lifes = lifes;

	if(world)
		this->initBox2d(stateData, world, sf::Vector2f(static_cast<float>(grid_x) * gridWidth, static_cast<float>(grid_y) * gridHeight));
	

}
Block::Block(StateData* stateData, unsigned grid_x, unsigned grid_y, float gridWidth, float gridHeight, int lifes, sf::Texture& texture,
	const sf::IntRect& textureRect, short type)
{
	this->m_body = nullptr;
	this->m_bodyDef = nullptr;
	this->m_fixtureDef = nullptr;
	this->m_shape = nullptr;
	this->userData = nullptr;

	this->textureRect = textureRect;
	this->bufor = textureRect;
	this->vm = stateData->gfxSettings->resolution;
	this->left = textureRect.left / textureRect.width;
	this->top = textureRect.top / textureRect.height;

	this->shape.setPosition(sf::Vector2f(static_cast<float>(grid_x) * gridWidth, static_cast<float>(grid_y) * gridHeight));


	this->shape.setTexture(texture);

	this->shape.setScale(
		gridWidth * 9 / shape.getLocalBounds().width,
		gridHeight * 6 / shape.getLocalBounds().height
	);

	this->shape.setTextureRect(this->textureRect);

	this->lifes = lifes;
	this->type = type;
}
Block::~Block()
{
	delete this->m_bodyDef;
	delete this->m_fixtureDef;
	delete this->userData;
	delete this->m_shape;
}

//ACCESSORS
const std::string Block::getAssString() const
{
	std::stringstream ss;
	ss << this->left << " " << this->top << " " << this->bufor.left << " " << this->bufor.top << " " << this->lifes << " " << this->type;
	return ss.str();
}
const sf::Vector2f& Block::getPosition() const
{
	return this->shape.getPosition();
}
const sf::FloatRect& Block::getGlobalBounds() const
{
	return this->shape.getGlobalBounds();
}
const short Block::getType() const
{
	return this->type;
}
int Block::getLifes() const
{
	return this->lifes;
}
const sf::IntRect Block::getRect() const
{
	return this->textureRect;
}
const sf::Sprite Block::getShape() const
{
	return this->shape;
}

//box2d accessors
b2BodyDef* Block::GetBodyDef()
{
	return m_bodyDef;
}
b2Body* Block::GetBody()
{
	return m_body;
}
b2FixtureDef* Block::GetFixtureDef()
{
	return m_fixtureDef;
}


//MODIFIERS	
void Block::setLifes(int lifes)
{
	this->lifes = lifes;
}
void Block::setTextureRect(sf::IntRect textureRect)
{
	this->textureRect = textureRect;
	this->shape.setTextureRect(this->textureRect);
}

//box2d modifiers
void Block::setRotation(float rotation)
{
	this->shape.setRotation(rotation);
}

//FUNCTIONS
void Block::render(sf::RenderTarget& target)
{
		target.draw(this->shape);
}

