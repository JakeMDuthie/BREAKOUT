#include "BallObject.h"

BallObject::BallObject(int screenWidth, int screenHeight, sf::Vector2f direction, float speed)
{
	screenHeight_ = screenHeight;
	screenWidth_ = screenWidth;

	ballSpeed_ = speed;

	// start off as white
	currentColour_ = 0;

	// populate the colour array
	ballColours_[0] = sf::Color::White;
	ballColours_[1] = sf::Color::Cyan;
	ballColours_[2] = sf::Color::Magenta;
	ballColours_[3] = sf::Color::Blue;

	radius_ = 10.0f;

	// set the ball's initial direction to be upwards
	direction_ = direction;

	objectShape_ = new sf::CircleShape(radius_);

	// by default, the shape's origin is in its top left corner
	// this is no use, so put it in the centre
	objectShape_->setOrigin(radius_,radius_);

	objectShape_->setFillColor(ballColours_[0]);

	// initialise the object's AABB
	aabb_ = new AABB(sf::Vector2f(2.0f*radius_, 2.0f*radius_));

	// like the blocks, we want a flag to determine whether or not a ball should be destroyed
	// for consistency, let's use the same variable name across both ball and blocks
	broken_ = false;
}

BallObject::~BallObject()
{
	// run parent destructor
	BaseGameObject::~BaseGameObject();
}

// this function isn't intended to be called and does nothing
sf::Vector2f BallObject::DetectCollision(BaseGameObject* target)
{
	return sf::Vector2f(0.0f, 0.0f);
}

void BallObject::Update()
{
	if (dirty_)
	{
		objectShape_->setPosition(position_);

		dirty_ = false;
	}
}

void BallObject::Move(float f)
{
	// move the ball by an amount equal to the direction and velocity
	position_ += (f*ballSpeed_*direction_);

	// check to see if the ball is touching the wall and bounce accordingly
	// we'll consider the direction the ball is traveling also to avoid instances where the ball may "bounce" when already leaving the wall
	float dRad = GetDirectionAsRadian();

	// left wall
	if (position_.x - radius_ < 0.0f && dRad >= M_PI)
		Bounce(sf::Vector2f(1.0f, 0.0f));
	// right wall
	if (position_.x + radius_ > screenWidth_ && dRad <= M_PI)
		Bounce(sf::Vector2f(-1.0f, 0.0f));
	// ceiling
	if (position_.y - radius_ < 0.0f && (dRad <= (M_PI/2.0f) || (dRad >= (1.5f*M_PI))))
		Bounce(sf::Vector2f(0.0f, 1.0f));
	// floor
	if (position_.y + radius_ > screenHeight_)
	{
		// if a ball hits the floor, don't bounce, instead flag it as fouled
		broken_ = true;
	}

	dirty_ = true;
}

// change the ball's direction based on its current direction and the normal vector of the object it struck
void BallObject::Bounce(sf::Vector2f n)
{
	// make sure the angle between the normal and direction is acute
	// if it isn't, errors can occur
	float theta;
	float dotProduct;

	// angle between two vectors formula
	dotProduct = (n.x * direction_.x) + (n.y * direction_.y);
	theta = asin(dotProduct);

	// pi/2 = 90'
	if (theta <= M_PI/2.0f)
	{
		// calculate the reflected ray: r, ( new direction_) across normal: n with d (old direction_)
		// formula: r = d - 2(d.n)n
		sf::Vector2f reflection;

		// reuse dotproduct from previous calculation
		reflection = direction_ - (2 * (dotProduct)*n);

		// update ball's direction
		direction_ = (reflection);
	}
}

// set direction with a vector2f
void BallObject::SetDirection(sf::Vector2f d)
{
	// normalise d
	float magnitude = sqrt((d.x*d.x)+(d.y*d.y));
	sf::Vector2f dNorm = d/magnitude;

	direction_ = dNorm;
}

// set direction with a radian theta
void BallObject::SetDirection(float theta)
{
	sf::Vector2f newDirection;

	newDirection.x = sin(theta);
	newDirection.y = 0.0f - cos(theta);

	direction_ = newDirection;
}

// get the direction of the ball as a radian [0,2pi)
float BallObject::GetDirectionAsRadian()
{
	float r = asin(direction_.x);

	if (direction_.y > 0.0f)
	{
		// 2nd quadrant
		if (direction_.x >= 0.0f)
		{
			r = M_PI - r;
			return r;
		}
		// 3rd quadrant
		else
		{
			r = M_PI - r;
			return r;
		}
	}
	// 4th quadrant
	else if (direction_.x < 0.0f)
	{
		r = 2 * M_PI - r;
		return r;
	}

	return r;
}

// cycle the colour of the ball
void BallObject::CycleColour()
{
	if (++currentColour_ > 3)
		currentColour_ = 0;

	objectShape_->setFillColor(ballColours_[currentColour_]);
}