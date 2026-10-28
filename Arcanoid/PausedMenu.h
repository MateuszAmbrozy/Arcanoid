#pragma once
#include "Gui.h"
#include "State.h"
#include "stdafx.h"
class PausedMenu
{
  private:
    sf::Text menuText;
    sf::Font font;
    sf::RectangleShape background;
    sf::RectangleShape container;
    std::map<std::string, std::unique_ptr<gui::Button>> buttons;
    sf::VideoMode vm;

    void initFont();

  public:
    PausedMenu(sf::VideoMode& vm, sf::Vector2f* size = nullptr);
    ~PausedMenu();

    std::map<std::string, std::unique_ptr<gui::Button>>& getButtons();

    bool isButtonPressed(const std::string key);
    void addButton(const std::string key, const float y, const float width, const float height,
                   const unsigned charSize, sf::Color text_idle_color, sf::Color text_hover_color,
                   sf::Color text_active_color, sf::Color idle_color, sf::Color hover_color,
                   sf::Color active_color, const std::string text);
    void update(StateData* stateData, const sf::Vector2i& mousePos);
    void render(sf::RenderTarget& target);
};
