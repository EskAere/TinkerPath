#pragma once
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <vector>
#include <utility>
#include <functional>
#include <map>

#include "State.h"
#include "StateIdentifiers.h"
#include "ResourceIdentifier.h"

namespace sf
{
	class Event;
	class RenderWindow;
}

class StateStack : private sf::NonCopyable
{
public:
	enum Action
	{
		Push,
		Pop,
		Clear,
	};


public:
	explicit			StateStack(State::Context context);

	template <typename T> //The derived state class we want to register
	void				registerState(States::ID stateID); //Register in the stack a state we may use

	void				update(sf::Time dt); //Update all active states from top to bottom (may stop)
	void				draw(); //Draw all active states from bottom to top
	void				handleEvent(const sf::Event& event); //Deliver event to the active states from top to bottom (may stop)


						/* These 3 methods add a PendingChange (an action to do later) in the mPendingList*/
	void				pushState(States::ID stateID); //Add state to the stack
	void				popState(); //Remove a state from the stack
	void				clearStates(); //Empty the stack

	bool				isEmpty() const;


private:
	State::Ptr			createState(States::ID stateID); //Returns a smart pointer to a newly created ..State class 
	void				applyPendingChanges(); //Apply the delayed requests


private:
	struct PendingChange
	{
		explicit			PendingChange(Action action, States::ID stateID = States::None);

		Action				action;
		States::ID			stateID;
	};


private:
	std::vector<State::Ptr>								mStack;
	std::vector<PendingChange>							mPendingList;

	State::Context										mContext;
	std::map<States::ID, std::function<State::Ptr()>>	mFactories; //Maps state IDs to the factory funtions (which create a new state on-demand)
};


template <typename T>
void StateStack::registerState(States::ID stateID)
{
	mFactories[stateID] = [this]()
	{
		return State::Ptr(new T(*this, mContext)); //Create a new ..State 
	};
}