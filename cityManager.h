#pragma once
#include "utils.h"

class CityManager
{
public:
	CityManager(GameContext& gameContext);
	void processCities();
	
private:
	GameContext& m_gameContext;
};