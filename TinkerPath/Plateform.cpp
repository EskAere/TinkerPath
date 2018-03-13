#include "Plateform.h"
#include "Utility.h"
#include "DataTables.h"

namespace
{
	const std::vector<PlateformData> Table = initializePlateformData();
}


Plateform::Plateform(Type type, const TextureHolder& textures)
	: mType(type)
	, mSprite(textures.get(Table[type].texture))
{
	centerOrigin(mSprite);
}


void Plateform::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(mSprite, states); //Draw mSprite
}

unsigned int Plateform::getCategory() const
{
	return Category::Plateform;
}

sf::FloatRect Plateform::getBoundingRect() const
{
	sf::FloatRect boxCollider(
		mSprite.getGlobalBounds().left,
		mSprite.getGlobalBounds().top,
		mSprite.getGlobalBounds().width,
		mSprite.getGlobalBounds().height - 10);
	return getWorldTransform().transformRect(boxCollider);
}

