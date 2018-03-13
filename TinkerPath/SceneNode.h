#pragma once
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics.hpp>

#include <vector>
#include <set>
#include <memory>
#include <cassert>

#include "Category.h"
#include "ResourceIdentifier.h"
#include "ResourceHolder.h"


struct Command;
class CommandQueue;

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
	typedef std::unique_ptr<SceneNode> Ptr;
	typedef std::pair<SceneNode*, SceneNode*> Pair;

public:
	explicit				SceneNode(Category::Type category = Category::None);

	void					attachChild(Ptr child);
	Ptr						detachChild(const SceneNode& node);

	void					update(sf::Time dt, CommandQueue& commands);

	sf::Transform			getWorldTransform() const; 
	sf::Vector2f			getWorldPosition() const; //Return the absolute position (sf::Transformable::getPosition() returns the relative position)

	void					onCommand(const Command& command, sf::Time dt);//Called every time a command is passed to the scene graph
	virtual unsigned int	getCategory() const;

	void					checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs); //Not Used
	void					checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs); 
	void					removeWrecks();
	virtual sf::FloatRect	getBoundingRect() const;
	virtual bool			isMarkedForRemoval() const;
	virtual bool			isDestroyed() const;

private:
	virtual void			updateCurrent(sf::Time dt, CommandQueue& commands);
	void					updateChildren(sf::Time dt, CommandQueue& commands);

	virtual void			draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void					drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;
	void					drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	std::vector<Ptr>		mChildren;
	SceneNode*				mParent;
	Category::Type			mDefaultCategory;
};

bool	collision(const SceneNode& lhs, const SceneNode& rhs);
//float	distance(const SceneNode& lhs, const SceneNode& rhs);