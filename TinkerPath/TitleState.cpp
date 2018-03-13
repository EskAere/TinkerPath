#include "TitleState.h"
#include "Utility.h"
#include "ResourceHolder.h"
#include "MusicPlayer.h"

#include <SFML/Graphics/RenderWindow.hpp>


TitleState::TitleState(StateStack& stack, Context context)
	: State(stack, context)
	, mTextTitle()
	, mText()
	, mShowText(true)
	, mTextEffectTime(sf::Time::Zero)
{
	/* BACKGROUND */
	mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen)); //Background sprite

	/* Text : Title */
	mTextTitle.setFont(context.fonts->get(Fonts::Title));
	mTextTitle.setCharacterSize(85);
	mTextTitle.setOutlineThickness(3.f);
	mTextTitle.setOutlineColor(sf::Color::White);
	mTextTitle.setFillColor(sf::Color(77, 230, 244));
	mTextTitle.setString("TINKER PATH");
	centerOrigin(mTextTitle);
	mTextTitle.setPosition(context.window->getView().getSize().x / 2.f, context.window->getView().getSize().y / 2.f - 50);

	/* Text : Press any key to start*/
	mText.setFont(context.fonts->get(Fonts::Main));
	mText.setString("Press any key to start");
	centerOrigin(mText);
	//mText.setPosition(context.window->getView().getSize() / 2.f); //Put the text in the middle of the view
	mText.setPosition(context.window->getView().getSize().x / 2.f , context.window->getView().getSize().y / 2.f + 50);

	context.music->setVolume(75.f);
	context.music->play(Music::MenuTheme);
}

void TitleState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.draw(mBackgroundSprite);

	window.draw(mTextTitle);
	
	if (mShowText)
		window.draw(mText);
}

bool TitleState::update(sf::Time dt)
{
	mTextEffectTime += dt;

	if (mTextEffectTime >= sf::seconds(0.5f))
	{
		mShowText = !mShowText;
		mTextEffectTime = sf::Time::Zero;
	}

	return true;
}

bool TitleState::handleEvent(const sf::Event& event)
{
	// If any key is pressed, trigger the next screen
	if (event.type == sf::Event::KeyReleased)
	{
		requestStackPop();
		requestStackPush(States::Menu);
	}

	return true;
}