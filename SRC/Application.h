#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <SFML/Graphics.hpp>
#include "Common/Timer.h"
#include "Gamestates/menu_state.h"
#include "Gamestates/play_state.h"

class Application
{
public:
	Application();
	~Application();
	bool Run();

private:
	void ChangeGameState(GAMESTATE newGameState);
	GameState* EnumToPointer(GAMESTATE gs);

	Timer* timer_;
	sf::RenderWindow* window_;

	MenuState* menuState_;
	PlayState* playState_;

	GameState* currentState_;
};

#endif