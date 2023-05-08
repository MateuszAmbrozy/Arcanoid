#pragma once
#include "State.h"
#include "Gui.h"
#include "Ball.h"
class SettingsState :
	public State
{
private:
	//VARIABLES
	sf::Texture ballTextures[3];//balls icons
	sf::Texture paddleTexture;
	sf::IntRect paddleTextureRect[7];
	sf::Texture bg;
	sf::IntRect bgTextureRect[5];

	sf::Font font;
	sf::Texture backgroundTexture;
	sf::RectangleShape background;

	std::map <std::string, gui::Button*> buttons;
	std::map <std::string, gui::DropDownList*> resolution;
	std::map <std::string, gui::DropDownList*> ballList;
	std::map <std::string, gui::DropDownList*> paddleList;
	std::map <std::string, gui::DropDownList*> bgList;

	gui::Slider* musicSlider;
	gui::Slider* endSoundSlider;
	gui::Slider* collisionSoundSlider;

	sf::Text optionsText;
	sf::Text resolutionText;
	sf::Text ballIconText;
	sf::Text paddleIconText;
	sf::Text bgIconText;

	std::vector<sf::VideoMode> modes;
	
	//PRIVATE FUNCTIONS
	void initVariables();
	void initKeyBinds();
	void initTextures();
	void initFonts();
	void initTexts();
	void initGui(StateData* stateData);
public:

	//CONSTRUCTOR AND DESTRUCTOR
	SettingsState(StateData* stateData);
	virtual ~SettingsState();

	//FUNCTIONS
	void resetGui();
	void updateGui(const float& dt);
	void updateInput(const float& dt);
	void update(const float& dt);
	void renderGui(sf::RenderTarget& target);
	void renderText(sf::RenderTarget& target);
	void render(sf::RenderTarget* target = nullptr);


};

