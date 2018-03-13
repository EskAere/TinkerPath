#include "MenuState.h"
#include "Utility.h"
#include "Button.h"
#include "ResourceHolder.h"
#include "MusicPlayer.h"

#include "pugixml.hpp"
#include "iostream"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


MenuState::MenuState(StateStack& stack, Context context)
	: State(stack, context)
	, mGUIContainer()
{
	sf::Texture& texture = context.textures->get(Textures::TitleScreen);
	mBackgroundSprite.setTexture(texture);

	sf::Vector2f windowSize(context.window->getSize());

	/* Text : Title */
	mTextTitle.setFont(context.fonts->get(Fonts::Title));
	mTextTitle.setCharacterSize(85);
	mTextTitle.setOutlineThickness(3.f);
	mTextTitle.setOutlineColor(sf::Color::White);
	mTextTitle.setFillColor(sf::Color(77, 230, 244));
	mTextTitle.setString("TINKER PATH");
	centerOrigin(mTextTitle);
	mTextTitle.setPosition(context.window->getView().getSize().x / 2.f, context.window->getView().getSize().y / 2.f - 290);

	auto playButton = std::make_shared<GUI::Button>(context);
	playButton->setPosition(windowSize.x / 2.f, 350);
	playButton->setText("Play");
	playButton->setCallback([this]()
	{
		requestStackPop();
		requestStackPush(States::Game);
	});

	auto settingsButton = std::make_shared<GUI::Button>(context);
	settingsButton->setPosition(windowSize.x / 2.f, 500);
	settingsButton->setText("Settings");
	settingsButton->setCallback([this]()
	{
		requestStackPush(States::Settings);
	});

	auto exitButton = std::make_shared<GUI::Button>(context);
	exitButton->setPosition(windowSize.x / 2.f, 650);
	exitButton->setText("Exit");
	exitButton->setCallback([this]()
	{
		requestStackPop();
	});

	mGUIContainer.pack(playButton);
	mGUIContainer.pack(settingsButton);
	mGUIContainer.pack(exitButton);

	if (context.music->getMusic() != Music::MenuTheme) {
		context.music->play(Music::MenuTheme);
	}
}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
	window.draw(mTextTitle);
	window.draw(mGUIContainer);
}

bool MenuState::update(sf::Time)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	return false;
}