#include "stdafx.h"
#include "SettingsState.h"

//PRIVATE FUNCTIONS
void SettingsState::initVariables()
{
	this->modes = sf::VideoMode::getFullscreenModes();

}
void SettingsState::initKeyBinds()
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
void SettingsState::initTextures()
{
	this->ballTextures[0].loadFromFile("Resources/Sprites/ball.png");
	this->ballTextures[1].loadFromFile("Resources/Sprites/ball1.png");
	this->ballTextures[2].loadFromFile("Resources/Sprites/ball2.png");

	this->paddleTexture.loadFromFile("Resources/Sprites/iconPaddle.png");
	this->bg.loadFromFile("Resources/Background/bg3.png");
	for (int i = 0; i < 7; i++)
	{
		paddleTextureRect[i].left = 0;
		paddleTextureRect[i].top = i * 30;
		paddleTextureRect[i].width = 129;
		paddleTextureRect[i].height = 30;
	}
	for (int i = 0; i < 5; i++)
	{
		this->bgTextureRect[i].left = i * 200;
		this->bgTextureRect[i].top = 0;
		this->bgTextureRect[i].width = 200;
		this->bgTextureRect[i].height = 241;
	}
	//BACKGROUND
	this->backgroundTexture.loadFromFile("Resources/Background/settings.png");
	this->background.setSize(sf::Vector2f(static_cast<float>(this->window->getSize().x), static_cast<float>(this->window->getSize().y)));
	this->background.setTexture(&this->backgroundTexture);
}
void SettingsState::initFonts()
{
	if (!this->font.loadFromFile("Fonts/Arialn.ttf"))
		throw("ERROR::MAINMENUSTATE::INITFONTS::Could not load font!");

}
void SettingsState::initTexts()
{
	const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;
	//INIT OPTION TEXT
	this->optionsText.setFont(this->font);

	this->optionsText.setPosition(sf::Vector2f(gui::p2pX(49.f, vm), gui::p2pY(3.4f, vm)));

	this->optionsText.setCharacterSize(gui::callCharSize(vm, 50));
	this->optionsText.setFillColor(sf::Color::White);
	this->optionsText.setString("OPTIONS\n ");

	//INIT BALL ICON TEXT
	this->ballIconText.setFont(this->font);

	this->ballIconText.setPosition(sf::Vector2f(gui::p2pX(25.6f, vm), gui::p2pY(15.6f, vm)));

	this->ballIconText.setCharacterSize(gui::callCharSize(vm, 80));
	this->ballIconText.setFillColor(sf::Color::White);
	this->ballIconText.setString("BALLS\n ");

	//RESOLUTION
	this->resolutionText.setFont(this->font);

	this->resolutionText.setPosition(sf::Vector2f(gui::p2pX(10.6f, vm), gui::p2pY(15.6f, vm)));

	this->resolutionText.setCharacterSize(gui::callCharSize(vm, 90));
	this->resolutionText.setFillColor(sf::Color::White);
	this->resolutionText.setString("RESOLUTION\n ");

	//INIT PADDLE ICON TEXT
	this->paddleIconText.setFont(this->font);

	this->paddleIconText.setPosition(sf::Vector2f(gui::p2pX(40.6f, vm), gui::p2pY(15.6f, vm)));

	this->paddleIconText.setCharacterSize(gui::callCharSize(vm, 90));
	this->paddleIconText.setFillColor(sf::Color::White);
	this->paddleIconText.setString("PADDLE\n ");
	//INIT BACKGROUND ICON TEXT
	this->bgIconText.setFont(this->font);

	this->bgIconText.setPosition(sf::Vector2f(gui::p2pX(57.6f, vm), gui::p2pY(15.6f, vm)));

	this->bgIconText.setCharacterSize(gui::callCharSize(vm, 90));
	this->bgIconText.setFillColor(sf::Color::White);
	this->bgIconText.setString("BACKGROUD\n ");
}
void SettingsState::initGui(StateData* stateData)
{
	//BUTTONS
	const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;
	this->buttons["BACK"] = new gui::Button(
		gui::p2pX(72.1f, vm), gui::p2pY(81.5f, vm),
		gui::p2pX(13.6f, vm), gui::p2pY(6.4f, vm),
		&this->font, "BACK", gui::callCharSize(vm, 60),
		sf::Color::Black, sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(200, 100, 100), sf::Color(200, 100, 100, 100), sf::Color(20, 20, 20, 50));

	this->buttons["APPLY"] = new gui::Button(
		gui::p2pX(55.f, vm), gui::p2pY(81.5f, vm),//<-position
		gui::p2pX(13.6f, vm), gui::p2pY(6.4f, vm), //<-size
		&this->font, "APPLY", gui::callCharSize(vm, 60),
		sf::Color::Black, sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(100, 200, 100), sf::Color(100, 200, 100, 100), sf::Color(20, 20, 20, 50));

	std::vector<std::string> modes_str;
	for (auto& i : modes)
	{
		modes_str.push_back(std::to_string(i.width) + 'x' + std::to_string(i.height));
	}

	this->resolution["RESOLUTION"] = new gui::DropDownList(
		gui::p2pX(10.6f, vm), gui::p2pY(20.6f, vm),
		gui::p2pX(10.4f, vm), gui::p2pY(4.5f, vm),
		this->stateData->gfxSettings->resolution, 
		font, modes_str.data(), static_cast<int>(modes_str.size()));

	this->ballList["BALLS"] = new gui::DropDownList(
		gui::p2pX(25.6f, vm), gui::p2pY(20.6f, vm),
		gui::p2pX(10.4f, vm), gui::p2pY(6.5f, vm),
		this->stateData->gfxSettings->resolution,
		font, modes_str.data(), 3, this->ballTextures);

	this->paddleList["PADDLES"] = new gui::DropDownList(
		gui::p2pX(40.6f, vm), gui::p2pY(20.6f, vm),
		gui::p2pX(10.4f, vm), gui::p2pY(4.5f, vm),
		this->stateData->gfxSettings->resolution,
		font, modes_str.data(), 7,
		&this->paddleTexture, this->paddleTextureRect);

	this->paddleList["BG"] = new gui::DropDownList(
		gui::p2pX(55.6f, vm), gui::p2pY(20.6f, vm),
		gui::p2pX(15.4f, vm), gui::p2pY(8.5f, vm),
		this->stateData->gfxSettings->resolution,
		font, modes_str.data(), 5,
		&this->bg, this->bgTextureRect);

	this->initTexts();

	//SLIDERS
	this->musicSlider = new gui::Slider(stateData, "GAME MUSIC VOLUME",
		gui::p2pX(10.f, vm), gui::p2pY(40.f, vm),
		gui::p2pX(10.f, vm), gui::p2pY(5.f, vm),
		gui::p2pX(1.f, vm), 0, 100, stateData->musicVolume);

	this->endSoundSlider = new gui::Slider(stateData, "END GAME VOLUME",
		gui::p2pX(30.f, vm), gui::p2pY(40.f, vm),
		gui::p2pX(10.f, vm), gui::p2pY(5.f, vm),
		gui::p2pX(1.f, vm), 0, 100, stateData->endVolume);

	this->collisionSoundSlider = new gui::Slider(stateData, "COLLISION VOLUME",
		gui::p2pX(50.f, vm), gui::p2pY(40.f, vm),
		gui::p2pX(10.f, vm), gui::p2pY(5.f, vm),
		gui::p2pX(1.f, vm), 0, 100, stateData->endVolume);

}

