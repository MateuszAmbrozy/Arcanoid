#include "Gui.h"
#include "Assets.h"
#include "stdafx.h"

float gui::p2pX(const float perc, const sf::VideoMode& vm)
{
    return std::floor(vm.width * perc / 100.f);
}
float gui::p2pY(const float perc, const sf::VideoMode& vm)
{
    return std::floor(vm.height * perc / 100.f);
}
unsigned gui::callCharSize(const sf::VideoMode& vm, const float modifier)
{

    if (modifier != 0)
        return static_cast<unsigned>((vm.width + vm.height) / modifier);
    else
        throw std::runtime_error("DIVISTION BY ZERO\n");
}

void gui::Button::initSound()
{
    assets::load(this->buffer, "Resources/Sounds/button.wav");
    this->sound.setBuffer(this->buffer);
}

gui::Button::Button(float x, float y, float width, float height, sf::Font* font, std::string text,
                    unsigned character_size, sf::Color text_idle_color, sf::Color text_hover_color,
                    sf::Color text_active_color, sf::Color idle_color, sf::Color hover_color,
                    sf::Color active_color, sf::Color outline_idle_color, sf::Color outline_hover_color,
                    sf::Color outline_active_color, short unsigned id, sf::Texture* texture,
                    sf::IntRect* textureRect)
{
    this->isIcon = false;
    this->isTextureRect = false;
    this->buttonState = BTN_IDLE;

    this->shape.setSize(sf::Vector2f(width, height));
    this->shape.setPosition(x, y);
    this->shape.setFillColor(idle_color);
    this->shape.setOutlineThickness(1.f);

    this->font = font;
    this->id = id;

    this->text.setFont(*this->font);
    this->text.setString(text);
    this->text.setFillColor(text_idle_color);
    this->text.setCharacterSize(character_size);
    this->text.setPosition(this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) -
                               this->text.getGlobalBounds().width / 2.f,
                           this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 2.f) -
                               this->text.getGlobalBounds().height);
    cursor.loadFromSystem(sf::Cursor::Hand);
    if (texture != nullptr)
    {
        this->iconTexture = *texture;

        this->icon.setSize(
            sf::Vector2f(this->shape.getGlobalBounds().width / 3, this->shape.getGlobalBounds().width / 3));
        this->icon.setTexture(&this->iconTexture);

        if (textureRect != nullptr)
        {
            this->icon.setSize(
                sf::Vector2f(this->shape.getGlobalBounds().width, this->shape.getGlobalBounds().height));
            this->textureRect = *textureRect;
            this->icon.setTextureRect(this->textureRect);

            this->isTextureRect = true;
        }

        this->icon.setPosition(sf::Vector2f(this->shape.getPosition().x +
                                                this->shape.getGlobalBounds().width / 2.f -
                                                this->icon.getGlobalBounds().width / 2.f,
                                            this->shape.getPosition().y));

        this->isIcon = true;
    }

    this->text_idle_color = text_idle_color;
    this->text_hover_color = text_hover_color;
    this->text_active_color = text_active_color;

    this->idle_color = idle_color;
    this->hover_color = hover_color;
    this->active_color = active_color;

    this->outline_idle_color = outline_idle_color;
    this->outline_hover_color = outline_hover_color;
    this->outline_active_color = outline_active_color;

    this->initSound();
}
gui::Button::~Button() = default;

void gui::Button::setText(const std::string text)
{
    this->text.setString(text);
}
void gui::Button::setId(const short unsigned id)
{
    this->id = id;
}
void gui::Button::setIconTexture(const sf::Texture& texture)
{
    if (this->isIcon)
    {
        this->iconTexture = texture;
        this->icon.setTexture(&this->iconTexture);
    }
}
void gui::Button::setIconTextureRect(sf::IntRect textureRect)
{
    if (this->isTextureRect)
    {
        this->textureRect = textureRect;
        this->icon.setTextureRect(textureRect);
    }
}
void gui::Button::setColor(sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
                           sf::Color idle_color, sf::Color hover_color, sf::Color active_color)
{
    this->text_idle_color = text_idle_color;
    this->text_hover_color = text_hover_color;
    this->text_active_color = text_active_color;

    this->idle_color = idle_color;
    this->hover_color = hover_color;
    this->active_color = active_color;

    this->shape.setFillColor(idle_color);
    this->text.setFillColor(text_idle_color);
}

