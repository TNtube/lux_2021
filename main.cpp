#include "utils.h"
#include "unitManager.h"
#include "cityManager.h"
#include "lux/define.cpp"
#include <iostream>

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

		GameContext gameContext{player,gameMap,actions,resourceTiles};

		UnitManager(gameContext).processUnits();
		CityManager(gameContext).processCities();

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
