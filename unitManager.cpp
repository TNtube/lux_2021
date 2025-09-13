#include "unitManager.h"
#include <iostream>
#include <unordered_set>

using namespace lux;

UnitManager::UnitManager(GameContext& gameContext):
	m_gameContext(gameContext)
{}

void UnitManager::processUnits()
{
	// Clean up reservations for units that no longer exist
	cleanupReservations();

	Player& player = m_gameContext.player;
	std::vector<string>& actions = m_gameContext.actions;

	for (int i = 0; i < player.units.size(); i++)
	{
		Unit& unit = player.units[i];

		if (unit.isWorker() && unit.canAct())
		{
			DIRECTIONS direction = CENTER;

			if (unit.getCargoSpaceLeft() > 0)
				collectRessource(unit, direction);
			else
			{
				if ((int)player.units.size() >= player.cityTileCount)
					buildNewCity(unit, direction);
				else if (CityTile* closestCityTile = getClosestCityTile(player, unit);
					closestCityTile != nullptr)
					direction = travelToward(unit, closestCityTile->pos);
			}

			if (direction != CENTER)
				actions.push_back(unit.move(direction));
		}
	}
}

void UnitManager::collectRessource(Unit& unit, DIRECTIONS& direction)
{
	Cell* closestResourceTile = getClosestRessourceOfType(m_gameContext.resourceTiles, unit, ResourceType::wood);
	if (closestResourceTile != nullptr)
		direction = unit.pos.directionTo(closestResourceTile->pos);
}

void UnitManager::buildNewCity(Unit& unit, DIRECTIONS& direction)
{
	// Trouver la ville la plus proche
	CityTile* closestCityTile = getClosestCityTile(m_gameContext.player, unit);
	if (closestCityTile != nullptr)
	{
		// Trouver une tuile adjacente vide pour construire une ville
		std::vector<Cell*> adj = getAdjacentCells(closestCityTile->pos);
		for (Cell* cell : adj)
		{
			if (cell->citytile == nullptr && !cell->hasResource())
			{
				// Si le Worker est déjà sur la tuile libre, construire la CityTile
				if (unit.pos == cell->pos)
				{
					m_gameContext.actions.push_back(unit.buildCity());
					m_gameContext.reservedDestinations.erase(cell->pos);
				}
				else if(m_gameContext.reservedDestinations.contains(cell->pos) && m_gameContext.reservedDestinations[cell->pos] != unit.id)
					continue;
				else
				{
					m_gameContext.reservedDestinations[cell->pos] = unit.id;
					direction = travelToward(unit, cell->pos);
				}
				break;
			}
		}
	}
}

DIRECTIONS UnitManager::travelToward(Unit& unit, const Position& dest)
{
	// Clear old reservation if unit is moving to a new destination
	if (m_unitDestinations.find(unit.id) != m_unitDestinations.end()
		&& m_unitDestinations[unit.id] != dest)
	{
		clearUnitReservation(unit.id);
	}

	m_unitDestinations[unit.id] = dest;

	// Try to find a path
	DIRECTIONS bestDirection = findBestDirection(unit, dest);

	if (bestDirection != CENTER)
	{
		Position nextPos = unit.pos.translate(bestDirection, 1);
		m_gameContext.reservedDestinations[nextPos] = unit.id;
	}

	return bestDirection;
}

// Retourne un vecteur de pointeurs vers les cases adjacentes orthogonales
std::vector<Cell*> UnitManager::getAdjacentCells(const Position& pos)
{
	std::vector<Cell*> adj;

	// Raccourci vers la carte
	GameMap& gameMap = m_gameContext.gameMap;

	// Directions N, E, S, O
	int dx[4] = {0, 1, 0, -1};
	int dy[4] = {-1, 0, 1, 0};

	for (int i = 0; i < 4; i++)
	{
		int nx = pos.x + dx[i];
		int ny = pos.y + dy[i];

		// Vérifier qu'on est toujours dans les limites de la map
		if (nx >= 0 && nx < gameMap.width && ny >= 0 && ny < gameMap.height)
			adj.push_back(gameMap.getCell(nx, ny));
	}

	return adj;
}


void UnitManager::cleanupReservations() {
	Player& player = m_gameContext.player;
	std::unordered_set<std::string> validUnitIds;

	// Collect all valid unit IDs
	for (const auto& unit : player.units)
		validUnitIds.insert(unit.id);

	// Remove reservations for invalid units
	for (auto it = m_gameContext.reservedDestinations.begin(); it != m_gameContext.reservedDestinations.end();)
	{
		if (validUnitIds.find(it->second) == validUnitIds.end())
			it = m_gameContext.reservedDestinations.erase(it);
		else
			++it;
	}

	// Clean unit destinations
	for (auto it = m_unitDestinations.begin(); it != m_unitDestinations.end();)
	{
		if (validUnitIds.find(it->first) == validUnitIds.end())
			it = m_unitDestinations.erase(it);
		else
			++it;
	}
}

void UnitManager::clearUnitReservation(const std::string& unitId)
{
	// Find and remove reservation made by this unit
	for (auto it = m_gameContext.reservedDestinations.begin(); it != m_gameContext.reservedDestinations.end(); ++it)
	{
		if (it->second == unitId)
		{
			m_gameContext.reservedDestinations.erase(it);
			break;
		}
	}
}

DIRECTIONS UnitManager::findBestDirection(const lux::Unit &unit, lux::Position dest)
{
	std::vector directions = {
		NORTH, EAST,
		SOUTH, WEST
	};

	DIRECTIONS bestDir = CENTER;
	float bestScore = -1;

	for (DIRECTIONS dir : directions)
	{
		Position nextPos = unit.pos.translate(dir, 1);

		// Check bounds
		if (nextPos.x < 0 || nextPos.x >= m_gameContext.gameMap.width ||
			nextPos.y < 0 || nextPos.y >= m_gameContext.gameMap.height)
			continue;

		// Check if position is reserved by another unit
		if (m_gameContext.reservedDestinations.find(nextPos) != m_gameContext.reservedDestinations.end() &&
			m_gameContext.reservedDestinations[nextPos] != unit.id)
			continue;

		// Calculate score (distance to destination, with some randomness to avoid deadlocks)
		float score = 1.0f / (nextPos.distanceTo(dest) + 0.1f);
		score += (rand() % 100) / 10000.0f; // Small random factor

		if (score > bestScore)
		{
			bestScore = score;
			bestDir = dir;
		}
	}

	return bestDir;
}

// Debug print for reserved destinations
void UnitManager::debugReservedDestinations() const
{
	// std::cerr << "---- Reserved Destinations ----" << std::endl;
	// for (const auto& [pos, unitId] : m_gameContext.reservedDestinations)
	// {
	//     auto unit = m_gameContext.
	//     std::cerr << "Pos(" << pos.x << "," << pos.y << ")";
	//     if (unit)
	//     {
	//         std::cerr << " <- Unit " << unit->id
	//                   << " at (" << unit->pos.x << "," << unit->pos.y << ")";
	//     }
	//     else
	//     {
	//         std::cerr << " <- nullptr";
	//     }
	//     std::cerr << std::endl;
	// }
	// std::cerr << "-------------------------------" << std::endl;
}
