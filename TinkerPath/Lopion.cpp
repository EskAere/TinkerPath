#include "Utility.h"
#include "iostream"
#include "Lopion.h"
#include "World.h"

Lopion::Lopion(Kalee* kalee, World* world, const TextureHolder& textures)
	: mAnimation(textures.get(Textures::AnimationLopion))
	, isFree(true)
	, angle(1.f)
	, radius(100.f)
	, delta(0.f)
{
	mKaleePtr = kalee;
	mWorldPtr = world;

	mAnimation.setFrameSize(sf::Vector2i(67, 42));
	mAnimation.setNumFrames(15);
	mAnimation.setDuration(sf::seconds(0.6f));
	mAnimation.setRepeating(true);
	centerOrigin(mAnimation);

}


void Lopion::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{

	target.draw(mAnimation, states);
}

void Lopion::updateCurrent(sf::Time dt, CommandQueue & commands)
{
	mAnimation.update(dt);
	
	if (!isFree) {
		angle += 0.05f;
		float x = radius * cos(angle) + mKaleePtr->getPosition().x;
		float y = radius * sin(angle) + mKaleePtr->getPosition().y;
		setPosition(x,y);
	}
	else {
		angle += 1;
		float x = 270 +   radius * cos(toRadian(angle));
		float y = getPosition().y - 1;
		setPosition(x,y);
	}
	Entity::updateCurrent(dt, commands);
}

unsigned int Lopion::getCategory() const
{
	return Category::Lopion;
}

sf::FloatRect Lopion::getBoundingRect() const
{
	sf::FloatRect boxCollider(mAnimation.getGlobalBounds());
	if (isFree) {
		boxCollider.left -= 33.f;
		boxCollider.top  -= 21.f;
		boxCollider.height += 52.f;
	}
	else {
		boxCollider.left -= 33.f;
		boxCollider.top += 15.f ;
		boxCollider.height -= 30.f;
	}
	
	return getWorldTransform().transformRect(boxCollider);
}

void Lopion::destroy()
{
	mWorldPtr->LopionDead();

	mKaleePtr = nullptr;
	mWorldPtr = nullptr;

	mIsDead = true;
}

bool Lopion::getIsFree()
{
	return isFree;
}

void Lopion::setIsFree(bool var)
{
	isFree = var;
}
