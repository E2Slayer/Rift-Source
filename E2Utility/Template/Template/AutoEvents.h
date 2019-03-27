#pragma once
#include "Others.h"

class AutoEvents
{
public:
	static void Init();

	static void MenuLoader();
	static void TickLoader();

	static void AutoLeveler();
	static void AutoTrinket(int itemID);
	static void Level1Trinket(int itemID);
	static int GetRLevel(int currentRLevel);
};

