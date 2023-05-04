#pragma once
class GraphicsSettings
{
public:
	//Variables
	std::string title;
	sf::VideoMode resolution;
	bool fullScreen;
	bool verticalSync;
	unsigned frameRateLimit;
	sf::ContextSettings contextSettings;
	std::vector<sf::VideoMode> videoModes;

	//CONSTRUCTOR
	GraphicsSettings();

	//FUNCTIONS
	void saveToFile(const std::string path);
	void loadFromFile(const std::string path);

};

