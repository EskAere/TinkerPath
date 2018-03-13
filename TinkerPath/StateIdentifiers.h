#pragma once

/* Identifier for the states of the game */
namespace States
{
	enum ID
	{
		None,
		Title,
		Menu,
		Settings,
		Game,
		Pause,
		GameOver,
		Loading, //Not implemented
	};
}