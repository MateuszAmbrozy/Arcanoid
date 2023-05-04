#include "stdafx.h"
#include "Gui.h"

const float gui::p2pX(const float perc, const sf::VideoMode& vm)
{
	/*
	* Convets a percentage value to pixels relative to the current resolution in the x-accis.
	*
	* @param		float perc		The percentage value.
	*
	* @return		float			The calculater pixel value.
	*
	*/
	if (perc != 0)
		return std::floor(static_cast<float>(vm.width * (perc / 100.f)));
	else
		throw("DIVISTION BY ZERO\n");
}
const float gui::p2pY(const float perc, const sf::VideoMode& vm)
{	/*
	* Convets a percentage value to pixels relative to the current resolution in the y-accis.
	*
	* @param		float perc		The percentage value.
	*
	* @return		float			The calculater pixel value.
	*
	*/
	if (perc != 0)
		return std::floor(static_cast<float>(vm.height * (perc / 100.f)));
	else
		throw("DIVISTION BY ZERO\n");
}
const unsigned gui::callCharSize(const sf::VideoMode& vm, const unsigned modifier)
{
	/*
	* Calculates the charater size for text using the current resolution nd a constant.
	*
	* @param			sf::VideoMode		The current videomode of the window (resolution).
	* @param			unsigned modifier	Used to moodify the character size in a mores custom way.
	*
	* @return		unsigned			The calculated character size value.
	*
	*/
	if(modifier != 0)
		return  static_cast<unsigned>((vm.width + vm.height) / modifier);
	else
		throw("DIVISTION BY ZERO\n");
}

//****************************************************************************************************************************************//
//**************************	BUTTON	 *************************************************************************************************//
//****************************************************************************************************************************************//

//Private function
void gui::Button::initSound()
{
	if (!this->buffer.loadFromFile("Resources/Sounds/button.wav"))
		std::cout << "ERROR::BUTTON::INITSOUND::Could not load button.wav \n";
	this->sound.setBuffer(this->buffer);


}

