#include "Container.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


namespace GUI
{

	Container::Container(): mChildren(), mSelectedChild(-1)
	{
	}

	/* Add a new component into the list */
	void Container::pack(Component::Ptr component)
	{
		mChildren.push_back(component);

		/* Check if we have a currently selected child */
		if (!hasSelection() && component->isSelectable())
			select(mChildren.size() - 1); //If not, select the child 
	}


	/* A container is not selectable */
	bool Container::isSelectable() const
	{
		return false;
	}

	void Container::handleEvent(const sf::Event& event)
	{
		/* If we have selected a child then give it events */
		if (hasSelection() && mChildren[mSelectedChild]->isActive())
		{
			mChildren[mSelectedChild]->handleEvent(event);
		}

		/* Navigation through the container or Activation of any selected component */
		else if (event.type == sf::Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::Z || event.key.code == sf::Keyboard::Up)
			{
				selectPrevious();
			}
			else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
			{
				selectNext();
			}
			else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
			{
				if (hasSelection())
					mChildren[mSelectedChild]->activate();
			}
		}
	}

	void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		for(const Component::Ptr& child : mChildren)
			target.draw(*child, states);
	}

	/* Check if the mSelectedChild variable is a valid index, zero or more */
	bool Container::hasSelection() const
	{
		return mSelectedChild >= 0;
	}

	/* Select a component by making sure only one is selected */
	void Container::select(std::size_t index)
	{
		if (mChildren[index]->isSelectable())
		{
			if (hasSelection()) //If a component is selected
				mChildren[mSelectedChild]->deselect(); //Deselect the previously selected child

			mChildren[index]->select();
			mSelectedChild = index;
		}
	}

	
	void Container::selectNext()
	{
		if (!hasSelection())
			return;

		// Search next component that is selectable, wrap around if necessary
		int next = mSelectedChild;
		do
			next = (next + 1) % mChildren.size();
		while (!mChildren[next]->isSelectable());

		// Select that component
		select(next);
	}

	void Container::selectPrevious()
	{
		if (!hasSelection())
			return;

		// Search previous component that is selectable, wrap around if necessary
		int prev = mSelectedChild;
		do
			prev = (prev + mChildren.size() - 1) % mChildren.size();
		while (!mChildren[prev]->isSelectable());

		// Select that component
		select(prev);
	}

}