#pragma once

#include "lux/kit.hpp"
#include <vector>
#include <set>

struct GameContext
{
	lux::Player& player;
	lux::GameMap& gameMap;
	std::vector<std::string>& actions;
	std::vector<lux::Cell*>& resourceTiles;
};

std::vector<lux::Cell *> getRessourcesTiles(lux::GameMap& gameMap);

lux::Cell* getClosestRessourceOfType(std::vector<lux::Cell *>& resourceTiles, lux::Unit& unit, lux::ResourceType type);

lux::CityTile* getClosestCityTile(lux::Player& player, lux::Unit& unit);