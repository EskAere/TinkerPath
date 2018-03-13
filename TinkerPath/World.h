#pragma once
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>
#include <queue>

#include "Command.h"
#include "CommandQueue.h"
#include "SceneNode.h"
#include "SpriteNode.h"
#include "Kalee.h"
#include "Ladder.h"
#include "PickUp.h"
#include "Enemy.h"
#include "Plateform.h"
#include "ResourceHolder.h"
#include "ResourceIdentifier.h"
#include "SoundPlayer.h"	

namespace sf
{
	class RenderWindow;
}

class Lopion;
class World : private sf::NonCopyable
{
public:
	explicit		World(sf::RenderWindow& window, SoundPlayer& sounds);
	void			update(sf::Time dt);
	void			draw();

	CommandQueue&	getCommandQueue();

	int				getScore();
	int				getKaleeLadder();
	int				getKaleeFeather();
	bool			hasDeadKalee();
	void			LopionDead();

private:
	void			loadTextures();
	void			adaptPlayerPosition();
	void			adaptPlayerVelocity();
	void			handleCollisions();

	void			buildScene();
	void			addInitialPlateforms();
	void			generatePlateforms();
	void			addPlateform(Plateform::Type type, float relX, float relY);
	void			spawnPlateforms();
	void			addLadder(Ladder::Type type, float relX, float relY);
	void			spawnLadders();
	void			generateEnemies(sf::Time dt);
	void			addEnemy(Enemy::Type type, float relX, float relY);
	void			spawnEnemies();
	void			addPickUp(PickUp::Type type, float relX, float relY);
	void			spawnPickUp();
	void			generateLopion();
	void			addLopion(float relX, float relY);
	void			checkKaleeActions();
	void			destroyEntitiesOutsideView();
	sf::FloatRect	getViewBounds() const;
	sf::FloatRect	getBattlefieldBounds() const;
	void			guideEnemies();
	void			setMaxHauteur();

private:
	enum Layer
	{
		Background,
		Air,
		AirLadder,
		AirPickUp,
		AirEnemy, //Lopion & Enemies
		Character,
		LayerCount
	};

	template<typename Element>
	struct SpawnPoint
	{
		typedef typename Element::Type Type;
							SpawnPoint(Type type, float x, float y): type(type), x(x), y(y){}

		Type				type;
		float				x;
		float				y;
	};

private:
	sf::RenderWindow&					mWindow;
	sf::View							mWorldView;
	TextureHolder						mTextures;
	SoundPlayer&						mSounds;

	SceneNode							mSceneGraph;
	std::array<SceneNode*, LayerCount>	mSceneLayers;
	CommandQueue						mCommandQueue;

	sf::FloatRect						mWorldBounds;
	SpriteNode*							mBackground;
	sf::Vector2f						mSpawnPosition;
	Kalee*								mKalee;
	bool								mKaleeDead;
	Lopion*								mLopion;
	bool								mLopionAlive;
	float								mHauteur;
	float								mMaxHauteur;

	float 								mNextPlateform;
	sf::Time							mNextEnemy;  
	float								mEnemyCoolDown;

	std::vector<SpawnPoint<Plateform>>	mPlateformSpawnPoints;
	std::vector<SpawnPoint<Ladder>>		mLadderSpawnPoints;
	std::vector<SpawnPoint<PickUp>>		mPickUpSpawnPoints;
	std::vector<SpawnPoint<Enemy>>		mEnemySpawnPoints;

};

