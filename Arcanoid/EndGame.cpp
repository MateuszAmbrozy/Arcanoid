#include "stdafx.h"
#include "EndGame.h"

//PRIVATE FUNCTIONS
void EndGame::initFont()
{
	if (!this->font.loadFromFile("Fonts/Arialn.ttf"))
		std::cout << ("ERROR::MAINMENUSTATE::INITFONTS::Could not load font!");
}

//CONSTRUCTORS AND DESTRUCTOR
EndGame::EndGame(sf::VideoMode& vm, std::string key1, std::string text) //LOSE
	:vm(vm)
{
	this->initFont();
	this->resizeTimer = 0.f;
	this->resizeTimerMax = 25.f;
	this->grow = false;
	//LOSE BACKGROUND
	this->background.setSize(sf::Vector2f(static_cast<float>(vm.width), static_cast<float>(vm.height)));
	this->background.setFillColor(sf::Color(150, 50, 50, 150));
	//LOSE TEXT
	this->text.setFont(this->font);
	this->text.setCharacterSize(gui::callCharSize(this->vm, 30));
	this->text.setString(text);
	this->text.setPosition
	(
		gui::p2pX(50.0f, vm),
		gui::p2pY(50.0f, vm)
		);
	this->text.setFillColor(sf::Color(240, 50, 50));
	
		//BACK BUTTON
		this->buttons[key1] = new gui::Button(
			gui::p2pX(45.f, vm), gui::p2pY(75.f, vm),
			gui::p2pX(10.4f, vm), gui::p2pY(9.26f, vm),
			&font, "BACK", gui::callCharSize(vm, 100),
			sf::Color::Black, sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
			sf::Color(200, 100, 100), sf::Color(200, 100, 100, 100), sf::Color(20, 20, 20, 50));
	
}
EndGame::EndGame(sf::VideoMode& vm, std::string key1,  std::string key2, std::string text) //WIN
	:vm(vm)
{
	this->initFont();
	this->resizeTimer = 0.f;
	this->resizeTimerMax = 25.f;
	this->grow = false;
	//LOSE BACKGROUND
	this->background.setSize(sf::Vector2f(static_cast<float>(vm.width), static_cast<float>(vm.height)));
	this->background.setFillColor(sf::Color(50, 150, 50, 150));

	//LOSE TEXT
	this->text.setFont(this->font);
	this->text.setCharacterSize(gui::callCharSize(this->vm, 30));
	this->text.setString(text);
	//this->text.setOrigin(this->text.getGlobalBounds().width / 2.f, this->text.getGlobalBounds().height / 2.f);
	this->text.setPosition
	(
		this->background.getGlobalBounds().left + this->background.getGlobalBounds().width / 2.f,
		gui::p2pY(50.0f, vm)
	);
	this->text.setFillColor(sf::Color(50, 250, 50));

	this->buttons[key2] = new gui::Button(
		gui::p2pX(75.2f, vm), gui::p2pY(75.f, vm),
		gui::p2pX(10.4f, vm), gui::p2pY(9.26f, vm),
		&font, "NEXT", gui::callCharSize(vm, 100),
		sf::Color::Black, sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(100, 100, 200), sf::Color(100, 100, 200, 100), sf::Color(20, 20, 20, 50));
	//BACK BUTTON
	this->buttons[key1] = new gui::Button(
		gui::p2pX(10.2f, vm), gui::p2pY(75.f, vm),
		gui::p2pX(10.4f, vm), gui::p2pY(9.26f, vm),
		&font, "BACK", gui::callCharSize(vm, 100),
		sf::Color::Black, sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(200, 100, 100), sf::Color(200, 100, 100, 100), sf::Color(20, 20, 20, 50));

	
}
EndGame::~EndGame()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it) //++it faster than it++
	{
		delete it->second;
	}
}

//FUNCTIONS
const bool EndGame::isButtonPressed(std::string key)
{
	return this->buttons[key]->isPressed();
}
void EndGame::updateText(const float& dt)
{
	this->text.setOrigin(this->text.getGlobalBounds().width / 2.f, this->text.getGlobalBounds().height / 2.f);

	if (this->resizeTimer > this->resizeTimerMax)
	{
		this->resizeTimer = 0.f;

		if (!this->grow)
		{
			this->text.setCharacterSize(this->text.getCharacterSize() - 1);
			if(this->text.getCharacterSize() <= 20)
				this->grow = true;
		}
		if (this->grow)
		{
			this->text.setCharacterSize(this->text.getCharacterSize() + 1);
			if (this->text.getCharacterSize() >= 100)
				this->grow = false;
		}
	}
	else
		this->resizeTimer += 750.f * dt;
}
void EndGame::update(StateData* stateData, const sf::Vector2i& mousePos, const float& dt)
{
	for (auto& i : this->buttons)
	{
		i.second->update(stateData, mousePos);
	}
	this->updateText(dt);
}
void EndGame::render(sf::RenderTarget* target)
{
	target->draw(this->background);
	for (auto& i : this->buttons)
	{
		i.second->render(*target);
	}
	target->draw(this->text);
}
