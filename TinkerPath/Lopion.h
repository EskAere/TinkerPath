#pragma once
#include "Entity.h"
#include "Animation.h"
#include "Kalee.h"

class World;
class Lopion : public Entity
{

public:
							Lopion(Kalee* kalee, World* world, const TextureHolder& textures);
	virtual unsigned int	getCategory() const;
	sf::FloatRect			getBoundingRect() const;
	void					destroy();

	bool					getIsFree();
	void					setIsFree(bool var);

private:
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void			updateCurrent(sf::Time dt, CommandQueue& commands);

private:
	Animation				mAnimation;

	Kalee*					mKaleePtr;
	World*					mWorldPtr;

	bool					isFree;

	float					angle;
	float					radius;
	float					delta;

};