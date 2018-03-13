#include "Application.h"
#include "Utility.h"
#include "State.h"
#include "StateIdentifiers.h"
#include "TitleState.h"
#include "GameState.h"
#include "MenuState.h"
#include "PauseState.h"
#include "SettingsState.h"
#include "GameOverState.h"


const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application()
	: mWindow(sf::VideoMode(540, 900), "Tinker Path", sf::Style::Close)
	, mTextures()
	, mFonts()
	, mSounds()
	, mPlayerController()
	, mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayerController, mMusic, mSounds))
{
	mWindow.setKeyRepeatEnabled(false);

	mFonts.load(Fonts::Main, "Assets/Polices/Main.ttf");
	mFonts.load(Fonts::Title, "Assets/Polices/Title.ttf");
	mTextures.load(Textures::TitleScreen, "Assets/Textures/TitleScreen.png");
	mTextures.load(Textures::ButtonNormal, "Assets/Textures/GUI/ButtonNormal.png");
	mTextures.load(Textures::ButtonSelected, "Assets/Textures/GUI/ButtonSelected.png");
	mTextures.load(Textures::ButtonPressed, "Assets/Textures/GUI/ButtonPressed.png");

	mTextures.load(Textures::Wood, "Assets/Textures/Wood.png");
	mTextures.load(Textures::Feather, "Assets/Textures/Feather.png");

	registerStates();
	mStateStack.pushState(States::Title); //Starting state
}

void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mWindow.isOpen())
	{
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();

			/* Prevent the game from changing if we are paused */
			if (!mIsPaused)
			{
				update(TimePerFrame);
			}

			// Check inside this loop, because stack might be empty before update() call
			if (mStateStack.isEmpty())
				mWindow.close();
		}

		render();
	}
}

void Application::processInput()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		mStateStack.handleEvent(event); //Pass the events to the stack

		switch (event.type) {
			/* When the window gains focus */
		case sf::Event::GainedFocus:
			mIsPaused = false;
			break;
			/* When the window loses focus */
		case sf::Event::LostFocus:
			mIsPaused = true;
			break;

			/* If the window is closed */
		case sf::Event::Closed:
			mWindow.close();
			break;
		}
	}
}

void Application::update(sf::Time dt)
{
	mStateStack.update(dt);
}

void Application::render()
{
	mWindow.clear();

	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());

	mWindow.display();
}


void Application::registerStates()
{
	mStateStack.registerState<TitleState>(States::Title);
	mStateStack.registerState<MenuState>(States::Menu);
	mStateStack.registerState<GameState>(States::Game);
	mStateStack.registerState<PauseState>(States::Pause);
	mStateStack.registerState<SettingsState>(States::Settings);
	mStateStack.registerState<GameOverState>(States::GameOver);
}