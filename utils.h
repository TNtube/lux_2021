#pragma once

#include "lux/kit.hpp"
#include <vector>
#include <set>

struct PositionHash
{
	std::size_t operator()(const lux::Position& pos) const noexcept
	{
		return std::hash<int>()(pos.x) ^ (std::hash<int>()(pos.y) << 1);
	}
};

struct GameContext
{
	lux::Player& player;
	lux::GameMap& gameMap;
	std::vector<std::string>& actions;
	std::vector<lux::Cell*>& resourceTiles;
	std::unordered_map<lux::Position, std::string, PositionHash> reservedDestinations;
};

std::vector<lux::Cell *> getRessourcesTiles(lux::GameMap& gameMap);

lux::Cell* getClosestRessourceOfType(std::vector<lux::Cell *>& resourceTiles, lux::Unit& unit, lux::ResourceType type);

lux::CityTile* getClosestCityTile(lux::Player& player, lux::Unit& unit);