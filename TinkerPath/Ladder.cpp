#include "Ladder.h"
#include "Utility.h"

Ladder::Ladder(Type type, const TextureHolder& textures):
	mType(type),
	mSprite(textures.get(Textures::Ladder))
{
	centerOrigin(mSprite);
}


void Ladder::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(mSprite, states); //Draw mSprite
}

unsigned int Ladder::getCategory() const
{
	return Category::Ladder;
}

sf::FloatRect Ladder::getBoundingRect() const
{
	sf::FloatRect boxCollider(
		mSprite.getGlobalBounds().left,
		mSprite.getGlobalBounds().top + 90,
		mSprite.getGlobalBounds().width,
		mSprite.getGlobalBounds().height - 110);
	return getWorldTransform().transformRect(boxCollider);
}

