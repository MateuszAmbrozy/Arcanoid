#pragma once
#include "Ball.h"
#include "Block.h"
#include "Gui.h"
#include "Paddle.h"
#include "Powers.h"
#include "stdafx.h"

class BlockSet
{
  private:
    void initAnimationComponent();
    void initSound(StateData* stateData);

    StateData* stateData;

    float gridWidth;
    float gridHeight;

    sf::Vector2i maxSizeWorldGrid;
    std::string textureFile;
    std::string fileName;
    std::vector<std::vector<std::unique_ptr<Block>>> blocks;
    std::vector<std::unique_ptr<Powers>> powers;
    sf::Texture tileTextureSheet;
    sf::VideoMode vm;

    sf::Texture destTexture;
    sf::Sprite destSprite;
    sf::IntRect currentRect;

    float timer;

    bool doAnimation = false;

    sf::SoundBuffer buffer;
    sf::Sound collisionSound;

    b2World* world = nullptr;

  public:
    BlockSet(StateData* stateData, float gridWidth, float gridHeight, int width, int height,
             std::string textureFile);
    BlockSet(StateData* stateData, b2World* world, const std::string fileName, float gridWidth,
             float gridHeight);
    ~BlockSet();

    const sf::Texture* getBlockSheet() const;

    const sf::Vector2i& getMaxSizeGrid() const;
    std::string getFileName() const;
    bool checkEndGame() const;

    void collisionBallBlock(StateData* stateData, float dt);
    void startSuperPower(StateData* stateData, Paddle* paddle, std::vector<std::unique_ptr<Ball>>* balls);
    void addBlock(const int x, const int y, int lives, const sf::IntRect& textureRect, const short& type);
    void removeBlock(const int x, const int y);

    void saveToFile(const std::string fileName);
    void loadFromFile(const std::string fileName, b2World* world = nullptr);

    void updateSuperPower(StateData* stateData, float dt);
    void update();
    void render(sf::RenderTarget& target);
};
