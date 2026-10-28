#pragma once
#include "stdafx.h"
class GraphicsSettings
{
  public:
    std::string title;
    sf::VideoMode resolution;
    bool fullScreen;
    bool verticalSync;
    unsigned frameRateLimit;
    sf::ContextSettings contextSettings;
    std::vector<sf::VideoMode> videoModes;

    GraphicsSettings();

    void saveToFile(const std::string path);
    void loadFromFile(const std::string path);
};
