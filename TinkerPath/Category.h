#pragma once


/* A group of one or multiple game objects which are likely to receive similar commands*/
namespace Category 
{
	enum Type
	{
		None				= 0,
		SceneAirLayer		= 1 << 0,
		Kalee				= 1 << 1,
		Plateform			= 1 << 2,
		Enemy				= 1 << 3,
		Lopion				= 1 << 4,
		Ladder				= 1 << 5,
		Wood				= 1 << 6,
		Feather				= 1 << 7,
		SoundEffect			= 1 << 8,

		PickUp = Wood | Feather,
	};
}