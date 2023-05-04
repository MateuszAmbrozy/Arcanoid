#pragma once
#include "State.h"
#include "Gui.h"
#include "GameState.h"
#include "EditorState.h"
class LevelState :
    public State
{
private:
    //VARIABLES
    sf::Vector2f buttonSize;
    std::string filename;

    int whichState;

    sf::Font font;

    sf::Text text;
    std::map <std::string, gui::Button*> buttons;
    sf::VideoMode vm;


    sf::RectangleShape chosenLevel;

    //background
    sf::Texture bgTexture;
    sf::RectangleShape background;

    //PRIVATE FUNCTIONS
    void initVariables();
    void initChosenLevelRectangle();
    void initKeyBinds();
    void initButtons();
    void initFont();
    void initText(StateData* stateData);
    void initBackground();
    
    
public:
    //CONSTRUCTOR AND DESTRUCTOR
    LevelState(StateData* stateData, int whichState); // 0-gamestate, 1-editorstate
    virtual ~LevelState();


    //FUNCSTIONS
    void updateInput(const float& dt);
    void updateButtons();
    void update(const float& dt);
    void renderButtons(sf::RenderTarget& target);
    void render(sf::RenderTarget* target = nullptr);
};