//constructor and destructor
gui::Button::Button(float x, float y, float width, float height,
	sf::Font* font, std::string text, unsigned character_size,
	sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
	sf::Color idle_color, sf::Color hover_color, sf::Color active_color,
	sf::Color outline_idle_color, sf::Color outline_hover_color, sf::Color outline_active_color,
	short unsigned id,
	sf::Texture* texture, sf::IntRect* textureRect)
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
	this->text.setPosition
	(
		this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height
	);
	cursor.loadFromSystem(sf::Cursor::Hand);
	if (texture != nullptr)
	{
		this->iconTexture = *texture;



		this->icon.setSize(sf::Vector2f(this->shape.getGlobalBounds().width / 3, this->shape.getGlobalBounds().width / 3));
		this->icon.setTexture(texture);
		//this->icon.setScale(this->shape.getGlobalBounds().width / this->icon.getGlobalBounds().width, this->icon.getGlobalBounds().height / this->shape.getGlobalBounds().height);

		if (textureRect != nullptr)
		{
			this->icon.setSize(sf::Vector2f(this->shape.getGlobalBounds().width, this->shape.getGlobalBounds().height));
			this->textureRect = *textureRect;
			this->icon.setTextureRect(this->textureRect);

			this->isTextureRect = true;
		}

		this->icon.setPosition(
			sf::Vector2f(
				this->shape.getPosition().x + this->shape.getGlobalBounds().width / 2.f - this->icon.getGlobalBounds().width / 2.f,
				this->shape.getPosition().y
			)
		);

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
gui::Button::~Button()
{

}

//Modifiers
void gui::Button::setText(const std::string text)
{
	this->text.setString(text);
}
void gui::Button::setId(const short unsigned id)
{
	this->id = id;
}
void gui::Button::setIconTexture(sf::Texture texture)
{
	if(this->isIcon)
	{
		this->iconTexture = texture;
		this->icon.setTexture(&this->iconTexture);
	}
}
void gui::Button::setIconTextureRect(sf::IntRect textureRect)
{
	if(this->isTextureRect)
	{
		this->textureRect = textureRect;
		this->icon.setTextureRect(textureRect);
	}
}
void gui::Button::setColor(sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color, sf::Color idle_color, sf::Color hover_color, sf::Color active_color)
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

//Accessors
const bool gui::Button::isPressed() const 
{
	if (this->buttonState == BTN_ACTIVE)
		return true;


	return false;
}
const sf::Vector2f gui::Button::getPosition() const
{
	return this->shape.getPosition();
}
const short unsigned& gui::Button::getId() const
{
	return this->id;
}
const std::string gui::Button::getText() const
{
	return this->text.getString();
}
const sf::FloatRect gui::Button::getGlobalBounds() const
{
	return this->shape.getGlobalBounds();
}
const sf::Texture gui::Button::getIconTexture() const
{
	if (this->isIcon)
		return this->iconTexture;
	else
		return sf::Texture();
}
const sf::IntRect gui::Button::getIconTextureRect() const
{
	if (this->isTextureRect)
		return this->textureRect;
	else
		return sf::IntRect();
}

//Functions
void gui::Button::update(StateData* stateData, const sf::Vector2i mousePos)
{
	/*Update the booleand for hover and pressed*/

	//Idle
	this->buttonState = BTN_IDLE;

	//Hover
	if (this->shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
	{
		this->buttonState = BTN_HOVER;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if(!stateData->mute)
				this->sound.play();
			this->buttonState = BTN_ACTIVE;
		}
	}
	switch (this->buttonState)
	{
	case BTN_IDLE:
		this->shape.setFillColor(this->idle_color);
		this->text.setFillColor(this->text_idle_color);


		break;
	case BTN_HOVER:
		this->shape.setFillColor(this->hover_color);
		this->text.setFillColor(this->text_hover_color);

		break;
	case BTN_ACTIVE:
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
	if(this->isIcon)
		target.draw(this->icon);
}


//***************************************************************************************************************************************//
//**************************	DROP DOWN LIST	*****************************************************************************************//
//***************************************************************************************************************************************//

//CONSTRUCTOR AND DESTRUCTOR
gui::DropDownList::DropDownList(float x, float y, float width, float height, sf::VideoMode vm, sf::Font& font, std::string list[], int nrOfElements, sf::Texture* textures, sf::IntRect* textureRect, const int default_index)
	: font(font), showList(false), keyTimeMax(1.f), keyTime(0.f), isIcon(false), isTextureRect(false)
{
	
	if (textures)
	{
		if (textureRect)
		{
			this->activeElement = new gui::Button(x, y, width, height,
				&this->font, "", 15,
				sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50),
				sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
				sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 200),
				0, textures, &textureRect[0]);

			for (int i = 0; i < nrOfElements; i++)
			{
				this->list.push_back(new gui::Button(x, y + ((i + 1) * height), width, height,
					&this->font, "", 15,
					sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 200), sf::Color(20, 20, 20, 50),
					sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
					sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0), sf::Color(20, 20, 20, 0),
					i, textures, &textureRect[i]));
			}
			isTextureRect = true;

		}
		else
		{
			this->activeElement = new gui::Button(x, y, width, height,
				&this->font, "", 15,
				sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50),
				sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
				sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 200),
				0, &textures[0]);

			for (int i = 0; i < nrOfElements; i++)
			{
				this->list.push_back(new gui::Button(x, y + ((i + 1) * height), width, height,
					&this->font, "", 15,
					sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 200), sf::Color(20, 20, 20, 50),
					sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
					sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0), sf::Color(20, 20, 20, 0),
					i, &textures[i]));
			}

			this->isIcon = true;
		}

	}
	else
	{
		this->activeElement = new gui::Button(x, y, width, height,
			&this->font, list[default_index], 15,
			sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50),
			sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
			sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 200));

		for (int i = 0; i < nrOfElements; i++)
		{
			this->list.push_back(new gui::Button(x, y + ((i + 1) * height), width, height,
				&this->font, list[i], 15,
				sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 200), sf::Color(20, 20, 20, 50),
				sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
				sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0), sf::Color(20, 20, 20, 0),
				i));
		}
	}

		


}
gui::DropDownList::~DropDownList()
{
	delete this->activeElement;
	for (auto*& i : this->list)
		delete i;
}


//ACCESSORS
const bool gui::DropDownList::getKeyTime()
{
	if (this->keyTime >= this->keyTimeMax)
	{
		this->keyTime = 0.f;
		return true;
	}
	return false;
}
const unsigned short& gui::DropDownList::getActiveElementId() const
{
	return this->activeElement->getId();
}

