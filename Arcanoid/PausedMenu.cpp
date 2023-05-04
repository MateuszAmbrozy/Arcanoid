#include "stdafx.h"
#include "PausedMenu.h"

//PRIVATE FUNCTIONS
void PausedMenu::initFont()
{
	if (!this->font.loadFromFile("Fonts/Arialn.ttf"))
		std::cout<<"ERROR::MAINMENUSTATE::INITFONTS::Could not load font!\n";

}

//CONSTRUTOR AND DESCTRUCTOR
PausedMenu::PausedMenu(sf::VideoMode& vm, sf::Vector2f* size)
{
	this->vm = vm;
	//init background
	this->background.setSize(
		sf::Vector2f(
			static_cast<float>(vm.width),
			static_cast<float>(vm.height)
		)
	);
	this->background.setFillColor(sf::Color(20, 20, 20, 100));

	//init container
	if (size)
	{
		this->container.setSize(sf::Vector2f(size->x , size->y));
	}
	else
	{
		this->container.setSize(
			sf::Vector2f(
				static_cast<float>(vm.width) / 4.f,
				static_cast<float>(vm.height) - 100.f
			)
		);
	}

	this->container.setFillColor(sf::Color(20, 20, 20, 200));

	this->container.setPosition(vm.width / 2.f - this->container.getSize().x / 2.f, 30.f);


	//Init Paused Text
	this->initFont();
	this->menuText.setFont(this->font);
	this->menuText.setFillColor(sf::Color(255, 255, 255, 200));
	this->menuText.setCharacterSize(gui::callCharSize(this->vm, 80));
	this->menuText.setString("PAUSED");
	this->menuText.setPosition
	(
		this->container.getPosition().x + this->container.getSize().x / 2.f - this->menuText.getGlobalBounds().width / 2.f,
		this->container.getPosition().y + gui::p2pY(1.85f, this->vm)
	);



}
PausedMenu::~PausedMenu()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it) //++it faster than it++
	{
		delete it->second;
	}
}

//ACCESSORS
std::map<std::string, gui::Button*>& PausedMenu::getButtons()
{
	return this->buttons;
}

//FUNCTIONS
const bool PausedMenu::isButtonPressed(std::string key)
{
	return this->buttons[key]->isPressed();
}
void PausedMenu::addButton(
	const std::string key,
	const float y,
	const float width,
	const float height,
	const unsigned charSize,
	sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
	sf::Color idle_color, sf::Color hover_color, sf::Color active_color,
	const std::string text)
{
	float x = this->container.getPosition().x + this->container.getSize().x / 2.f - width / 2.f;

	this->buttons[key] = new gui::Button(
		x, y, width, height,
		&this->font, text, charSize,
		text_idle_color, text_hover_color, text_active_color,
		idle_color, hover_color, active_color);

}
void PausedMenu::update(StateData* stateData, const sf::Vector2i& mousePos)
{
	for (auto& i : this->buttons)
	{
		i.second->update(stateData, mousePos);
	}
}
void PausedMenu::render(sf::RenderTarget& target)
{
	target.draw(this->background);
	target.draw(this->container);

	for (auto& i : this->buttons)
	{
		i.second->render(target);
	}
	target.draw(this->menuText);
}
