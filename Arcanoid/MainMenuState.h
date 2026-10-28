#pragma once
#include "EditorState.h"
#include "GameState.h"
#include "Gui.h"
#include "LevelState.h"
#include "SettingsState.h"
#include "State.h"
#include "stdafx.h"
class MainMenuState : public State
{
  private:
    sf::RectangleShape background;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Font font;

    std::map<std::string, std::unique_ptr<gui::Button>> buttons;
    sf::VideoMode vm;

    sf::Texture muteTexture;
    sf::Texture unmuteTexture;
    sf::Sprite mute;
    sf::Sprite unmute;

    void initMute();
    void initVariables();
    void initKeyBinds();
    void initFont();
    void initGUI();

  public:
    MainMenuState(StateData* stateData);
    ~MainMenuState() override;

    void resetGUI();
    void updateButtons();
    void updateInput(float dt) override;
    void updateMute();
    void update(float dt) override;
    void renderButtons(sf::RenderTarget& target);
    void render(sf::RenderTarget* target = nullptr) override;
};
