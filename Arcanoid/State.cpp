#include "State.h"
#include "stdafx.h"

State::State(StateData* stateData)
{
    this->gridWidth = stateData->gridWidth;
    this->gridHeight = stateData->gridHeight;

    this->stateData = stateData;
    this->window = stateData->window;
    this->supportedKeys = stateData->supportedKeys;
    this->states = stateData->states;
    this->paused = false;
    this->quit = false;
}
State::~State() = default;

void State::pauseState()
{
    this->paused = true;
}
void State::unpauseState()
{
    this->paused = false;
}
void State::endState()
{
    this->quit = true;
}

void State::checkForQuit()
{
    if (stateData->isKeyPressed(sf::Keyboard::Key(this->keyBinds.at("CLOSE"))))
        this->quit = true;
}
void State::updateMousePosition()
{
    this->mousePosWindow = stateData->mousePosition;
    this->mousePosGrid =
        sf::Vector2i(static_cast<int>(std::floor(this->mousePosWindow.x / this->stateData->gridWidth)),
                     static_cast<int>(std::floor(this->mousePosWindow.y / this->stateData->gridHeight)));
}
bool State::getQuit() const
{
    return this->quit;
}
void State::loadKeyBinds(const std::string& path)
{
    std::ifstream input(path);
    if (!input)
        throw std::runtime_error("Cannot open key bindings: " + path);
    std::string action, key;
    while (input >> action >> key)
    {
        const auto supported = supportedKeys->find(key);
        if (supported == supportedKeys->end())
            throw std::runtime_error("Unknown key: " + key);
        keyBinds[action] = supported->second;
    }
    if (!input.eof())
        throw std::runtime_error("Invalid key bindings: " + path);
}
