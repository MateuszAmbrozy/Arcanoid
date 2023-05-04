#pragma once
#include "Block.h"
#include "Gui.h"
#include "Ball.h"
#include "Powers.h"
#include "Paddle.h"
#include"Ball.h"

enum SurfaceSide
{
	BOTTOM = 0,
	LEFT,
	TOP,
	RIGHT
};

class BlockSet
{
private:
	void clear();
	void initAnimationComponent();
	void initSound(StateData* stateData);

	StateData* stateData;

	float gridWidth;
	float gridHeight;
	int layers;
	sf::Vector2i maxSizeWorldGrid;
	std::string textureFile;
	std::string fileName;
	std::vector< std::vector<Block*> >   blocks;
	std::vector<Powers*> powers;
	sf::Texture tileTextureSheet;
	sf::VideoMode vm;
	sf::FloatRect overlap;
	//ANIMATION VARIABLES
	sf::Texture destTexture;
	sf::Sprite destSprite;
	sf::IntRect currentRect;
	sf::IntRect startRect;
	float timer;
	float animation_timer;
	bool doAnimation;
	///////////////////////

	unsigned short type;
	int layer;
	int fromX;
	int toX;
	int fromY;
	int toY;
	//SOUND
	sf::SoundBuffer buffer;
	sf::Sound collisionSound;

	b2World* world;

public:
	BlockSet(StateData* stateData, float gridWidth, float gridHeight, int width, int height, std::string textureFile);
	BlockSet(StateData* stateData, b2World* world,  const std::string fileName, float gridWidth, float gridHeight);
	virtual ~BlockSet();

	//ACCESSORS
	const sf::Texture* getBlockSheet() const;

	const sf::Vector2i& getMaxSizeGrid() const;
	std::string getFileName();
	bool checkEndGame();
	//FUNCTIONS

	void collisionBallBlock(StateData* stateData, std::vector<Ball*> balls, Paddle* paddle, const float& dt);
	void startSuperPower(StateData* stateData, Paddle* paddle, std::vector<Ball*>* balls);
	void addBlock(const int x, const int y, int lifes, const sf::IntRect& textureRect, const short& type);
	void removeBlock(const int x, const int y);

	void saveToFile(const std::string fileName);
	void loadFromFile(const std::string fileName, b2World* world = nullptr);

	void updateSuperPower(StateData* stateData, const float& dt);
	void update();
	void render(sf::RenderTarget& target);
};

