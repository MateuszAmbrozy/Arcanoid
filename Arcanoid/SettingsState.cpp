#include "SettingsState.h"
#include "Assets.h"
#include "stdafx.h"

void SettingsState::initVariables()
{
    this->modes = sf::VideoMode::getFullscreenModes();
    const auto current = stateData->gfxSettings->resolution;
    modes.erase(std::unique(modes.begin(), modes.end(), [](const auto& a, const auto& b)
                            { return a.width == b.width && a.height == b.height; }),
                modes.end());
    const auto it = std::find_if(modes.begin(), modes.end(), [&](const auto& mode)
                                 { return mode.width == current.width && mode.height == current.height; });
    if (it == modes.end())
        modes.insert(modes.begin(), current);
    else
        std::rotate(modes.begin(), it, it + 1);
}
void SettingsState::initKeyBinds()
{
    loadKeyBinds("Config/mainmenu_keybinds.ini");
}
void SettingsState::initTextures()
{
    assets::load(this->ballTextures[0], "Resources/Sprites/ball.png");
    assets::load(this->ballTextures[1], "Resources/Sprites/ball1.png");
    assets::load(this->ballTextures[2], "Resources/Sprites/ball2.png");

    assets::load(this->paddleTexture, "Resources/Sprites/iconPaddle.png");
    assets::load(this->bg, "Resources/Background/bg3.png");
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
    assets::load(this->backgroundTexture, "Resources/Background/settings.png");
    this->background.setSize(sf::Vector2f(static_cast<float>(this->window->getSize().x),
                                          static_cast<float>(this->window->getSize().y)));
    this->background.setTexture(&this->backgroundTexture);
}
void SettingsState::initFonts()
{
    assets::load(this->font, "Fonts/Arialn.ttf");
}
void SettingsState::initTexts()
{
    const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;
    this->optionsText.setFont(this->font);

    this->optionsText.setPosition(sf::Vector2f(gui::p2pX(49.f, vm), gui::p2pY(3.4f, vm)));

    this->optionsText.setCharacterSize(gui::callCharSize(vm, 50));
    this->optionsText.setFillColor(sf::Color::White);
    this->optionsText.setString("OPTIONS\n ");

    this->ballIconText.setFont(this->font);

    this->ballIconText.setPosition(sf::Vector2f(gui::p2pX(25.6f, vm), gui::p2pY(15.6f, vm)));

    this->ballIconText.setCharacterSize(gui::callCharSize(vm, 80));
    this->ballIconText.setFillColor(sf::Color::White);
    this->ballIconText.setString("BALLS\n ");

    this->resolutionText.setFont(this->font);

    this->resolutionText.setPosition(sf::Vector2f(gui::p2pX(10.6f, vm), gui::p2pY(15.6f, vm)));

    this->resolutionText.setCharacterSize(gui::callCharSize(vm, 90));
    this->resolutionText.setFillColor(sf::Color::White);
    this->resolutionText.setString("RESOLUTION\n ");

    this->paddleIconText.setFont(this->font);

    this->paddleIconText.setPosition(sf::Vector2f(gui::p2pX(40.6f, vm), gui::p2pY(15.6f, vm)));

    this->paddleIconText.setCharacterSize(gui::callCharSize(vm, 90));
    this->paddleIconText.setFillColor(sf::Color::White);
    this->paddleIconText.setString("PADDLE\n ");
    this->bgIconText.setFont(this->font);

    this->bgIconText.setPosition(sf::Vector2f(gui::p2pX(57.6f, vm), gui::p2pY(15.6f, vm)));

    this->bgIconText.setCharacterSize(gui::callCharSize(vm, 90));
    this->bgIconText.setFillColor(sf::Color::White);
    this->bgIconText.setString("BACKGROUND\n ");
}
void SettingsState::initGui(StateData* stateData)
{
    const sf::VideoMode& vm = this->stateData->gfxSettings->resolution;
    this->buttons["BACK"] = std::make_unique<gui::Button>(
        gui::p2pX(72.1f, vm), gui::p2pY(81.5f, vm), gui::p2pX(13.6f, vm), gui::p2pY(6.4f, vm), &this->font,
        "BACK", gui::callCharSize(vm, 60), sf::Color::Black, sf::Color(250, 250, 250, 250),
        sf::Color(20, 20, 20, 50), sf::Color(200, 100, 100), sf::Color(200, 100, 100, 100),
        sf::Color(20, 20, 20, 50));

    this->buttons["APPLY"] = std::make_unique<gui::Button>(
        gui::p2pX(55.f, vm), gui::p2pY(81.5f, vm), gui::p2pX(13.6f, vm), gui::p2pY(6.4f, vm), &this->font,
        "APPLY", gui::callCharSize(vm, 60), sf::Color::Black, sf::Color(250, 250, 250, 250),
        sf::Color(20, 20, 20, 50), sf::Color(100, 200, 100), sf::Color(100, 200, 100, 100),
        sf::Color(20, 20, 20, 50));

    std::vector<std::string> modes_str;
    for (auto& i : modes)
    {
        modes_str.push_back(std::to_string(i.width) + 'x' + std::to_string(i.height));
    }

    this->resolution["RESOLUTION"] = std::make_unique<gui::DropDownList>(
        gui::p2pX(10.6f, vm), gui::p2pY(20.6f, vm), gui::p2pX(10.4f, vm), gui::p2pY(4.5f, vm),
        this->stateData->gfxSettings->resolution, font, modes_str.data(), static_cast<int>(modes_str.size()));

    this->ballList["BALLS"] = std::make_unique<gui::DropDownList>(
        gui::p2pX(25.6f, vm), gui::p2pY(20.6f, vm), gui::p2pX(10.4f, vm), gui::p2pY(6.5f, vm),
        this->stateData->gfxSettings->resolution, font, modes_str.data(), 3, this->ballTextures, nullptr,
        stateData->selectedBall);

    this->paddleList["PADDLES"] = std::make_unique<gui::DropDownList>(
        gui::p2pX(40.6f, vm), gui::p2pY(20.6f, vm), gui::p2pX(10.4f, vm), gui::p2pY(4.5f, vm),
        this->stateData->gfxSettings->resolution, font, modes_str.data(), 7, &this->paddleTexture,
        this->paddleTextureRect, stateData->paddleTopRect / 30);

    this->paddleList["BG"] = std::make_unique<gui::DropDownList>(
        gui::p2pX(55.6f, vm), gui::p2pY(20.6f, vm), gui::p2pX(15.4f, vm), gui::p2pY(8.5f, vm),
        this->stateData->gfxSettings->resolution, font, modes_str.data(), 5, &this->bg, this->bgTextureRect,
        stateData->bgLeftRect / 200);

    this->initTexts();

    this->musicSlider = std::make_unique<gui::Slider>(
        stateData, "GAME MUSIC VOLUME", gui::p2pX(10.f, vm), gui::p2pY(40.f, vm), gui::p2pX(10.f, vm),
        gui::p2pY(5.f, vm), gui::p2pX(1.f, vm), 0, 100, stateData->musicVolume);

    this->endSoundSlider = std::make_unique<gui::Slider>(
        stateData, "END GAME VOLUME", gui::p2pX(30.f, vm), gui::p2pY(40.f, vm), gui::p2pX(10.f, vm),
        gui::p2pY(5.f, vm), gui::p2pX(1.f, vm), 0, 100, stateData->endVolume);

    this->collisionSoundSlider = std::make_unique<gui::Slider>(
        stateData, "COLLISION VOLUME", gui::p2pX(50.f, vm), gui::p2pY(40.f, vm), gui::p2pX(10.f, vm),
        gui::p2pY(5.f, vm), gui::p2pX(1.f, vm), 0, 100, stateData->collisionVolume);
}

