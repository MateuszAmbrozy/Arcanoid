#include "stdafx.h"
#include "State.h"

//CONSTRUCTOR AND DESTRUCTOR
State::State(StateData* stateData)
{
	this->gridWidth = stateData->gridWidth;
	this->gridHeight = stateData->gridHeight;

	this->stateData = stateData;
	this->window = stateData->window;
	this->supportedKeys = stateData->supportedKeys;
	this->states = stateData->states;
	this->paused = false;
	this->quit = false;

	this->keyTimaMax = 0.5f;
	this->keyTimer.restart();

}
State::~State()
{
}

//MODIFIERS
void State::pauseState()
{
	this->paused = true;
}
void State::unpauseState()
{
	this->paused = false;
}
void State::endState()
{
	this->quit = true;
}

//FUNCTIONS
void State::checkForQuit()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keyBinds.at("CLOSE"))))
		this->quit = true;
}
void State::updateMousePosition()
{
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosGrid =
		sf::Vector2i(
			static_cast<int>(this->mousePosWindow.x) / static_cast<int>(this->gridWidth),
			static_cast<int>(this->mousePosWindow.y) / static_cast<int>(this->gridHeight)
		);
}
const bool& State::getQuit() const
{
	return this->quit;
}
const bool State::getKeyTime()
{
	if (this->keyTimer.getElapsedTime().asSeconds() >= this->keyTimaMax)
	{
		this->keyTimer.restart();
		return true;
	}
	else
		return false;
}