//CONSTRUCTOR AND DESTRUCTOR
SettingsState::SettingsState(StateData* stateData)
	:State(stateData)
{
	this->initVariables();
	this->initKeyBinds();
	this->initTextures();
	this->initFonts();
	this->initGui(stateData);
}
SettingsState::~SettingsState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it) //++it faster than it++
	{
		delete it->second;
	}
	auto it2 = this->resolution.begin();
	for (it2 = this->resolution.begin(); it2 != this->resolution.end(); ++it2) //++it faster than it++
	{
		delete it2->second;
	}
	auto it3 = this->ballList.begin();
	for (it3 = this->ballList.begin(); it3 != this->ballList.end(); ++it3) //++it faster than it++
	{
		delete it3->second;
	}
	auto it4 = this->paddleList.begin();
	for (it4 = this->paddleList.begin(); it4 != this->paddleList.end(); ++it4) //++it faster than it++
	{
		delete it4->second;
	}
	auto it5 = this->bgList.begin();
	for (it5 = this->bgList.begin(); it5 != this->bgList.end(); ++it5) //++it faster than it++
	{
		delete it5->second;
	}
	delete this->musicSlider;
}

//FUNCTIONS
void SettingsState::resetGui()
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

	auto it2 = this->resolution.begin();
	for (it2 = this->resolution.begin(); it2 != this->resolution.end(); ++it2)
	{
		delete it2->second;
	}
	this->resolution.clear();

	auto it3 = this->ballList.begin();
	for (it3 = this->ballList.begin(); it3 != this->ballList.end(); ++it3) //++it faster than it++
	{
		delete it3->second;
	}
	this->ballList.clear();

	auto it4 = this->paddleList.begin();
	for (it4 = this->paddleList.begin(); it4 != this->paddleList.end(); ++it4) //++it faster than it++
	{
		delete it4->second;
	}
	this->bgList.clear();
	auto it5 = this->bgList.begin();
	for (it5 = this->bgList.begin(); it5 != this->bgList.end(); ++it5) //++it faster than it++
	{
		delete it5->second;
	}
	this->paddleList.clear();
	delete this->musicSlider;

	this->initGui(this->stateData);
}
void SettingsState::updateGui(const float& dt)
{
	//Updates all the gui elements in this state and handles their fucntionality
	for (auto& it : this->buttons)
	{
		it.second->update(this->stateData, this->mousePosWindow);
	}
	//Button functionality
	// 
	//QUIT THE GAME
	if (this->buttons["BACK"]->isPressed())
	{
		Sleep(100);
		this->endState();
	}

	//APPLY THE GAME
	if (this->buttons["APPLY"]->isPressed())
	{
		//TEST REMOVE LATER
		Sleep(100);

		this->stateData->gfxSettings->resolution = this->modes[this->resolution["RESOLUTION"]->getActiveElementId()];
		this->stateData->ballTexture = this->ballTextures[this->ballList["BALLS"]->getActiveElementId()];

		this->stateData->paddleTopRect = this->paddleTextureRect[this->paddleList["PADDLES"]->getActiveElementId()].top;
		this->stateData->bgLeftRect = this->bgTextureRect[this->paddleList["BG"]->getActiveElementId()].left;
		
		this->window->create(this->stateData->gfxSettings->resolution, this->stateData->gfxSettings->title, sf::Style::Default);
		this->stateData->reset = 1;

		this->stateData->musicVolume = this->musicSlider->getSliderValue();
		this->stateData->endVolume = this->endSoundSlider->getSliderValue();
		this->stateData->collisionVolume = this->collisionSoundSlider->getSliderValue();


		this->stateData->gridHeight = 0.046296296296296f * this->stateData->gfxSettings->resolution.height;
		this->stateData->gridWidth = 0.0520833333333333f * this->stateData->gfxSettings->resolution.width;

		this->resetGui();
	}

	//Drop down box lists
	for (auto& it : this->resolution)
	{
		it.second->update(this->stateData, this->mousePosWindow, dt);
	}
	for (auto& it : this->ballList)
	{
		it.second->update(this->stateData, this->mousePosWindow, dt);
	}
	for (auto& it : this->paddleList)
	{
		it.second->update(this->stateData, this->mousePosWindow, dt);
	}
	for (auto& it : this->bgList)
	{
		it.second->update(this->stateData, this->mousePosWindow, dt);
	}
	//Drop down box functionalities
	this->musicSlider->update(stateData);
	this->endSoundSlider->update(stateData);
	this->collisionSoundSlider->update(stateData);

}
void SettingsState::updateInput(const float& dt)
{

}
void SettingsState::update(const float& dt)
{
	this->updateMousePosition();
	this->updateInput(dt);
	this->updateGui(dt);
}
void SettingsState::renderGui(sf::RenderTarget& target)
{
	//SLIDERS
	this->musicSlider->render(target);
	this->endSoundSlider->render(target);
	this->collisionSoundSlider->render(target);


	//Buttons
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}
	//Drop down box
	for (auto& it : this->resolution)
	{
		it.second->render(target);
	}
	for (auto& it : this->ballList)
	{
		it.second->render(target);
	}
	for (auto& it : this->paddleList)
	{
		it.second->render(target);
	}	for (auto& it : this->bgList)
	{
		it.second->render(target);
	}
}
void SettingsState::renderText(sf::RenderTarget& target)
{
	target.draw(this->optionsText);
	target.draw(this->resolutionText);
	target.draw(this->ballIconText);
	target.draw(this->paddleIconText);
	target.draw(this->bgIconText);
}
void SettingsState::render(sf::RenderTarget* target)
{
	target->draw(this->background);
	this->renderText(*target);
	this->renderGui(*target);
	

}