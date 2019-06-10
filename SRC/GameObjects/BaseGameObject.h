#pragma once

#include <SFML/Graphics.hpp>

// this class stores the information for a game object's bounding box
// this is used to detect collisions in the game
class AABB
{
public:
	AABB(sf::Vector2f dimensions);
	~AABB();

	sf::Vector2f GetTopLeft() { return topLeft_; }
	sf::Vector2f GetBottomRight() { return bottomRight_; }

protected:
	sf::Vector2f topLeft_, bottomRight_;
};

//#ifndef _BASE_GAME_OBJECT_H
//#define _BASE_GAME_OBJECT_H

// This class is a base game object that all game objects inherit from
// It provides only the base functionality required for all game objects
class BaseGameObject
{
public:
	BaseGameObject();
	~BaseGameObject();

	// returns the object's shape in order to render to screen
	sf::Shape* Shape() { return objectShape_; }

	// returns the object's position
	sf::Vector2f Position() { return position_; }

	// returns the object's collision box
	AABB* Collider() { return aabb_; }

	void SetPosition(sf::Vector2f p);

	// these may be required for child objects
	virtual void Update() = 0;

	// child objects have different collision behaviours
	virtual sf::Vector2f DetectCollision(BaseGameObject* target) = 0;

protected:
	sf::Shape* objectShape_;
	sf::Vector2f position_;
	AABB* aabb_;

	// this flag indicates that the object needs to be updated so the position of the object and shape match up
	bool dirty_;

	bool AABBOverlap(BaseGameObject* target);
};

//#endif