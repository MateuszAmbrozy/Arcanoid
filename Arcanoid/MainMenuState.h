#pragma once
#include "State.h"
#include "Gui.h"
#include "GameState.h"
#include "LevelState.h"
#include "EditorState.h"
#include "SettingsState.h"
class MainMenuState :
    public State
{
private:
    //VARIABLES
    sf::RectangleShape background;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Font font;

    std::map <std::string, gui::Button*> buttons;
    sf::VideoMode vm;

    sf::Texture muteTexture;
    sf::Texture unmuteTexture;
    sf::Sprite mute;
    sf::Sprite unmute;

    //PRIVATE FUNCTIONS
    void initMute();
    void initVariables();
    void initKeyBinds();
    void initFont();
    void initGUI();
public:
    //CONSTRUCTOR AND DESTRUCTOR
    MainMenuState(StateData* stateData);
    virtual ~MainMenuState();

    //FUNCSTIONS
    void resetGUI();
    void updateButtons();
    void updateMute();
    void update(const float& dt);
    void renderButtons(sf::RenderTarget& target);
    void render(sf::RenderTarget* target = nullptr);
};

