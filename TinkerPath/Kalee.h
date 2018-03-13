#pragma once
#include "Entity.h"
#include "Animation.h"

class Kalee : public Entity
{
public:
	explicit				Kalee(const TextureHolder& textures);
	virtual unsigned int	getCategory() const;
	sf::FloatRect			getBoundingRect() const;
	bool					isInAir();
	void					setInAir();
	void					stopInAir();
	bool					isJumping();
	void					setJumping(bool bol);

	int						getLadderStock();
	void					increaseLadder();
	void					decreaseLadder();
	bool					isCraftingLadder();
	void					setCrafting(bool bol);

	int						getFeatherStock();
	void					increaseFeather();
	void					emptyFeather();
	bool					isInvokingPleat();
	void					startInvoking();
	void					stopInvoking();
	

private:
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void			updateCurrent(sf::Time dt, CommandQueue& commands);

private:
	sf::Sprite				mSprite;
	bool					mInAir = false;

	Animation				mPleatAnimation;
	bool					mShowAnimation;

	int						mLadderStock;
	bool					isCrafting;

	int						mFeatherStock;
	bool					isInvoking;

	bool					jumping;
};