bool gui::Button::isPressed() const
{
    if (this->buttonState == BTN_ACTIVE)
        return true;

    return false;
}
sf::Vector2f gui::Button::getPosition() const
{
    return this->shape.getPosition();
}
unsigned short gui::Button::getId() const
{
    return this->id;
}
std::string gui::Button::getText() const
{
    return this->text.getString();
}
sf::FloatRect gui::Button::getGlobalBounds() const
{
    return this->shape.getGlobalBounds();
}
const sf::Texture& gui::Button::getIconTexture() const
{
    return iconTexture;
}
sf::IntRect gui::Button::getIconTextureRect() const
{
    if (this->isTextureRect)
        return this->textureRect;
    else
        return sf::IntRect();
}

void gui::Button::update(StateData* stateData, const sf::Vector2i mousePos)
{

    this->buttonState = BTN_IDLE;

    if (this->shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
    {
        this->buttonState = BTN_HOVER;

        if (stateData->mouseClicked)
        {
            if (!stateData->mute)
                this->sound.play();
            this->buttonState = BTN_ACTIVE;
        }
    }
    switch (this->buttonState)
    {
    case BTN_IDLE:
        this->shape.setOutlineColor(this->outline_idle_color);
        this->shape.setFillColor(this->idle_color);
        this->text.setFillColor(this->text_idle_color);

        break;
    case BTN_HOVER:
        this->shape.setOutlineColor(this->outline_hover_color);
        this->shape.setFillColor(this->hover_color);
        this->text.setFillColor(this->text_hover_color);

        break;
    case BTN_ACTIVE:
        this->shape.setOutlineColor(this->outline_active_color);
        this->shape.setFillColor(this->active_color);
        this->text.setFillColor(this->text_active_color);

        break;
    default:
        this->shape.setFillColor(sf::Color::Red);
        this->text.setFillColor(sf::Color::Blue);
        this->shape.setOutlineColor(sf::Color::Green);
        break;
    }
}
void gui::Button::render(sf::RenderTarget& target)
{
    target.draw(this->shape);
    target.draw(this->text);
    if (this->isIcon)
        target.draw(this->icon);
}

gui::DropDownList::DropDownList(float x, float y, float width, float height, sf::VideoMode, sf::Font& font,
                                std::string list[], int nrOfElements, sf::Texture* textures,
                                sf::IntRect* textureRect, const int default_index)
    : font(font), showList(false), isIcon(false), isTextureRect(false)
{
    if (nrOfElements <= 0 || default_index < 0 || default_index >= nrOfElements || (!textures && !list))
        throw std::invalid_argument("Invalid dropdown selection");
    isIcon = textures && !textureRect;
    isTextureRect = textures && textureRect;
    const auto makeButton = [&](int index, float top)
    {
        auto* texture = textures ? (textureRect ? textures : &textures[index]) : nullptr;
        auto* rect = textureRect ? &textureRect[index] : nullptr;
        return std::make_unique<gui::Button>(
            x, top, width, height, &this->font, textures ? "" : list[index], 15,
            sf::Color(255, 255, 255, 200), sf::Color::White, sf::Color(20, 20, 20, 50),
            sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
            sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent,
            static_cast<unsigned short>(index), texture, rect);
    };
    activeElement = makeButton(default_index, y);
    for (int index = 0; index < nrOfElements; ++index)
        this->list.push_back(makeButton(index, y + (index + 1) * height));
}
gui::DropDownList::~DropDownList() = default;

unsigned short gui::DropDownList::getActiveElementId() const
{
    return this->activeElement->getId();
}

void gui::DropDownList::update(StateData* stateData, const sf::Vector2i& mousePos, float)
{

    this->activeElement->update(stateData, static_cast<sf::Vector2i>(mousePos));

    if (this->activeElement->isPressed())
    {
        if (this->showList)
            this->showList = false;
        else
            this->showList = true;
        return;
    }

    if (this->showList)
    {
        for (auto& i : this->list)
        {
            i->update(stateData, static_cast<sf::Vector2i>(mousePos));

            if (i->isPressed())
            {
                this->showList = false;
                this->activeElement->setText(i->getText());
                this->activeElement->setId(i->getId());
                if (this->isIcon)
                    this->activeElement->setIconTexture(i->getIconTexture());

                if (isTextureRect)
                    this->activeElement->setIconTextureRect(i->getIconTextureRect());
            }
        }
    }
}
void gui::DropDownList::render(sf::RenderTarget& target)
{
    this->activeElement->render(target);
    if (this->showList)
    {
        for (auto& i : this->list)
        {
            i->render(target);
        }

        this->activeElement->render(target);
    }
}

gui::TextureSelector::TextureSelector(float x, float y, float, float, float gridWidth, float gridHeight,
                                      const sf::Texture* textureSheet, sf::Font& font, std::string)
{
    this->active = false;
    this->hidden = false;
    this->gridWidth = gridWidth;
    this->gridHeight = gridHeight;
    float offset = gridWidth;

    this->bounds.setSize(sf::Vector2f(gridWidth * 9, gridHeight * 6));
    this->bounds.setPosition(x + offset, y);
    this->bounds.setFillColor(sf::Color(50, 50, 50, 100));
    this->bounds.setOutlineColor(sf::Color(255, 255, 255, 200));
    this->bounds.setOutlineThickness(1.f);

    this->sheet.setTexture(textureSheet);
    this->sheet.setSize(sf::Vector2f(gridWidth * 9.f, gridHeight * 6.f));
    this->sheet.setPosition(x + offset, y);

    if (this->sheet.getGlobalBounds().width > this->bounds.getGlobalBounds().width)
    {
        this->sheet.setTextureRect(sf::IntRect(0, 0, static_cast<int>(this->bounds.getGlobalBounds().width),
                                               static_cast<int>(this->bounds.getGlobalBounds().height)));
    }
    if (this->sheet.getGlobalBounds().height > this->bounds.getGlobalBounds().height)
    {
        this->sheet.setTextureRect(sf::IntRect(0, 0, static_cast<int>(this->sheet.getGlobalBounds().width),
                                               static_cast<int>(this->sheet.getGlobalBounds().height)));
    }

    this->selector.setPosition(x, y);
    this->selector.setSize(sf::Vector2f(gridWidth, gridHeight));
    this->selector.setFillColor(sf::Color::Transparent);
    this->selector.setOutlineThickness(-1.f);
    this->selector.setOutlineColor(sf::Color::Red);

    this->textureRect.width = static_cast<int>(gridWidth);
    this->textureRect.height = static_cast<int>(gridHeight);

    this->hide_btn = std::make_unique<gui::Button>(x, y, gridWidth / 2.f, gridWidth / 2.f, &font, "", 24,
                                                   sf::Color(70, 70, 70, 200), sf::Color(255, 255, 255, 250),
                                                   sf::Color(255, 255, 255, 50), sf::Color(70, 70, 70, 200),
                                                   sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20, 50));
}
gui::TextureSelector::~TextureSelector() = default;

