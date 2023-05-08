#pragma once
#include "GraphicsSettings.h"

class State;

class StateData
{
public:
	//VARIABLES
	float gridWidth = 0;
	float gridHeight = 0;
	GraphicsSettings* gfxSettings = nullptr;
	sf::RenderWindow* window = nullptr;
	std::map < std::string, int >* supportedKeys = nullptr;
	std::stack<State*>* states = nullptr;

	bool reset = false; //reset for mainMenu

	sf::Texture ballTexture;
	int paddleTopRect = 0;
	int bgLeftRect = 0;
	int levels[16];
	bool refreshLevelState = 0;
	
	//Music
	bool mute;
	float musicVolume = 0;
	float collisionVolume = 0;
	float endVolume = 0;

	bool inversePaddle = false;
	sf::Clock clock;
};

class State
{
protected:
	float gridWidth;
	float gridHeight;
	StateData* stateData;
	std::stack<State*>* states;
	sf::RenderWindow* window;
	std::map<std::string, sf::Texture> textures;
	bool paused;
	bool quit;

	sf::Vector2i mousePosWindow;
	sf::Vector2i mousePosGrid;

	std::map<std::string, int>* supportedKeys;
	std::map<std::string, int> keyBinds;

	sf::Clock keyTimer;
	float keyTimaMax;

public:
	//CONSTRUCTOR AND DESTRUCTOR
	State(StateData* stateData);
	virtual ~State();

	//MODIFIERS
	void pauseState();
	void unpauseState();

	//FUNCTIONS	
	virtual void endState();
	virtual void checkForQuit();
	virtual void updateMousePosition();

	const bool& getQuit() const;
	const bool getKeyTime();

	virtual void updateInput(const float& dt) = 0;

	virtual void update(const float& dt) = 0;
	virtual void render(sf::RenderTarget* target = nullptr) = 0;
};

