#include "PickUp.h"
#include "Utility.h"
#include "DataTables.h"

namespace
{
	const std::vector<PickupData> Table = initializePickupData();
}

PickUp::PickUp(Type type, const TextureHolder& textures) 
	: mType(type)
	, mSprite(textures.get(Table[type].texture))
{
	centerOrigin(mSprite); //Set the origin of the sprite to its center
}


void PickUp::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(mSprite, states); //Draw mSprite
}

unsigned int PickUp::getCategory() const
{
	switch (mType)
	{
	case Feather:
		return Category::Feather;

	case Wood:
		return Category::Wood;

	default:
		return Category::PickUp;
	}
}

sf::FloatRect PickUp::getBoundingRect() const
{
	sf::FloatRect boxCollider(
		mSprite.getGlobalBounds().left,
		mSprite.getGlobalBounds().top,
		mSprite.getGlobalBounds().width,
		mSprite.getGlobalBounds().height + 50);
	return getWorldTransform().transformRect(boxCollider);
}
