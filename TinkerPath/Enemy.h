#pragma once
#include "Entity.h"
#include "Animation.h"

class Enemy : public Entity
{
public:
	enum Type
	{
		EnemyClassic,	
		EnemyFollow, 
		EnemyPleat,
		TypeCount
	};

public:
							Enemy(Type type, const TextureHolder& textures);
	virtual unsigned int	getCategory() const;
	virtual sf::FloatRect	getBoundingRect() const;
	float					getMaxSpeed() const;

	bool					isFollowing();
	void					guideTowards(sf::Vector2f position);

	void					setDeadAnimation();
	virtual bool			isMarkedForRemoval() const;


private:
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void			updateCurrent(sf::Time dt, CommandQueue& commands);
	void					updateMovementPattern(sf::Time dt);

	void					setLifeTime(sf::Time dt);

private:
	Type					mType;
	Animation				mAnimation;
	Animation				mDeadAnimation;
	bool					mShowExplosion;

	/* Classic enemy */
	float					mSide;
	float					mTravelledDistance;
	std::size_t				mDirectionIndex;

	/* Follow enemy */
	sf::Vector2f			mTargetDirection;
	sf::Time				mLifeTime;
	
};

