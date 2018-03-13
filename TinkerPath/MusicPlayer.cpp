#include "MusicPlayer.h"





MusicPlayer::MusicPlayer()
	: mMusic()
	, mFilenames()
	, mVolume(100.f)
	, mID(Music::MenuTheme)
{
	mFilenames[Music::MenuTheme] = "Assets/Music/MenuTheme.ogg";
	mFilenames[Music::GameTheme] = "Assets/Music/GameTheme.ogg";
}

void MusicPlayer::play(Music::ID theme)
{
	std::string filename = mFilenames[theme];

	if (!mMusic.openFromFile(filename))
		throw std::runtime_error("Music " + filename + " could not be loaded.");

	mID = theme;
	mMusic.setVolume(mVolume);
	mMusic.setLoop(true);
	mMusic.play();
}



void MusicPlayer::stop()
{
	mMusic.stop();
}

Music::ID MusicPlayer::getMusic()
{
	return mID;
}

void MusicPlayer::setVolume(float volume)
{
	mVolume = volume;
}

void MusicPlayer::setPaused(bool paused)
{
	if (paused)
		mMusic.pause();
	else
		mMusic.play();
}