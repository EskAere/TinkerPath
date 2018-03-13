#include "World.h"
#include "Lopion.h"
#include "Utility.h"
#include "SoundNode.h"
#include <SFML/Graphics/RenderWindow.hpp>


#include <algorithm>
#include <cmath>
#include <limits>

World::World(sf::RenderWindow & window, SoundPlayer& sounds) :
	mWindow(window),
	mWorldView(window.getDefaultView()),
	mTextures(),
	mSounds(sounds),
	mSceneGraph(),
	mSceneLayers(),
	mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 1800.f),
	mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - 73 - 40), //TODO : Code sale
	mKalee(nullptr),
	mKaleeDead(false),
	mHauteur(0.f),
	mMaxHauteur(0.f),
	mNextPlateform(0.f),
	mNextEnemy(sf::seconds(15.f)),
	mEnemyCoolDown(20.f),
	mLopion(nullptr),
	mLopionAlive(false)
{
	loadTextures();
	buildScene();

	mWorldView.setCenter(mWorldView.getSize().x / 2.f, mWorldBounds.height * 3.f/4.f); //Set the inital view of our world
}

void World::update(sf::Time dt)
{
	/* Scroll of the view & the background */
	mHauteur = mKalee->getPosition().y - mSpawnPosition.y;
	mWorldView.setCenter(mWorldView.getCenter().x, mHauteur + 1350.f); //The view follows Kalee's movements
	mBackground->setPosition(mBackground->getPosition().x, mHauteur*0.95f); //Background's movements

	/* Remove Entities outside the view */
	destroyEntitiesOutsideView();
	guideEnemies();

	/* Forward commands to scene graph */
	while (!mCommandQueue.isEmpty())
	{
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	}
	adaptPlayerVelocity(); //Set kalee's velocity

	/* Collision detection and response */
	handleCollisions();

	/* Remove destroyed entities, Spawn new plateforms */
	mSceneGraph.removeWrecks();
	spawnPlateforms();
	spawnLadders();
	spawnPickUp();
	spawnEnemies();

	/* Regular update step */
	mSceneGraph.update(dt, mCommandQueue);
	adaptPlayerPosition(); //Avoid kalee leaving the view
	
	checkKaleeActions();
	generatePlateforms();
	generateEnemies(dt);
	setMaxHauteur();

	/* Sound */
	mSounds.removeStoppedSounds();
}

void World::draw()
{
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}


CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}

void World::loadTextures()
{
	mTextures.load(Textures::Kalee, "Assets/Textures/Kalee.png");
	mTextures.load(Textures::Background, "Assets/Textures/BG.png");
	mTextures.load(Textures::Floor, "Assets/Textures/Floor.png");

	/* Plateform */
	mTextures.load(Textures::GrandeHerbe, "Assets/Textures/GrandeHerbe.png");
	mTextures.load(Textures::MoyenneHerbe, "Assets/Textures/MoyenneHerbe.png");
	mTextures.load(Textures::MoyenneNuage, "Assets/Textures/MoyenneNuage.png");
	mTextures.load(Textures::PetiteNuage, "Assets/Textures/PetiteNuage.png");

	mTextures.load(Textures::Ladder, "Assets/Textures/Ladder.png");

	/* Enemy */
	mTextures.load(Textures::AnimationEnemyClassic, "Assets/Textures/AnimationEnemyClassic.png");
	mTextures.load(Textures::AnimationEnemyFollow, "Assets/Textures/AnimationEnemyFollow.png");
	mTextures.load(Textures::AnimationEnemyPleat, "Assets/Textures/AnimationEnemyPleat.png");

	/* PickUp */
	mTextures.load(Textures::Wood, "Assets/Textures/Wood.png");
	mTextures.load(Textures::Feather, "Assets/Textures/Feather.png");

	/* Other */
	mTextures.load(Textures::AnimationPleat, "Assets/Textures/AnimationPleat.png");
	mTextures.load(Textures::AnimationLopion, "Assets/Textures/AnimationLopion.png");
	mTextures.load(Textures::AnimationExplosion, "Assets/Textures/Explosion.png");


}

