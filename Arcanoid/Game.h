#pragma once
#include "GraphicsSettings.h"
#include "MainMenuState.h"

class Game
{
private:
	//VARIABLES
	GraphicsSettings gfxSettings;
	StateData stateData;

	sf::RenderWindow* window;
	sf::Event event;
	sf::ContextSettings windowSettings;
	std::vector<sf::VideoMode> videoModes;
	std::stack<State*> states;

	sf::Clock clock;
	float dt;
	sf::Vector2u prevSize;

	bool fullScreen;

	std::map<std::string, int> supportedKeys;

	//PRIVATE FUNCTIONS
	void initVariables();
	void initKeys();
	void initStateData();
	void initStates();
	void initWindow();
	void initGraphicsSettings();
public:
	//CONSTRUCTOR AND DESTRUCTOR
	Game();
	virtual ~Game();

	//FUNCTIONS
	void run();
	void pollEvents();
	void updateDT();
	void update();
	void render();
};

