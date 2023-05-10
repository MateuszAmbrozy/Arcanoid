#pragma once
#include "Paddle.h"
#include "Gui.h"
#include "ObjectData.h"
class Ball
{
private:
	//VARIABLES
	sf::CircleShape shape;
	float maxSpeed;
	 sf::Texture texture;
	 sf::VideoMode vm;
	 sf::Vector2f position;

	 //BOX2D
	 b2Body* m_body;
	 b2BodyDef* m_bodyDef;
	 b2Shape* m_shape;
	 b2FixtureDef* m_fixtureDef;
	 UserData* userData;

	//PRIVATE FUNCTIONS
	 void initTexture(StateData* stateData, const float radious);
	 void initbox2d(StateData* stateData, b2World* world, sf::Vector2f position, const float radious);
public:
	//CONSTRUCTOR AND DESTRUCTOR

	sf::Vector2f velocity;

	Ball(StateData* stateData, b2World* world, const Paddle* paddle, const float radious);
	Ball(StateData* stateData, b2World* world, const float radious, sf::Vector2f position);
	virtual ~Ball();

	/////////ACCESSORS////////////
	const sf::Vector2i getGridPosition(const int gridWidth, const int gridHeight) const;
	const sf::Vector2f getPosition() const;
	const sf::FloatRect getGlobalBounds() const;

	//box2d accessors
	b2BodyDef* GetBodyDef();
	b2Body* GetBody();
	b2FixtureDef* GetFixtureDef();

	////////MODIFIERS////////////
	void setPosition(sf::Vector2f position);
	void setTexture(sf::Texture texture);

	//box2d modifiers
	void setRotation(float rotation);

	////////FUNCTIONS/////////
	void move(const float dirX, const float dirY, const float& dt);
	void move(sf::Vector2f velocity, const float& dt);

	void update(const float& dt);
	void render(sf::RenderTarget* target);
};

