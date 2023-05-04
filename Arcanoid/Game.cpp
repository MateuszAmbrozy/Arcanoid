#include "stdafx.h"
#include "Game.h"

//PRIVATE FUNCTIONS
void Game::initGraphicsSettings()
{
	this->gfxSettings.loadFromFile("Config/graphics.ini");
}
void Game::initVariables()
{
	this->window = nullptr;
	this->fullScreen = false;
	this->dt = 0.f;

}
void Game::initWindow()
{
	//create a SFML window
	if (this->gfxSettings.fullScreen)
		this->window = new sf::RenderWindow
		(
			this->gfxSettings.resolution,
			this->gfxSettings.title,
			sf::Style::Fullscreen,
			windowSettings
		);
	else
		this->window = new sf::RenderWindow
		(
			this->gfxSettings.resolution,
			this->gfxSettings.title,
			sf::Style::Titlebar | sf::Style::Close,
			windowSettings
		);
	this->window->setFramerateLimit(this->gfxSettings.frameRateLimit);
	this->window->setVerticalSyncEnabled(this->gfxSettings.verticalSync);
	window->setMouseCursorGrabbed(true);
}
void Game::initKeys()
{
	std::fstream ifs("Config/supported_keys.ini");
	if (ifs.is_open())
	{
		std::string key = "";
		int key_value = 0;

		while (ifs >> key >> key_value)
		{
			this->supportedKeys[key] = key_value;
		}
	}
	ifs.close();
}
void Game::initStateData()
{
	this->stateData.gridHeight = 0.0462962962962963f * this->gfxSettings.resolution.height;
	this->stateData.gridWidth = 0.0520833333333333f * this->gfxSettings.resolution.width;
	this->stateData.window = this->window;
	this->stateData.gfxSettings = &this->gfxSettings;
	this->stateData.supportedKeys = &this->supportedKeys;
	this->stateData.states = &this->states;
	this->stateData.reset = 0;
	this->stateData.paddleTopRect = -1;
	this->stateData.bgLeftRect = -1;
	this->stateData.ballTexture;

	for (int i = 1; i < 16; i++)
		this->stateData.levels[i] = 0;
	this->stateData.levels[0] = 1;

	this->stateData.collisionVolume = 50.f;
	this->stateData.endVolume = 50.f;
	this->stateData.musicVolume = 50.f;
	this->stateData.inversePaddle = false;
}
void Game::initStates()
{
	this->states.push(new MainMenuState(&this->stateData));
}

//CONSTRUCTOR AND DESTRUCTOR
Game::Game()
{
	this->initVariables();
	this->initGraphicsSettings();
	
	this->initWindow();
	this->initKeys();
	this->initStateData();
	this->initStates();
	prevSize = window->getSize();
}
Game::~Game()
{
	delete this->window;
	while (!this->states.empty())
	{
		this->states.top()->endState();
		delete this->states.top();
		this->states.pop();
	}
}

//FUNCTIONS
void Game::run()
{
	while (this->window->isOpen())
	{
		this->pollEvents();
		this->update();
		this->render();
	}
}
void Game::pollEvents()
{
	while (window->pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:
				window->close();
			break;
				default:
			break;
		}
	}
	// Sprawdzenie, czy rozmiar okna siê zmieni³
	sf::Vector2u currSize = window->getSize();
	if (currSize != prevSize)
	{
		window->setMouseCursorGrabbed(true);
		prevSize = currSize;
	}

}
void Game::updateDT()
{
	this->dt = this->clock.restart().asSeconds();
}
void Game::update()
{
	this->updateDT();
	if (!this->states.empty())
	{
		this->states.top()->update(this->dt);
		if (this->states.top()->getQuit())
		{
			this->states.top()->endState();
			delete this->states.top();
			this->states.pop();
		}
	}
	else
	{
		this->window->close();
	}
}
void Game::render()
{
	this->window->clear();

	if (!this->states.empty())
		this->states.top()->render(this->window);

	this->window->display();
}
