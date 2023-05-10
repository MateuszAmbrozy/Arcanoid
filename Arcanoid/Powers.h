#pragma once
#include "Ball.h"
#include "Paddle.h"
#include "Gui.h"

enum PowerTypes { BIGGERPADDLE = 0, SMALLPADDLE, NEWBALLS, NEWBALLFROMPADDLE, FASTBALL, SLOWBALL, VICEVERSA};
class Powers
{
private:
	sf::Texture texture[3];

	sf::CircleShape shape;
	float maxVelocity;
	short unsigned powerType;
	sf::VideoMode vm;
public:

	//CONSTRUCTOR AND DESTRUCTOR
	Powers(StateData* stateData, sf::Vector2f position);
	virtual ~Powers();

	//ACCESSORS
	const bool intersects(const sf::FloatRect bounds) const;
	const unsigned short getType() const;
	const sf::FloatRect getGlobalBounds() const;

	//FUNCTIONS

	void move(const float& dt);

	void initNewBalls(StateData* stateData, b2World* world, std::vector<Ball*>* balls);
	void initNewBallsFromPaddle(StateData* stateData, b2World* world, Paddle* paddle, std::vector<Ball*>* balls);
	void initBiggerPaddle(Paddle* paddle);
	void initSmallerPaddle(Paddle* paddle);
	void initFastBalls(std::vector<Ball*>* balls);
	void initSlowBalls(std::vector<Ball*>* balls);


	void update(const float& dt);
	void render(sf::RenderTarget& target);

};

