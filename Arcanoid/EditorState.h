#pragma once
#include "State.h"
#include "Gui.h"
#include "BlockSet.h"
#include "PausedMenu.h"
class EditorState :
    public State
{
private:
    //VARIABLES
    sf::Font font;
    sf::Text cursorText;

    std::map <std::string, gui::Button*> buttons;
    BlockSet* blockSet;
    PausedMenu* pausedMenu;
    gui::TextureSelector* textureSelector;

    sf::RectangleShape selectorRect;
    sf::IntRect textureRect;
    sf::VideoMode vm;

    std::string filename;
    short type;
    int lifes;

    //Background
    sf::Texture bgTexture;
    sf::RectangleShape background;
    sf::IntRect bgTextureRect;

    //PRIVATE FUNCTIONS
    void initVariables();
    void initKeyBinds();
    void initFonts();
    void initText();
    void initPausedMenu();
    void initBlockSet();
    void initGUI();
    void initBackground();
public:
    //CONSTRUCTOR AND DESTRUCTOR
    EditorState(StateData* stateData, std::string filename);
    virtual ~EditorState();

    //FUNCSTIONS
    void updateButtons();
    void updateEditorInput(const float& dt);
    void updatePausedMenuButtons();
    void updateInput(const float& dt);
    void updateGUI();
    void update(const float& dt);
    void renderButtons(sf::RenderTarget& target);
    void renderGUI(sf::RenderTarget& target);
    void render(sf::RenderTarget* target = NULL);
};