bool gui::TextureSelector::getActive() const
{
    return this->active;
}
const sf::IntRect& gui::TextureSelector::getTextureRect() const
{
    return this->textureRect;
}

void gui::TextureSelector::update(StateData* stateData, const sf::Vector2i& mousePosWindow)
{
    hide_btn->update(stateData, mousePosWindow);
    if (hide_btn->isPressed())
        hidden = !hidden;
    const auto mouse = sf::Vector2f(mousePosWindow);
    const bool overSheet = !hidden && sheet.getGlobalBounds().contains(mouse);
    active = overSheet || hide_btn->getGlobalBounds().contains(mouse);
    if (!overSheet)
        return;
    const int column = std::clamp(static_cast<int>((mouse.x - sheet.getPosition().x) / gridWidth), 0, 8);
    const int row = std::clamp(static_cast<int>((mouse.y - sheet.getPosition().y) / gridHeight), 0, 5);
    selector.setPosition(sheet.getPosition().x + column * gridWidth,
                         sheet.getPosition().y + row * gridHeight);
    textureRect = sf::IntRect(column * gameConfig::tileWidth, row * gameConfig::tileHeight,
                              gameConfig::tileWidth, gameConfig::tileHeight);
}
void gui::TextureSelector::render(sf::RenderTarget& target)
{

    if (!this->hidden)
    {
        target.draw(this->bounds);
        target.draw(this->sheet);
        if (this->active)
            target.draw(this->selector);
    }
    this->hide_btn->render(target);
}

