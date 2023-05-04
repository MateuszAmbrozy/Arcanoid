#pragma once
class Timer
{
private:
	//VARIABLES
	clock_t start_;
	clock_t end_;

	sf::Text menuText;
	sf::Font font;

	sf::Text text;

	void initFont();

public:



	//CONSTRUCTOR AND DESTRUCTOR
	Timer();
	virtual ~Timer();

	//MODIFIERS
	void setPosition(sf::Vector2f position);
	void setString(std::string str);
	void setCharacterSize(unsigned int charSize);
	//ACCESSORS
	int getElapsedTime();

	//FUNCTIONS
	void start();
	void reset();
	void stop();
	void resume();
	


	void update(const float& dt);
	void render(sf::RenderTarget& target);
};

