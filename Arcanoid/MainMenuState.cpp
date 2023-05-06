#include "stdafx.h"
#include "MainMenuState.h"


//PRIVATE FUNCTIONS
void MainMenuState::initMute()
{
	if (!this->muteTexture.loadFromFile("Resources/Sprites/mute.png"))
		std::cout << "ERROR::MAINMENU::INITMUTE::Could not load mute.png \n";
	this->mute.setTexture(this->muteTexture);

	if (!this->unmuteTexture.loadFromFile("Resources/Sprites/unmute.png"))
		std::cout << "ERROR::MAINMENU::INITMUTE::Could not load mute.png \n";
	this->unmute.setTexture(this->unmuteTexture);

}
void MainMenuState::initVariables()
{
	this->vm = this->stateData->gfxSettings->resolution;
}
void MainMenuState::initKeyBinds()
{
	std::fstream ifs("Config/mainmenu_keybinds.ini");
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
void MainMenuState::initFont()
{
	if (!this->font.loadFromFile("Fonts/Arialn.ttf"))
		std::cout<<"ERROR::MAINMENUSTATE::INITFONTS::Could not load font!";
}
void MainMenuState::initGUI()
{
	this->texture.loadFromFile("Resources/Background/bg1.png");
	this->sprite.setTexture(texture);
	this->background.setSize(sf::Vector2f(static_cast<float>(this->window->getSize().x), static_cast<float>(this->window->getSize().y)));
	this->background.setTexture(&this->texture);

	this->buttons["LEVEL_STATE_BTN"] = new gui::Button(gui::p2pX(50.05f, this->vm), gui::p2pY(60.8f, this->vm), gui::p2pX(20.02f, this->vm), gui::p2pY(8.6, this->vm),
		&this->font, "PLAY", gui::callCharSize(this->vm, 60),
		sf::Color::Black, sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(100, 200, 100), sf::Color(100, 200, 100, 100), sf::Color(20, 20, 20, 50));

	this->buttons["CONFIG_STATE_BTN"] = new gui::Button(gui::p2pX(75.05f, this->vm), gui::p2pY(60.8f, this->vm), gui::p2pX(20.02f, this->vm), gui::p2pY(8.6, this->vm),
		&this->font, "CONFIG", gui::callCharSize(this->vm, 60),
		sf::Color::Black, sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(200, 200, 100), sf::Color(200, 200, 100, 100), sf::Color(20, 20, 20, 50));

	this->buttons["EDITOR_STATE_BTN"] = new gui::Button(gui::p2pX(50.05f, this->vm), gui::p2pY(75.8f, this->vm), gui::p2pX(20.02f, this->vm), gui::p2pY(8.6, this->vm),
		&this->font, "EDITOR", gui::callCharSize(this->vm, 60),
		sf::Color::Black, sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(100, 100, 200), sf::Color(100, 100, 200, 100), sf::Color(20, 20, 20, 50));

	this->buttons["EXIT_STATE_BTN"] = new gui::Button(gui::p2pX(75.05f, this->vm), gui::p2pY(75.8f, this->vm), gui::p2pX(20.02f, this->vm), gui::p2pY(8.6, this->vm),
		&this->font, "QUIT", gui::callCharSize(this->vm, 60),
		sf::Color::Black, sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(200, 100, 100), sf::Color(200, 100, 100, 100), sf::Color(20, 20, 20, 50));


//MUTE / UNMUTE
	this->mute.setScale(
		gui::p2pX(5.f, this->stateData->gfxSettings->resolution) / this->mute.getLocalBounds().width,
		gui::p2pX(5.f, this->stateData->gfxSettings->resolution) / this->mute.getLocalBounds().width
	);
	this->unmute.setScale(
		gui::p2pX(5.f, this->stateData->gfxSettings->resolution) / this->mute.getLocalBounds().width,
		gui::p2pX(5.f, this->stateData->gfxSettings->resolution) / this->mute.getLocalBounds().width
	);
	this->mute.setPosition(
		this->stateData->gfxSettings->resolution.width - this->mute.getGlobalBounds().width * 1.5f,
		this->stateData->gfxSettings->resolution.height - this->mute.getGlobalBounds().height);
	this->unmute.setPosition(
		this->stateData->gfxSettings->resolution.width - this->unmute.getGlobalBounds().width * 1.5f,
		this->stateData->gfxSettings->resolution.height - this->unmute.getGlobalBounds().height);
}

//CONSTRUCTOR AND DESTRUCTOR
MainMenuState::MainMenuState(StateData* stateData)
	:State(stateData)
{
	this->initMute();
	this->initVariables();
	this->initKeyBinds();
	this->initFont();
	this->initGUI();
}
MainMenuState::~MainMenuState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it) //++it faster than it++
	{
		delete it->second;
	}
}

//FUNCTIONS
void MainMenuState::resetGUI()
{
	/*
	 * Clears the GUI elements and re-initialises the GUI.
	 *
	 * @return void
	 */

	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}
	this->buttons.clear();
	this->vm = this->stateData->gfxSettings->resolution;
	this->stateData->reset = 0;
	this->initGUI();
}
void MainMenuState::updateButtons()
{
	for (auto& it : this->buttons)
	{
		it.second->update(this->stateData, this->mousePosWindow);
	}
	if (this->buttons["LEVEL_STATE_BTN"]->isPressed())
	{
		this->states->push(new LevelState(this->stateData, 0));//0-gamestate
	}

	if (this->buttons["EDITOR_STATE_BTN"]->isPressed())
	{
		this->states->push(new LevelState(this->stateData, 1));//1-editorState
	}
	if (this->buttons["CONFIG_STATE_BTN"]->isPressed())
	{
		this->states->push(new SettingsState(this->stateData));
	}
	//QUIT THE GAME
	if (this->buttons["EXIT_STATE_BTN"]->isPressed())
	{
		this->endState();
	}
}
void MainMenuState::updateInput(const float& dt)
{
}
void MainMenuState::updateMute()
{
	if(this->mute.getGlobalBounds().contains(sf::Vector2f(this->mousePosWindow)) ||
		this->unmute.getGlobalBounds().contains(sf::Vector2f(this->mousePosWindow)))
	{
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->getKeyTime())
		{
			if (this->stateData->mute)
			{
				this->stateData->mute = false;

			}
			else if (!this->stateData->mute)
			{
				this->stateData->mute = true;
			}
		}
	}
	
}
void MainMenuState::update(const float& dt)
{
	if (this->stateData->reset)
	{
		this->resetGUI();
	}
	this->updateMute();
	this->updateMousePosition();
	this->updateInput(dt);
	this->updateButtons();
}
void MainMenuState::renderButtons(sf::RenderTarget& target)
{
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}
}
void MainMenuState::render(sf::RenderTarget* target)
{

	target->draw(this->background);

	if (this->stateData->mute)
		target->draw(this->mute);
	else if (!this->stateData->mute)
		target->draw(this->unmute);

	this->renderButtons(*target);
}
