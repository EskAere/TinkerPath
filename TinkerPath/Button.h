#pragma once

#include "Component.h"
#include "ResourceIdentifier.h"
#include "ResourceHolder.h"
#include "State.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>
#include <string>
#include <memory>
#include <functional>

class SoundPlayer;

namespace GUI
{

	class Button : public Component
	{
	public:
		typedef std::shared_ptr<Button>		Ptr;
		typedef std::function<void()>		Callback;


	public:
								Button(State::Context context);

		void					setCallback(Callback callback);
		void					setText(const std::string& text);
		void					setToggle(bool flag);

		virtual bool			isSelectable() const;
		virtual void			select();
		virtual void			deselect();

		virtual void			activate();
		virtual void			deactivate();

		virtual void			handleEvent(const sf::Event& event);


	private:
		virtual void			draw(sf::RenderTarget& target, sf::RenderStates states) const;


	private:
		Callback				mCallback;
		const sf::Texture&		mNormalTexture;
		const sf::Texture&		mSelectedTexture;
		const sf::Texture&		mPressedTexture;
		sf::Sprite				mSprite;
		sf::Text				mText;
		bool					mIsToggle; //Allow the button to stay pressed 
		SoundPlayer&			mSounds;
	};

}