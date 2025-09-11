#include "cityManager.h"

using namespace std;
using namespace lux;

CityManager::CityManager(GameContext& gameContext):
	m_gameContext(gameContext)
{}

void CityManager::processCities()
{
	Player& player = m_gameContext.player;
	vector<string>& actions = m_gameContext.actions;

	for (auto& kv : player.cities)
		for (auto& cityTile : kv.second.citytiles)
			if (cityTile.canAct() && (int)player.units.size() < player.cityTileCount)
				actions.push_back(cityTile.buildWorker());
}
