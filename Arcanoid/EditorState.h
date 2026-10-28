#pragma once
#include "BlockSet.h"
#include "Gui.h"
#include "PausedMenu.h"
#include "State.h"
#include "stdafx.h"
class EditorState : public State
{
  private:
    sf::Font font;
    sf::Text cursorText;

    std::map<std::string, std::unique_ptr<gui::Button>> buttons;
    std::unique_ptr<BlockSet> blockSet;
    std::unique_ptr<PausedMenu> pausedMenu;
    std::unique_ptr<gui::TextureSelector> textureSelector;

    sf::RectangleShape selectorRect;
    sf::IntRect textureRect;
    sf::VideoMode vm;

    std::string filename;
    short type;
    int lives;

    sf::Texture bgTexture;
    sf::RectangleShape background;
    sf::IntRect bgTextureRect;

    void initVariables();
    void initKeyBinds();
    void initFonts();
    void initText();
    void initPausedMenu();
    void initBlockSet();
    void initGUI();
    void initBackground();

  public:
    EditorState(StateData* stateData, std::string filename);
    ~EditorState() override;

    void updateButtons();
    void updateEditorInput(float dt);
    void updatePausedMenuButtons();
    void updateInput(float dt) override;
    void updateGUI();
    void update(float dt) override;
    void renderButtons(sf::RenderTarget& target);
    void renderGUI(sf::RenderTarget& target);
    void render(sf::RenderTarget* target = nullptr) override;
};
