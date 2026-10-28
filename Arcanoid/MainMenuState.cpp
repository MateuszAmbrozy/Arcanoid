#include "MainMenuState.h"
#include "Assets.h"
#include "stdafx.h"

void MainMenuState::initMute()
{
    assets::load(this->muteTexture, "Resources/Sprites/mute.png");
    this->mute.setTexture(this->muteTexture);

    assets::load(this->unmuteTexture, "Resources/Sprites/unmute.png");
    this->unmute.setTexture(this->unmuteTexture);
}
void MainMenuState::initVariables()
{
    this->vm = this->stateData->gfxSettings->resolution;
}
void MainMenuState::initKeyBinds()
{
    loadKeyBinds("Config/mainmenu_keybinds.ini");
}
void MainMenuState::initFont()
{
    assets::load(this->font, "Fonts/Arialn.ttf");
}
void MainMenuState::initGUI()
{
    assets::load(this->texture, "Resources/Background/bg1.png");
    this->sprite.setTexture(texture);
    this->background.setSize(sf::Vector2f(static_cast<float>(this->window->getSize().x),
                                          static_cast<float>(this->window->getSize().y)));
    this->background.setTexture(&this->texture);

    this->buttons["LEVEL_STATE_BTN"] = std::make_unique<gui::Button>(
        gui::p2pX(50.05f, this->vm), gui::p2pY(60.8f, this->vm), gui::p2pX(20.02f, this->vm),
        gui::p2pY(8.6, this->vm), &this->font, "PLAY", gui::callCharSize(this->vm, 60), sf::Color::Black,
        sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50), sf::Color(100, 200, 100),
        sf::Color(100, 200, 100, 100), sf::Color(20, 20, 20, 50));

    this->buttons["CONFIG_STATE_BTN"] = std::make_unique<gui::Button>(
        gui::p2pX(75.05f, this->vm), gui::p2pY(60.8f, this->vm), gui::p2pX(20.02f, this->vm),
        gui::p2pY(8.6, this->vm), &this->font, "CONFIG", gui::callCharSize(this->vm, 60), sf::Color::Black,
        sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50), sf::Color(200, 200, 100),
        sf::Color(200, 200, 100, 100), sf::Color(20, 20, 20, 50));

    this->buttons["EDITOR_STATE_BTN"] = std::make_unique<gui::Button>(
        gui::p2pX(50.05f, this->vm), gui::p2pY(75.8f, this->vm), gui::p2pX(20.02f, this->vm),
        gui::p2pY(8.6, this->vm), &this->font, "EDITOR", gui::callCharSize(this->vm, 60), sf::Color::Black,
        sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50), sf::Color(100, 100, 200),
        sf::Color(100, 100, 200, 100), sf::Color(20, 20, 20, 50));

    this->buttons["EXIT_STATE_BTN"] = std::make_unique<gui::Button>(
        gui::p2pX(75.05f, this->vm), gui::p2pY(75.8f, this->vm), gui::p2pX(20.02f, this->vm),
        gui::p2pY(8.6, this->vm), &this->font, "QUIT", gui::callCharSize(this->vm, 60), sf::Color::Black,
        sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50), sf::Color(200, 100, 100),
        sf::Color(200, 100, 100, 100), sf::Color(20, 20, 20, 50));

    this->mute.setScale(
        gui::p2pX(5.f, this->stateData->gfxSettings->resolution) / this->mute.getLocalBounds().width,
        gui::p2pX(5.f, this->stateData->gfxSettings->resolution) / this->mute.getLocalBounds().width);
    this->unmute.setScale(
        gui::p2pX(5.f, this->stateData->gfxSettings->resolution) / this->mute.getLocalBounds().width,
        gui::p2pX(5.f, this->stateData->gfxSettings->resolution) / this->mute.getLocalBounds().width);
    this->mute.setPosition(
        this->stateData->gfxSettings->resolution.width - this->mute.getGlobalBounds().width * 1.5f,
        this->stateData->gfxSettings->resolution.height - this->mute.getGlobalBounds().height);
    this->unmute.setPosition(
        this->stateData->gfxSettings->resolution.width - this->unmute.getGlobalBounds().width * 1.5f,
        this->stateData->gfxSettings->resolution.height - this->unmute.getGlobalBounds().height);
}

MainMenuState::MainMenuState(StateData* stateData) : State(stateData)
{
    this->initMute();
    this->initVariables();
    this->initKeyBinds();
    this->initFont();
    this->initGUI();
}
MainMenuState::~MainMenuState() = default;

void MainMenuState::resetGUI()
{

    auto it = this->buttons.begin();
    for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
    {
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
        this->states->push(std::make_unique<LevelState>(this->stateData, 0));
    }

    else if (this->buttons["EDITOR_STATE_BTN"]->isPressed())
    {
        this->states->push(std::make_unique<LevelState>(this->stateData, 1));
    }
    else if (this->buttons["CONFIG_STATE_BTN"]->isPressed())
    {
        this->states->push(std::make_unique<SettingsState>(this->stateData));
    }
    else if (this->buttons["EXIT_STATE_BTN"]->isPressed())
    {
        this->endState();
    }
}
void MainMenuState::updateInput(float) {}
void MainMenuState::updateMute()
{
    if (this->mute.getGlobalBounds().contains(sf::Vector2f(this->mousePosWindow)) ||
        this->unmute.getGlobalBounds().contains(sf::Vector2f(this->mousePosWindow)))
    {
        if (stateData->mouseClicked)
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
void MainMenuState::update(float dt)
{
    if (this->stateData->reset)
    {
        this->resetGUI();
    }
    this->updateMousePosition();
    this->updateMute();
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