void World::adaptPlayerPosition()
{
	// Keep player's position inside the screen bounds, at least borderDistance units from the border
	sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
	const float borderDistance = 25.f;

	sf::Vector2f position = mKalee->getPosition();
	position.x = std::max(position.x, viewBounds.left + borderDistance);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	position.y = std::max(position.y, viewBounds.top + borderDistance);
	position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
	mKalee->setPosition(position);

	/* Stop Kalee on the floor */
	if (position.y >=1687) { 
		mKalee->stopInAir();
		mKalee->setPosition(mKalee->getPosition().x, 1687);
	}
}

void World::adaptPlayerVelocity()
{
	sf::Vector2f velocity = mKalee->getVelocity();

	// Maximal X speed
	velocity.x = std::max(velocity.x, -500.f);
	velocity.x = std::min(velocity.x, 500.f);

	// Add friction force 
	if (velocity.x > 20) {
		velocity.x -= 20;
	}
	else if (velocity.x < -20){
		velocity.x += 20;
	}
	else {
		velocity.x = 0;
	}

	// Add gravity (Only if we are jumping)
	if (mKalee->isInAir()) {
		velocity.y += 30;
		velocity.y = std::min(velocity.y, 1000.f); //Maximal Y speed
	}

	mKalee->setVelocity(velocity);
}

/* Return true if a pair matches two categories */
bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
	unsigned int category1 = colliders.first->getCategory();
	unsigned int category2 = colliders.second->getCategory();

	// Make sure first pair entry has category type1 and second has type2
	if (type1 & category1 && type2 & category2)
	{
		return true;
	}
	else if (type1 & category2 && type2 & category1)
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
	{
		return false;
	}
}

void World::handleCollisions()
{
	bool plateformCollisionOccured = false; 

	std::set<SceneNode::Pair> collisionPairs;
	/* Kalee collision */
	mSceneLayers[Air]->checkNodeCollision(*mKalee, collisionPairs);			//Plateform collisions
	mSceneLayers[AirLadder]->checkNodeCollision(*mKalee, collisionPairs);	//Ladder collisions		
	mSceneLayers[AirPickUp]->checkNodeCollision(*mKalee, collisionPairs);	//PickUp collisions
	mSceneLayers[AirEnemy]->checkNodeCollision(*mKalee, collisionPairs);	//Enemy collisions

	/* Lopion collision */
	if (mLopionAlive) {
		if (!mLopion->getIsFree()) {
			mSceneLayers[AirPickUp]->checkNodeCollision(*mLopion, collisionPairs);	
			mSceneLayers[AirEnemy]->checkNodeCollision(*mLopion, collisionPairs);	
		}
	}

	for(SceneNode::Pair pair: collisionPairs)
	{
		if (matchesCategories(pair, Category::Kalee, Category::Plateform))
		{
			auto& kalee = static_cast<Kalee&>(*pair.first);
			auto& plateform = static_cast<Plateform&>(*pair.second);

			if (kalee.getVelocity().y > 0) { /* Avoid to stop Kalee when she goes Up , Stop only when Kalee is falling*/
				kalee.stopInAir();
				kalee.setPosition(kalee.getPosition().x, plateform.getPosition().y - 40 - 13); //Code sale //-1px to detect collision when staying on a plateform
			}
			plateformCollisionOccured = true;
		}

		else if (matchesCategories(pair, Category::Kalee, Category::Ladder))
		{
			auto& kalee = static_cast<Kalee&>(*pair.first);
			auto& ladder = static_cast<Ladder&>(*pair.second);

			if (kalee.getVelocity().y > 0) { /* Avoid to stop Kalee when she goes Up , Stop only when Kalee is falling*/
				kalee.stopInAir();
				kalee.setPosition(kalee.getPosition().x, ladder.getPosition().y); 
			}
			plateformCollisionOccured = true;
		}

		else if (matchesCategories(pair, Category::Kalee, Category::Wood))
		{
			auto& kalee = static_cast<Kalee&>(*pair.first);
			auto& wood = static_cast<PickUp&>(*pair.second);

			kalee.increaseLadder();
			wood.destroy();
			generateLopion();
			mSounds.play(SoundEffect::PickUp);
		}

		else if (matchesCategories(pair, Category::Kalee, Category::Feather))
		{
			auto& kalee = static_cast<Kalee&>(*pair.first);
			auto& feather = static_cast<PickUp&>(*pair.second);

			kalee.increaseFeather();
			feather.destroy();
			generateLopion();
			mSounds.play(SoundEffect::PickUp);
		}

		else if (matchesCategories(pair, Category::Kalee, Category::Enemy))
		{
			auto& kalee = static_cast<Kalee&>(*pair.first);
			auto& enemy = static_cast<Enemy&>(*pair.second);

			enemy.destroy();
			enemy.setDeadAnimation();
			mKaleeDead = true;
			mSounds.play(SoundEffect::EnemyDead);
		}

		else if (matchesCategories(pair, Category::Kalee, Category::Lopion))
		{
			auto& kalee = static_cast<Kalee&>(*pair.first);
			auto& lopion = static_cast<Lopion&>(*pair.second);

			lopion.setIsFree(false);
		}
		else if (matchesCategories(pair, Category::Lopion, Category::Wood))
		{
			//auto& lopion = static_cast<Lopion&>(*pair.first);
			auto& wood = static_cast<PickUp&>(*pair.second);

			wood.destroy();
			mKalee->increaseLadder();
			mSounds.play(SoundEffect::PickUp);

		}
		else if (matchesCategories(pair, Category::Lopion, Category::Feather))
		{
			//auto& lopion = static_cast<Lopion&>(*pair.first);
			auto& feather = static_cast<PickUp&>(*pair.second);

			feather.destroy();
			mKalee->increaseFeather();
			mSounds.play(SoundEffect::PickUp);

		}
		else if (matchesCategories(pair, Category::Lopion, Category::Enemy))
		{
			auto& lopion = static_cast<Lopion&>(*pair.first);
			auto& enemy = static_cast<Enemy&>(*pair.second);

			lopion.destroy();
			enemy.destroy();
			enemy.setDeadAnimation();
			mSounds.play(SoundEffect::EnemyDead);

		}
	}

	if (!plateformCollisionOccured) {
		mKalee->setInAir();
	}
}


