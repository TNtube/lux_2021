#pragma once
#include "utils.h"

struct PositionHash
{
    std::size_t operator()(const lux::Position& pos) const noexcept
    {
        return std::hash<int>()(pos.x) ^ (std::hash<int>()(pos.y) << 1);
    }
};

class UnitManager
{
public:
	UnitManager(GameContext& gameContext);
	void processUnits();
	void collectRessource(lux::Unit& unit, lux::DIRECTIONS& direction);
	void buildNewCity(lux::Unit& unit, lux::DIRECTIONS& direction);
	lux::DIRECTIONS travelToward(lux::Unit& unit, const lux::Position& dest);
	std::vector<lux::Cell*> getAdjacentCells(const lux::Position& pos);
	
private:
	GameContext& m_gameContext;
	std::unordered_map<lux::Position, lux::Unit*, PositionHash> m_reservedDestinations;

	void debugReservedDestinations() const;
};