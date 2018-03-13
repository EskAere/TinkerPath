#include "SettingsState.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>


SettingsState::SettingsState(StateStack& stack, Context context)
	: State(stack, context)
	, mTextTitle()
	, mGUIContainer()
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));

	sf::Font& font = context.fonts->get(Fonts::Main);
	sf::Vector2f windowSize(context.window->getSize());

	/* Title */
	mTextTitle.setFont(font);
	mTextTitle.setString("Settings");
	mTextTitle.setCharacterSize(80);
	centerOrigin(mTextTitle);
	mTextTitle.setPosition(0.5f * windowSize.x, 0.1f * windowSize.y);

	// Build key binding buttons and labels
	addButtonLabel(PlayerController::MoveLeft, 220.f, "Move Left", context);
	addButtonLabel(PlayerController::MoveRight, 320.f, "Move Right", context);
	addButtonLabel(PlayerController::Jump, 420.f, "Jump", context);
	addButtonLabel(PlayerController::Build, 520.f, "Build", context);
	addButtonLabel(PlayerController::Invoke, 620.f, "Invoke", context);
	updateLabels();

	auto backButton = std::make_shared<GUI::Button>(context);
	backButton->setPosition(windowSize.x / 2.f, 800.f);
	backButton->setText("Back");
	//backButton->setCallback(std::bind(&SettingsState::requestStackPop, this)); //Autre methode
	backButton->setCallback([this]()
	{
		requestStackPop();
	});

	mGUIContainer.pack(backButton);
}

void SettingsState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.draw(mBackgroundSprite);
	window.draw(mTextTitle);
	window.draw(mGUIContainer);
}

bool SettingsState::update(sf::Time)
{
	return true;
}

bool SettingsState::handleEvent(const sf::Event& event)
{
	bool isKeyBinding = false;

	// Iterate through all key binding buttons to see if they are being pressed, waiting for the user to enter a key
	for (std::size_t action = 0; action < PlayerController::ActionCount; ++action)
	{
		if (mBindingButtons[action]->isActive())
		{
			isKeyBinding = true;
			if (event.type == sf::Event::KeyReleased)
			{
				getContext().playerController->assignKey(static_cast<PlayerController::Action>(action), event.key.code);
				mBindingButtons[action]->deactivate();
			}
			break;
		}
	}

	// If pressed button changed key bindings, update labels; otherwise consider other buttons in container
	if (isKeyBinding)
		updateLabels();
	else
		mGUIContainer.handleEvent(event);

	return false;
}

/* Make sure the labels are writing out the correct name for the key */
void SettingsState::updateLabels()
{
	PlayerController& playerController = *getContext().playerController;

	for (std::size_t i = 0; i < PlayerController::ActionCount; ++i)
	{
		sf::Keyboard::Key key = playerController.getAssignedKey(static_cast<PlayerController::Action>(i));
		mBindingLabels[i]->setText(toString(key));
	}
}

void SettingsState::addButtonLabel(PlayerController::Action action, float y, const std::string& text, Context context)
{
	mBindingButtons[action] = std::make_shared<GUI::Button>(context);
	mBindingButtons[action]->setScale(0.6f, 0.6f);
	mBindingButtons[action]->setPosition(context.window->getView().getSize().x / 2.f - 100, y);
	mBindingButtons[action]->setText(text);
	mBindingButtons[action]->setToggle(true);

	mBindingLabels[action] = std::make_shared<GUI::Label>("", *context.fonts, 30);
	mBindingLabels[action]->setPosition(context.window->getView().getSize().x / 2.f + 80.f, y - 25);

	mGUIContainer.pack(mBindingButtons[action]);
	mGUIContainer.pack(mBindingLabels[action]);
}