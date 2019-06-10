#include "play_state.h"

#include <stdio.h>

#include <iostream>

PlayState::PlayState(sf::RenderWindow* window)
{
	window_ = window;

	screenHeight_ = window_->getSize().y;
	screenWidth_ = window_->getSize().x;

	// init level loader here, it's very lightweight so we don't need to worry about loading/unloading it every time we switch state
	levelLoader_ = new LevelLoader();
}

PlayState::~PlayState()
{
	if (levelLoader_)
	{
		delete levelLoader_;
		levelLoader_ = NULL;
	}
}

void PlayState::OnEnter()
{
	// set desired state to be this state
	SetDesiredState(PLAY);

	// set paused
	paused_ = true;

	// set game over false
	gameOver_ = false;

	// set lives
	lives_ = 3;

	// initialise level objects
	// init player
	player_ = new PlayerObject(screenWidth_, sf::Vector2f(((float)screenWidth_ / 2.0f), (float)screenHeight_ - 100.0f));

	player_->Update();

	// init first ball
	BallObject* ball = new BallObject(screenWidth_, screenHeight_);

	// place the ball just above the player initially
	sf::Vector2f ballPos;
	ballPos = player_->Position();
	ballPos.y -= 30.0f;

	// update ball position
	ball->SetPosition(ballPos);

	// test ball bouncing
	ball->SetDirection(sf::Vector2f(0.0f, -1.0f));

	ball->Update();

	balls_.push_back(ball);
	
	// UI elements
	// initialise and load font
	font_ = new sf::Font();
	font_->loadFromFile("Resources/Bison.ttf");

	// initliase level text
	levelText_ = new sf::Text();

	levelText_->setString("Level: ?");
	levelText_->setFont(*font_);
	levelText_->setCharacterSize(30);
	levelText_->setPosition(20.0f, 20.0f);

	// initialise lives text
	livesText_ = new sf::Text();

	sf::String	levelText = "Lives: " + std::to_string(lives_);
	livesText_->setString(levelText);
	livesText_->setFont(*font_);
	livesText_->setCharacterSize(30);
	livesText_->setPosition(200.0f, 20.0f);

	// initliase endcard text
	endText_ = new sf::Text();

	endText_->setString("You ????");
	endText_->setFont(*font_);
	endText_->setCharacterSize(90);
	endText_->setPosition((screenWidth_/2.0f)-120.0f, 400.0f);

	endSubtext_ = new sf::Text();

	endSubtext_->setString("Press escape to end");
	endSubtext_->setFont(*font_);
	endSubtext_->setCharacterSize(30);
	endSubtext_->setPosition((screenWidth_ / 2.0f) - 120.0f, 500.0f);

	// initialise some blocks as a test
	/*sf::Vector2f blockPos = sf::Vector2f(0.0f, 0.0f);

	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			blockPos.x = (i*100.0f) + 100.0f;
			blockPos.y = (j*50.0f) + 100.0f;
			BaseBlockObject* block;

			if (j != 1)
			{
				if ((i + j) % 5 == 0)
					block = new SplitterBlock(blockPos);
				else
					block = new StandardBlock(blockPos);
			}
			else
				block = new ChangerBlock(blockPos);
			//else
			//	block = new StandardBlock(blockPos);

			blocks_.push_back(block);
		}
	}*/
}

void PlayState::OnExit()
{
	// release state elements
	if (player_)
	{
		delete player_;
		player_ = NULL;
	}

	if (!balls_.empty())
	{
		for (auto ball : balls_)
		{
			delete ball;
		}
		balls_.clear();
	}

	if (!blocks_.empty())
	{
		for (auto block : blocks_)
		{
			delete block;
		}
		blocks_.clear();
	}

	if (font_)
	{
		delete font_;
		font_ = NULL;
	}

	if (levelText_)
	{
		delete levelText_;
		levelText_ = NULL;
	}

	if (livesText_)
	{
		delete livesText_;
		livesText_ = NULL;
	}

	if (endText_)
	{
		delete endText_;
		endText_ = NULL;
	}

	if (endSubtext_)
	{
		delete endSubtext_;
		endSubtext_ = NULL;
	}
}

// this calls the level loader for the game
void PlayState::LoadLevel(int level)
{
	// update level text
	sf::String	levelText = "Level: " + std::to_string(level);
	levelText_->setString(levelText);

	// convert level number into a filename
	std::string filename = "Levels/level" + std::to_string(level) + ".txt";

	// load level, store vector of blocks
	blocks_ = levelLoader_->LoadLevel(filename);
}

void PlayState::Render()
{
	// add all active elements to a vector of render items
	std::vector<BaseGameObject*> renderItems;

	// add player
	renderItems.push_back(player_);

	// add all balls
	if (!balls_.empty())
		for (auto i : balls_)
			renderItems.push_back(i);

	// add all blocks
	if (!blocks_.empty())
		for (auto i : blocks_)
			renderItems.push_back(i);

	//
	// MAIN RENDER
	//
	// clear the display before rendering any objects
	window_->clear();
	// cycle through all render items
	for (auto i : renderItems)
		window_->draw(*i->Shape());

	// render UI elements over everything else
	window_->draw(*levelText_);
	window_->draw(*livesText_);

	if (gameOver_)
	{
		window_->draw(*endText_);
		window_->draw(*endSubtext_);
	}

	// present the completed frame to the window
	window_->display();
}

