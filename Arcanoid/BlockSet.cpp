#include "stdafx.h"
#include "BlockSet.h"

//PRIVATE FUNCTIONS
void BlockSet::clear()
{
	if (!blocks.empty())
	{
		for (auto& row : blocks)
		{
			for (auto& elem : row)
			{
				if (elem != nullptr)
					delete elem;
			}
			row.clear();
		}
		blocks.clear();
	}

}
void BlockSet::initAnimationComponent()
{
	if (!this->destTexture.loadFromFile("Resources/Animations/destroyBlock.png"))
	{
		std::cout << "ERROR::BLOCKSET::INITANIMATIONCOMPONENT::Could not load destroyBlock \n";
	}

	this->doAnimation = false;
	this->timer = 0;
	this->animation_timer = 100.f;
	this->currentRect = sf::IntRect(0, 0, static_cast<int>(this->gridWidth) * 1920 / vm.width, static_cast<int>(this->gridHeight) * 1080 / vm.height);
	this->destSprite.setTexture(destTexture);

	this->destSprite.setScale(
		this->gridWidth * 7 / destSprite.getLocalBounds().width,
		this->gridHeight * 6 / destSprite.getLocalBounds().height
	);

	this->destSprite.setTextureRect(this->startRect);

}
void BlockSet::initSound(StateData* stateData)
{
	if (!this->buffer.loadFromFile("Resources/Sounds/w.wav"))
		std::cout << "ERROR::BLOCKSET::INITSOUND::Could not load sound \n";
	this->collisionSound.setBuffer(this->buffer);
	this->collisionSound.setVolume(stateData->collisionVolume);
}

//CONSTRUCTOR AND DESCTRUCTOR
BlockSet::BlockSet(StateData* stateData, float gridWidth, float gridHeight, int width, int height, std::string textureFile)
{
	this->gridWidth = gridWidth;
	this->gridHeight = gridHeight;
	this->maxSizeWorldGrid.x = width;
	this->maxSizeWorldGrid.y = height;
	this->textureFile = textureFile;
	this->layers = 1;
	this->layer = 0;
	this->fromX = 0;
	this->toX = 0;
	this->fromY = 0;
	this->toY = 0;
	this->vm = stateData->gfxSettings->resolution;
	this->stateData = stateData;

	this->update();

	if (!this->tileTextureSheet.loadFromFile(textureFile))
		throw("ERROR::TILEMAP::Could not load the texture 'blocks.png' \n ");

}
BlockSet::BlockSet(StateData* stateData, b2World* world,  const std::string fileName, float gridWidth, float gridHeight) //Game
	:world(world)
{
	this->gridWidth = gridWidth;
	this->gridHeight = gridHeight;
	this->vm = stateData->gfxSettings->resolution;
	this->stateData = stateData;
	this->fromX = 0;
	this->toX = 0;
	this->fromY = 0;
	this->toY = 0;
	this->fileName = fileName;
	this->loadFromFile(fileName, this->world);
	this->initAnimationComponent();
	this->initSound(stateData);
}
BlockSet::~BlockSet()
{
	this->clear();
	for (auto power : powers)
	{
		delete power;
	}
}

//ACCESSORS
const sf::Texture* BlockSet::getBlockSheet() const
{
	return &this->tileTextureSheet;
}
const sf::Vector2i& BlockSet::getMaxSizeGrid() const
{
	return this->maxSizeWorldGrid;
}
bool BlockSet::checkEndGame()
{
	for (int x = 0; x < this->maxSizeWorldGrid.x; x++)
		for (int y = 0; y < this->maxSizeWorldGrid.y; y++)
			if (blocks[x][y] != nullptr && this->blocks[x][y]->getType() != BlockTypes::UNBREAKING)
				return false;
	return true;
}
std::string BlockSet::getFileName()
{
	return this->fileName;
}

