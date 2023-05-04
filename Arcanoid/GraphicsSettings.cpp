#include "stdafx.h"
#include "GraphicsSettings.h"

GraphicsSettings::GraphicsSettings()
{
	this->title = "DEFAULT";
	this->resolution = sf::VideoMode::getDesktopMode();
	this->fullScreen = false;
	this->verticalSync = false;
	this->frameRateLimit = 30;
	this->contextSettings.antialiasingLevel = 10;
	//this->videoModes = sf::VideoMode::getFullscreenModes();
}


void GraphicsSettings::saveToFile(const std::string path)
{
	std::ofstream ofs(path.c_str());
	if (ofs.is_open())
	{
		ofs << this->title;
		ofs << this->resolution.width << " " << this->resolution.height;
		ofs << this->fullScreen;
		ofs << this->frameRateLimit;
		ofs << this->verticalSync;
		ofs << this->contextSettings.antialiasingLevel;
	}
	ofs.close();
}
void GraphicsSettings::loadFromFile(const std::string path)
{
	std::ifstream ifs(path.c_str());
	if (ifs.is_open())
	{
		std::getline(ifs, this->title);
		ifs >> this->resolution.width >> this->resolution.height;
		ifs >> this->fullScreen;
		ifs >> this->frameRateLimit;
		ifs >> this->verticalSync;
		ifs >> this->contextSettings.antialiasingLevel;
	}
	ifs.close();
}
