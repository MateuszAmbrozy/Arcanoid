#pragma once
#include "Gui.h"
#include "Timer.h"
#include "stdafx.h"
enum class EndGameResult
{
    Loss,
    Victory
};

class EndGame
{
  private:
    sf::RectangleShape background;
    std::map<std::string, std::unique_ptr<gui::Button>> buttons;
    sf::Text text;
    sf::Font font;
    float resizeTimer;
    float resizeTimerMax;
    bool grow;
    sf::VideoMode vm;

    void initFont();

  public:
    EndGame(sf::VideoMode& vm, std::string key1, std::string text,
            EndGameResult result = EndGameResult::Loss);
    EndGame(sf::VideoMode& vm, std::string key1, std::string key2, std::string text);
    ~EndGame();

    bool isButtonPressed(std::string key);

    void updateText(float dt);
    void update(StateData* stateData, const sf::Vector2i& mousePos, float dt);
    void render(sf::RenderTarget* target);
};
