#pragma once
#include <SFML/Window/Event.hpp>
#include <map>
#include "Command.h"


class CommandQueue;


/* Handle player inputs */
class PlayerController
{
public:
	/* The different kinds of actions (associated with a key) */
	enum Action
	{
		MoveLeft,	//Realtime
		MoveRight,  //Realtime
		Jump,		//Event
		Build,		//Event
		Invoke,     //Event
		ActionCount
	};


public:
	PlayerController();

	void					handleEvent(const sf::Event& event, CommandQueue& commands); //React to the SFML events
	void					handleRealtimeInput(CommandQueue& commands);				 //React to the SFML real-time inputs

	void					assignKey(Action action, sf::Keyboard::Key key);
	sf::Keyboard::Key		getAssignedKey(Action action) const;


private:
	void					initializeActions();
	static bool				isRealtimeAction(Action action);


private:
	std::map<sf::Keyboard::Key, Action>		mKeyBinding;
	std::map<Action, Command>				mActionBinding;
};