//FUNCTIONS
void gui::DropDownList::updateKeyTime(const float& dt)
{
	if (this->keyTime < this->keyTimeMax)
	{
		this->keyTime += 10.f * dt;
	}
}
void gui::DropDownList::update(StateData* stateData, const sf::Vector2i& mousePos, const float& dt)
{
	this->updateKeyTime(dt);
	this->activeElement->update(stateData, static_cast<sf::Vector2i>(mousePos));

	if (this->activeElement->isPressed() && this->getKeyTime())
	{
			if (this->showList)
				this->showList = false;
			else
				this->showList = true;
	}

	if (this->showList)
	{
		for (auto& i : this->list)
		{
			i->update(stateData, static_cast<sf::Vector2i>(mousePos));

			if (i->isPressed() && this->getKeyTime())
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

//*******************************************************************************************************************************//
//**************************	TEXTURE SELECTOR	*****************************************************************************//
//*******************************************************************************************************************************//

//CONSTRUCTOR AND DESTRUCTOR
gui::TextureSelector::TextureSelector(float x, float y, float width, float height, float gridWidth, float gridHeight, const sf::Texture* textureSheet, sf::Font& font, std::string text)
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
		this->sheet.setTextureRect(sf::IntRect(0, 0, static_cast<int>(this->bounds.getGlobalBounds().width), static_cast<int>(this->bounds.getGlobalBounds().height)));
	}
	if (this->sheet.getGlobalBounds().height > this->bounds.getGlobalBounds().height)
	{
		this->sheet.setTextureRect(sf::IntRect(0, 0, static_cast<int>(this->sheet.getGlobalBounds().width), static_cast<int>(this->sheet.getGlobalBounds().height)));
	}

	this->selector.setPosition(x, y);
	this->selector.setSize(sf::Vector2f(gridWidth, gridHeight));
	this->selector.setFillColor(sf::Color::Transparent);
	this->selector.setOutlineThickness(-1.f);
	this->selector.setOutlineColor(sf::Color::Red);

	this->textureRect.width = static_cast<int>(gridWidth);
	this->textureRect.height = static_cast<int>(gridHeight);

	this->hide_btn = new gui::Button(x, y, gridWidth / 2.f, gridWidth / 2.f,
		&font, "", 24,
		sf::Color(70, 70, 70, 200), sf::Color(255, 255, 255, 250), sf::Color(255, 255, 255, 50),
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20, 50) );
}
gui::TextureSelector::~TextureSelector()
{
	delete this->hide_btn;
}

//ACCESSOR
const bool& gui::TextureSelector::getActive()
{
	return this->active;
}
const sf::IntRect& gui::TextureSelector::getTextureRect() const
{
	return this->textureRect;
}

//FUNCTIONS
void gui::TextureSelector::update(StateData* stateData, const sf::Vector2i& mousePosWindow)
{
	this->hide_btn->update(stateData, mousePosWindow);

	if (this->hide_btn->isPressed())
	{
		if (this->hidden)
		{
			Sleep(100);
			this->hidden = false;
		}

		else
		{
			Sleep(100);
			this->hidden = true;
		}

	}

	if (!this->hidden)
	{
		if (this->bounds.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)))
		{
			this->active = true;
		}
		else
		{
			this->active = false;
		}


		if (this->active)
		{
			this->mousePosGrid.x = (mousePosWindow.x - static_cast<int>(this->bounds.getPosition().x)) / static_cast<unsigned>(this->gridWidth); //ustawia grid na bounds
			this->mousePosGrid.y = (mousePosWindow.y - static_cast<int>(this->bounds.getPosition().y)) / static_cast<unsigned>(this->gridHeight);

			this->selector.setPosition //ustawia selector na grid na której jest myszka
			(
				this->bounds.getPosition().x + this->mousePosGrid.x * this->gridWidth,
				this->bounds.getPosition().y + this->mousePosGrid.y * this->gridHeight 
			);
		}
		//Update texture rectangle
		this->textureRect.left = static_cast<int>(this->selector.getPosition().x - this->bounds.getPosition().x);
		this->textureRect.top = static_cast<int>(this->selector.getPosition().y - this->bounds.getPosition().y);
		
	}
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

//*******************************************************************************************************************************//
//**************************	SLIDER	*****************************************************************************************//
//*******************************************************************************************************************************//

