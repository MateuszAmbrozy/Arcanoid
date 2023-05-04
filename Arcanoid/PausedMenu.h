#pragma once
#include "Gui.h"
#include "State.h"
class PausedMenu
{
private:

	sf::Text menuText;
	sf::Font font;
	sf::RectangleShape background;
	sf::RectangleShape container;
	std::map<std::string, gui::Button*> buttons;
	sf::VideoMode vm;

	//INITIALIZATIONS
	void initFont();

public:
	PausedMenu(sf::VideoMode& vm, sf::Vector2f* size = nullptr);
	virtual ~PausedMenu();

	//ACCESSOR

	std::map<std::string, gui::Button*>& getButtons();

	//FUNCTIONS
	const bool isButtonPressed(const std::string key);
	void addButton(const std::string key,
		const float y,
		const float width,
		const float height,
		const unsigned charSize,
		sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
		sf::Color idle_color, sf::Color hover_color, sf::Color active_color,
		const std::string text);
	void update(StateData* stateData, const sf::Vector2i& mousePos);
	void render(sf::RenderTarget& target);

};
