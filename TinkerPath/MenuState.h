#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "State.h"
#include "Container.h"


class MenuState : public State
{
public:
							MenuState(StateStack& stack, Context context);

	virtual void			draw();
	virtual bool			update(sf::Time dt);
	virtual bool			handleEvent(const sf::Event& event);


private:
	sf::Sprite				mBackgroundSprite;
	sf::Text				mTextTitle;
	GUI::Container			mGUIContainer;
};