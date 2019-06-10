#include "PlayerObject.h"

PlayerObject::PlayerObject(int screenWidth, sf::Vector2f origin, float speed)
	: BaseGameObject::BaseGameObject()
{
	paddleOrigin_ = origin;
	paddleSpeed_ = speed;
	screenWidth_ = screenWidth;

	position_ = paddleOrigin_;

	dimensions_ = sf::Vector2f(160.0f, 30.0f);

	// create the paddle shape, a rectangle
	objectShape_ = new sf::RectangleShape(dimensions_);

	// by default, the shape's origin is in its top left corner
	// this is no use, so put it in the centre
	objectShape_->setOrigin(dimensions_ / 2.0f);
	
	objectShape_->setPosition(paddleOrigin_);
	objectShape_->setFillColor(sf::Color::Red);

	// initialise the object's AABB
	aabb_ = new AABB(dimensions_);
}

PlayerObject::~PlayerObject()
{
	// run parent destructor
	BaseGameObject::~BaseGameObject();
}

void PlayerObject::Update()
{
	if (dirty_)
	{
		objectShape_->setPosition(position_);

		dirty_ = false;
	}
}

// takes in a direction and moves the paddle accordingly within the bounds of the screen
// negative values go left, postive values go right
// input = deltaTime
void PlayerObject::Move(float f)
{
	position_.x += (f*paddleSpeed_);

	// make sure that the paddle is not out of bounds
	if (position_.x < 0.0f) 
		position_.x = 0.0f;
	if (position_.x > screenWidth_)
		position_.x = screenWidth_;

	dirty_ = true;
}

// reset the paddle to its origin position
// this will likely be called when the ball goes out of bounds
void PlayerObject::Reset()
{
	position_ = paddleOrigin_;

	dirty_ = true;
	Update();
}

// run the collision and return a normal direction if a collision is detected
sf::Vector2f PlayerObject::DetectCollision(BaseGameObject* target)
{
	sf::Vector2f resultantNormal = sf::Vector2f(0.0f, 0.0f);

	// check if there is an overlap with the target
	if (AABBOverlap(target))
	{
		// find the direction vector between the ball and  an offset from the paddle
		// taking the true position of the paddle for this calculation makes the paddle too "round"
		sf::Vector2f pOff = sf::Vector2f(position_.x, position_.y + 100.0f);
		sf::Vector2f d = target->Position() - pOff;

		// normalise this direction
		float magnitude = sqrt((d.x*d.x) + (d.y*d.y));
		resultantNormal = d / magnitude;
	}

	return resultantNormal;
}