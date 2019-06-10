#pragma once

// This class contains the data for all the blocks required for the breakout game

#include "BaseGameObject.h"
#include <cmath>

// block ID enum
enum BLOCK_ID
{
	STANDARD = 0,
	CHANGER = 1,
	SPLITTER = 2
};

// create a base block class for other blocks to inherit from
class BaseBlockObject :
	public BaseGameObject
{
public:
	BaseBlockObject(sf::Vector2f pos);
	~BaseBlockObject();

	bool IsBroken() { return broken_; }

	void Update() override;

	BLOCK_ID GetID() { return id_; }

protected:
	bool broken_;
	sf::Vector2f dimensions_;
	BLOCK_ID id_;

	sf::Vector2f GetCollisionNormal(sf::Vector2f targetPos);
};

// the standard block class
// this block needs one collision to break and has no special properties
class StandardBlock :
	public BaseBlockObject
{
public:
	StandardBlock(sf::Vector2f pos);
	~StandardBlock();

	sf::Vector2f DetectCollision(BaseGameObject* target) override;
};

// the changer block class
// this block needs two collisions to break and it will change a ball's colour on impact
class ChangerBlock :
	public BaseBlockObject
{
public:
	ChangerBlock(sf::Vector2f pos);
	~ChangerBlock();

	sf::Vector2f DetectCollision(BaseGameObject* target) override;

private:
	int hitPoints_;
};

// the splitter block class
// this block does not bounce the ball, adding two new balls is handled in the play state
class SplitterBlock :
	public BaseBlockObject
{
public:
	SplitterBlock(sf::Vector2f pos);
	~SplitterBlock();

	sf::Vector2f DetectCollision(BaseGameObject* target) override;
};