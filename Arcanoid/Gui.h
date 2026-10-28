#pragma once
#include "State.h"
#include "stdafx.h"
enum button_state
{
    BTN_IDLE = 0,
    BTN_HOVER,
    BTN_ACTIVE
};

namespace gui
{
float p2pX(const float perc, const sf::VideoMode& vm);
float p2pY(const float perc, const sf::VideoMode& vm);
unsigned callCharSize(const sf::VideoMode& vm, const float modifier = 60);

class Button
{
  private:
    short unsigned buttonState;
    short unsigned id;

    sf::RectangleShape shape;
    sf::RectangleShape icon;
    sf::Texture iconTexture;
    sf::IntRect textureRect;
    bool isIcon;
    bool isTextureRect;
    sf::Font* font;
    sf::Text text;
    sf::Cursor cursor;

    sf::Color idle_color;
    sf::Color hover_color;
    sf::Color active_color;

    sf::Color text_idle_color;
    sf::Color text_hover_color;
    sf::Color text_active_color;

    sf::Color outline_idle_color;
    sf::Color outline_hover_color;
    sf::Color outline_active_color;

    sf::SoundBuffer buffer;
    sf::Sound sound;

    void initSound();

  public:
    Button(float x, float y, float width, float height, sf::Font* font, std::string text,
           unsigned character_size, sf::Color text_idle_color, sf::Color text_hover_color,
           sf::Color text_active_color, sf::Color idle_color, sf::Color hover_color, sf::Color active_color,
           sf::Color outline_idle_color = sf::Color::Transparent,
           sf::Color outline_hover_color = sf::Color::Transparent,
           sf::Color outline_active_color = sf::Color::Transparent, short unsigned id = 0,
           sf::Texture* texture = nullptr, sf::IntRect* textureRect = nullptr);
    ~Button();
    void setText(const std::string text);
    void setId(const short unsigned id);
    void setIconTexture(const sf::Texture& texture);
    void setIconTextureRect(sf::IntRect textureRect);
    void setColor(sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
                  sf::Color idle_color, sf::Color hover_color, sf::Color active_color);
    bool isPressed() const;
    sf::Vector2f getPosition() const;
    unsigned short getId() const;
    std::string getText() const;
    sf::FloatRect getGlobalBounds() const;
    const sf::Texture& getIconTexture() const;
    sf::IntRect getIconTextureRect() const;

    void update(StateData* stateData, const sf::Vector2i mousePos);
    void render(sf::RenderTarget& target);
};
class DropDownList
{
  private:
    sf::Font& font;
    std::unique_ptr<gui::Button> activeElement;
    std::vector<std::unique_ptr<gui::Button>> list;
    bool showList;
    bool isIcon;
    bool isTextureRect;

  public:
    DropDownList(float x, float y, float width, float height, sf::VideoMode vm, sf::Font& font,
                 std::string list[], int nrOfElements, sf::Texture* textures = nullptr,
                 sf::IntRect* textureRect = nullptr, const int default_index = 0);
    ~DropDownList();

    unsigned short getActiveElementId() const;

    void update(StateData* stateData, const sf::Vector2i& mousePos, float dt);
    void render(sf::RenderTarget& target);
};
class TextureSelector
{
  private:
    float gridHeight;
    float gridWidth;
    bool active;
    bool hidden;
    std::unique_ptr<gui::Button> hide_btn;

    sf::RectangleShape bounds;
    sf::RectangleShape sheet;
    sf::RectangleShape selector;
    sf::Vector2u mousePosGrid;
    sf::IntRect textureRect;

  public:
    TextureSelector(float x, float y, float width, float height, float gridWidth, float gridHeight,
                    const sf::Texture* textureSheet, sf::Font& font, std::string text);
    ~TextureSelector();

    bool getActive() const;
    const sf::IntRect& getTextureRect() const;

    void update(StateData* stateData, const sf::Vector2i& mousePosWindow);
    void render(sf::RenderTarget& target);
};
class Slider
{
  private:
    sf::RectangleShape slider;
    sf::RectangleShape axis;
    sf::Font font;
    sf::Text text;
    int minValue;
    int maxValue;
    float posX;
    float posY;
    float axisWidth;
    float height;
    float sliderWidth;
    float sliderValue;
    float sliderPosX;
    bool dragging = false;

    void initText(std::string text);
    void initSlider();

  public:
    Slider(StateData* stateData, std::string text, float x, float y, float axisWidth, float height,
           float sliderWidth, int min, int max, float currentValue);
    ~Slider();
    float getSliderValue();
    void update(StateData* stateData);
    void render(sf::RenderTarget& target);
};
class Bar
{
  private:
    std::string barString;
    sf::Text barText;

    std::string infoString;
    sf::Text infoText;

    float maxWidth;
    int maxValue;
    float width;
    float height;
    float x;
    float y;
    sf::RectangleShape back;
    sf::RectangleShape inner;

  public:
    Bar(float x, float y, float width, float height, int maxValue, sf::VideoMode& vm, sf::Font& font,
        sf::Color color);
    ~Bar();

    void update(std::string bar, const float current_value);
    void render(sf::RenderTarget& target);
};
} // namespace gui
