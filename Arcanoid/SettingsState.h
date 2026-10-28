#pragma once
#include "Ball.h"
#include "Gui.h"
#include "State.h"
#include "stdafx.h"
class SettingsState : public State
{
  private:
    sf::Texture ballTextures[3];
    sf::Texture paddleTexture;
    sf::IntRect paddleTextureRect[7];
    sf::Texture bg;
    sf::IntRect bgTextureRect[5];

    sf::Font font;
    sf::Texture backgroundTexture;
    sf::RectangleShape background;

    std::map<std::string, std::unique_ptr<gui::Button>> buttons;
    std::map<std::string, std::unique_ptr<gui::DropDownList>> resolution;
    std::map<std::string, std::unique_ptr<gui::DropDownList>> ballList;
    std::map<std::string, std::unique_ptr<gui::DropDownList>> paddleList;
    std::map<std::string, std::unique_ptr<gui::DropDownList>> bgList;

    std::unique_ptr<gui::Slider> musicSlider;
    std::unique_ptr<gui::Slider> endSoundSlider;
    std::unique_ptr<gui::Slider> collisionSoundSlider;

    sf::Text optionsText;
    sf::Text resolutionText;
    sf::Text ballIconText;
    sf::Text paddleIconText;
    sf::Text bgIconText;

    std::vector<sf::VideoMode> modes;

    void initVariables();
    void initKeyBinds();
    void initTextures();
    void initFonts();
    void initTexts();
    void initGui(StateData* stateData);

  public:
    SettingsState(StateData* stateData);
    ~SettingsState() override;

    void resetGui();
    void updateGui(float dt);
    void updateInput(float dt) override;
    void update(float dt) override;
    void renderGui(sf::RenderTarget& target);
    void renderText(sf::RenderTarget& target);
    void render(sf::RenderTarget* target = nullptr) override;
};
