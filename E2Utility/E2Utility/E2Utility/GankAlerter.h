#pragma once
#include "Drawings.h"


struct ChampionObject
{
	AIHeroClient* Hero;
	bool Enabled;
	float Distance;
	float LastTrigger;
	bool isAlly;
	bool isJungler;
};

class GankAlerter
{
public:
	static void Init();

	static void MenuLoader();
	static void TickLoader();
	static void __cdecl Draw(_In_ void* UserData);
	static void GankChecker(ChampionObject* value, bool isDrawing);
};

