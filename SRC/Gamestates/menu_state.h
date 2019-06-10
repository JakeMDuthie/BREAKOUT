#ifndef _MENU_STATE_H
#define _MENU_STATE_H

#include "game_state.h"

class MenuState :
	public GameState
{
public:
	MenuState(sf::RenderWindow* window);
	~MenuState();
	void Render() override;
	bool Update(float frame_time) override;
	void OnEnter() override;
	void OnExit() override;

	int Level() { return level_; }

private:
	sf::Font* font_;

	sf::Text* menuLogo_;
	sf::Text* subLogo_;

	sf::Text* levelSelector_;

	int level_;
};

#endif
