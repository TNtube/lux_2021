#include "unitManager.h"
#include <iostream>

using namespace std;
using namespace lux;

UnitManager::UnitManager(GameContext& gameContext):
	m_gameContext(gameContext)
{}

void UnitManager::processUnits()
{
    Player& player = m_gameContext.player;
    vector<string>& actions = m_gameContext.actions;

    for (int i = 0; i < player.units.size(); i++)
    {
        Unit& unit = player.units[i];

        // Ne traiter que les Workers qui peuvent agir
        if (unit.isWorker() && unit.canAct())
        {
            DIRECTIONS direction = DIRECTIONS::CENTER;

            if (unit.getCargoSpaceLeft() > 0)
                collectRessource(unit, direction);
            else
            {
                // Vérifier si on a autant d'unités que de CityTiles
                if ((int)player.units.size() >= player.cityTileCount)
                    buildNewCity(unit,direction);
                else if (CityTile* closestCityTile = getClosestCityTile(player, unit);
                    closestCityTile != nullptr)
                    direction = unit.pos.directionTo(closestCityTile->pos);
            }

            // Envoyer le mouvement
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
        vector<Cell*> adj = getAdjacentCells(closestCityTile->pos);
        for (Cell* cell : adj)
        {
            if (cell->citytile == nullptr && !cell->hasResource())
            {
                // Si le Worker est déjà sur la tuile libre, construire la CityTile
                if (unit.pos == cell->pos)
                {
                    m_gameContext.actions.push_back(unit.buildCity());
                    m_reservedDestinations.erase(cell->pos);
                }
                else if(m_reservedDestinations.contains(cell->pos) && m_reservedDestinations[cell->pos] != &unit)
                    continue;
                else
                {
                    m_reservedDestinations[cell->pos] = &unit;
                    direction = travelToward(unit, cell->pos);
                }
                break;
            }
        }
    }
}

DIRECTIONS UnitManager::travelToward(Unit& unit, const Position& dest)
{
    if(m_reservedDestinations.contains(unit.pos))
        m_reservedDestinations.erase(unit.pos);

    debugReservedDestinations();

    Position nextPos = unit.pos.translate(unit.pos.directionTo(dest), 1);
    if(m_reservedDestinations.contains(nextPos) && m_reservedDestinations[nextPos] != &unit)
        return DIRECTIONS::CENTER;

    cerr << "OOOOOOOOOOOOOOOOOOOO" << endl;

    debugReservedDestinations();

    m_reservedDestinations[nextPos] = &unit;

    return unit.pos.directionTo(dest);
}

// Retourne un vecteur de pointeurs vers les cases adjacentes orthogonales
vector<Cell*> UnitManager::getAdjacentCells(const Position& pos)
{
    vector<Cell*> adj;

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


#include <iostream>

// Debug print for reserved destinations
void UnitManager::debugReservedDestinations() const
{
    std::cerr << "---- Reserved Destinations ----" << std::endl;
    for (const auto& [pos, unit] : m_reservedDestinations)
    {
        std::cerr << "Pos(" << pos.x << "," << pos.y << ")";
        if (unit)
        {
            std::cerr << " <- Unit " << unit->id
                      << " at (" << unit->pos.x << "," << unit->pos.y << ")";
        }
        else
        {
            std::cerr << " <- nullptr";
        }
        std::cerr << std::endl;
    }
    std::cerr << "-------------------------------" << std::endl;
}
