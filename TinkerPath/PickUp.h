#pragma once
#include "Entity.h"


class PickUp : public Entity
{
public:
	enum Type
	{
		Wood,
		Feather,
		TypeCount
	};

public:
							PickUp(Type type, const TextureHolder& textures);
	virtual unsigned int	getCategory() const;
	sf::FloatRect			getBoundingRect() const;

private:
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	Type					mType;
	sf::Sprite				mSprite;

};

