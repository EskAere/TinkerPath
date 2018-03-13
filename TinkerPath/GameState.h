#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "State.h"
#include "World.h"
#include "PlayerController.h"
#include "pugixml.hpp"


class GameState : public State
{
public:
						GameState(StateStack& stack, Context context);

	virtual void		draw();
	virtual bool		update(sf::Time dt);
	virtual bool		handleEvent(const sf::Event& event);

	void				updateText();

private:
	void				checkHighScore();

private:
	World				mWorld;
	PlayerController&	mPlayerController;

	sf::Text			mScoreText;
	sf::Text			mHighScoreText;
	sf::Text			mLadderText;
	sf::Text			mFeatherText;

	sf::Sprite			mWoodSprite;
	sf::Sprite			mFeatherSprite;

	pugi::xml_document	mDoc;
	int					mCurrentHighScore;
};