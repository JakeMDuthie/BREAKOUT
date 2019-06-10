#include "menu_state.h"
#include <math.h>

MenuState::MenuState(sf::RenderWindow* window)
{
	window_ = window;

	screenHeight_ = window_->getSize().y;
	screenWidth_ = window_->getSize().x;
}

MenuState::~MenuState()
{
	
}

void MenuState::OnEnter()
{
	// set desired state to be this state
	SetDesiredState(MENU);

	level_ = 1;

	// initialise and load font
	font_ = new sf::Font();

	font_->loadFromFile("Resources/Bison.ttf");

	// initialise text elements
	menuLogo_ = new sf::Text();

	menuLogo_->setString("BREAKOUT");
	menuLogo_->setFont(*font_);
	menuLogo_->setCharacterSize(90);
	menuLogo_->setPosition((screenWidth_/2.0f)-90.0f, 400.0f);

	subLogo_ = new sf::Text();

	subLogo_->setString("Press Space To Begin...");
	subLogo_->setFont(*font_);
	subLogo_->setCharacterSize(30);
	subLogo_->setPosition((screenWidth_ / 2.0f) - 90.0f, 500.0f); 
	
	levelSelector_ = new sf::Text();

	levelSelector_->setString("Level: ");
	levelSelector_->setFont(*font_);
	levelSelector_->setCharacterSize(30);
	levelSelector_->setPosition((screenWidth_ / 2.0f) - 90.0f, 550.0f);
}

void MenuState::OnExit()
{
	if (font_)
	{
		delete font_;
		font_ = NULL;
	}

	if (menuLogo_)
	{
		delete menuLogo_;
		menuLogo_ = NULL;
	}

	if (subLogo_)
	{
		delete subLogo_;
		subLogo_ = NULL;
	}

	if (levelSelector_)
	{
		delete levelSelector_;
		levelSelector_ = NULL;
	}
}

void MenuState::Render()
{
	// clear the display before rendering any objects
	window_->clear();

	window_->draw(*menuLogo_);
	window_->draw(*subLogo_);
	window_->draw(*levelSelector_);

	// present the completed frame to the window
	window_->display();
}

bool MenuState::Update(float frame_time)
{
	// handle user input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		// play the game
		SetDesiredState(PLAY);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
	{
		level_ = 1;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
	{
		level_ = 2;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
	{
		level_ = 3;
	}
	sf::String	levelText = "Level: " + std::to_string(level_);
	levelSelector_->setString(levelText);

	return true;
}
