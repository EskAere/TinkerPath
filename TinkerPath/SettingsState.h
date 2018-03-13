#pragma once
#include "State.h"
#include "PlayerController.h"
#include "Container.h"
#include "Button.h"
#include "Label.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <array>


class SettingsState : public State
{
public:
									SettingsState(StateStack& stack, Context context);

	virtual void					draw();
	virtual bool					update(sf::Time dt);
	virtual bool					handleEvent(const sf::Event& event);


private:
	void							updateLabels();
	void							addButtonLabel(PlayerController::Action action, float y, const std::string& text, Context context);


private:
	sf::Sprite													mBackgroundSprite;
	sf::Text													mTextTitle;
	GUI::Container												mGUIContainer;
	std::array<GUI::Button::Ptr, PlayerController::ActionCount>	mBindingButtons;
	std::array<GUI::Label::Ptr, PlayerController::ActionCount> 	mBindingLabels;
};