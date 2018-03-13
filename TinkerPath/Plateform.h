#pragma once
#include "Entity.h"


class Plateform : public Entity
{

public:
	enum Type
	{
		GrandeHerbe,
		MoyenneHerbe,
		MoyenneNuage,
		PetiteNuage,
		TypeCount
	};

public:
							Plateform(Type type, const TextureHolder& textures);
	virtual unsigned int	getCategory() const;
	virtual sf::FloatRect	getBoundingRect() const;

private:
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
	Type					mType;
	sf::Sprite				mSprite;
};