//FUNCTIONS
void BlockSet::collisionBallBlock(StateData* stateData, std::vector<Ball*> balls, Paddle* paddle, const float& dt)
{
	for (auto& ball : balls)
	{
		this->fromX = ball->getGridPosition(static_cast<int>(this->gridWidth) - 1, static_cast<int>(this->gridHeight)).x - 1;
		if (this->fromX < 0)
			this->fromX = 0;
		else if (this->fromX >= this->maxSizeWorldGrid.x)
			this->fromX = this->maxSizeWorldGrid.x;

		this->toX = ball->getGridPosition(static_cast<int>(this->gridWidth) + 3, static_cast<int>(this->gridHeight)).x + 3;
		if (this->toX < 0)
			this->toX = 0;
		else if (this->toX >= this->maxSizeWorldGrid.x)
			this->toX = this->maxSizeWorldGrid.x;

		this->fromY = ball->getGridPosition(static_cast<int>(this->gridWidth) - 1, static_cast<int>(this->gridHeight)).y - 1;
		if (this->fromY < 0)
			this->fromY = 0;
		else if (this->fromY >= this->maxSizeWorldGrid.y)
			this->fromY = this->maxSizeWorldGrid.y;

		this->toY = ball->getGridPosition(static_cast<int>(this->gridWidth) + 3, static_cast<int>(this->gridHeight)).y + 3;
		if (this->toY < 0)
			this->toY = 0;
		else if (this->toY >= this->maxSizeWorldGrid.y)
			this->toY = this->maxSizeWorldGrid.y;

		for (int x = this->fromX; x < this->toX; x++)
		{
			for (int y = this->fromY; y < this->toY; y++)
			{
				if (blocks[x][y] != nullptr)

				{
					UserData* blockData = (UserData*)blocks[x][y]->GetFixtureDef()->userData.pointer;
					if (blockData->hit)
					{
						if (!stateData->mute)
							this->collisionSound.play();

						if (blocks[x][y]->getLifes() > 0 && this->blocks[x][y]->getType() == BlockTypes::DEFAULT)
						{
							this->blocks[x][y]->setLifes(this->blocks[x][y]->getLifes() - 1);
							this->blocks[x][y]->setTextureRect(
							sf::IntRect(
								this->blocks[x][y]->getRect().left,
								this->blocks[x][y]->getRect().top + 50,
								this->blocks[x][y]->getRect().width,
								this->blocks[x][y]->getRect().height
								)
							);
						}

						else if (this->blocks[x][y]->getType() == BlockTypes::DEFAULT ||
							this->blocks[x][y]->getType() == BlockTypes::POWER)
						{
							if (this->blocks[x][y]->getLifes() < 1)
							{


								//if type is power block -  push super power
								if (this->blocks[x][y]->getType() == BlockTypes::POWER)
								{
									this->powers.push_back(new Powers(
										stateData,
										sf::Vector2f(
											this->blocks[x][y]->getPosition().x + this->blocks[x][y]->getGlobalBounds().width / 2.f,
											this->blocks[x][y]->getPosition().y + this->blocks[x][y]->getGlobalBounds().height / 2.f
										)));

								}

									//configurtions to destroing animation
								if (this->blocks[x][y]->getType() == BlockTypes::DEFAULT)
									this->doAnimation = true;
								this->destSprite.setPosition(sf::Vector2f(this->blocks[x][y]->getPosition()));
								this->currentRect.left = this->blocks[x][y]->getRect().left;

								//remove
								this->world->DestroyBody(this->blocks[x][y]->GetBody());
								this->blocks[x][y] = nullptr;
								}

						}

						
						blockData->hit = false;
						}
								
					}

				}



			}

		if (doAnimation)
		{
			if (this->currentRect.top >= static_cast<int>(this->gridHeight * 5))
			{
				this->currentRect.top = 0;
				this->doAnimation = false;
			}

			else
			{
				this->timer += (30.f + dt);
				if (this->timer >= this->animation_timer)
				{
					this->timer = 0.f;
					this->currentRect.top += static_cast<int>(this->gridHeight);
					this->destSprite.setTextureRect(this->currentRect);
				}
			}
		}
	}
}
void BlockSet::addBlock(const int x, const int y, int lifes,
	const sf::IntRect& textureRect, const short& type)
{
	if (x < this->maxSizeWorldGrid.x && x >= 0 &&
		y < this->maxSizeWorldGrid.y && y >= 0 &&
		this->blocks[x][y] == nullptr)
	{
		this->blocks[x][y] = new Block(this->stateData, x, y, this->gridWidth, this->gridHeight, lifes, this->tileTextureSheet, textureRect, type);
	}

}
void BlockSet::removeBlock(const int x, const int y)
{
	if (x < this->maxSizeWorldGrid.x && x >= 0 &&
		y < this->maxSizeWorldGrid.y && y >= 0)
	{
		if (this->blocks[x][y] != nullptr)
		{
			this->blocks[x][y] = nullptr;
		}

	}
}
void BlockSet::saveToFile(const std::string fileName)
{
	/*
	Saved the entire tileblocks to text - file
	Format:
		Basic:
			Size x y
			gridWidth gridHeight
			texture file
		All tiles:
			gridPos x y (all tiles), Texture rect x y, type
	*/
	std::ofstream out_file;

	out_file.open(fileName);

	if (out_file.is_open())
	{
		out_file << this->maxSizeWorldGrid.x << " " << this->maxSizeWorldGrid.y << "\n"
			<< this->textureFile << "\n";


		for (int x = 0; x < maxSizeWorldGrid.x; x++)
		{
			for (int y = 0; y < this->maxSizeWorldGrid.y; y++)
			{

				if (this->blocks[x][y] != nullptr)
				{
					out_file << x << " " << y << " " << this->blocks[x][y]->getAssString() << " ";
				}
			}
		}

	}
	else
		std::cout << "ERROR::TILEMAP::Could not save to file::FILENAME: " << fileName << std::endl;

	out_file.close();
}
void BlockSet::loadFromFile(const std::string fileName, b2World* world)
{
	std::ifstream in_file;

	in_file.open(fileName);

	if (in_file.is_open())
	{
		sf::Vector2i size;
		int x = 0, y = 0, trX = 0, trY = 0, left = 0, top = 0;
		std::string textureFile = "";
		int lifes = 1;
		short type = 0;


		//BASICS
		in_file >> size.x >> size.y >> textureFile;

		this->maxSizeWorldGrid.x = size.x;
		this->maxSizeWorldGrid.y = size.y;
		this->textureFile = textureFile;

		this->clear();
		this->update();

		if (!this->tileTextureSheet.loadFromFile(textureFile))
			std::cout << "ERROR::TILEMAP::Could not load the texture FILENAME: " << textureFile << "\n ";

		//LOAD ALL TILES
		while (in_file >> x >> y >> left >> top >> trX >> trY >> lifes >> type)
		{
			if(world)
				this->blocks[x][y] = new Block(this->stateData, x, y, left, top, this->gridWidth, this->gridHeight, lifes, this->tileTextureSheet, sf::IntRect(trX, trY, static_cast<int>(this->gridWidth), static_cast<int>(this->gridHeight)), type, this->world);
			else
				this->blocks[x][y] = new Block(this->stateData, x, y, left, top, this->gridWidth, this->gridHeight, lifes, this->tileTextureSheet, sf::IntRect(trX, trY, static_cast<int>(this->gridWidth), static_cast<int>(this->gridHeight)), type);

		}
	}
	else
		std::cout << "ERROR::TILEMAP::Could not from from file::FILENAME: " << fileName << std::endl;

	in_file.close();
}
void BlockSet::startSuperPower(StateData* stateData, Paddle* paddle, std::vector<Ball*>* balls)
{
	for (int i = 0; i < this->powers.size(); i++)
	{
		if (powers[i]->intersects(paddle->getGlobalBounds()))
		{
			switch (powers[i]->getType())
			{
			case BIGGERPADDLE:
				powers[i]->initBiggerPaddle(paddle);
				break;
			case SMALLPADDLE:
				powers[i]->initSmallerPaddle(paddle);
				break;
			case NEWBALLS:
				powers[i]->initNewBalls(stateData, this->world, balls);
				break;
			case NEWBALLFROMPADDLE:
				powers[i]->initNewBallsFromPaddle(stateData, this->world, paddle, balls);
				break;
			case FASTBALL:
				powers[i]->initFastBalls(balls);
				break;
			case SLOWBALL:
				powers[i]->initSlowBalls(balls);
				break;
			case VICEVERSA:
				stateData->inversePaddle = true;
				stateData->clock.restart();
				break;
			}
			delete powers[i];
			this->powers.erase(this->powers.begin() + i);
		}
		else if (powers[i]->getGlobalBounds().top > paddle->getGlobalBounds().top + paddle->getGlobalBounds().height * 3.f)
		{
			delete powers[i];
			this->powers.erase(this->powers.begin() + i);
		}
	}
}
void BlockSet::updateSuperPower(StateData* stateData, const float& dt)
{
	for (auto& power : this->powers)
	{
		power->update(dt);
	}
}
void BlockSet::update()
{
	this->blocks.resize(this->maxSizeWorldGrid.x, std::vector<Block*>(maxSizeWorldGrid.y));
}
void BlockSet::render(sf::RenderTarget& target)
{
	if (doAnimation)
	{
		target.draw(this->destSprite);
	}
	for (auto& power : this->powers)
		power->render(target);

	for (auto& x : this->blocks)
		for (auto& y : x)
		{
			if (y != nullptr)
				y->render(target);
		}
}