void World::buildScene()
{
	/* Initialize the different scene layers */
	for (std::size_t i = 0; i < LayerCount; ++i) 
	{
		Category::Type category = (i == Air) ? Category::SceneAirLayer : Category::None;

		SceneNode::Ptr layer(new SceneNode(category)); //Create a new SceneNode
		mSceneLayers[i] = layer.get(); //Add a raw pointer in the array (we don't transfer ownership to the array)

		mSceneGraph.attachChild(std::move(layer)); //Add all layers to the root
	}

	/* Scrolling background */
	sf::Texture& texture = mTextures.get(Textures::Background);
	sf::IntRect textureRect(mWorldBounds);	//Our background texture dimensions (the same as the whole world)

	/* Create the SpriteNode class that links the background texture */
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	mBackground = backgroundSprite.get();
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

	/* Floor */	
	sf::Texture& textureFloor = mTextures.get(Textures::Floor);
	std::unique_ptr<SpriteNode> floorSprite(new SpriteNode(textureFloor));
	floorSprite->setPosition(mWorldBounds.left, mWorldBounds.height - 141);
	mSceneLayers[Air]->attachChild(std::move(floorSprite));


	/* Create Kalee */
	std::unique_ptr<Kalee> kalee(new Kalee(mTextures));
	mKalee = kalee.get(); //Set the world's pointeur mKalee to the newly created SceneNode
	mKalee->setPosition(mSpawnPosition);
	mSceneLayers[Character]->attachChild(std::move(kalee));

	/* Plateforms */
	addInitialPlateforms();

	
}

/*********************************************************************************************************/
/*****************************************PLATEFORMS******************************************************/
/*********************************************************************************************************/

