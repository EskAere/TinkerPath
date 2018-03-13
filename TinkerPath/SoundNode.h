#pragma once

#include "SceneNode.h"
#include "ResourceIdentifier.h"


class SoundPlayer;

class SoundNode : public SceneNode
{
public:
	explicit				SoundNode(SoundPlayer& player);
	void					playSound(SoundEffect::ID sound);

	virtual unsigned int	getCategory() const;


private:
	SoundPlayer&			mSounds;
};