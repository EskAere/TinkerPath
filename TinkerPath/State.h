#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>

#include "StateIdentifiers.h"
#include "ResourceIdentifier.h"


namespace sf
{
	class RenderWindow;
}

class StateStack;
class PlayerController;
class MusicPlayer;
class SoundPlayer;

class State
{
public:
	typedef std::unique_ptr<State> Ptr;

	/* Save system memory by reusing the same fonts, textures. Provide access to the window's view */
	struct Context
	{
							Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, PlayerController& playerController, MusicPlayer& music, SoundPlayer& sounds);
		sf::RenderWindow*	window;
		TextureHolder*		textures;
		FontHolder*			fonts;
		PlayerController*	playerController;
		MusicPlayer*		music;
		SoundPlayer*		sounds;
	};


public:
						State(StateStack& stack, Context context);
	virtual				~State();

	virtual void		draw() = 0;
	virtual bool		update(sf::Time dt) = 0;
	virtual bool		handleEvent(const sf::Event& event) = 0;


protected:
	void				requestStackPush(States::ID stateID); //Ask for a pushState
	void				requestStackPop(); //Ask for a popState
	void				requestStateClear(); //Ask for a clearStates

	Context				getContext() const;


private:
	StateStack*			mStack;
	Context				mContext;
};