//PRIVATE FUNCTIONS
void gui::Slider::initText(std::string text)
{
	if (!this->font.loadFromFile("Fonts/Arialn.ttf"))
		std::cout << "ERROR::SLIDER::INITTEXT::Could not load font \n";
	this->text.setFont(this->font);
	this->text.setString(text);
	this->text.setPosition(this->posX, this->posY - this->text.getGlobalBounds().height * 2.f);
}
void gui::Slider::initSlider()
{
	this->sliderPosX = this->sliderValue * axisWidth / (this->maxValue - this->minValue) - this->minValue + this->posX/* -  this->sliderWidth / 2.f*/;
	this->slider.setPosition(this->sliderPosX, this->posY);
	
	this->slider.setSize(sf::Vector2f(this->sliderWidth, this->height));
	this->slider.setOrigin(slider.getGlobalBounds().width / 2, 0);
	this->slider.setFillColor(sf::Color(34, 34, 34));

	this->axis.setPosition(this->posX, this->posY);
	this->axis.setSize(sf::Vector2f(this->axisWidth, this->height));
	this->axis.setFillColor(sf::Color(164, 164, 164, 150));
}


//CONSTRUCTOR AND DESTRUCTOR
gui::Slider::Slider(StateData* stateData, std::string text, float x, float y, float axisWidth, float height, float sliderWidth, int min, int max, float currentValue)
{
	this->posX = x;
	this->posY = y;
	this->axisWidth = axisWidth;
	this->height = height;
	this->sliderWidth = sliderWidth;
	this->height = height;
	this->minValue = min;
	this->maxValue = max;
	this->sliderValue = currentValue;


	this->initText(text);
	
	this->text.setCharacterSize(gui::callCharSize(stateData->gfxSettings->resolution, 80));


	this->initSlider();
}
gui::Slider::~Slider()
{
}
float gui::Slider::getSliderValue()
{
	return this->sliderValue;
}

//FUNCTIONS
void gui::Slider::update(StateData* stateData)
{
	if (slider.getGlobalBounds().contains(sf::Mouse::getPosition(*stateData->window).x, sf::Mouse::getPosition(*stateData->window).y)
		&& sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (sf::Mouse::getPosition(*stateData->window).x >= this->posX && sf::Mouse::getPosition(*stateData->window).x <= this->posX + axisWidth)
		{
			slider.setPosition(sf::Mouse::getPosition(*stateData->window).x, this->posY);
			sliderValue = (minValue + ((slider.getPosition().x - this->posX) / axisWidth * (maxValue - minValue)));
		}
	}
}
void gui::Slider::render(sf::RenderTarget& target)
{
	target.draw(this->text);
	target.draw(this->axis);
	target.draw(this->slider);
}

//***************************************************************************************************************************************8//
//*************************	BAR	*****************************************************************************************8//
//****************************************************************************************************************************************8//

//PRIVATE FUNCTIONS


//CONSTRUCTOR AND DESTRUCTOR
gui::Bar::Bar(
	float x, float y, float width, float height,
	int maxValue,
	sf::VideoMode& vm, sf::Font& font, sf::Color color)
{

	this->width = width;
	this->height = height;
	this->x = x;
	this->y = y;
	this->maxValue = maxValue;
	this->maxWidth = width;


	this->back.setSize(sf::Vector2f(width, height));
	this->back.setFillColor(sf::Color(50, 50, 50));
	this->back.setPosition(x, y);

	this->inner.setSize(sf::Vector2f(width, height));
	this->inner.setFillColor(color);
	this->inner.setPosition(this->back.getPosition());

	this->barText.setCharacterSize(gui::callCharSize(vm, 100));
	this->barText.setFont(font);
	this->barText.setPosition(this->inner.getPosition().x + this->inner.getGlobalBounds().width/ 3, this->inner.getPosition().y + height / 3);

	this->infoText.setCharacterSize(gui::callCharSize(vm, 100));
	this->infoText.setFont(font);
	
}

gui::Bar::~Bar()
{
}

//FUNCTIONS
void gui::Bar::update(std::string bar, const float current_value)
{
	float percent = current_value / static_cast<float>(this->maxValue)/*static_cast<float>(max_value)*/;


	this->inner.setSize(
		sf::Vector2f
		(
			static_cast<float>(std::floor((this->maxWidth * percent))),
			static_cast<float>(std::floor((this->inner.getSize().y)))
		)
	);
	//std::to_string is expensive operation
	this->barString = std::to_string(static_cast<int>(current_value)) + "/" + std::to_string(this->maxValue);
	this->infoString = bar;
	this->infoText.setPosition(this->inner.getPosition().x + this->inner.getGlobalBounds().width / 2.f - infoText.getGlobalBounds().width / 2.f, this->inner.getPosition().y - height / 2);
	this->infoText.setString(bar);
	this->barText.setString(this->barString);
}
void gui::Bar::render(sf::RenderTarget& target)
{
	target.draw(this->back);
	target.draw(this->inner);
	target.draw(this->barText);
	target.draw(this->infoText);
}