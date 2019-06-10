// This class is a base class that all game states inherit.
// It comes with the virtual functions for the render, update and enter/exit functions
// It also contains an ENUM for refering to gamestates more easily.

#include <SFML/Graphics.hpp>

#ifndef _GAME_STATE_H
#define _GAME_STATE_H

enum GAMESTATE
{
	MENU = 0,
	PLAY = 1
};

class GameState
{
public:
	GameState();
	virtual ~GameState();
	virtual void Render() = 0;
	virtual bool Update(float frame_time) = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;
	GAMESTATE GetDesiredState() { return desired_state_; }
	void SetDesiredState(GAMESTATE ds) { desired_state_ = ds; }
protected:
	GAMESTATE desired_state_;
	sf::RenderWindow* window_;

	int screenWidth_, screenHeight_;
};

#endif