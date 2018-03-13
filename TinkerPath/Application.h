#pragma once
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include "ResourceHolder.h"
#include "ResourceIdentifier.h"
#include "PlayerController.h"
#include "MusicPlayer.h"
#include "SoundPlayer.h"
#include "StateStack.h"


class Application
{
public:
							Application();
	void					run();


private:
	void					processInput();
	void					update(sf::Time dt);
	void					render();

	void					registerStates();


private:
	static const sf::Time	TimePerFrame;
	bool					mIsPaused = false; //Is the game paused or not

	sf::RenderWindow		mWindow;
	TextureHolder			mTextures;
	FontHolder				mFonts;
	PlayerController		mPlayerController;

	MusicPlayer				mMusic;
	SoundPlayer				mSounds;

	StateStack				mStateStack;
};

