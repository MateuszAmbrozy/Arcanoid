#include "stdafx.h"
#include "Timer.h"

//PRIVATE FUNCTIONS
void Timer::initFont()
{
	if (!this->font.loadFromFile("Fonts/Arialn.ttf"))
		std::cout << "ERROR::MAINMENUSTATE::INITFONTS::Could not load font!\n";
}

//CONSTRUCTOR AND DESTRUCTOR
Timer::Timer()
{
	this->start_ = 0;
	this->end_ = 0;


	this->initFont();

	this->text.setFont(font);
	this->text.setFillColor(sf::Color::White);
	this->text.setCharacterSize(25);
	this->text.setString("Time: 0 sec.");
	this->text.setPosition(100.f, 100.f);
}
Timer::~Timer()
{
}

//MODIFIERS
void Timer::setPosition(sf::Vector2f position)
{
	this->text.setPosition(position);
}
void Timer::setString(std::string str)
{
	this->text.setString(str);
}
void Timer::setCharacterSize(unsigned int charSize)
{
	this->text.setCharacterSize(charSize);
}

//ACCESSORS
int Timer::getElapsedTime()
{
	return int(clock() - start_) / CLOCKS_PER_SEC;
}

//FUNCTIONS
void Timer::start()
{
	start_ = clock();
}
void Timer::reset()
{
	start_ = clock();
}
void Timer::stop()
{
	end_ = clock();
}
void Timer::resume()
{
	start_ = clock() - (end_ - start_);
}
void Timer::update(const float& dt)
{
	this->text.setString("Time: " + std::to_string(this->getElapsedTime()) + " sec");
}
void Timer::render(sf::RenderTarget& target)
{
	target.draw(this->text);
}
