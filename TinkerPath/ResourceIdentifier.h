#pragma once

namespace sf {
	class Texture;
	class Font;
	class SoundBuffer;
}

namespace Textures
{
	enum ID
	{
		/* GUI */
		TitleScreen,
		ButtonNormal,
		ButtonPressed,
		ButtonSelected,

		/* GAME */
		Background,
		Kalee,	

		Floor,
		GrandeHerbe,
		MoyenneHerbe,
		MoyenneNuage,
		PetiteNuage,
		Ladder,

		Wood,
		Feather,

		/* Animation */
		AnimationIdle,
		AnimationRun,
		AnimationJump,
		AnimationDead, //???
		AnimationEnemyFollow,
		AnimationEnemyClassic,
		AnimationEnemyPleat,
		AnimationLopion,
		AnimationPleat,
		AnimationExplosion,
	};
}

namespace Fonts
{
	enum ID
	{
		Main,
		Title,
	};
}

namespace SoundEffect
{
	enum ID
	{
		Jump,
		PickUp,
		EnemyPop,
		EnemyDead,
		KaleeDead,
		SetLadder,
		Aile,
		Button,
	};
}

namespace Music
{
	enum ID
	{
		MenuTheme,
		GameTheme,
	};
}

template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID>	TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>			FontHolder;
typedef ResourceHolder<sf::SoundBuffer, SoundEffect::ID> SoundBufferHolder;