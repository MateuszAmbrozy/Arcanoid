#include "stdafx.h"
#include "GameState.h"

//PRIVATE FUNCTIONS
void GameState::initVariables()
{
	this->startGame = false;
	this->vm = this->stateData->gfxSettings->resolution;
	this->renderEnd = false;
	this->paddleIsMoving = false;
}
void GameState::initB2World()
{
	this->world = new b2World(b2Vec2(0.f, 0.f));
}
void GameState::initB2WorldBounds()
{
	float  widthInMeters = stateData->gfxSettings->resolution.width / SCALE;
	float  heightInMeters = stateData->gfxSettings->resolution.height / SCALE;
	b2Vec2 lowerLeftCorner = b2Vec2(0, 0);
	b2Vec2 lowerRightCorner = b2Vec2(widthInMeters, 0);
	b2Vec2 upperLeftCorner = b2Vec2(0, heightInMeters);
	b2Vec2 upperRightCorner = b2Vec2(widthInMeters, heightInMeters);

	// static container body, with the collisions at screen borders
	b2BodyDef screenBorderDef;
	screenBorderDef.position.Set(0, 0);
	screenBorderDef.type = b2_staticBody;
	b2Body* screenBorderBody = world->CreateBody(&screenBorderDef);
	b2EdgeShape screenBorderShape;

	// Create fixtures for the three borders (the border shape is re-used)
	screenBorderShape.SetTwoSided(lowerLeftCorner, lowerRightCorner);
	screenBorderBody->CreateFixture(&screenBorderShape, 0);
	screenBorderShape.SetTwoSided(lowerRightCorner, upperRightCorner);
	screenBorderBody->CreateFixture(&screenBorderShape, 0);
	screenBorderShape.SetTwoSided(upperLeftCorner, lowerLeftCorner);
	screenBorderBody->CreateFixture(&screenBorderShape, 0);
}
void GameState::initContactListener()
{
	this->contactListener = new ContactListener(world);
	world->SetContactListener(this->contactListener);
}
void GameState::initPaddle()
{
	this->paddle = new Paddle(this->stateData, this->world);
}
void GameState::initBalls()
{
	this->balls.push_back(new Ball(this->stateData, this->world, this->paddle, gui::p2pX(1.4f, this->vm)));
}
void GameState::initBackground()
{
	this->textureRect = sf::IntRect(0, 0, 200, 241);

	if (!this->bgTexture.loadFromFile("Resources/Background/bg3.png"))
		std::cout << "ERROR::GAMESTATE::INITBACKGROUND::Could not load bg3.png \n";

	this->background.setSize(sf::Vector2f(static_cast<float>(vm.width), static_cast<float>(vm.height)));
	if(this->stateData->bgLeftRect != -1)
	{
		this->textureRect.left = stateData->bgLeftRect;
	}
	std::cout << "BGLEFT: " << this->stateData->bgLeftRect << std::endl;
	this->background.setTextureRect(this->textureRect);
	this->background.setTexture(&this->bgTexture);
}
void GameState::initPausedMenu()
{
	//PAUSED MENU
	this->pausedMenu = new PausedMenu(this->stateData->gfxSettings->resolution);
	this->pausedMenu->addButton("BACK", gui::p2pX(26.f, this->vm),
		gui::p2pY(23.8f, this->vm), gui::p2pX(6.6f, this->vm),
		gui::callCharSize(this->vm, 100),
		sf::Color::Black, sf::Color(250, 250, 250, 250), sf::Color(20, 20, 20, 50),
		sf::Color(200, 100, 100), sf::Color(200, 100, 100, 100), sf::Color(20, 20, 20, 50),
		"BACK");

}
void GameState::initBar()
{
	this->inversePaddleBar = new gui::Bar(gui::p2pX(50.f, this->vm), gui::p2pY(5.f, this->vm), gui::p2pX(12.f, this->vm), gui::p2pY(4.f, this->vm), 10, this->vm, this->font, sf::Color::Red);
}
void GameState::initTimer()
{
	this->timer = new Timer();
	this->timer->setPosition(sf::Vector2f(gui::p2pX(5.2f, this->vm), gui::p2pY(9.25f, this->vm)));
	this->timer->setCharacterSize(gui::callCharSize(this->vm, 80));
}
void GameState::initLifes()
{
	this->lifeCounter = 3;
	if (!this->lifeTexture.loadFromFile("Resources/Sprites/heart.png"))
		std::cout << "ERROR::GAMESTATE::INITLIFES::Could not load heart texture" << std::endl;
	this->lifeSprite.setTexture(this->lifeTexture);
	this->lifeSprite.setScale(0.03f, 0.0346f);
	
	for (size_t i = 0; i < lifeCounter; i++)
	{
		this->lifes.push_back(this->lifeSprite);
		this->lifes[i].setPosition(i * this->lifeSprite.getGlobalBounds().width + gui::p2pX(1.0f, this->vm), 0);
	}
}
void GameState::initKeyBinds()
{
	std::fstream ifs("Config/gamestate_keybinds.ini");
	if (ifs.is_open())
	{
		std::string key = "";
		std::string key2 = "";

		while (ifs >> key >> key2)
		{
			this->keyBinds[key] = this->supportedKeys->at(key2);
		}
	}
	ifs.close();
}
void GameState::initFont()
{
	if (!this->font.loadFromFile("Fonts/Arialn.ttf"))
		std::cout << ("ERROR::GAMESTATE::INITFONTS::Could not load font!");
}
void GameState::initBlockSet(std::string filename)
{
	this->blockSet = new BlockSet(this->stateData, this->world,  filename, this->stateData->gridWidth, this->stateData->gridHeight);
}
void GameState::initEndGame()
{
	this->lose = new EndGame(this->stateData->gfxSettings->resolution, "BACK", "YOU LOSE!");
	this->win = new EndGame(this->stateData->gfxSettings->resolution, "BACK", "NEXT", "YOU WIN!");
	
}
void GameState::initSound()
{

	this->playMusic = true;

	//collision sound
	if (!this->buffer[0].loadFromFile("Resources/Sounds/w.wav"))
		std::cout << "ERROR::BLOCKSET::INITSOUND::Could not load collision sound \n";
	this->collisionSound.setBuffer(this->buffer[0]);
	this->collisionSound.setVolume(this->stateData->collisionVolume);

		
	//game music
	if(!this->music.openFromFile("Resources/Sounds/music.wav"))
		std::cout << "ERROR::BLOCKSET::INITSOUND::Could not load music \n";
	this->music.setLoop(true);
	this->music.setVolume(this->stateData->musicVolume);
	if(!this->stateData->mute)
		this->music.play();


	//lose soud
	if (!this->buffer[1].loadFromFile("Resources/Sounds/lose.wav"))
		std::cout << "ERROR::BLOCKSET::INITSOUND::Could not load lose sound \n";
	this->loseSound.setBuffer(this->buffer[1]);
	this->loseSound.setVolume(this->stateData->endVolume);

	//win sound
	if (!this->buffer[2].loadFromFile("Resources/Sounds/win.wav"))
		std::cout << "ERROR::BLOCKSET::INITSOUND::Could not load win sound \n";
	this->winSound.setBuffer(this->buffer[2]);
	this->winSound.setVolume(this->stateData->endVolume);
}

