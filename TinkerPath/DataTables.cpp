#include "DataTables.h"
#include "Plateform.h"
#include "Enemy.h"
#include "PickUp.h"


std::vector<PlateformData> initializePlateformData()
{
	std::vector<PlateformData> data(Plateform::TypeCount);

	data[Plateform::GrandeHerbe].texture = Textures::GrandeHerbe;

	data[Plateform::MoyenneHerbe].texture = Textures::MoyenneHerbe;

	data[Plateform::MoyenneNuage].texture = Textures::MoyenneNuage;

	data[Plateform::PetiteNuage].texture = Textures::PetiteNuage;

	return data;
}

std::vector<EnemyData> initializeEnemyData()
{
	std::vector<EnemyData> data(Enemy::TypeCount);

	data[Enemy::EnemyClassic].texture = Textures::AnimationEnemyClassic;
	data[Enemy::EnemyClassic].directions.push_back(Direction(90.f, 175.f));
	data[Enemy::EnemyClassic].directions.push_back(Direction(-90.f, 350.f));
	data[Enemy::EnemyClassic].directions.push_back(Direction(90.f, 350.f));
	data[Enemy::EnemyClassic].speed = 100.f;


	data[Enemy::EnemyFollow].texture = Textures::AnimationEnemyFollow;
	data[Enemy::EnemyFollow].speed = 120.f;

	data[Enemy::EnemyPleat].texture = Textures::AnimationEnemyPleat;

	return data;
}

std::vector<PickupData> initializePickupData()
{
	std::vector<PickupData> data(PickUp::TypeCount);

	data[PickUp::Wood].texture = Textures::Wood;

	data[PickUp::Feather].texture = Textures::Feather;

	return data;
}