void gui::Slider::initText(std::string text)
{
    assets::load(this->font, "Fonts/Arialn.ttf");
    this->text.setFont(this->font);
    this->text.setString(text);
    this->text.setPosition(this->posX, this->posY - this->text.getGlobalBounds().height * 2.f);
}
void gui::Slider::initSlider()
{
    this->sliderPosX =
        (this->sliderValue - this->minValue) * axisWidth / (this->maxValue - this->minValue) + this->posX;
    this->slider.setPosition(this->sliderPosX, this->posY);

    this->slider.setSize(sf::Vector2f(this->sliderWidth, this->height));
    this->slider.setOrigin(slider.getGlobalBounds().width / 2, 0);
    this->slider.setFillColor(sf::Color(34, 34, 34));

    this->axis.setPosition(this->posX, this->posY);
    this->axis.setSize(sf::Vector2f(this->axisWidth, this->height));
    this->axis.setFillColor(sf::Color(164, 164, 164, 150));
}

gui::Slider::Slider(StateData* stateData, std::string text, float x, float y, float axisWidth, float height,
                    float sliderWidth, int min, int max, float currentValue)
{
    this->posX = x;
    this->posY = y;
    this->axisWidth = axisWidth;
    this->height = height;
    this->sliderWidth = sliderWidth;
    this->height = height;
    this->minValue = min;
    this->maxValue = max;
    if (max <= min || axisWidth <= 0.f)
        throw std::invalid_argument("Invalid slider range");
    this->sliderValue = std::clamp(currentValue, static_cast<float>(min), static_cast<float>(max));

    this->initText(text);

    this->text.setCharacterSize(gui::callCharSize(stateData->gfxSettings->resolution, 80));

    this->initSlider();
}
gui::Slider::~Slider() = default;
float gui::Slider::getSliderValue()
{
    return this->sliderValue;
}

void gui::Slider::update(StateData* stateData)
{
    const auto mouse = sf::Vector2f(stateData->mousePosition);
    if (stateData->mouseClicked &&
        (axis.getGlobalBounds().contains(mouse) || slider.getGlobalBounds().contains(mouse)))
        dragging = true;
    if (!stateData->mouseDown)
        dragging = false;
    if (dragging)
    {
        const float x = std::clamp(mouse.x, posX, posX + axisWidth);
        slider.setPosition(x, posY);
        sliderValue = minValue + (x - posX) / axisWidth * (maxValue - minValue);
    }
}
void gui::Slider::render(sf::RenderTarget& target)
{
    target.draw(this->text);
    target.draw(this->axis);
    target.draw(this->slider);
}

gui::Bar::Bar(float x, float y, float width, float height, int maxValue, sf::VideoMode& vm, sf::Font& font,
              sf::Color color)
    : maxWidth(width), maxValue(maxValue), width(width), height(height), x(x), y(y)
{
    if (maxValue <= 0)
        throw std::invalid_argument("Invalid progress bar range");
    back.setPosition(x, y);
    back.setSize({width, height});
    back.setFillColor(sf::Color(50, 50, 50));
    inner.setPosition(x, y);
    inner.setSize({width, height});
    inner.setFillColor(color);
    barText.setFont(font);
    barText.setCharacterSize(gui::callCharSize(vm, 100));
    barText.setPosition(x + width / 3.f, y + height / 3.f);
    infoText.setFont(font);
    infoText.setCharacterSize(gui::callCharSize(vm, 100));
}

gui::Bar::~Bar() = default;

void gui::Bar::update(std::string label, float currentValue)
{
    const float value = std::clamp(currentValue, 0.f, static_cast<float>(maxValue));
    inner.setSize({maxWidth * value / maxValue, height});
    barText.setString(std::to_string(static_cast<int>(value)) + "/" + std::to_string(maxValue));
    infoText.setString(label);
    infoText.setPosition(x + maxWidth / 2.f - infoText.getGlobalBounds().width / 2.f, y - height / 2.f);
}

void gui::Bar::render(sf::RenderTarget& target)
{
    target.draw(back);
    target.draw(inner);
    target.draw(barText);
    target.draw(infoText);
}
