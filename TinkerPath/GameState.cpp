#include "GameState.h"
#include "Utility.h"
#include "MusicPlayer.h"
#include "iostream"



GameState::GameState(StateStack& stack, Context context) :
	State(stack, context),
	mWorld(*context.window, *context.sounds),
	mPlayerController(*context.playerController),
	mHighScoreText(),
	mScoreText(),
	mLadderText(),
	mFeatherText()
{
	sf::Font& font = context.fonts->get(Fonts::Main);
	sf::Vector2f windowSize(context.window->getSize());

	mHighScoreText.setFont(font);
	mHighScoreText.setCharacterSize(20);
	mHighScoreText.setOutlineThickness(2.f);
	mHighScoreText.setOutlineColor(sf::Color::Black);
	mHighScoreText.setFillColor(sf::Color::White);

	mScoreText.setFont(font);
	mScoreText.setCharacterSize(80);
	mScoreText.setOutlineThickness(3.f);
	mScoreText.setOutlineColor(sf::Color::Black);
	mScoreText.setFillColor(sf::Color::White);
	centerOrigin(mScoreText);
	mScoreText.setPosition(0.5f * windowSize.x, 0);

	mLadderText.setFont(font);
	mLadderText.setCharacterSize(35);
	mLadderText.setOutlineThickness(3.f);
	mLadderText.setOutlineColor(sf::Color::Black);
	mLadderText.setFillColor(sf::Color::White);
	centerOrigin(mLadderText);
	mLadderText.setPosition(0.7f * windowSize.x + 90, 35);

	mFeatherText.setFont(font);
	mFeatherText.setCharacterSize(35);
	mFeatherText.setOutlineThickness(3.f);
	mFeatherText.setOutlineColor(sf::Color::Black);
	mFeatherText.setFillColor(sf::Color::White);
	centerOrigin(mFeatherText);
	mFeatherText.setPosition(0.7f * windowSize.x + 20, 35);

	sf::Texture& textureWood = context.textures->get(Textures::Wood);
	mWoodSprite.setTexture(textureWood);
	mWoodSprite.setPosition(0.7f * windowSize.x + 85, 10);

	sf::Texture& textureFeather = context.textures->get(Textures::Feather);
	mFeatherSprite.setTexture(textureFeather);
	mFeatherSprite.setPosition(0.7f * windowSize.x + 10, 10);

	context.music->setVolume(75.f);
	context.music->play(Music::GameTheme);


	/* Loading score data */
	pugi::xml_parse_result result = mDoc.load_file("Data/Save.xml", pugi::parse_default | pugi::parse_declaration);
	if (!result)
	{
		std::cout << "Parse error: " << result.description() << ", character pos= " << result.offset;
	}
	pugi::xml_node root = mDoc.document_element();
	pugi::xpath_node xpathNode = root.select_single_node("highscore");
	pugi::xml_node selectedNode;
	if (!xpathNode)
	{
		std::cout << "No highscore node";
	}
	else {
		selectedNode = xpathNode.node();
	}
	mCurrentHighScore = selectedNode.text().as_int();
	mHighScoreText.setString(toString(mCurrentHighScore));
	centerOrigin(mHighScoreText);
	mHighScoreText.setPosition(0.5f * windowSize.x, 0.02f * windowSize.y);
}

void GameState::draw()
{
	mWorld.draw();

	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(mWoodSprite);
	window.draw(mFeatherSprite);
	window.draw(mScoreText);
	window.draw(mHighScoreText);
	window.draw(mLadderText);
	window.draw(mFeatherText);

}

bool GameState::update(sf::Time dt)
{
	mWorld.update(dt);
	updateText();

	if (mWorld.hasDeadKalee()) {
		checkHighScore();
		requestStackPush(States::GameOver);
	}

	/* REAL-TIME INPUT HANDLING */
	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayerController.handleRealtimeInput(commands);

	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	// Game input handling
	CommandQueue& commands = mWorld.getCommandQueue();
	mPlayerController.handleEvent(event, commands);

	// Escape pressed, trigger the pause screen
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::Pause);

	return true;
}

void GameState::updateText()
{
	mScoreText.setString(toString(mWorld.getScore()));
	/* Keep the text centered */
	sf::Vector2f windowSize(getContext().window->getSize());
	centerOrigin(mScoreText);
	mScoreText.setPosition(0.5f * windowSize.x , 0.06f * windowSize.y);

	mLadderText.setString(toString(mWorld.getKaleeLadder()));
	mFeatherText.setString(toString(mWorld.getKaleeFeather()));
}

void GameState::checkHighScore()
{
	int score = mWorld.getScore();
	if (score > mCurrentHighScore) {
		pugi::xml_node root = mDoc.document_element();
		pugi::xpath_node xpathNode = root.select_single_node("highscore");
		pugi::xml_node selectedNode = xpathNode.node();
		selectedNode.text().set(score);
		bool saveSucceeded = mDoc.save_file("Data/Save.xml", "  ");
		assert(saveSucceeded);
	}
}
