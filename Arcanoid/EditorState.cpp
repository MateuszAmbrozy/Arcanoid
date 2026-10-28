#include "EditorState.h"
#include "Assets.h"
#include "stdafx.h"

void EditorState::initVariables()
{
    this->vm = this->stateData->gfxSettings->resolution;
    this->textureRect = sf::IntRect(0, 0, 100, 50);
    this->type = BlockTypes::DEFAULT;
    this->lives = 1;
}
void EditorState::initKeyBinds()
{
    loadKeyBinds("Config/editorstate_keybinds.ini");
}
void EditorState::initFonts()
{
    assets::load(this->font, "Fonts/Arialn.ttf");
}
void EditorState::initText()
{
    this->cursorText.setFont(this->font);
    this->cursorText.setCharacterSize(12);
}
void EditorState::initPausedMenu()
{
    this->pausedMenu = std::make_unique<PausedMenu>(this->stateData->gfxSettings->resolution);
    this->pausedMenu->addButton("QUIT", gui::p2pY(46.29f, this->vm), gui::p2pX(17.8f, this->vm),
                                gui::p2pY(4.629f, this->vm), gui::callCharSize(this->vm, 60),
                                sf::Color::Black, sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
                                sf::Color(200, 100, 100), sf::Color(200, 100, 100, 100),
                                sf::Color(20, 20, 20, 50), "Quit");
    this->pausedMenu->addButton("SAVE", gui::p2pY(18.51f, this->vm), gui::p2pX(17.8f, this->vm),
                                gui::p2pY(4.629f, this->vm), gui::callCharSize(this->vm, 60),
                                sf::Color::Black, sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
                                sf::Color(100, 200, 100), sf::Color(100, 200, 100, 100),
                                sf::Color(20, 20, 20, 50), "Save");
    this->pausedMenu->addButton("LOAD", gui::p2pY(27.27f, this->vm), gui::p2pX(17.8f, this->vm),
                                gui::p2pY(4.629f, this->vm), gui::callCharSize(this->vm, 60),
                                sf::Color::Black, sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
                                sf::Color(200, 200, 100), sf::Color(200, 200, 100, 100),
                                sf::Color(20, 20, 20, 50), "Load");
}
void EditorState::initBlockSet()
{
    this->blockSet =
        std::make_unique<BlockSet>(this->stateData, this->stateData->gridWidth, this->stateData->gridHeight,
                                   32, 36, "Resources/Sprites/blocks.png");
}
void EditorState::initGUI()
{
    this->selectorRect.setFillColor(sf::Color(255, 255, 255, 120));
    this->selectorRect.setSize(sf::Vector2f(this->stateData->gridWidth, this->stateData->gridHeight));
    this->selectorRect.setOutlineColor(sf::Color::Red);
    this->selectorRect.setOutlineThickness(1.f);

    this->textureSelector = std::make_unique<gui::TextureSelector>(
        20.f, 20.f, gui::p2pX(46.875f, this->vm), gui::p2pY(27.778f, this->vm), this->stateData->gridWidth,
        this->gridHeight, this->blockSet->getBlockSheet(), this->font, "TS");
}
void EditorState::initBackground()
{
    this->bgTextureRect = sf::IntRect(0, 0, 200, 241);

    assets::load(this->bgTexture, "Resources/Background/bg3.png");

    this->background.setSize(sf::Vector2f(static_cast<float>(vm.width), static_cast<float>(vm.height)));
    if (this->stateData->bgLeftRect != -1)
    {
        this->bgTextureRect.left = stateData->bgLeftRect;
    }

    this->background.setTextureRect(this->bgTextureRect);
    this->background.setTexture(&this->bgTexture);
}

EditorState::EditorState(StateData* stateData, std::string filename) : State(stateData), filename(filename)
{
    this->initVariables();
    this->initKeyBinds();
    this->initFonts();
    this->initText();
    this->initPausedMenu();
    this->initBlockSet();
    if (std::filesystem::exists(this->filename))
        this->blockSet->loadFromFile(this->filename);
    this->initGUI();
    this->initBackground();
}
EditorState::~EditorState() = default;

void EditorState::updateButtons()
{
    for (auto& it : this->buttons)
    {
        it.second->update(this->stateData, this->mousePosWindow);
    }
}
void EditorState::updateEditorInput(float)
{

    if (stateData->mouseDown)
    {
        if (!this->textureSelector->getActive())
        {
            this->blockSet->addBlock(this->mousePosGrid.x, this->mousePosGrid.y, this->lives,
                                     this->textureRect, this->type);
        }
        else
        {
            this->textureRect = this->textureSelector->getTextureRect();
        }
    }

    if (stateData->rightMouseDown && !this->textureSelector->getActive())
    {
        this->blockSet->removeBlock(this->mousePosGrid.x, this->mousePosGrid.y);
    }

    if (stateData->isKeyJustPressed(sf::Keyboard::Key(keyBinds.at("INCREASE_TYPE"))))
    {
        if (this->type < 2)
            this->type++;
    }
    else if (stateData->isKeyJustPressed(sf::Keyboard::Key(keyBinds.at("DECREASE_TYPE"))))
    {
        if (this->type > 0)
            this->type--;
    }
    if (stateData->isKeyJustPressed(sf::Keyboard::Key(keyBinds.at("INCREASE_LIFE"))) && this->lives < 5)
    {
        this->lives++;
    }
    else if (stateData->isKeyJustPressed(sf::Keyboard::Key(keyBinds.at("DECREASE_LIFE"))) && this->lives > 0)
    {
        this->lives--;
    }
}
void EditorState::updateGUI()
{
    this->textureSelector->update(this->stateData, this->mousePosWindow);
    if (!this->textureSelector->getActive())
    {
        this->selectorRect.setTexture(this->blockSet->getBlockSheet());
        this->selectorRect.setPosition(this->mousePosGrid.x * this->stateData->gridWidth,
                                       this->mousePosGrid.y * this->stateData->gridHeight);
    }

    this->selectorRect.setTextureRect(this->textureRect);

    std::stringstream ss;
    this->cursorText.setPosition(static_cast<float>(this->mousePosWindow.x) + 50.f,
                                 static_cast<float>(this->mousePosWindow.y) - 50.f);

    ss << this->mousePosWindow.x << " " << this->mousePosWindow.y << "\n " << this->mousePosGrid.x << " "
       << this->mousePosGrid.y << "\n " << this->textureRect.left << " " << this->textureRect.top << "\n "
       << "Type: " << this->type + 1 << "\n"
       << "Lives: " << this->lives + 1 << "\n";

    this->cursorText.setString(ss.str());
}
void EditorState::updateInput(float)
{
    if (stateData->isKeyJustPressed(sf::Keyboard::Key(this->keyBinds.at("CLOSE"))))
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
    {
        try
        {
            blockSet->saveToFile(filename);
            stateData->refreshLevelState = true;
        }
        catch (const std::exception& error)
        {
            std::cerr << error.what() << '\n';
        }
    }

    if (this->pausedMenu->isButtonPressed("LOAD"))
    {
        try
        {
            blockSet->loadFromFile(filename);
        }
        catch (const std::exception& error)
        {
            std::cerr << error.what() << '\n';
        }
    }
}
void EditorState::update(float dt)
{
    this->updateMousePosition();
    this->updateInput(dt);
    if (this->paused)
    {
        this->pausedMenu->update(this->stateData, this->mousePosWindow);
        this->updatePausedMenuButtons();
    }
    else
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

    if (this->paused)
    {
        this->pausedMenu->render(*target);
    }
}
