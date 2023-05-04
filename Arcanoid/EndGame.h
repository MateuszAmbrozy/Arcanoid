#pragma once
#include "Gui.h"
#include "Timer.h"
class EndGame
{
private:
	sf::RectangleShape background;
	std::map<std::string, gui::Button*> buttons;
	sf::Text text;
	sf::Font font;
	float resizeTimer;
    float resizeTimerMax;
	bool grow;
	sf::VideoMode vm;

	void initFont();

public:
	EndGame(sf::VideoMode& vm, std::string key1, std::string text);
	EndGame(sf::VideoMode& vm, std::string key1, std::string key2, std::string text);
	virtual ~EndGame();

	const bool isButtonPressed(std::string key);

	void updateText(const float& dt);
	void update(StateData* stateData, const sf::Vector2i& mousePos, const float& dt);
	void render(sf::RenderTarget* target);
};

