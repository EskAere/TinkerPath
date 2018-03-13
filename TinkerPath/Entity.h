#pragma once
#include "SceneNode.h"

class Entity : public SceneNode
{
public:
	void			setVelocity(sf::Vector2f velocity);
	void			setVelocity(float vx, float vy);
	void			accelerate(sf::Vector2f velocity);
	void			accelerate(float vx, float vy);
	sf::Vector2f	getVelocity() const;
	virtual void	destroy();
	virtual bool	isDestroyed() const;

	void			playLocalSound(CommandQueue& commands, SoundEffect::ID effect);

protected:
	virtual void	updateCurrent(sf::Time dt, CommandQueue& commands);

protected:
	sf::Vector2f	mVelocity;
	bool			mIsDead = false;
};