/* Used to keep plateforms in the view */
float getPlateformOffset(Plateform::Type type) {
	switch (type)
	{
	case Plateform::GrandeHerbe:
		return 76.f;

	case Plateform::MoyenneHerbe:
	case Plateform::MoyenneNuage:
		return 57.f;

	case Plateform::PetiteNuage:
		return 38.f;

	default:
		return 76.f;
	}
} 

/* Add plateforms for the spawn view */
void World::addInitialPlateforms()
{
	float yPos = 100.f;
	int side = (randomInt(2)) ? -1 : 1;
	/* Add plateforms to the spawn point container */ 
	while(yPos < mWorldView.getSize().y -73 -40) { //TODO : Code sale
		addPlateform(Plateform::GrandeHerbe, side * ((float)randomInt((int)(mWorldView.getSize().x * 0.5f - getPlateformOffset(Plateform::GrandeHerbe)))), yPos);
		yPos += randomRangeInt(50, 150);
		side *= -1;
	}

	// Sort all plateforms according to their y value, such that lower plateforms are checked first for spawning
	std::sort(mPlateformSpawnPoints.begin(), mPlateformSpawnPoints.end(), [](SpawnPoint<Plateform> lhs, SpawnPoint<Plateform> rhs)
	{
		return lhs.y < rhs.y;
	});
}

void World::generatePlateforms()
{
	float hauteur = -mHauteur;
	if (hauteur >= mNextPlateform) {
		/* Add a new Plateform */
		float side = (randomInt(2)) ? -1.f : 1.f;
		int random = randomInt(10);
		int randomEnemy = randomInt(10);

		sf::Vector2f popPosition(0.f, mNextPlateform + mWorldView.getSize().y);
		
		/* CLOUD */
		if (hauteur > 10000) {
			/* 70 % little */
			if (random < 7) {
				popPosition.x = side * ((float)randomInt((int)(mWorldView.getSize().x * 0.5f - getPlateformOffset(Plateform::PetiteNuage))));
				addPlateform(Plateform::PetiteNuage, popPosition.x, popPosition.y);

				/* 20 % enemy */
				if (randomEnemy < 2) {
					addEnemy(Enemy::EnemyClassic, popPosition.x, popPosition.y + 50);
				}
			}
			/* 30 % medium */
			else {
				popPosition.x = side * ((float)randomInt((int)(mWorldView.getSize().x * 0.5f - getPlateformOffset(Plateform::MoyenneNuage))));
				addPlateform(Plateform::MoyenneNuage, popPosition.x, popPosition.y);

				/* 30 % enemy */
				if (randomEnemy < 3) {
					addEnemy(Enemy::EnemyClassic, popPosition.x, popPosition.y + 50);
				}
			}
		}
		/* GRASS */
		else {
			/* 70% large */
			if (random < 7) {
				popPosition.x = side * ((float)randomInt((int)(mWorldView.getSize().x * 0.5f - getPlateformOffset(Plateform::GrandeHerbe))));
				addPlateform(Plateform::GrandeHerbe, popPosition.x, popPosition.y);

				/* 10 % enemy */
				if (randomEnemy < 1) {
					addEnemy(Enemy::EnemyClassic, popPosition.x, popPosition.y + 50);
				}
			}
			/* 30% medium */
			else {
				popPosition.x = side * ((float)randomInt((int)(mWorldView.getSize().x * 0.5f - getPlateformOffset(Plateform::MoyenneHerbe))));
				addPlateform(Plateform::MoyenneHerbe, popPosition.x, popPosition.y);

				/* 30 % enemy */
				if (randomEnemy < 3) {
					addEnemy(Enemy::EnemyClassic, popPosition.x, popPosition.y + 50);
				}
			}
		}
		mNextPlateform = hauteur + randomRangeInt(100, 150); 

		int randomPickUp = randomInt(10);
		/* 30% Pop a PickUp */
		if (randomPickUp < 3) {
			int randomPU = randomInt(10);
			/* 30% wood */
			if (randomPU < 3) {
				addPickUp(PickUp::Wood, popPosition.x , popPosition.y + 50 );
			}
			/* 70% feather */
			else {
				addPickUp(PickUp::Feather, popPosition.x, popPosition.y + 50);
			}
		}
	}
}

