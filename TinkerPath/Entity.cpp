#include "Entity.h"
#include "CommandQueue.h"
#include "SoundNode.h"

void Entity::setVelocity(sf::Vector2f velocity)
{
	mVelocity = velocity;
}

void Entity::setVelocity(float vx, float vy)
{
	mVelocity.x = vx;
	mVelocity.y = vy;
}

sf::Vector2f Entity::getVelocity() const
{
	return mVelocity;
}

void Entity::accelerate(sf::Vector2f velocity)
{
	mVelocity += velocity;
}

void Entity::accelerate(float vx, float vy)
{
	mVelocity.x += vx;
	mVelocity.y += vy;
}

void Entity::updateCurrent(sf::Time dt, CommandQueue& command)
{
	move(mVelocity * dt.asSeconds()); //Shortcut from sf::Transformable for : setPosition(getPosition() + mVelocity * dt.asSeconds())
}

void Entity::destroy()
{
	mIsDead = true;
}

bool Entity::isDestroyed() const
{
	return mIsDead;
}

void Entity::playLocalSound(CommandQueue & commands, SoundEffect::ID effect)
{

	Command command;
	command.category = Category::SoundEffect;
	command.action = derivedAction<SoundNode>(
		[effect](SoundNode& node, sf::Time)
	{
		node.playSound(effect);
	});

	commands.push(command);
}


