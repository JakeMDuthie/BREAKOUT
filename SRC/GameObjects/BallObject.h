#pragma once

#define _USE_MATH_DEFINES

#include "BaseGameObject.h"
#include <cmath>

// this class use used to represent the active balls on screen
class BallObject :
	public BaseGameObject
{
public:
	BallObject(int screenWidth, int screenHeight, 
				sf::Vector2f direction = sf::Vector2f(0.0f,-1.0f), 
				float speed = 600.0f);
	~BallObject();

	void Update() override;
	
	// the ball has no collision behaviour, this is included so that all virtual functions are overridden
	sf::Vector2f DetectCollision(BaseGameObject* target) override;

	void Move(float f);

	void Bounce(sf::Vector2f n);

	void SetDirection(sf::Vector2f d);
	void SetDirection(float theta);

	sf::Vector2f Direction() { return direction_; }

	float GetDirectionAsRadian();

	bool IsBroken() { return broken_; }

	void CycleColour();

private:
	float radius_;
	sf::Vector2f direction_;
	float ballSpeed_;
	int currentColour_;
	sf::Color ballColours_[4];

	bool broken_;

	int screenWidth_, screenHeight_;
};