#include "stdafx.h"
#include "EditorState.h"

//PRIVATE FUNCTIONS
void EditorState::initVariables()
{
	this->vm = this->stateData->gfxSettings->resolution;
	this->textureRect = sf::IntRect(0, 0, static_cast<int>(this->stateData->gridWidth * 1920 / vm.width), static_cast<int>(this->stateData->gridHeight * 1080 / vm.height));
	this->type = BlockTypes::DEFAULT;
	this->lifes = 1;
}
void EditorState::initKeyBinds()
{
	std::fstream ifs("Config/EditorState_keybinds.ini");
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
void EditorState::initFonts()
{
	if (!this->font.loadFromFile("Fonts/Arialn.ttf"))
		throw("ERROR::EDITORSTATE::INITFONTS::Could not load font!");

}
void EditorState::initText()
{
	this->cursorText.setFont(this->font);
	this->cursorText.setCharacterSize(12);

}
void EditorState::initPausedMenu()
{
	this->pausedMenu = new PausedMenu(this->stateData->gfxSettings->resolution);
	this->pausedMenu->addButton("QUIT", gui::p2pY(46.29f, this->vm), gui::p2pX(17.8f, this->vm),
		gui::p2pY(4.629f, this->vm), gui::callCharSize(this->vm, 60),
		sf::Color::Black, sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(200, 100, 100), sf::Color(200, 100, 100, 100), sf::Color(20, 20, 20, 50),
		"Quit");
	this->pausedMenu->addButton("SAVE", gui::p2pY(18.51f, this->vm), gui::p2pX(17.8f, this->vm),
		gui::p2pY(4.629f, this->vm), gui::callCharSize(this->vm, 60),
		sf::Color::Black, sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(100, 200, 100), sf::Color(100, 200, 100, 100), sf::Color(20, 20, 20, 50),
		"Save");
	this->pausedMenu->addButton("LOAD", gui::p2pY(27.27f, this->vm), gui::p2pX(17.8f, this->vm),
		gui::p2pY(4.629f, this->vm), gui::callCharSize(this->vm, 60),
		sf::Color::Black, sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(200, 200, 100), sf::Color(200, 200, 100, 100), sf::Color(20, 20, 20, 50),
		"Load");
}
void EditorState::initBlockSet()
{
	this->blockSet = new BlockSet(this->stateData, this->stateData->gridWidth, this->stateData->gridHeight, 32, 36, "Resources/Sprites/blocks.png");
}
void EditorState::initGUI()
{
	this->selectorRect.setFillColor(sf::Color(255, 255, 255, 120));
	this->selectorRect.setSize(sf::Vector2f(this->stateData->gridWidth, this->stateData->gridHeight));
	this->selectorRect.setOutlineColor(sf::Color::Red);
	this->selectorRect.setOutlineThickness(1.f);

	this->textureSelector = new gui::TextureSelector(20.f, 20.f, gui::p2pX(46.875f, this->vm), gui::p2pY(27.778f, this->vm), this->stateData->gridWidth, this->gridHeight, this->blockSet->getBlockSheet(), this->font, "TS");
}
void EditorState::initBackground()
{
	this->bgTextureRect = sf::IntRect(0, 0, 200, 241);

	if (!this->bgTexture.loadFromFile("Resources/Background/bg3.png"))
		std::cout << "ERROR::GAMESTATE::INITBACKGROUND::Could not load bg3.png \n";

	this->background.setSize(sf::Vector2f(static_cast<float>(vm.width), static_cast<float>(vm.height)));
	if (this->stateData->bgLeftRect != -1)
	{
		this->bgTextureRect.left = stateData->bgLeftRect;
	}
	std::cout << "BGLEFT: " << this->stateData->bgLeftRect << std::endl;
	this->background.setTextureRect(this->bgTextureRect);
	this->background.setTexture(&this->bgTexture);
}

//CONSTRUCTOR AND DESTRUCTOR
EditorState::EditorState(StateData* stateData, std::string filename)
	:State(stateData), filename(filename)
{
	this->initVariables();
	this->initKeyBinds();
	this->initFonts();
	this->initText();
	this->initPausedMenu();
	this->initBlockSet();
	this->initGUI();
	this->initBackground();
}
EditorState::~EditorState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it) //++it faster than it++
	{
		delete it->second;
	}
	delete this->blockSet;
	delete this->pausedMenu;
	delete this->textureSelector;

}

