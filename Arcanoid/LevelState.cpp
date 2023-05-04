#include "stdafx.h"
#include "LevelState.h"

//PRIVATE FUNCTIONS
void LevelState::initVariables()
{
	this->filename = "1.slmp";
	this->vm = this->stateData->gfxSettings->resolution;
}
void LevelState::initChosenLevelRectangle()
{
	this->buttonSize = sf::Vector2f(gui::p2pX(5.15f, this->vm), gui::p2pX(5.15f, this->vm));
	this->chosenLevel.setFillColor(sf::Color::Transparent);
	this->chosenLevel.setOutlineColor(sf::Color::Green);
	this->chosenLevel.setOutlineThickness(5.f);
	this->chosenLevel.setSize(this->buttonSize);
	this->chosenLevel.setPosition(gui::p2pX(18.3f, this->vm), gui::p2pY(9.25f, this->vm));
}
void LevelState::initKeyBinds()
{
	std::fstream ifs("Config/levelstate_keybinds.ini");
	if (ifs.is_open())
	{
		std::string key = "";
		std::string key2 = "";

		while (ifs >> key >> key2)
		{
			this->keyBinds[key] = this->supportedKeys->at(key2);
		}
	}
	ifs.close();
}
void LevelState::initButtons()
{
	unsigned short levelCounter = 0;
	std::string str;
	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++)
		{
			levelCounter++;
			this->buttons[std::to_string(levelCounter)] = new gui::Button(x * gui::p2pX(18.2, this->vm) + gui::p2pX(18.2, this->vm), y * gui::p2pY(23.14f, this->vm) + gui::p2pY(9.25f, this->vm), gui::p2pX(5.25f, this->vm), gui::p2pX(5.25f, this->vm)/*gui::p2pY(9.25f, this->vm)*/,
				&this->font, std::to_string(levelCounter), gui::callCharSize(this->vm, 100),
				sf::Color::Black, sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
				sf::Color(100, 200, 100), sf::Color(100, 200, 100, 100), sf::Color(20, 20, 20, 50),
				sf::Color::Black, sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
				this->stateData->levels[levelCounter - 1]);
		}
	this->buttons["1"]->setId(1);
	


	if (this->whichState == 0)
	{
		str = "PLAY";
		for (int i = 0; i < 16; i++)
		{
			if (this->stateData->levels[i]==0)
			{
				this->buttons[std::to_string(i + 1)]->setColor(
					sf::Color::Black, sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
					sf::Color(150, 100, 100), sf::Color(200, 100, 100, 100), sf::Color(20, 20, 20, 50));
			}
		}
	}
	else if (this->whichState == 1)
		str = "EDIT";

	this->buttons["PUSH_STATE_BTN"] = new gui::Button(gui::p2pX(83.f, this->vm), gui::p2pY(83.f, this->vm), gui::p2pX(10.8f, this->vm), gui::p2pY(6.6f, this->vm),
		&this->font, str, gui::callCharSize(this->vm, 80),
		sf::Color::Black, sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(100, 100, 200), sf::Color(100, 100, 200, 100), sf::Color(20, 20, 20, 50));

	this->buttons["EXIT_STATE_BTN"] = new gui::Button(gui::p2pX(5.3f, this->vm), gui::p2pY(83.f, this->vm), gui::p2pX(10.8f, this->vm), gui::p2pY(6.6f, this->vm),
		&this->font, "BACK", gui::callCharSize(this->vm, 80),
		sf::Color::Black, sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(200, 100, 100), sf::Color(200, 100, 100, 100), sf::Color(20, 20, 20, 50));
}
void LevelState::initFont()
{
	if (!this->font.loadFromFile("Fonts/Arialn.ttf"))
		std::cout << ("ERROR::MAINMENUSTATE::INITFONTS::Could not load font!");
}
void LevelState::initText(StateData* stateData)
{
	if (this->whichState == 0)
		this->text.setString("CHOSE LEVEL TO PLAY");
	else if (this->whichState == 1)
		this->text.setString("CHOSE LEVEL TO EDIT");

	this->text.setFont(this->font);
	this->text.setCharacterSize(gui::callCharSize(this->vm, 50));
	this->text.setPosition(stateData->gfxSettings->resolution.width / 2.f - this->text.getGlobalBounds().width / 2.f, 10.f);

}
void LevelState::initBackground()
{
	if (!this->bgTexture.loadFromFile("Resources/Background/level.png"))
		std::cout << "ERROR::GAMESTATE::INITBACKGROUND::Could not load level.png \n";

	this->background.setSize(sf::Vector2f(static_cast<float>(vm.width), static_cast<float>(vm.height)));
	this->background.setTexture(&this->bgTexture);
}

//CONSTRUCTOR AND DESTRUCTOR
LevelState::LevelState(StateData* stateData, int whichState)
	:State(stateData), whichState(whichState)
{
	this->initVariables();
	this->initChosenLevelRectangle();
	this->initKeyBinds();
	this->initFont();
	this->initButtons();
	this->initText(stateData);
	this->initBackground();
}
LevelState::~LevelState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it) //++it faster than it++
	{
		delete it->second;
	}
}

//FUNCTIONS
void LevelState::updateInput(const float& dt)
{
	if (this->stateData->refreshLevelState)
	{
		for(int i=0; i< 16; i++)
		{
			this->buttons[std::to_string(i + 1)]->setId(this->stateData->levels[i]);

			if (this->stateData->levels[i] == 1)
			{
				this->buttons[std::to_string(i + 1)]->setColor(
					sf::Color::Black, sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
					sf::Color(100, 200, 100), sf::Color(100, 200, 100, 100), sf::Color(20, 20, 20, 50));
			}
		}
		this->stateData->refreshLevelState = false;

	}
}
void LevelState::updateButtons()
{
	for (auto& it : this->buttons)
	{
		it.second->update(this->stateData, this->mousePosWindow);
	}



	if (this->whichState == 0)
	{
		for (int i = 1; i <= 16; i++)
			if (this->buttons[std::to_string(i)]->isPressed() && this->getKeyTime() && this->buttons[std::to_string(i)]->getId() == 1)
			{
				this->filename = std::to_string(i) + ".slmp";
				this->chosenLevel.setPosition(this->buttons[std::to_string(i)]->getPosition());
			}
		if (this->buttons["PUSH_STATE_BTN"]->isPressed() && this->getKeyTime() )
		{
			this->states->push(new GameState(this->stateData, this->filename));
		}
	}
	else if (this->whichState == 1)
	{
		for (int i = 1; i <= 16; i++)
			if (this->buttons[std::to_string(i)]->isPressed() && this->getKeyTime())
			{
				this->filename = std::to_string(i) + ".slmp";
				this->chosenLevel.setPosition(this->buttons[std::to_string(i)]->getPosition());
			}
		if (this->buttons["PUSH_STATE_BTN"]->isPressed() && this->getKeyTime())
		{
			this->states->push(new EditorState(this->stateData, this->filename));
		}
	}

	//QUIT THE GAME
	if (this->buttons["EXIT_STATE_BTN"]->isPressed() && this->getKeyTime())
	{
		Sleep(100);
		this->endState();
	}
}
void LevelState::update(const float& dt)
{
	this->updateMousePosition();
	this->updateInput(dt);
	this->updateButtons();
}
void LevelState::renderButtons(sf::RenderTarget& target)
{
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}
}
void LevelState::render(sf::RenderTarget* target)
{
	target->draw(this->background);
	this->renderButtons(*target);
	target->draw(this->chosenLevel);

	target->draw(this->text);
}