bool PlayState::Update(float frame_time)
{
	if (paused_)
	{
		if (gameOver_)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				SetDesiredState(MENU);
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			paused_ = false;
		}
	}
	else
	{
		// handle user input
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			player_->Move(-frame_time);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			player_->Move(frame_time);
		}

		player_->Update();

		// move balls
		for (auto i : balls_)
		{
			i->Move(frame_time);
			i->Update();
		}

		// process collisions, test balls against other game objects
		ProcessCollisions();

		// process destruction and remove blocks/balls as required
		ProcessDestructions();

		// if there are balls to be added from the temporary container, add them to the ball vector
		if (!temporaryBalls_.empty())
		{
			for (auto i : temporaryBalls_)
			{
				balls_.push_back(i);
			}
			temporaryBalls_.clear();
		}

		// check to see if the player has won by eliminating all the blocks
		if (blocks_.empty())
		{
			// the game has ended
			gameOver_ = true;
			paused_ = true;

			// set text to "lost"
			endText_->setString("You win!");
			endText_->setFillColor(sf::Color::Green);
		}
	}

	return true;
}

// move collision detection into a seperate class to tidy up the update function
void PlayState::ProcessCollisions()
{
	sf::Vector2f normal;
	for (auto i : balls_)
	{
		normal = player_->DetectCollision(i);

		// make sure the ball isn't already going away from the paddle when detecting a bounce
		if (normal != sf::Vector2f(0.0f, 0.0f) && i->Direction().y >=0.0f)
		{
			// ball has collided with the player paddle, bounce accordingly
			i->Bounce(normal);
		}

		// compare balls to blocks
		for (auto block : blocks_)
		{
			// this function handles what happens to the block
			normal = block->DetectCollision(i);

			// update the ball if required
			if (normal != sf::Vector2f(0.0f, 0.0f))
			{
				if (block->GetID() == SPLITTER)
				{
					// a splitter block ignores collisions, so do not bounce
					// instead, add 2 new balls to the field
					SplitterCollision(i);
				}
				else
				{
					// ball has collided with the player paddle, bounce accordingly
					i->Bounce(normal);

					// if the ball collided with a changer block, change its colour
					if (block->GetID() == CHANGER)
					{
						i->CycleColour();
					}
				}
			}
		}
	}
}

// if a splitter block has been hit, we want to add two new balls, one cw, one ccw to the original ball
void PlayState::SplitterCollision(BallObject* sourceBall)
{
	BallObject* cwBall = new BallObject(screenWidth_, screenHeight_);
	BallObject* ccwBall = new BallObject(screenWidth_, screenHeight_);

	// clockwise ball first
	cwBall->SetPosition(sourceBall->Position());
	cwBall->SetDirection(sourceBall->GetDirectionAsRadian() + (M_PI/4.0f));

	// counter clockwise ball first
	ccwBall->SetPosition(sourceBall->Position());
	ccwBall->SetDirection(sourceBall->GetDirectionAsRadian() - (M_PI / 4.0f));

	temporaryBalls_.push_back(cwBall);
	temporaryBalls_.push_back(ccwBall);
}

// handle the removal of balls/blocks here
void PlayState::ProcessDestructions()
{
	// if any blocks are flagged as broken, remove them
	if (!blocks_.empty())
	{
		int index = 0;

		for (auto i : blocks_)
		{
			// check if particle has timed out and erase if required
			// ! IS THERE A MORE EFFICIENT WAY TO DO THIS?
			if (i->IsBroken())
			{
				blocks_.erase(blocks_.begin() + index);
			}
			index++;
		}
	}

	// if any balls are flagged as broken, remove them
	if (!balls_.empty())
	{
		int index = 0;

		for (auto i : balls_)
		{
			if (i->IsBroken())
			{
				balls_.erase(balls_.begin() + index);
			}
			index++;
		}

		// if the last ball has been destroyed, reset the game and subtract a life
		if (balls_.empty())
		{
			Reset();
		}
	}
}

// reset the game by returning the paddle to the centre and creating a new ball above the paddle
void PlayState::Reset()
{
	// decrement the lives
	lives_--;
	sf::String	levelText = "Lives: " + std::to_string(lives_);
	livesText_->setString(levelText);

	// pause the game so it doesn't auto launch
	paused_ = true;

	// reset player
	player_->Reset();

	if (lives_ == 0)
	{
		// the game has ended
		gameOver_ = true;
		
		// set text to "lost"
		endText_->setString("You lose!");
		endText_->setFillColor(sf::Color::Red);
	}
	else
	{
		// add new ball
		BallObject* ball = new BallObject(screenWidth_, screenHeight_);

		// place the ball just above the player initially
		sf::Vector2f ballPos;
		ballPos = player_->Position();
		ballPos.y -= 30.0f;

		// update ball position
		ball->SetPosition(ballPos);

		// test ball bouncing
		ball->SetDirection(sf::Vector2f(0.0f, -1.0f));

		ball->Update();

		balls_.push_back(ball);
	}
}