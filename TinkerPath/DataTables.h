#pragma once

#include "ResourceIdentifier.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

#include <vector>
#include <functional>


class Kalee;

/* Direction of an enemy */
struct Direction
{
	Direction(float angle, float distance) : angle(angle), distance(distance) {}

	float angle;
	float distance;
};

struct PlateformData
{
	Textures::ID					texture;
};

struct EnemyData
{
	Textures::ID					texture;
	std::vector<Direction>			directions;
	float							speed;
};

struct PickupData
{
	std::function<void(Kalee&)>		action;
	Textures::ID					texture;
};


std::vector<PlateformData>			initializePlateformData();
std::vector<EnemyData>				initializeEnemyData();
std::vector<PickupData>				initializePickupData();



