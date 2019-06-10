#include "BaseGameObject.h"

AABB::AABB(sf::Vector2f dimensions)
{
	// (x, y)
	bottomRight_ = (dimensions / 2.0f);

	// (-x, -y)
	topLeft_ =  -bottomRight_;
}

AABB::~AABB()
{

}

BaseGameObject::BaseGameObject()
{
	dirty_ = false;
}

BaseGameObject::~BaseGameObject()
{
	if (objectShape_)
	{
		delete objectShape_;
		objectShape_ = NULL;
	}

	if (aabb_)
	{
		delete aabb_;
		aabb_ = NULL;
	}
}

void BaseGameObject::SetPosition(sf::Vector2f p)
{
	position_ = p;
	dirty_ = true;
}

// this code simply returns whether or not the object is colliding with the target object
bool BaseGameObject::AABBOverlap(BaseGameObject* target)
{
	// tl = top left, br = bottom right
	sf::Vector2f thisTL, thisBR, thatTL, thatBR;

	// get this object's corners and transform them
	thisTL = this->aabb_->GetTopLeft() + this->Position();
	thisBR = this->aabb_->GetBottomRight() + this->Position();

	// get the target object's corners and transform them
	thatTL = target->aabb_->GetTopLeft() + target->Position();
	thatBR = target->aabb_->GetBottomRight() + target->Position();

	// determine if there is a collision using aabb collision detection
	if (
		thisTL.x < thatBR.x &&
		thisBR.x > thatTL.x &&
		thisTL.y < thatBR.y &&
		thisBR.y > thatTL.y
		)
	{
		// all conditions satisified, collision detected
		return true;
	}
	else
	{
		// no overlap, no collision
		return false;
	}
}