SettingsState::SettingsState(StateData* stateData) : State(stateData)
{
    this->initVariables();
    this->initKeyBinds();
    this->initTextures();
    this->initFonts();
    this->initGui(stateData);
}
SettingsState::~SettingsState() = default;

void SettingsState::resetGui()
{
    buttons.clear();
    resolution.clear();
    ballList.clear();
    paddleList.clear();
    bgList.clear();
    initGui(stateData);
    background.setSize(sf::Vector2f(window->getSize()));
}
void SettingsState::updateGui(float dt)
{
    for (auto& it : this->buttons)
    {
        it.second->update(this->stateData, this->mousePosWindow);
    }
    if (this->buttons["BACK"]->isPressed())
    {

        this->endState();
    }

    if (this->buttons["APPLY"]->isPressed())
    {

        this->stateData->gfxSettings->resolution =
            this->modes[this->resolution["RESOLUTION"]->getActiveElementId()];
        this->stateData->selectedBall = this->ballList["BALLS"]->getActiveElementId();
        this->stateData->ballTexture = this->ballTextures[this->ballList["BALLS"]->getActiveElementId()];

        this->stateData->paddleTopRect =
            this->paddleTextureRect[this->paddleList["PADDLES"]->getActiveElementId()].top;
        this->stateData->bgLeftRect = this->bgTextureRect[this->paddleList["BG"]->getActiveElementId()].left;

        this->window->create(this->stateData->gfxSettings->resolution, this->stateData->gfxSettings->title,
                             stateData->gfxSettings->fullScreen ? sf::Style::Fullscreen
                                                                : sf::Style::Titlebar | sf::Style::Close,
                             stateData->gfxSettings->contextSettings);
        window->setFramerateLimit(stateData->gfxSettings->frameRateLimit);
        window->setVerticalSyncEnabled(stateData->gfxSettings->verticalSync);
        this->stateData->reset = 1;

        this->stateData->musicVolume = this->musicSlider->getSliderValue();
        this->stateData->endVolume = this->endSoundSlider->getSliderValue();
        this->stateData->collisionVolume = this->collisionSoundSlider->getSliderValue();

        this->stateData->gridHeight =
            this->stateData->gfxSettings->resolution.height / static_cast<float>(gameConfig::gridRows);
        this->stateData->gridWidth =
            this->stateData->gfxSettings->resolution.width / static_cast<float>(gameConfig::gridColumns);

        this->initVariables();
        this->resetGui();
        return;
    }

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
    this->musicSlider->update(stateData);
    this->endSoundSlider->update(stateData);
    this->collisionSoundSlider->update(stateData);
}
void SettingsState::updateInput(float) {}
void SettingsState::update(float dt)
{
    this->updateMousePosition();
    this->updateInput(dt);
    this->updateGui(dt);
}
void SettingsState::renderGui(sf::RenderTarget& target)
{
    this->musicSlider->render(target);
    this->endSoundSlider->render(target);
    this->collisionSoundSlider->render(target);

    for (auto& it : this->buttons)
    {
        it.second->render(target);
    }
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
    }
    for (auto& it : this->bgList)
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
