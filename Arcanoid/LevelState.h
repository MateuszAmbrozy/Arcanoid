#pragma once
#include "EditorState.h"
#include "GameState.h"
#include "Gui.h"
#include "State.h"
#include "stdafx.h"
class LevelState : public State
{
  private:
    sf::Vector2f buttonSize;
    std::string filename;

    int whichState;

    sf::Font font;

    sf::Text text;
    std::map<std::string, std::unique_ptr<gui::Button>> buttons;
    sf::VideoMode vm;

    sf::RectangleShape chosenLevel;

    sf::Texture bgTexture;
    sf::RectangleShape background;

    void initVariables();
    void initChosenLevelRectangle();
    void initKeyBinds();
    void initButtons();
    void initFont();
    void initText(StateData* stateData);
    void initBackground();

  public:
    LevelState(StateData* stateData, int whichState);
    ~LevelState() override;

    void updateInput(float dt) override;
    void updateButtons();
    void update(float dt) override;
    void renderButtons(sf::RenderTarget& target);
    void render(sf::RenderTarget* target = nullptr) override;
};