//FUNCTIONS
void EditorState::updateButtons()
{
	for (auto& it : this->buttons)
	{
		it.second->update(this->stateData, this->mousePosWindow);
	}

}
void EditorState::updateEditorInput(const float& dt)
{

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) /*&& this->getKeyTime()*/)
	{
		if (!this->textureSelector->getActive())
		{
			this->blockSet->addBlock(this->mousePosGrid.x, this->mousePosGrid.y, this->lifes, this->textureRect, this->type);
		}
		else
		{
			this->textureRect = this->textureSelector->getTextureRect();

			this->textureRect.left = textureRect.left * 1920 / vm.width;
			this->textureRect.top = textureRect.top * 1080 / vm.height;
			this->textureRect.width = textureRect.width * 1920 / vm.width;
			this->textureRect.height = textureRect.height * 1080 / vm.height;
		}
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !this->textureSelector->getActive()/* && this->getKeyTime()*/)
	{
		this->blockSet->removeBlock(this->mousePosGrid.x, this->mousePosGrid.y);
	}
	
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keyBinds.at("INCREASE_TYPE"))) && this->getKeyTime())
	{
		if(this->type < 2)
			this->type++;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keyBinds.at("DECREASE_TYPE"))) && this->getKeyTime())
	{
		if (this->type > 0)
			this->type--;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keyBinds.at("INCREASE_LIFE"))) && this->getKeyTime() && this->lifes < 5 )
	{
		this->lifes++;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keyBinds.at("DECREASE_LIFE"))) && this->getKeyTime() && this->lifes > 0 )
	{
			this->lifes--;
	}
}
void EditorState::updateGUI()
{
	this->textureSelector->update(this->stateData, this->mousePosWindow);
	if (!this->textureSelector->getActive())
	{
		this->selectorRect.setTexture(this->blockSet->getBlockSheet());
		this->selectorRect.setPosition(this->mousePosGrid.x * this->stateData->gridWidth, this->mousePosGrid.y * this->stateData->gridHeight);
	}



	this->selectorRect.setTextureRect(this->textureRect);

	std::stringstream ss;
	this->cursorText.setPosition(static_cast<float>(this->mousePosWindow.x) + 50.f, static_cast<float>(this->mousePosWindow.y) - 50.f);

	ss << this->mousePosWindow.x << " " << this->mousePosWindow.y << "\n "
		<< this->mousePosGrid.x << " " << this->mousePosGrid.y << "\n "
		<< this->textureRect.left << " " << this->textureRect.top << "\n "
		<< "Type: " << this->type + 1 << "\n"
		<< "Lifes: " << this->lifes + 1 << "\n";

	this->cursorText.setString(ss.str());
}
void EditorState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keyBinds.at("CLOSE"))) && this->getKeyTime())
	{
		if (!this->paused)
			this->pauseState();
		else
			this->unpauseState();
	}
}
void EditorState::updatePausedMenuButtons()
{
	if (this->pausedMenu->isButtonPressed("QUIT"))
		this->endState();

	if (this->pausedMenu->isButtonPressed("SAVE"))
		this->blockSet->saveToFile(this->filename);

	if (this->pausedMenu->isButtonPressed("LOAD"))
		this->blockSet->loadFromFile(this->filename);
}
void EditorState::update(const float& dt)
{
	this->updateMousePosition();
	this->updateInput(dt);
	this->updateButtons();
	if (this->paused) //PAUSED	
	{
		this->pausedMenu->update(this->stateData, this->mousePosWindow);
		this->updatePausedMenuButtons();
	}
	else //UNPAUSED
	{
		this->updateButtons();
		this->updateGUI();
		this->updateEditorInput(dt);
	}
}
void EditorState::renderButtons(sf::RenderTarget& target)
{
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}
}
void EditorState::renderGUI(sf::RenderTarget& target)
{
	if (!this->textureSelector->getActive())
	{
		target.draw(this->selectorRect);
		target.draw(this->cursorText);
	}

	this->textureSelector->render(target);
}
void EditorState::render(sf::RenderTarget* target)
{
	target->draw(this->background);
	this->blockSet->render(*target);

	this->renderGUI(*target);
	this->renderButtons(*target);

	if (this->paused)//Paused menu State 
	{
		this->pausedMenu->render(*target);
	}
}