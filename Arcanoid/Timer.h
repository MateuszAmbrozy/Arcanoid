#pragma once
#include "stdafx.h"
class Timer
{
  private:
    float elapsed = 0.f;
    bool running = false;

    sf::Text menuText;
    sf::Font font;

    sf::Text text;

    void initFont();

  public:
    Timer();
    ~Timer();

    void setPosition(sf::Vector2f position);
    void setString(std::string str);
    void setCharacterSize(unsigned int charSize);
    int getElapsedTime() const;

    void start();
    void reset();
    void stop();
    void resume();

    void update(float dt);
    void render(sf::RenderTarget& target);
};
