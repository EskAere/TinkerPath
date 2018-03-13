#include "Kalee.h"
#include "Utility.h"


Kalee::Kalee(const TextureHolder& textures)
	: mSprite(textures.get(Textures::Kalee)),
	mLadderStock(3),
	mFeatherStock(3),
	isCrafting(false),
	isInvoking(false),
	jumping(false),
	mPleatAnimation(textures.get(Textures::AnimationPleat)),
	mShowAnimation(false)
{
	
	mPleatAnimation.setFrameSize(sf::Vector2i(338, 150));
	mPleatAnimation.setNumFrames(5);
	mPleatAnimation.setDuration(sf::seconds(0.3f));
	centerOrigin(mPleatAnimation);

	/*mIdle.setFrameSize(sf::Vector2i(316, 212));
	mIdle.setNumFrames(8);
	mIdle.setScale(0.5f, 0.5f);
	mIdle.setDuration(sf::seconds(0.8f));
	mIdle.setRepeating(true);
	centerOrigin(mIdle);*/



	centerOrigin(mSprite); 
}

int Kalee::getLadderStock()
{
	return mLadderStock;
}

void Kalee::increaseLadder()
{
	++mLadderStock;
}

void Kalee::decreaseLadder()
{
	--mLadderStock;
}

bool Kalee::isCraftingLadder()
{
	return isCrafting;
}

void Kalee::setCrafting(bool bol)
{	
	isCrafting = bol;
}

int Kalee::getFeatherStock()
{
	return mFeatherStock;
}

void Kalee::increaseFeather()
{
	++mFeatherStock;
}

void Kalee::emptyFeather()
{
	mFeatherStock -= 1;
}

bool Kalee::isInvokingPleat()
{
	return isInvoking;
}

void Kalee::startInvoking()
{
	isInvoking = true;
	mShowAnimation = true;
	mPleatAnimation.restart();
}

void Kalee::stopInvoking()
{
	isInvoking = false;
}

void Kalee::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
	if (mShowAnimation) {
		target.draw(mPleatAnimation, states);
	}
}

void Kalee::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (mShowAnimation) {
		mPleatAnimation.update(dt);

		if (mPleatAnimation.isFinished()) {
			mPleatAnimation.restart();
			mShowAnimation = false;
		}
	}
	Entity::updateCurrent(dt, commands);
}

unsigned int Kalee::getCategory() const
{
	return Category::Kalee;
}

/* Collision rectangle */
sf::FloatRect Kalee::getBoundingRect() const
{
	sf::FloatRect boxCollider(
		mSprite.getGlobalBounds().left + 20, 
		mSprite.getGlobalBounds().top + 79, 
		mSprite.getGlobalBounds().width - 35, 
		mSprite.getGlobalBounds().height - 79);
	return getWorldTransform().transformRect(boxCollider);
}


bool Kalee::isInAir()
{
	return mInAir;
}

void Kalee::setInAir()
{
	mInAir = true;
}

void Kalee::stopInAir()
{
	mInAir = false;
	mVelocity.y = 0.0f;
}

bool Kalee::isJumping()
{
	return jumping;
}

void Kalee::setJumping(bool bol)
{
	jumping = bol;
}



