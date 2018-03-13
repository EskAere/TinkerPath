#include <map>
#include <string>
#include <algorithm>

#include "PlayerController.h"
#include "Kalee.h"
#include "CommandQueue.h"

#include "iostream"

/* Functor */
struct KaleeMover
{
	KaleeMover(float vx)
		: vx(vx)
	{
	}

	void operator() (Kalee& Kalee, sf::Time) const
	{
		Kalee.accelerate(vx , 0.f);
	}

	float vx;
};

struct KaleeJumper
{
	KaleeJumper(float vy)
		: vy(vy)
	{
	}

	void operator() (Kalee& Kalee, sf::Time) const
	{
		if (!Kalee.isInAir()) {
			Kalee.setJumping(true);
			Kalee.setInAir();
			Kalee.accelerate(0.f, vy);
		}
	}

	float vy;
};

struct KaleeCrafter
{
	KaleeCrafter()
	{
	}

	void operator() (Kalee& Kalee, sf::Time) const
	{
		if (Kalee.getLadderStock()>0) {
			Kalee.decreaseLadder();
			Kalee.setCrafting(true);
		}
	}

};

struct KaleeInvoker
{
	KaleeInvoker()
	{
	}

	void operator() (Kalee& Kalee, sf::Time) const
	{
		if (Kalee.getFeatherStock()>=1) {
			Kalee.emptyFeather();
			Kalee.startInvoking();
		}
	}

};

PlayerController::PlayerController()
{
	// Set initial key bindings
	mKeyBinding[sf::Keyboard::Left] = MoveLeft;
	mKeyBinding[sf::Keyboard::Right] = MoveRight;
	mKeyBinding[sf::Keyboard::Space] = Jump;
	mKeyBinding[sf::Keyboard::Down] = Build;
	mKeyBinding[sf::Keyboard::Up] = Invoke;

	// Set initial action bindings
	initializeActions();

	// Assign all the commands to Kalee
	for(auto& pair : mActionBinding)
		pair.second.category = Category::Kalee;
}

void PlayerController::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		// Check if pressed key appears in key binding, trigger command if so
		auto found = mKeyBinding.find(event.key.code);
		if (found != mKeyBinding.end() && !isRealtimeAction(found->second))
			commands.push(mActionBinding[found->second]);
	}
}

void PlayerController::handleRealtimeInput(CommandQueue& commands)
{
	// Traverse all assigned keys and check if they are pressed
	for(auto pair : mKeyBinding)
	{
		// If key is pressed, lookup action and trigger corresponding command
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second)) {
			commands.push(mActionBinding[pair.second]); //Add the command which corresponds to the action
		}
	}
}

void PlayerController::assignKey(Action action, sf::Keyboard::Key key)
{
	// Remove all keys that already map to action
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); )
	{
		if (itr->second == action)
			mKeyBinding.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	mKeyBinding[key] = action;
}

sf::Keyboard::Key PlayerController::getAssignedKey(Action action) const
{
	for(auto pair : mKeyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}

void PlayerController::initializeActions()
{
	const float playerSpeed = 80.f;

	mActionBinding[MoveLeft].action = derivedAction<Kalee>(KaleeMover(-playerSpeed));
	mActionBinding[MoveRight].action = derivedAction<Kalee>(KaleeMover(+playerSpeed));
	mActionBinding[Jump].action = derivedAction<Kalee>(KaleeJumper(-900.f));
	mActionBinding[Build].action = derivedAction<Kalee>(KaleeCrafter());
	mActionBinding[Invoke].action = derivedAction<Kalee>(KaleeInvoker());
}

/* To avoid mixtures between a real-time action and an event action */
bool PlayerController::isRealtimeAction(Action action)
{
	switch (action)
	{
	case MoveLeft:
	case MoveRight:
	case Jump:
		return true;

	default:
		return false;
	}
}