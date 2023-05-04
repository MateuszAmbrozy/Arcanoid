#pragma once
#include "State.h"
#include "ObjectData.h"
enum BlockTypes
{
	DEFAULT = 0,
	POWER,
	UNBREAKING
};

class Block
{
private:
	//BOX2D
	b2Body* m_body;
	b2BodyDef* m_bodyDef;
	b2Shape* m_shape;
	b2FixtureDef* m_fixtureDef;
	UserData* userData;

	void initBox2d(StateData* stateData, b2World* world, sf::Vector2f position);

protected:
	int lifes;
	sf::Sprite shape;
	sf::IntRect bufor;
	int left, top;
	short type;
	sf::IntRect textureRect;
	sf::VideoMode vm;

public:
	//CONSTRUCTOR AND DESTRUCTOR
	//constructor for GameState and loadFromFile function
	Block(StateData* stateData, unsigned grid_x, unsigned grid_y, int left, int top, float gridWidth, float gridHeight, int lifes, sf::Texture& texture,
		const sf::IntRect& textureRect, short type, b2World* world = nullptr); 
	//constructor for push blocks in EditorState
	Block(StateData* stateData, unsigned grid_x, unsigned grid_y, float gridWidth, float gridHeight, int lifes, sf::Texture& texture,
		const sf::IntRect& textureRect, short type);

	virtual ~Block();

	//ACCESSORS
	const std::string getAssString() const;
	const sf::Vector2f& getPosition() const;
	const sf::FloatRect& getGlobalBounds() const;
	const short getType() const;
	int getLifes() const;
	const sf::IntRect getRect() const;
	const sf::Sprite getShape() const;

	//box2d accessors
	b2BodyDef* GetBodyDef();
	b2Body* GetBody();
	b2FixtureDef* GetFixtureDef();

	//MODIFIERS
	void setLifes(int lifes);
	void setTextureRect(sf::IntRect textureRect);

	//box2d modifiers
	void setRotation(float rotation);

	//FUNCTIONS
	void render(sf::RenderTarget& target);

};