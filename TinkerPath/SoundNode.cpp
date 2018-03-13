#include "SoundNode.h"
#include "SoundPlayer.h"


SoundNode::SoundNode(SoundPlayer& player)
	: SceneNode()
	, mSounds(player)
{
}

void SoundNode::playSound(SoundEffect::ID sound)
{
	mSounds.play(sound);
}

unsigned int SoundNode::getCategory() const
{
	return Category::SoundEffect;
}