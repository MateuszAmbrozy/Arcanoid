#include "Timer.h"
#include "Assets.h"
#include "stdafx.h"

void Timer::initFont()
{
    assets::load(this->font, "Fonts/Arialn.ttf");
}

Timer::Timer()
{

    this->initFont();

    this->text.setFont(font);
    this->text.setFillColor(sf::Color::White);
    this->text.setCharacterSize(25);
    this->text.setString("Time: 0 sec.");
    this->text.setPosition(100.f, 100.f);
}
Timer::~Timer() = default;

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

int Timer::getElapsedTime() const
{
    return static_cast<int>(elapsed);
}

void Timer::start()
{
    elapsed = 0.f;
    running = true;
}
void Timer::reset()
{
    elapsed = 0.f;
}
void Timer::stop()
{
    running = false;
}
void Timer::resume()
{
    running = true;
}
void Timer::update(float dt)
{
    if (running)
        elapsed += dt;
    this->text.setString("Time: " + std::to_string(this->getElapsedTime()) + " sec");
}
void Timer::render(sf::RenderTarget& target)
{
    target.draw(this->text);
}
