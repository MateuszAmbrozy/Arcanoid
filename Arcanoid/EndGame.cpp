#include "EndGame.h"
#include "Assets.h"
#include "stdafx.h"

void EndGame::initFont()
{
    assets::load(this->font, "Fonts/Arialn.ttf");
}

EndGame::EndGame(sf::VideoMode& vm, std::string key1, std::string text, EndGameResult result) : vm(vm)
{
    this->initFont();
    this->resizeTimer = 0.f;
    this->resizeTimerMax = 25.f;
    this->grow = false;
    this->background.setSize(sf::Vector2f(static_cast<float>(vm.width), static_cast<float>(vm.height)));
    this->background.setFillColor(result == EndGameResult::Victory ? sf::Color(50, 150, 50, 150)
                                                                   : sf::Color(150, 50, 50, 150));
    this->text.setFont(this->font);
    this->text.setCharacterSize(gui::callCharSize(this->vm, 30));
    this->text.setString(text);
    this->text.setPosition(gui::p2pX(50.0f, vm), gui::p2pY(50.0f, vm));
    this->text.setFillColor(result == EndGameResult::Victory ? sf::Color(50, 250, 50)
                                                             : sf::Color(240, 50, 50));

    this->buttons[key1] = std::make_unique<gui::Button>(
        gui::p2pX(45.f, vm), gui::p2pY(75.f, vm), gui::p2pX(10.4f, vm), gui::p2pY(9.26f, vm), &font, "BACK",
        gui::callCharSize(vm, 100), sf::Color::Black, sf::Color(250, 250, 250, 250),
        sf::Color(20, 20, 20, 50), sf::Color(200, 100, 100), sf::Color(200, 100, 100, 100),
        sf::Color(20, 20, 20, 50));
}
EndGame::EndGame(sf::VideoMode& vm, std::string key1, std::string key2, std::string text) : vm(vm)
{
    this->initFont();
    this->resizeTimer = 0.f;
    this->resizeTimerMax = 25.f;
    this->grow = false;
    this->background.setSize(sf::Vector2f(static_cast<float>(vm.width), static_cast<float>(vm.height)));
    this->background.setFillColor(sf::Color(50, 150, 50, 150));

    this->text.setFont(this->font);
    this->text.setCharacterSize(gui::callCharSize(this->vm, 30));
    this->text.setString(text);
    this->text.setPosition(this->background.getGlobalBounds().left +
                               this->background.getGlobalBounds().width / 2.f,
                           gui::p2pY(50.0f, vm));
    this->text.setFillColor(sf::Color(50, 250, 50));

    this->buttons[key2] = std::make_unique<gui::Button>(
        gui::p2pX(75.2f, vm), gui::p2pY(75.f, vm), gui::p2pX(10.4f, vm), gui::p2pY(9.26f, vm), &font, "NEXT",
        gui::callCharSize(vm, 100), sf::Color::Black, sf::Color(250, 250, 250, 250),
        sf::Color(20, 20, 20, 50), sf::Color(100, 100, 200), sf::Color(100, 100, 200, 100),
        sf::Color(20, 20, 20, 50));
    this->buttons[key1] = std::make_unique<gui::Button>(
        gui::p2pX(10.2f, vm), gui::p2pY(75.f, vm), gui::p2pX(10.4f, vm), gui::p2pY(9.26f, vm), &font, "BACK",
        gui::callCharSize(vm, 100), sf::Color::Black, sf::Color(250, 250, 250, 250),
        sf::Color(20, 20, 20, 50), sf::Color(200, 100, 100), sf::Color(200, 100, 100, 100),
        sf::Color(20, 20, 20, 50));
}
EndGame::~EndGame() = default;

bool EndGame::isButtonPressed(std::string key)
{
    const auto it = buttons.find(key);
    return it != buttons.end() && it->second->isPressed();
}
void EndGame::updateText(float dt)
{
    this->text.setOrigin(this->text.getGlobalBounds().width / 2.f, this->text.getGlobalBounds().height / 2.f);

    if (this->resizeTimer > this->resizeTimerMax)
    {
        this->resizeTimer = 0.f;

        if (!this->grow)
        {
            this->text.setCharacterSize(this->text.getCharacterSize() - 1);
            if (this->text.getCharacterSize() <= 20)
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
void EndGame::update(StateData* stateData, const sf::Vector2i& mousePos, float dt)
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
