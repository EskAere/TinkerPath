#include "State.h"
#include "StateStack.h"




State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, PlayerController& playerController, MusicPlayer& music, SoundPlayer& sounds)
	: window(&window)
	, textures(&textures)
	, fonts(&fonts)
	, playerController(&playerController)
	, music(&music)
	, sounds(&sounds)
{
}

State::State(StateStack& stack, Context context)
	: mStack(&stack)
	, mContext(context)
{
}

State::~State()
{
}

void State::requestStackPush(States::ID stateID)
{
	mStack->pushState(stateID);
}

void State::requestStackPop()
{
	mStack->popState();
}

void State::requestStateClear()
{
	mStack->clearStates();
}

State::Context State::getContext() const
{
	return mContext;
}