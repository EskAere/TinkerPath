#pragma once
#include "ResourceHolder.h"
#include "ResourceIdentifier.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

#include <list>


class SoundPlayer : private sf::NonCopyable
{
public:
								SoundPlayer();

	void						play(SoundEffect::ID effect);
	void						removeStoppedSounds();

private:
	SoundBufferHolder			mSoundBuffers;
	std::list<sf::Sound>		mSounds;
};
