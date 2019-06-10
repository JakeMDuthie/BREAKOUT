#include "Application.h"

Application::Application()
{
	// initialise the window for rendering
	window_ = new sf::RenderWindow(sf::VideoMode(1200, 900), "Breakout Game");

	// initialise the game's timer
	timer_ = new Timer();

	// initialise the game states
	menuState_ = new MenuState(window_);
	playState_ = new PlayState(window_);

	// set the initial state to be the menu state
	currentState_ = menuState_;

	currentState_->OnEnter();
}

Application::~Application()
{
	if (window_)
	{
		delete window_;
		window_ = NULL;
	}

	if (timer_)
	{
		delete timer_;
		timer_ = NULL;
	}

	if (menuState_)
	{
		delete menuState_;
		menuState_ = NULL;
	}

	if (playState_)
	{
		delete playState_;
		playState_ = NULL;
	}
}

// while running, process the active game state's update then render function
bool Application::Run()
{
	// check to see if the window has been closed
	sf::Event event;
	while (window_->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window_->close();
			return false;
		}
	}

	// update the game timer
	timer_->frame();

	// Retrieve desired state from current state
	GAMESTATE desiredState = currentState_->GetDesiredState();

	// Check to see if the current state is still the desired state
	if (currentState_ != EnumToPointer(desiredState))
	{
		// the current state is different to the desired state, change to the desired state
		ChangeGameState(desiredState);
	}

	// process the current game state
	bool running_ = currentState_->Update(timer_->getTime());
	currentState_->Render();

	return running_;
}

// This function handles changing the game's state
void Application::ChangeGameState(GAMESTATE newGameState)
{
	// Unload the current state
	currentState_->OnExit();

	// Determine the state designated by the GAMESTATE enum
	// Change the current state to be the new game state
	currentState_ = EnumToPointer(newGameState);

	// Initialise the new game state
	currentState_->OnEnter();

	if (currentState_ == playState_)
	{
		playState_->LoadLevel(menuState_->Level());
	}
}

// This helper function takes in a gamestate enum variable and returns its correct pointer
GameState* Application::EnumToPointer(GAMESTATE gs)
{
	switch (gs)
	{
	case MENU:
		return menuState_;
	case PLAY:
		return playState_;
	default:
		// Shouldn't get here
		return nullptr;
	}
}