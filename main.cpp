#include "utils.h"
#include "lux/define.cpp"

using namespace std;
using namespace lux;
int main()
{
	kit::Agent gameState = kit::Agent();
	// initialize
	gameState.initialize();

	while (true)
	{
		/** Do not edit! **/
		// wait for updates
		gameState.update();

		vector<string> actions = vector<string>();
		
		/** AI Code Goes Below! **/

		Player &player = gameState.players[gameState.id];
		Player &opponent = gameState.players[(gameState.id + 1) % 2];

		GameMap &gameMap = gameState.map;

		vector<Cell *> resourceTiles = getRessourcesTiles(gameMap);

		// we iterate over all our units and do something with them
		for (int i = 0; i < player.units.size(); i++)
		{
			Unit unit = player.units[i];
			if (unit.isWorker() && unit.canAct())
			{
				DIRECTIONS direction = DIRECTIONS::CENTER;
				if (unit.getCargoSpaceLeft() > 0)
				{
					Cell *closestResourceTile = getClosestRessourceOfType(resourceTiles, unit, ResourceType::wood);
					if (closestResourceTile != nullptr)
						direction = unit.pos.directionTo(closestResourceTile->pos);
				}
				else if (CityTile* closestCityTile = getClosestCityTile(player, unit);
					closestCityTile != nullptr)
					direction = unit.pos.directionTo(closestCityTile->pos);

				actions.push_back(unit.move(direction));
			}
		}

		/** AI Code Goes Above! **/

		/** Do not edit! **/
		for (int i = 0; i < actions.size(); i++)
		{
			if (i != 0)
				cout << ",";
			cout << actions[i];
		}
		cout << endl;
		// end turn
		gameState.end_turn();
	}

	return 0;
}
