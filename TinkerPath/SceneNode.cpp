#include "SceneNode.h"
#include "Command.h"
#include "Utility.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>

#include <iostream>


SceneNode::SceneNode(Category::Type category)
	: mChildren()
	, mParent(nullptr)
	, mDefaultCategory(category)
{
}

void SceneNode::attachChild(Ptr child)
{
	child->mParent = this; //Set child's parent pointer to the current node
	mChildren.push_back(std::move(child)); //Move the smart pointer to the container
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode & node)
{
	auto found = std::find_if(mChildren.begin(), mChildren.end(), //Search for the specified node in the container with a lambda expression
		[&](Ptr& p)->bool { return p.get() == &node; }); //Remind : [&] capture all variable by reference

	assert(found != mChildren.end());

	Ptr result = std::move(*found); //Move the found node out of the container
	result->mParent = nullptr; //Remove the parent
	mChildren.erase(found); //Erase the empty element from the container
	return result;
}

void SceneNode::update(sf::Time dt, CommandQueue& commands)
{
	updateCurrent(dt, commands);
	updateChildren(dt, commands);
}

void SceneNode::updateCurrent(sf::Time, CommandQueue&)
{
	// Do nothing by default
}

void SceneNode::updateChildren(sf::Time dt, CommandQueue& commands)
{
	for(Ptr& child : mChildren)
		child->update(dt, commands);
}

sf::Transform SceneNode::getWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity;//The identity transform (does nothing)
	for (const SceneNode* node = this; node != nullptr; node = node->mParent) //From the current node to the root
		transform = node->getTransform() * transform;	//Multiplies all the sf::Transform objects
	return transform;
}

sf::Vector2f SceneNode::getWorldPosition() const
{
	return getWorldTransform() * sf::Vector2f(); //Returns the position by transforming the origin sf::Vector2f() using the absolute transform
}


/*
Current node's transform (Transformable::getTransform()) : Relative to the parent
states : Contains the information where to render the parent node
*/
void SceneNode::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform(); //Combine the parent's absolute transform with the current node's relative one => Absolute transform of the current node

	// Draw node and children with changed transform
	drawCurrent(target, states);
	drawChildren(target, states);

	// Draw bounding rectangle - TO DISABLE
	//drawBoundingRect(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
	for(const Ptr& child: mChildren)
		child->draw(target, states);
}

/* Used for dev to show the hitbox */
void SceneNode::drawBoundingRect(sf::RenderTarget& target, sf::RenderStates) const
{
	sf::FloatRect rect = getBoundingRect();

	sf::RectangleShape shape;
	shape.setPosition(sf::Vector2f(rect.left, rect.top));
	shape.setSize(sf::Vector2f(rect.width, rect.height));
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Green);
	shape.setOutlineThickness(1.f);

	target.draw(shape);
}

void SceneNode::onCommand(const Command& command, sf::Time dt)
{
	//Check if the current scene node is a receiver of the command
	if (command.category & getCategory()) {
		command.action(*this, dt);
	}
		

	//Forward the command to all the child nodes
	for(Ptr& child : mChildren)
		child->onCommand(command, dt);
}

unsigned int SceneNode::getCategory() const
{
	return mDefaultCategory;
}

/* Evaluates collisions of the whole scene graph against all nodes */
void SceneNode::checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs)
{
	checkNodeCollision(sceneGraph, collisionPairs);

	for(Ptr& child : sceneGraph.mChildren)
		checkSceneCollision(*child, collisionPairs);
}

/* Evaluates a collision between *this+children and the argument "node" */
void SceneNode::checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs)
{
	if (this != &node && collision(*this, node)) {
		collisionPairs.insert(std::minmax(this, &node)); //std::minmax ensures that a collision between two entities is inserted only once
	}

	for (Ptr& child : mChildren)
		child->checkNodeCollision(node, collisionPairs);
}

void SceneNode::removeWrecks()
{
	// Remove all children which request so
	auto wreckfieldBegin = std::remove_if(mChildren.begin(), mChildren.end(), std::mem_fn(&SceneNode::isMarkedForRemoval));
	mChildren.erase(wreckfieldBegin, mChildren.end());

	// Call function recursively for all remaining children
	std::for_each(mChildren.begin(), mChildren.end(), std::mem_fn(&SceneNode::removeWrecks));
}

sf::FloatRect SceneNode::getBoundingRect() const
{
	return sf::FloatRect();
}


bool SceneNode::isMarkedForRemoval() const
{
	// By default, remove node if entity is destroyed
	return isDestroyed();
}

bool SceneNode::isDestroyed() const
{
	// By default, scene node needn't be removed
	return false;
}


/* Check for rectangle intersection */
bool collision(const SceneNode& lhs, const SceneNode& rhs)
{
	return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}


//float distance(const SceneNode& lhs, const SceneNode& rhs)
//{
//	return length(lhs.getWorldPosition() - rhs.getWorldPosition());
//}
