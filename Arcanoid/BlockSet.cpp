#include "BlockSet.h"
#include "Assets.h"
#include "MapFile.h"
#include "stdafx.h"

void BlockSet::initAnimationComponent()
{
    assets::load(destTexture, "Resources/Animations/destroyBlock.png");
    destSprite.setTexture(destTexture);
    currentRect = sf::IntRect(0, 0, static_cast<int>(destTexture.getSize().x / 7), 50);
    destSprite.setTextureRect(currentRect);
    destSprite.setScale(gridWidth / currentRect.width, gridHeight / currentRect.height);
    timer = 0.f;
    doAnimation = false;
}

void BlockSet::initSound(StateData* data)
{
    assets::load(buffer, "Resources/Sounds/w.wav");
    collisionSound.setBuffer(buffer);
    collisionSound.setVolume(data->collisionVolume);
}

BlockSet::BlockSet(StateData* data, float cellWidth, float cellHeight, int width, int height,
                   std::string texture)
    : stateData(data), gridWidth(cellWidth), gridHeight(cellHeight), maxSizeWorldGrid(width, height),
      textureFile(std::move(texture)), vm(data->gfxSettings->resolution)
{
    if (!tileTextureSheet.loadFromFile(textureFile))
        throw std::runtime_error("Cannot load block texture: " + textureFile);
    update();
}

BlockSet::BlockSet(StateData* data, b2World* physicsWorld, const std::string filename, float cellWidth,
                   float cellHeight)
    : stateData(data), gridWidth(cellWidth), gridHeight(cellHeight), vm(data->gfxSettings->resolution),
      world(physicsWorld)
{
    loadFromFile(filename, world);
    initAnimationComponent();
    initSound(data);
}

BlockSet::~BlockSet() = default;

const sf::Texture* BlockSet::getBlockSheet() const
{
    return &tileTextureSheet;
}
const sf::Vector2i& BlockSet::getMaxSizeGrid() const
{
    return maxSizeWorldGrid;
}
std::string BlockSet::getFileName() const
{
    return fileName;
}

bool BlockSet::checkEndGame() const
{
    for (const auto& column : blocks)
        for (const auto& block : column)
            if (block && block->getType() != BlockTypes::UNBREAKING)
                return false;
    return true;
}

void BlockSet::collisionBallBlock(StateData* data, float dt)
{
    // Process each contact once; scanning once also handles simultaneous multi-ball hits.
    for (auto& column : blocks)
    {
        for (auto& block : column)
        {
            if (!block || !block->getFixtureDefinition())
                continue;
            auto* contact = reinterpret_cast<UserData*>(block->getFixtureDefinition()->userData.pointer);
            if (!contact || !contact->hit)
                continue;
            contact->hit = false;
            if (!data->mute)
                collisionSound.play();
            if (block->getType() == BlockTypes::UNBREAKING)
                continue;
            if (block->getLives() > 0)
            {
                block->setLives(block->getLives() - 1);
                auto rect = block->getRect();
                rect.top = std::min(rect.top + 50, 250);
                block->setTextureRect(rect);
                continue;
            }
            if (block->getType() == BlockTypes::POWER)
                powers.push_back(std::make_unique<Powers>(
                    data, block->getPosition() + sf::Vector2f(gridWidth / 2.f, gridHeight / 2.f)));
            doAnimation = true;
            timer = 0.f;
            currentRect.left = std::min(block->getRect().left / 100, 6) * currentRect.width;
            currentRect.top = 0;
            destSprite.setTextureRect(currentRect);
            destSprite.setPosition(block->getPosition());
            block.reset();
        }
    }
    if (doAnimation)
    {
        timer += dt;
        while (timer >= 1.f / 30.f && doAnimation)
        {
            timer -= 1.f / 30.f;
            currentRect.top += 50;
            if (currentRect.top >= 300)
                doAnimation = false;
            else
                destSprite.setTextureRect(currentRect);
        }
    }
}

