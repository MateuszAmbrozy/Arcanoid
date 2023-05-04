#pragma once
#include "State.h"
enum button_state { BTN_IDLE = 0, BTN_HOVER, BTN_ACTIVE };

namespace gui
{
	const float p2pX(const float perc, const sf::VideoMode& vm);//percent too pixels X
	const float p2pY(const float perc, const sf::VideoMode& vm);//percent too pixels X
	const unsigned callCharSize(const sf::VideoMode& vm, const unsigned modifier = 60);

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

		sf::Color idle_color; //idle - bezczynny
		sf::Color hover_color; //hover - wahaæ siê 
		sf::Color active_color; //active - aktywny

		sf::Color text_idle_color;
		sf::Color text_hover_color;
		sf::Color text_active_color;

		sf::Color outline_idle_color; //idle - bezczynny
		sf::Color outline_hover_color; //hover - wahaæ siê 
		sf::Color outline_active_color; //active - aktywny

		sf::SoundBuffer buffer;
		sf::Sound sound;

		void initSound();
	public:
		//constructor and destructor
		Button(float x, float y, float width, float height,
			sf::Font* font, std::string text, unsigned character_size,
			sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
			sf::Color idle_color, sf::Color hover_color, sf::Color active_color,
			sf::Color outline_idle_color = sf::Color::Transparent, sf::Color outline_hover_color = sf::Color::Transparent, sf::Color outline_active_color = sf::Color::Transparent,
			short unsigned id = 0,
			sf::Texture* texture = nullptr, sf::IntRect* textureRect = nullptr);
		~Button();
		//MODIFIERS
		void setText(const std::string text);
		void setId(const short unsigned id);
		void setIconTexture(sf::Texture texture);
		void setIconTextureRect(sf::IntRect textureRect);
		void setColor(sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
					sf::Color idle_color, sf::Color hover_color, sf::Color active_color);
		//Accessors
		const bool isPressed() const;
		const sf::Vector2f getPosition() const;
		const short unsigned& getId() const;
		const std::string getText() const;
		const sf::FloatRect getGlobalBounds() const;
		const sf::Texture getIconTexture() const;
		const sf::IntRect getIconTextureRect() const;
		
		//Functions
		void update(StateData* stateData, const sf::Vector2i mousePos);
		void render(sf::RenderTarget& target);
	};
	class DropDownList
	{
	private:
		float keyTime;
		float keyTimeMax;
		sf::Font& font;
		gui::Button* activeElement;
		std::vector<gui::Button*> list;
		bool showList;
		bool isIcon;
		bool isTextureRect;

	public:
		//CONSTRUCTOR AND DESBRUCTOR	
		DropDownList(float x, float y, float width, float height, sf::VideoMode vm, sf::Font& font, std::string list[], int nrOfElements, sf::Texture* textures = nullptr, sf::IntRect* textureRect = nullptr, const int default_index = 0);
		~DropDownList();

		//ACCESSORS
		const unsigned short& getActiveElementId() const;

		//MODIFIERS

		//FUNCTIONCS
		const bool getKeyTime();
		void updateKeyTime(const float& dt);
		void update(StateData* stateData, const sf::Vector2i& mousePos, const float& dt);
		void render(sf::RenderTarget& target);
	};
	class TextureSelector
	{
	private:
		float gridHeight;
		float gridWidth;
		bool active; //sprawdza bounds zawiera myszke 
		bool hidden;
		gui::Button* hide_btn;

		sf::RectangleShape bounds; //prostok¹t w którym narysowane s¹ textury
		sf::RectangleShape sheet; //sprite na tekstury 
		sf::RectangleShape selector; //czerwony prostok¹cik który renderuje siê na p³ytce na której jest myszka
		sf::Vector2u mousePosGrid; //pozycja p³ytek 
		sf::IntRect textureRect; //zmienna do ustawiania rectu na teksturze która bêdzie siê wstawia³o


	public:
		//CONSRUCTOR AND DESTRUCTOR
		TextureSelector(float x, float y, float width, float height, float gridWidth, float gridHeight, const sf::Texture* textureSheet, sf::Font& font, std::string text);
		~TextureSelector();

		//ACCESSOR
		const bool& getActive();
		const sf::IntRect& getTextureRect() const;

		//Functions


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
		float sliderWidth;;
		float sliderValue;
		float sliderPosX;

		//Private functions
		void initText(std::string text);
		void initSlider();
	public:
		Slider(StateData* stateData, std::string text, float x, float y, float axisWidth, float height, float sliderWidth, int min, int max, float currentValue);
		~Slider();
		//Accessors
		float getSliderValue();
		//Functions
		void update(StateData* stateData);
		void render(sf::RenderTarget& target);
	};
	//PROGRES BAR
	class Bar
	{
	private:
		//VARIABLES
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

		//PRIVATE FUNCTIONS
	public:
		//CONSTRUCTOR AND DESTRUCTOR
		Bar(
			float x, float y, float width, float height, int maxValue,
			sf::VideoMode& vm, sf::Font& font, sf::Color color);
		~Bar();

		//FUNCTIONS
		void update(std::string bar, const float current_value);
		void render(sf::RenderTarget& target);
	};
}

