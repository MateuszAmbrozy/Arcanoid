#pragma once
#include "State.h"
#include "Gui.h"
#include "ObjectData.h"
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

	//box2d variables
	b2Body* m_body;
	b2BodyDef* m_bodyDef;
	b2Shape* m_shape;
	b2FixtureDef* m_fixtureDef;
	UserData* userData;
	
	void initTexture(StateData* stateData);
	void initBox2D(b2World* world);

public:
	//CONSTRUCTOR AND DESTRUCTOR
	Paddle(StateData* stateData, b2World* world);
	virtual ~Paddle();

	//ACCESSORS
	const sf::Vector2f getPosition() const;
	const sf::FloatRect getGlobalBounds() const;
	const sf::Vector2f getSize() const;

	//box2d accessors
	b2BodyDef* GetBodyDef();
	b2Body* GetBody();
	b2FixtureDef* GetFixtureDef();

	//MODIFIERS
	void setPosition(sf::Vector2f position);
	void biggerPaddle();
	void smallerPaddle();

	//box2d modifiers
	void setBodySize();

	//FUNCTIONS
	void move(const float dirX, const float dirY, const float& dt);
	void updateWorldBoundsCollision();
	void update(const float& dt);
	void render(sf::RenderTarget* target);
};

