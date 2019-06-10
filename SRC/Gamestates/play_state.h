#ifndef _PLAY_STATE_H
#define _PLAY_STATE_H

#define _USE_MATH_DEFINES

#include "game_state.h"
#include <vector>
#include <cmath>

#include "../GameObjects/PlayerObject.h"
#include "../GameObjects/BallObject.h"
#include "../GameObjects/Blocks.h"

#include "../LevelLoader.h"

class PlayState :
	public GameState
{
public:
	PlayState(sf::RenderWindow* window);
	~PlayState();
	void Render() override;
	bool Update(float frame_time) override;
	void OnEnter() override;
	void OnExit() override;

	void LoadLevel(int level);

private:
	PlayerObject* player_;

	LevelLoader* levelLoader_;

	std::vector<BallObject*> balls_;
	std::vector<BallObject*> temporaryBalls_;
	std::vector<BaseBlockObject*> blocks_;

	// UI Elements
	sf::Font* font_;
	sf::Text* levelText_;
	sf::Text* livesText_;

	sf::Text* endText_;
	sf::Text* endSubtext_;

	bool paused_;
	bool gameOver_;

	int lives_;

	void Reset();

	void ProcessCollisions();
	void ProcessDestructions();
	void SplitterCollision(BallObject* sourceBall);
};

#endif