#include "Enemy.h"
#include "Utility.h"
#include "DataTables.h"
#include "iostream"

namespace
{
	const std::vector<EnemyData> Table = initializeEnemyData();
}

Enemy::Enemy(Type type, const TextureHolder& textures)
	: mType(type)
	, mAnimation(textures.get(Table[type].texture))
	, mTravelledDistance(0.f)
	, mDirectionIndex(0)
	, mTargetDirection(1.f,1.f)
	, mLifeTime(sf::seconds(10.f))
	, mDeadAnimation(textures.get(Textures::AnimationExplosion))
	, mShowExplosion(false)
{

	mSide = randomInt(2) ? -1.f : 1.f; //Used for the classic enemy to change direction

	/* Animation */
	mAnimation.setFrameSize(sf::Vector2i(160, 120));
	mAnimation.setNumFrames(8);
	mAnimation.setDuration(sf::seconds(0.8f));
	mAnimation.setRepeating(true);
	centerOrigin(mAnimation);

	mDeadAnimation.setFrameSize(sf::Vector2i(128, 128));
	mDeadAnimation.setNumFrames(9);
	mDeadAnimation.setDuration(sf::seconds(0.7f));
	centerOrigin(mDeadAnimation);
}


void Enemy::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (isDestroyed() && mShowExplosion)
		target.draw(mDeadAnimation, states);
	else
		target.draw(mAnimation, states);
	
}

void Enemy::updateCurrent(sf::Time dt, CommandQueue & commands)
{
	mAnimation.update(dt);

	/* Classic enemy */
	if (mType == Enemy::EnemyClassic) {
		updateMovementPattern(dt);
	}

	/* Follow enemy */
	if (mType == Enemy::EnemyFollow) {
		const float approachRate = 1000.f;

		sf::Vector2f newVelocity = unitVector(approachRate * dt.asSeconds() * mTargetDirection + getVelocity());
		newVelocity *= getMaxSpeed();

		setVelocity(newVelocity);

		setLifeTime(dt);
	}

	if (isDestroyed() && mShowExplosion) {
		mDeadAnimation.update(dt);
		return;
	}

	Entity::updateCurrent(dt, commands);
}

unsigned int Enemy::getCategory() const
{
	return Category::Enemy;
}

sf::FloatRect Enemy::getBoundingRect() const
{
	if (!mIsDead) {
		sf::FloatRect boxCollider(
			mAnimation.getGlobalBounds().left  - 25 ,
			mAnimation.getGlobalBounds().top   - 25 ,
			mAnimation.getGlobalBounds().width - 110 ,
			mAnimation.getGlobalBounds().height- 20 );
		return getWorldTransform().transformRect(boxCollider);
	}
	else {
		sf::FloatRect boxCollider(0.f,0.f,0.f,0.f);
		return getWorldTransform().transformRect(boxCollider);
	}
}

float Enemy::getMaxSpeed() const
{
	return Table[mType].speed;
}

bool Enemy::isFollowing()
{
	return mType == Enemy::EnemyFollow;
}

void Enemy::guideTowards(sf::Vector2f position)
{
	assert(mType == Enemy::EnemyFollow);
	mTargetDirection = unitVector(position - getWorldPosition());
}

void Enemy::setDeadAnimation()
{
	mShowExplosion = true;
}

bool Enemy::isMarkedForRemoval() const
{
	return isDestroyed() && (mDeadAnimation.isFinished() || !mShowExplosion);
}

void Enemy::updateMovementPattern(sf::Time dt)
{
	const std::vector<Direction>& directions = Table[mType].directions;
	if (!directions.empty())
	{
		// Moved long enough in current direction: Change direction
		if (mTravelledDistance > directions[mDirectionIndex].distance)
		{
			mDirectionIndex = (mDirectionIndex + 1) % directions.size();
			if (mDirectionIndex == 0) ++mDirectionIndex;
			mTravelledDistance = 0.f;
		}

		// Compute velocity from direction
		float radians = toRadian(directions[mDirectionIndex].angle + 90.f);
		float vx =  mSide * getMaxSpeed() * std::cos(radians);
		//float vy = getMaxSpeed() * std::sin(radians);

		setVelocity(vx, 0.f);

		mTravelledDistance += getMaxSpeed() * dt.asSeconds();
	}
}

void Enemy::setLifeTime(sf::Time dt)
{
	mLifeTime -= dt;
	if (mLifeTime < sf::Time::Zero) {
		this->destroy();
		mShowExplosion = true;
	}
}