void World::addPlateform(Plateform::Type type, float relX, float relY)
{
	SpawnPoint<Plateform> spawn(type, mSpawnPosition.x + relX, mSpawnPosition.y - relY);
	mPlateformSpawnPoints.push_back(spawn);
}

void World::spawnPlateforms()
{
	// Spawn all plateforms entering the view area (including distance) this frame
	while (!mPlateformSpawnPoints.empty() && mPlateformSpawnPoints.back().y > getBattlefieldBounds().top)
	{
		SpawnPoint<Plateform> spawn = mPlateformSpawnPoints.back();

		std::unique_ptr<Plateform> plateform(new Plateform(spawn.type, mTextures));
		plateform->setPosition(spawn.x, spawn.y);

		mSceneLayers[Air]->attachChild(std::move(plateform));

		// Plateform is spawned, remove from the list to spawn
		mPlateformSpawnPoints.pop_back();
	}
}

/*********************************************************************************************************/
/*********************************************LADDER******************************************************/
/*********************************************************************************************************/


void World::addLadder(Ladder::Type type, float relX, float relY)
{
	SpawnPoint<Ladder> spawn(type, relX, relY);
	mLadderSpawnPoints.push_back(spawn);
}

void World::spawnLadders()
{
	while (!mLadderSpawnPoints.empty() && mLadderSpawnPoints.back().y > getBattlefieldBounds().top)
	{
		SpawnPoint<Ladder> spawn = mLadderSpawnPoints.back();

		std::unique_ptr<Ladder> ladder(new Ladder(spawn.type, mTextures));
		ladder->setPosition(spawn.x, spawn.y);

		mSceneLayers[AirLadder]->attachChild(std::move(ladder));

		mLadderSpawnPoints.pop_back();
	}
}



/*********************************************************************************************************/
/*******************************************ENNEMIES******************************************************/
/*********************************************************************************************************/

/* Used to keep enemies in the view */
float getEnemyOffset(Enemy::Type type) {
	switch (type)
	{
	case Enemy::EnemyClassic:
		return 0.f;

	case Enemy::EnemyFollow:
		return 0.f;

	case Enemy::EnemyPleat:
		return 0.f;

	default:
		return 0.f;
	}
}

void World::generateEnemies(sf::Time dt)
{
	if (mNextEnemy < sf::Time::Zero) {
		float side = (randomInt(2)) ? -1.f : 1.f;

		sf::Vector2f popPosition(side * ((float)randomInt((int)(mWorldView.getSize().x * 0.5f))), mWorldView.getSize().y - mHauteur);
		addEnemy(Enemy::EnemyFollow, popPosition.x, popPosition.y);
		mSounds.play(SoundEffect::EnemyPop);
		
		float randomTime = randomInt((int)mEnemyCoolDown) + 15.f;
		mNextEnemy = sf::seconds(randomTime);
		if (mEnemyCoolDown > 1.f) mEnemyCoolDown -= 0.1f; //Increase the frequence of Enemy with the time
	}
	else {
		mNextEnemy -= dt;
	}
}

void World::addEnemy(Enemy::Type type, float relX, float relY)
{
	SpawnPoint<Enemy> spawn(type, mSpawnPosition.x + relX, mSpawnPosition.y - relY);
	mEnemySpawnPoints.push_back(spawn);
}

void World::spawnEnemies()
{
	// Spawn all plateforms entering the view area (including distance) this frame
	while (!mEnemySpawnPoints.empty() && mEnemySpawnPoints.back().y > getBattlefieldBounds().top)
	{
		SpawnPoint<Enemy> spawn = mEnemySpawnPoints.back();

		std::unique_ptr<Enemy> enemy(new Enemy(spawn.type, mTextures));
		enemy->setPosition(spawn.x, spawn.y);

		mSceneLayers[AirEnemy]->attachChild(std::move(enemy));

		mEnemySpawnPoints.pop_back();
	}
}

/*********************************************************************************************************/
/*********************************************PICKUP******************************************************/
/*********************************************************************************************************/

void World::addPickUp(PickUp::Type type, float relX, float relY)
{
	SpawnPoint<PickUp> spawn(type, mSpawnPosition.x + relX, mSpawnPosition.y - relY);
	mPickUpSpawnPoints.push_back(spawn);
}

