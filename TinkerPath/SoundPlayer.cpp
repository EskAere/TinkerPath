#include "SoundPlayer.h"
#include "iostream"

SoundPlayer::SoundPlayer()
	: mSoundBuffers()
	, mSounds()
{
	mSoundBuffers.load(SoundEffect::Aile, "Assets/Music/Sound/Aile.wav");
	mSoundBuffers.load(SoundEffect::PickUp, "Assets/Music/Sound/PickUp.wav");
	mSoundBuffers.load(SoundEffect::EnemyPop, "Assets/Music/Sound/EnemyPop.wav");
	mSoundBuffers.load(SoundEffect::EnemyDead, "Assets/Music/Sound/EnemyDead.wav");
	mSoundBuffers.load(SoundEffect::SetLadder, "Assets/Music/Sound/SetLadder.wav");
	mSoundBuffers.load(SoundEffect::Jump, "Assets/Music/Sound/Jump.wav");
	mSoundBuffers.load(SoundEffect::Button, "Assets/Music/Sound/Button.wav");
}

void SoundPlayer::play(SoundEffect::ID effect)
{
	mSounds.push_back(sf::Sound(mSoundBuffers.get(effect)));
	mSounds.back().play();
}

void SoundPlayer::removeStoppedSounds()
{
	mSounds.remove_if([](const sf::Sound& s)
	{
		return s.getStatus() == sf::Sound::Stopped;
	});
}