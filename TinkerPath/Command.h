#pragma once
#include <functional>
#include <SFML/System/Time.hpp>
#include <cassert>

#include "Category.h"

class SceneNode;

struct Command
{
												Command();
	std::function<void(SceneNode&, sf::Time)>	action; //Function object which can be called on any game object represented by a scene node
	unsigned int								category; //Specify who receive the command
};


/* Takes a function on a class derived from SceneNode and converts it to a function on the SceneNode base class */
template <typename GameObject, typename Function>
std::function<void(SceneNode& node, sf::Time dt)> derivedAction(Function fn)
{
	return [=](SceneNode& node, sf::Time dt)
	{
		assert(dynamic_cast<GameObject*>(&node) != nullptr); //Check if cast is safe

		fn(static_cast<GameObject&>(node), dt); //Downcast and invoke function on it
	};
}