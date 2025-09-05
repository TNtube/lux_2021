#pragma once

#include "lux/kit.hpp"
#include <vector>
#include <set>

std::vector<lux::Cell *> getRessourcesTiles(lux::GameMap& gameMap);

lux::Cell* getClosestRessourceOfType(std::vector<lux::Cell *>& resourceTiles, lux::Unit& unit, lux::ResourceType type);

lux::CityTile* getClosestCityTile(lux::Player& player, lux::Unit& unit);