//CONSTRUCTOR AND DESTRUCTOR
GameState::GameState(StateData* stateData, std::string filename)
	:State(stateData)
{
	this->initVariables();
	this->initB2World();
	this->initB2WorldBounds();
	this->initContactListener();
	this->initFont();
	this->initKeyBinds();
	this->initPausedMenu();
	this->initBar();
	this->initTimer();
	this->initPaddle();
	this->initBalls();
	this->initBackground();
	this->initLifes();
	this->initBlockSet(filename);
	this->initEndGame();
	this->initSound();



}
GameState::~GameState()
{
	delete this->pausedMenu;
	delete this->inversePaddleBar;
	delete this->paddle;
	delete this->timer;
	for (auto& ball : balls)
		delete ball;
	delete this->blockSet;
	delete this->win;
	delete this->lose;
	delete this->world;
	delete this->contactListener;

	this->stateData->inversePaddle = false;
	this->stateData->clock.restart();
}

//ACCESSORS
std::vector<Ball*> GameState::getBalls()
{
	return this->balls;
}

//FUNCTIONS
void GameState::updateInput(const float& dt)
{
	//player input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keyBinds.at("CLOSE"))) && this->getKeyTime())
	{
		if (!this->paused)
		{
			velocityBuffer.clear();
			for (int i = 0; i < balls.size(); i++)
			{
				velocityBuffer.push_back(balls[i]->GetBody()->GetLinearVelocity());
				// Zatrzymaj cia³o
				b2Vec2 zeroVelocity(0, 0);
				balls[i]->GetBody()->SetLinearVelocity(zeroVelocity);
			}


			if (!this->stateData->mute)
			{
				this->music.pause();
			}
			this->pauseState();
			this->timer->stop();
		}

		else
		{
			if (!this->stateData->mute)
			{
				this->music.play();
			}
			this->unpauseState();
			if (this->startGame)
				this->timer->resume();
			for (int i = 0; i < balls.size(); i++)
			{
				balls[i]->GetBody()->SetLinearVelocity(velocityBuffer[i]);
			}
		}
	}

			
	
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keyBinds.at("MOVE_LEFT")))
		&& !sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keyBinds.at("MOVE_RIGHT"))))
		this->paddleIsMoving = false;

	float  paddleDir = 0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keyBinds.at("MOVE_LEFT"))))
	{
		if (!startGame)
		{
			this->startGame = true;
			if (this->lifeCounter == 3)
				this->timer->start();

		}
		if (this->stateData->inversePaddle)
			paddleDir = 1;
		else if (!this->stateData->inversePaddle)
			paddleDir = -1;
		this->paddleIsMoving = true;

	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keyBinds.at("MOVE_RIGHT"))))
	{
		if (!startGame)
		{
			this->startGame = true;
			if (this->lifeCounter == 3)
				this->timer->start();
		}
		if (stateData->inversePaddle)
			paddleDir = -1;
		else if (!this->stateData->inversePaddle)
			paddleDir = 1;
		this->paddleIsMoving = true;
	}

	if(this->paddleIsMoving)
	{
		this->paddle->move(paddleDir, 0.f, dt);
	}
	else 
		this->paddle->move(0.f, 0.f, dt);
		
	//this->paddle->GetBodyDef()->position.Set(this->mousePosWindow.x, paddle->getPosition().x);

}
void GameState::updatePausedButtons()
{
	if (this->pausedMenu->isButtonPressed("BACK") && this->getKeyTime())
	{
		this->endState();
	}

}
void GameState::updateEndGameButtons()
{
	if (this->lose->isButtonPressed("BACK") && this->getKeyTime())
		this->endState();
	else if (this->win->isButtonPressed("BACK") && this->getKeyTime())
		this->endState();
	else if (this->win->isButtonPressed("NEXT") && this->getKeyTime())
	{
		std::cout << "NEXT \n";
		this->startGame = false;
		std::string fileName;
		fileName = this->blockSet->getFileName();
		fileName.erase(fileName.size() - 5, 5);
		int numMap = std::stoi(fileName);
		if (numMap < 16)
			numMap++;
		this->stateData->levels[numMap - 1] = 1;
		this->stateData->refreshLevelState = true;
		fileName = std::to_string(numMap);
		fileName += ".slmp";
		//std::cout << fileName << std::endl;
		balls.clear();
		balls.resize(0);

		this->paddle = nullptr;
		this->initPaddle();

		this->initBalls();

		lifes.clear();
		lifes.resize(0);
		this->lifeCounter = 3;
		this->initLifes();

		this->blockSet = nullptr;
		this->blockSet = new BlockSet(this->stateData, this->world, fileName, this->stateData->gridWidth, this->stateData->gridHeight);
		this->initBlockSet(fileName);

		this->timer = nullptr;
		this->initTimer();

		this->win = nullptr;
		this->initEndGame();

		if(!this->stateData->mute)
		{
			this->music.play();
			this->playMusic = true;
		}

		
	}
}
void GameState::updateEndGame(const float& dt)
{
	if (this->lifeCounter < 1)
	{
		this->lose->update(this->stateData, this->mousePosWindow, dt);
		
		if (!stateData->mute)
		{
			this->music.stop();
			if (this->playMusic)
			{
				this->loseSound.play();
				this->playMusic = false;
			}
		}
		this->updateEndGameButtons();
	}
	else if (this->blockSet->checkEndGame())
	{
		this->win->update(this->stateData, this->mousePosWindow, dt);
		std::string fileName;
		fileName = this->blockSet->getFileName();
		fileName.erase(fileName.size() - 5, 5);
		int numMap = std::stoi(fileName);
		if (numMap < 16)
			numMap++;
		this->stateData->levels[numMap - 1] = 1;
		this->stateData->refreshLevelState = true;

		this->stateData->inversePaddle = false;
		this->stateData->clock.restart();

		if (!stateData->mute)
		{
			this->music.stop();
			if (this->playMusic)
			{
				this->winSound.play();
				this->playMusic = false;
			}
		}
		this->updateEndGameButtons();
	}

}
void GameState::updateBlockSet(const float& dt)
{
	this->blockSet->startSuperPower(this->stateData, this->paddle, &this->balls);
	this->blockSet->collisionBallBlock(this->stateData, this->balls, this->paddle, dt);

	this->blockSet->updateSuperPower(this->stateData, dt);
	this->blockSet->update();
}
void GameState::updateTimer(const float& dt)
{
	if (this->startGame)
	{

		if (!this->paused)
			timer->update(dt);
	}
	if (this->lifeCounter < 0)
	{
		this->timer->stop();
	}
	
}
void GameState::updateBall(const float& dt)
{

	if (this->startGame && this->lifeCounter > 0)
	{
		for (int i = 0; i < this->balls.size(); i++)
		{
			if (balls[i]->GetBody()->GetLinearVelocity().x == 0 && balls[i]->GetBody()->GetLinearVelocity().y == 0)
				balls[i]->move(balls[i]->velocity.x, balls[i]->velocity.y, dt);

			

			this->balls[i]->update(dt);

			UserData* ballData = (UserData*)balls[i]->GetFixtureDef()->userData.pointer;
			if (ballData->hit)
			{
				if (!this->stateData->mute)
					this->collisionSound.play();


				// Oblicz offset pi³ki od lewego górnego rogu klocka
				float offsetX = balls[i]->getPosition().x - (paddle->getPosition().x + paddle->getGlobalBounds().width / 2.f);


				// Calculate the angle of the ball's bounce
				float angle = offsetX / paddle->getGlobalBounds().width;

				//// Increase the multiplier for the deflection factor if the ball hits the edge of the block
				if (offsetX < 0 || offsetX > paddle->getSize().x)
				{
					angle *= 2.0f;
				}

				// Calculate the velocity vector of the ball after bouncing
				sf::Vector2f velocity;
				velocity.x = balls[i]->GetBody()->GetLinearVelocity().x + angle * std::abs(balls[i]->GetBody()->GetLinearVelocity().x);
				velocity.y = balls[i]->GetBody()->GetLinearVelocity().y;
				balls[i]->GetBody()->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
				std::cout << velocity.x << std::endl;
				ballData->hit = false;
			}
			

			if (balls[i]->GetBody()->GetPosition().y * SCALE >= this->stateData->gfxSettings->resolution.height)
			{
				//REMOVE BALL,
				if (this->balls.size() >= 1)
				{
					world->DestroyBody(balls[i]->GetBody());
					this->balls.erase(this->balls.begin() + i);
				}
				//CHECK LIFES
				if (this->balls.size() < 1 && this->lifeCounter > 0)
				{
					this->lifes.erase(this->lifes.begin() + this->lifeCounter - 1);
					this->lifeCounter -= 1;
					this->startGame = false;
					this->balls.push_back(new Ball(this->stateData, this->world, this->paddle, gui::p2pX(1.4f, this->vm)));
				}
			}
		}

	}
}
void GameState::updateInverseTimer()
{
	if (this->stateData->inversePaddle)
	{
		if (this->stateData->clock.getElapsedTime().asSeconds() >= 10)
		{
			this->stateData->inversePaddle = false;
			this->stateData->clock.restart();
		}
	}
	this->inversePaddleBar->update("INVERSE PADDLE TIME", this->stateData->clock.getElapsedTime().asSeconds());
}
void GameState::update(const float& dt)
{
	this->world->Step(1 / 60.f / s, 8, 3);
	this->updateMousePosition();
	if(!this->blockSet->checkEndGame() && this->lifeCounter > 0)
	{
		this->updateInput(dt);
		this->updateTimer(dt);

		if (this->paused)
		{
			this->pausedMenu->update(this->stateData, this->mousePosWindow);
			this->updatePausedButtons();
		}
		else
		{
			this->updateBall(dt);
			this->paddle->update(dt);
			this->updateInverseTimer();
			this->updateBlockSet(dt);
		}
	}
	else
	{
		this->updateEndGame(dt);
		this->updateEndGameButtons();
	}
	
} 
void GameState::renderEndGame(sf::RenderTarget* target)
{
		if (lifeCounter < 1)
		{
			this->timer->stop();
			this->timer->setCharacterSize(70);
			this->lose->render(target);
		}
		else if (this->blockSet->checkEndGame())
		{
			this->timer->stop();
			this->timer->setCharacterSize(70);
			this->win->render(target);

		}
}
void GameState::renderBalls(sf::RenderTarget* target)
{
	for (auto& ball : this->balls)
	{
		ball->render(target);
	}
}
void GameState::renderLifes(sf::RenderTarget* target)
{
	for (auto& life : this->lifes)
	{
		target->draw(life);
	}
}
void GameState::render(sf::RenderTarget* target)
{
	

	target->draw(this->background);

	this->blockSet->render(*target);
	if(this->stateData->inversePaddle)
		this->inversePaddleBar->render(*target);
	this->paddle->render(target);
	this->renderBalls(target);
	this->renderEndGame(target);
	this->timer->render(*target);
	this->renderLifes(target);


	if (this->paused)
	{
		this->pausedMenu->render(*target);
	}
}


