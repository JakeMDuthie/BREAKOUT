#include "Blocks.h"

//
// BASE BLOCK CLASS
//
BaseBlockObject::BaseBlockObject(sf::Vector2f pos)
{
	// build the block geometry, colour is applied in a specific block's constructor
	position_ = pos;

	dimensions_ = sf::Vector2f(100.0f, 30.0f);

	// create the paddle shape, a rectangle
	objectShape_ = new sf::RectangleShape(dimensions_);

	// by default, the shape's origin is in its top left corner
	// this is no use, so put it in the centre
	objectShape_->setOrigin(dimensions_ / 2.0f);

	objectShape_->setPosition(position_);

	// initialise the object's AABB
	aabb_ = new AABB(dimensions_);

	// ensure a block starts off unbroken
	broken_ = false;
}

BaseBlockObject::~BaseBlockObject()
{
	// call parent destructor
	BaseGameObject::~BaseGameObject();
}

// intentionally does nothing, but same child objects may want to further override this function
void BaseBlockObject::Update()
{
}

// return a collision normal if a collision is detected
// unlike with the paddle, this normal will always be an orthogonal vector
// when this is called, it is because a collision has been detected, so only worry about deciding what the vector is
sf::Vector2f BaseBlockObject::GetCollisionNormal(sf::Vector2f targetPos)
{
	// first, get the direction from this object to the target object
	sf::Vector2f direction = targetPos - position_;

	// determine the suitable orthogonal normal
	// is the ball below the block? Is y positive and absolutely greater than/equal to x?
	if (direction.y > 0.0f && abs(direction.y) >= abs(direction.x))
	{
		return sf::Vector2f(0.0f, 1.0f);
	}

	// is the ball above the block? Is y negative and absolutely greater than/equal to x?
	if (direction.y < 0.0f && abs(direction.y) >= abs(direction.x))
	{
		return sf::Vector2f(0.0f, -1.0f);
	}

	// is the ball to the left of the block? Is x negative and absolutely greater than/equal to y?
	if (direction.x < 0.0f && abs(direction.x) >= abs(direction.y))
	{
		return sf::Vector2f(-1.0f, 0.0f);
	}

	// is the ball to the right of the block? Is x positive and absolutely greater than/equal to y?
	if (direction.x > 0.0f && abs(direction.x) >= abs(direction.y))
	{
		return sf::Vector2f(1.0f, 0.0f);
	}

	// return 0,0 in case code gets to here
	return sf::Vector2f(0.0f, 0.0f);
}

//
// STANDARD BLOCK
//
StandardBlock::StandardBlock(sf::Vector2f pos) :
	BaseBlockObject(pos)
{
	// geometry/bounding box initialisation is already handled, so we only worry about colour here
	objectShape_->setFillColor(sf::Color::Yellow);

	id_ = STANDARD;
}

StandardBlock::~StandardBlock()
{
	// call parent destructor
	BaseBlockObject::~BaseBlockObject();
}

// the standard block's collision behaviour
sf::Vector2f StandardBlock::DetectCollision(BaseGameObject* target)
{
	sf::Vector2f resultantNormal = sf::Vector2f(0.0f, 0.0f);

	// check if there is an overlap with the target
	if (AABBOverlap(target))
	{
		// there is a collision, flag the block as broken
		broken_ = true;

		resultantNormal = GetCollisionNormal(target->Position());
	}

	return resultantNormal;
}

//
// CHANGER BLOCK
//
ChangerBlock::ChangerBlock(sf::Vector2f pos) :
	BaseBlockObject(pos)
{
	// geometry/bounding box initialisation is already handled, so we only worry about colour here
	// sfml does not have an orange colour by default
	// use the orangered colour to stand out against the yellow of the standard block
	objectShape_->setFillColor(sf::Color(255,69,0,255));

	id_ = CHANGER;

	hitPoints_ = 2;
}

ChangerBlock::~ChangerBlock()
{
	// call parent destructor
	BaseBlockObject::~BaseBlockObject();
}

// the changer block's collision behaviour
sf::Vector2f ChangerBlock::DetectCollision(BaseGameObject* target)
{
	sf::Vector2f resultantNormal = sf::Vector2f(0.0f, 0.0f);

	// check if there is an overlap with the target
	if (AABBOverlap(target))
	{
		// decrement HP, set broken if hp goes to 0
		if (--hitPoints_ <= 0)
		{
			broken_ = true;
		}

		resultantNormal = GetCollisionNormal(target->Position());
	}

	return resultantNormal;
}

//
// SPLITTER BLOCK
//
SplitterBlock::SplitterBlock(sf::Vector2f pos) :
	BaseBlockObject(pos)
{
	// geometry/bounding box initialisation is already handled, so we only worry about colour here
	objectShape_->setFillColor(sf::Color::Green);

	id_ = SPLITTER;
}

SplitterBlock::~SplitterBlock()
{
	// call parent destructor
	BaseBlockObject::~BaseBlockObject();
}

// the splitter block's collision behaviour
// this doesn't bounce the ball, so just make the collision return an arbitrary 1.0,1.0 value
sf::Vector2f SplitterBlock::DetectCollision(BaseGameObject* target)
{
	sf::Vector2f resultantNormal = sf::Vector2f(0.0f, 0.0f);

	// check if there is an overlap with the target
	if (AABBOverlap(target))
	{
		broken_ = true;
		resultantNormal = sf::Vector2f(1.0f, 1.0f);
	}

	return resultantNormal;
}