void BlockSet::addBlock(int x, int y, int lives, const sf::IntRect& rect, const short& type)
{
    if (x < 0 || y < 0 || x >= maxSizeWorldGrid.x || y >= maxSizeWorldGrid.y || blocks[x][y])
        return;
    blocks[x][y] =
        std::make_unique<Block>(stateData, x, y, gridWidth, gridHeight, lives, tileTextureSheet, rect, type);
}

void BlockSet::removeBlock(int x, int y)
{
    if (x >= 0 && y >= 0 && x < maxSizeWorldGrid.x && y < maxSizeWorldGrid.y)
        blocks[x][y].reset();
}

void BlockSet::saveToFile(const std::string filename)
{
    std::ofstream output(filename);
    if (!output)
        throw std::runtime_error("Cannot save map: " + filename);
    output << maxSizeWorldGrid.x << ' ' << maxSizeWorldGrid.y << '\n' << textureFile << '\n';
    for (int x = 0; x < maxSizeWorldGrid.x; ++x)
        for (int y = 0; y < maxSizeWorldGrid.y; ++y)
            if (blocks[x][y])
                output << x << ' ' << y << ' ' << blocks[x][y]->serialize() << '\n';
    output.flush();
    if (!output)
        throw std::runtime_error("Map write failed: " + filename);
}

void BlockSet::loadFromFile(const std::string filename, b2World* physicsWorld)
{
    const auto map = maps::load(filename);
    sf::Texture texture;
    if (!texture.loadFromFile(map.texture))
        throw std::runtime_error("Cannot load map texture: " + map.texture);

    // Build a replacement grid first so invalid input never destroys the open map.
    std::vector<std::vector<std::unique_ptr<Block>>> replacement(map.width);
    for (auto& column : replacement)
        column.resize(map.height);
    for (const auto& tile : map.tiles)
        replacement[tile.x][tile.y] = std::make_unique<Block>(
            stateData, tile.x, tile.y, tile.column, tile.row, gridWidth, gridHeight, tile.lives,
            tileTextureSheet, sf::IntRect(tile.textureX, tile.textureY, 100, 50),
            static_cast<short>(tile.type), physicsWorld);
    tileTextureSheet = texture;
    blocks.swap(replacement);
    powers.clear();
    maxSizeWorldGrid = {map.width, map.height};
    textureFile = map.texture;
    fileName = filename;
    world = physicsWorld;
    doAnimation = false;
}

void BlockSet::startSuperPower(StateData* data, Paddle* paddle, std::vector<std::unique_ptr<Ball>>* balls)
{
    for (auto it = powers.begin(); it != powers.end();)
    {
        auto& power = *it;
        const bool caught = power->intersects(paddle->getGlobalBounds());
        if (caught)
        {
            switch (power->getType())
            {
            case BIGGERPADDLE:
                power->initBiggerPaddle(paddle);
                break;
            case SMALLPADDLE:
                power->initSmallerPaddle(paddle);
                break;
            case NEWBALLS:
                power->initNewBalls(data, world, balls);
                break;
            case NEWBALLFROMPADDLE:
                power->initNewBallsFromPaddle(data, world, paddle, balls);
                break;
            case FASTBALL:
                power->initFastBalls(balls);
                break;
            case SLOWBALL:
                power->initSlowBalls(balls);
                break;
            case VICEVERSA:
                data->inversePaddle = true;
                data->inversePaddleElapsed = 0.f;
                break;
            }
        }
        if (caught || power->getGlobalBounds().top > vm.height)
            it = powers.erase(it);
        else
            ++it;
    }
}

void BlockSet::updateSuperPower(StateData* /*data*/, float dt)
{
    for (auto& power : powers)
        power->update(dt);
}

void BlockSet::update()
{
    blocks.resize(maxSizeWorldGrid.x);
    for (auto& column : blocks)
        column.resize(maxSizeWorldGrid.y);
}

void BlockSet::render(sf::RenderTarget& target)
{
    for (const auto& column : blocks)
        for (const auto& block : column)
            if (block)
                block->render(target);
    if (doAnimation)
        target.draw(destSprite);
    for (auto& power : powers)
        power->render(target);
}
