#include "Label.h"
#include "Utility.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


namespace GUI
{

	Label::Label(const std::string& text, const FontHolder& fonts, unsigned int charSize = 16)
		: mText(text, fonts.get(Fonts::Main), charSize)
	{
	}

	/* A label is not selectable */
	bool Label::isSelectable() const
	{
		return false;
	}

	void Label::handleEvent(const sf::Event&)
	{
	}

	void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(mText, states);
	}

	void Label::setText(const std::string& text)
	{
		mText.setString(text);
	}

}