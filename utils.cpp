#include "utils.h"

using namespace std;
using namespace lux;

vector<Cell *> getRessourcesTiles(GameMap& gameMap)
{
	vector<Cell *> resourceTiles = vector<Cell *>();
	for (int y = 0; y < gameMap.height; y++)
		for (int x = 0; x < gameMap.width; x++)
			if (Cell *cell = gameMap.getCell(x, y); cell->hasResource())
				resourceTiles.push_back(cell);
	return resourceTiles;
}

Cell* getClosestRessourceOfType(vector<Cell*>& resourceTiles, Unit& unit, ResourceType type)
{
	Cell *closestResourceTile;
	float closestDist = 9999999;
	for (Cell* cell: resourceTiles)
	{
		if (cell->resource.type != type) continue;

		float dist = cell->pos.distanceTo(unit.pos);
		if (dist >= closestDist) continue;

		closestDist = dist;
		closestResourceTile = cell;
	}
	return closestResourceTile;
}

CityTile* getClosestCityTile(Player& player, Unit& unit)
{
	if(player.cities.size() == 0)
		return nullptr;

	float closestDist = 999999;
	CityTile *closestCityTile;
	for(auto& [_, city]: player.cities)
	{
		for(CityTile& citytile: city.citytiles)
		{
			float dist = citytile.pos.distanceTo(unit.pos);
			if (dist >= closestDist) continue;

			closestCityTile = &citytile;
			closestDist = dist;
		}
	}
	return closestCityTile;
}