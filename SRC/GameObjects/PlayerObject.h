#ifndef _PLAYER_OBJECT_H
#define _PLAYER_OBJECT_H

#include "BaseGameObject.h"

// This class represent the player's paddle
class PlayerObject :
	public BaseGameObject
{
public:
	PlayerObject(int screenWidth, sf::Vector2f origin, float speed = 500.0f);
	~PlayerObject();

	void Update() override;
	sf::Vector2f DetectCollision(BaseGameObject* target) override;
	void Move(float f);
	void Reset();

private:
	float paddleSpeed_;
	sf::Vector2f paddleOrigin_;
	sf::Vector2f dimensions_;
	int screenWidth_;
};

#endif