void World::spawnPickUp()
{
	while (!mPickUpSpawnPoints.empty() && mPickUpSpawnPoints.back().y > getBattlefieldBounds().top)
	{
		SpawnPoint<PickUp> spawn = mPickUpSpawnPoints.back();

		std::unique_ptr<PickUp> pickUp(new PickUp(spawn.type, mTextures));
		pickUp->setPosition(spawn.x, spawn.y);

		mSceneLayers[AirPickUp]->attachChild(std::move(pickUp));

		mPickUpSpawnPoints.pop_back();
	}
}


/* Each time we take a pickUp */
void World::generateLopion()
{
	if (!mLopionAlive) { 
		int random = randomInt(5);
		if (random) {
			sf::Vector2f popPosition(0.0f  , mWorldView.getSize().y - mHauteur);
			addLopion(popPosition.x, popPosition.y);
			mLopionAlive = true;
		}
	}
}


void World::addLopion(float relX, float relY)
{
	std::unique_ptr<Lopion> lopion(new Lopion(mKalee, this, mTextures));
	mLopion = lopion.get();
	mLopion->setPosition(mSpawnPosition.x + relX, mSpawnPosition.y - relY);
	mSceneLayers[AirEnemy]->attachChild(std::move(lopion));
}

void World::LopionDead()
{
	mLopionAlive = false;
}

void World::checkKaleeActions()
{
	if (mKalee->isCraftingLadder()) {
		sf::Vector2f position = mKalee->getPosition();
		addLadder(Ladder::Normal, position.x, position.y);
		mKalee->setCrafting(false);
		mKalee->setVelocity(0.f, 0.f);
		mSounds.play(SoundEffect::SetLadder);
	}
	if (mKalee->isInvokingPleat()) {
		sf::Vector2f velocity = mKalee->getVelocity();

		velocity.y = -900;
		mKalee->setVelocity(velocity);

		mKalee->stopInvoking();
		mSounds.play(SoundEffect::Aile);
	}
	/* Only useful for sound */
	if (mKalee->isJumping()) {
		mKalee->setJumping(false);
		//mSounds.play(SoundEffect::Jump); A bit annoying
	}
}

void World::destroyEntitiesOutsideView()
{
	Command command;
	command.category = Category::Plateform | Category::Enemy | Category::PickUp | Category::Ladder | Category::Lopion ;
	command.action = derivedAction<Entity>([this](Entity& e, sf::Time)
	{
		if (!getBattlefieldBounds().intersects(e.getBoundingRect())) {
			e.destroy();
		}
			
	});

	mCommandQueue.push(command);
}



sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
	// Return view bounds + some area at top, where plateforms spawn
	sf::FloatRect bounds = getViewBounds();
	bounds.top -= 200.f + 750.f;
	bounds.height += 200.f + 750.f + 750.f;
	bounds.left -= 300.f;
	bounds.width += 600.f;

	return bounds;
}

void World::guideEnemies()
{
	// Setup command that guides all ennemies to Kalee
	Command enemyGuider;
	enemyGuider.category = Category::Enemy;
	enemyGuider.action = derivedAction<Enemy>([this](Enemy& enemy, sf::Time)
	{
		// Ignore unfollowing enemies
		if (!enemy.isFollowing())
			return;
		
		
		enemy.guideTowards(mKalee->getWorldPosition());
	});

	mCommandQueue.push(enemyGuider);
}

void World::setMaxHauteur()
{
	mMaxHauteur = std::min(mMaxHauteur, mHauteur);
}

int World::getScore()
{
	int score = (int)(-mMaxHauteur) / 100;
	return score;
}

int World::getKaleeLadder()
{
	return mKalee->getLadderStock();
}

int World::getKaleeFeather()
{
	return mKalee->getFeatherStock();
}

bool World::hasDeadKalee()
{
	bool isDead = ((mMaxHauteur - mHauteur) < -1250.f) ? true : false;
	isDead = isDead || mKaleeDead;
	return isDead;
}





