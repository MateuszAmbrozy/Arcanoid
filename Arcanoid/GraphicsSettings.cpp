#include "GraphicsSettings.h"
#include "stdafx.h"

GraphicsSettings::GraphicsSettings()
    : title("Arcanoid"), resolution(1280, 720), fullScreen(false), verticalSync(false), frameRateLimit(60)
{
    contextSettings.antialiasingLevel = 0;
}

void GraphicsSettings::saveToFile(const std::string path)
{
    std::ofstream output(path);
    if (!output)
        throw std::runtime_error("Cannot save graphics settings: " + path);
    output << title << '\n'
           << resolution.width << ' ' << resolution.height << '\n'
           << fullScreen << '\n'
           << frameRateLimit << '\n'
           << verticalSync << '\n'
           << contextSettings.antialiasingLevel << '\n';
    output.flush();
    if (!output)
        throw std::runtime_error("Graphics settings write failed: " + path);
}

void GraphicsSettings::loadFromFile(const std::string path)
{
    std::ifstream input(path);
    if (!input)
        return;
    GraphicsSettings candidate;
    if (!std::getline(input, candidate.title) ||
        !(input >> candidate.resolution.width >> candidate.resolution.height >> candidate.fullScreen >>
          candidate.frameRateLimit >> candidate.verticalSync >>
          candidate.contextSettings.antialiasingLevel) ||
        candidate.title.empty() || candidate.resolution.width < 640 || candidate.resolution.width > 7680 ||
        candidate.resolution.height < 480 || candidate.resolution.height > 4320 ||
        candidate.frameRateLimit < 30 || candidate.frameRateLimit > 1000 ||
        candidate.contextSettings.antialiasingLevel > 16)
        throw std::runtime_error("Invalid graphics settings: " + path);
    *this = std::move(candidate);
}
