#include "stdafx.h"
#include "Powers.h"

//CONSTRUCTOR AND DESTRUCTOR
Powers::Powers(StateData* stateData,  sf::Vector2f position)
{
	this->vm = stateData->gfxSettings->resolution;
	this->maxVelocity = static_cast<float>(gui::callCharSize(stateData->gfxSettings->resolution, 15));

	this->shape.setPosition(position);
	this->powerType =  std::rand()%7;	
	if(this->powerType == 2 || this->powerType == 3)
	{
		if (!this->texture[1].loadFromFile("Resources/Sprites/ballsPower.png"))
			std::cout << "ERROR::TEXTURE::LOADFROMFILE::Could not load power.bmp" << std::endl;
		this->shape.setTexture(&this->texture[1]);
	}
	else if (this->powerType == 1 || this->powerType == 5 || this->powerType == 6)
	{
		if (!this->texture[2].loadFromFile("Resources/Sprites/warningPower.png"))
			std::cout << "ERROR::TEXTURE::LOADFROMFILE::Could not load power.bmp" << std::endl;
		this->shape.setTexture(&this->texture[2]);
	}
	else 
	{
		if (!this->texture[0].loadFromFile("Resources/Sprites/power.png"))
			std::cout << "ERROR::TEXTURE::LOADFROMFILE::Could not load power.bmp" << std::endl;
		this->shape.setTexture(&this->texture[0]);
	}
	this->shape.setSize(sf::Vector2f(gui::p2pX(3.9f, this->vm), gui::p2pX(3.9f, this->vm)));
}
Powers::~Powers()
{
}

//ACCESSORS
const bool Powers::intersects(const sf::FloatRect bounds) const
{
	return this->shape.getGlobalBounds().intersects(bounds);
}
const unsigned short Powers::getType() const
{
	return this->powerType;
}
const sf::FloatRect Powers::getGlobalBounds() const
{
	return this->shape.getGlobalBounds();
}

//FUNCTIONS
void Powers::move(const float& dt)
{
	this->shape.move(0.f, this->maxVelocity * dt);
}
void Powers::initNewBalls(StateData* stateData, b2World* world, std::vector<Ball*>* balls)
{
	size_t size = balls->size();
		for(size_t i=0; i<size; i++)
		{
			balls->push_back(new Ball(stateData, world, gui::p2pX(1.4f, this->vm), balls->at(i)->getPosition()));
		}
}
void Powers::initNewBallsFromPaddle(StateData* stateData, b2World* world, Paddle* paddle, std::vector<Ball*>* balls)
{
	balls->push_back(new Ball(stateData, world, paddle, gui::p2pX(1.4f, this->vm)));
	balls->push_back(new Ball(stateData, world, paddle, gui::p2pX(1.4f, this->vm)));
	balls->push_back(new Ball(stateData, world, paddle, gui::p2pX(1.4f, this->vm)));
}
void Powers::initBiggerPaddle(Paddle* paddle)
{
	paddle->biggerPaddle();
}
void Powers::initSmallerPaddle(Paddle* paddle)
{
	paddle->smallerPaddle();
}
void Powers::initFastBalls(std::vector<Ball*>* balls)
{
	for (auto& ball : *balls)
	{
		ball->GetBody()->SetLinearVelocity(b2Vec2(ball->GetBody()->GetLinearVelocity().x * 1.3f, ball->GetBody()->GetLinearVelocity().y * 1.3f));
	}
}
void Powers::initSlowBalls(std::vector<Ball*>* balls)
{
	for (auto& ball : *balls)
	{
		ball->GetBody()->SetLinearVelocity(b2Vec2(ball->GetBody()->GetLinearVelocity().x * 0.9f, ball->GetBody()->GetLinearVelocity().y * 0.9f));;
	}
}

void Powers::update(const float& dt)
{
	this->move(dt);
}
void Powers::render(sf::RenderTarget& target)
{
	target.draw(this->shape);
}


