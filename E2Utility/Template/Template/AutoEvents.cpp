#include "stdafx.h"
#include "AutoEvents.h"
#include <string.h>
Spell::Active LevelerQ = Spell::Active(SpellSlot::Q, 1000.0f, DamageType::Physical);
Spell::Active LevelerW = Spell::Active(SpellSlot::W, 1000.0f, DamageType::Physical);
Spell::Active LevelerE = Spell::Active(SpellSlot::E, 1000.0f, DamageType::Physical);
Spell::Active LevelerR = Spell::Active(SpellSlot::R, 1200.0f, DamageType::Physical);


DWORD lastCheck = 0;
Vector3 shopLocation;
bool Level1Bought = false;
bool AutoBuyDone = false;

void AutoEvents::Init()
{
	lastCheck = 0;
	Level1Bought = false;
	AutoBuyDone = false;
	LevelerQ = Spell::Active(SpellSlot::Q, 1000.0f, DamageType::Physical);
	LevelerW = Spell::Active(SpellSlot::W, 1000.0f, DamageType::Physical);
	LevelerE = Spell::Active(SpellSlot::E, 1000.0f, DamageType::Physical);
	LevelerR = Spell::Active(SpellSlot::R, 1200.0f, DamageType::Physical);


	auto turrets = pSDK->EntityManager->GetAllyTurrets();

	if (turrets.empty())
	{
		return;
	}

	for (auto &[netid, turret] : turrets)
	{
		if (netid && turret)
		{
			if (turret->GetName() == NULL)
			{
				continue;
			}

			if (strstr(turret->GetName(), "Turret_OrderTurretShrine_A") && turret->IsAlly() || strstr(turret->GetName(), "Turret_ChaosTurretShrine") && turret->IsAlly())
			{
				if (turret->GetPosition().IsValid())
				{
					shopLocation = turret->GetPosition();
				}
			}
		}

	}
}

void AutoEvents::MenuLoader()
{
	Menu::Tree("Auto Events", "Others.AutoEvents", false, []()
	{
		Menu::Checkbox("Use Auto Events", "Others.AutoEvents.Use", false);

		Menu::Checkbox("Auto Level R", "Others.AutoEvents.LevelR", true);

		Menu::Checkbox("Auto Buy Yellow Trinket (Warding Totem) at Level 1", "Others.AutoEvents.YellowLevel1", false);
		Menu::SliderInt("^-> Minimum Delay (ms) To Buy", "Others.AutoEvents.YellowDelay", 300, 10, 1000);
		Menu::Checkbox("Auto Buy or Change Trinket", "Others.AutoEvents.BuyTrinket", false);
		Menu::DropList("Auto Buy Trinket Type", "Others.AutoEvents.BuyTrinketType", std::vector<std::string>{ "Red", "Blue" }, 1);
		Menu::SliderInt("^-> Minimum # Level to Buy or Change Trinket", "Others.AutoEvents.MinimumLevel", 9, 1, 18);
		SdkUiText("Note: If you selected Blue Trinket and set below level 9, it will not buy Blue Trinket.");
		Menu::SliderInt("^-> Minimum Delay (ms) To Buy or Change", "Others.AutoEvents.ButTrinketDelay", 300, 10, 1000);

	});
}

void AutoEvents::TickLoader()
{

	if (lastCheck + 500 > GetTickCount() || !Menu::Get<bool>("Others.AutoEvents.Use"))
	{
		return;
	}

	if ( Menu::Get<bool>("Others.AutoEvents.LevelR") && Player.GetLevel() < 18)
	{

	

		AutoLeveler();
	}

	if (Player.Distance(&shopLocation) >= 1500.0f)
	{
		return;
	}
	auto currentTrinket = Player.GetItem((unsigned char)SpellSlot::Trinket - 6);
	//SdkUiConsoleWrite("1 p %d ", currentTrinket.ItemID);

	int itemID = currentTrinket.ItemID;


	if (Menu::Get<bool>("Others.AutoEvents.YellowLevel1") && Player.GetLevel() == 1 && Level1Bought == false)
	{
		Level1Trinket(itemID);
	}

	if (Menu::Get<bool>("Others.AutoEvents.BuyTrinket") && Player.GetLevel() >= Menu::Get<int>("Others.AutoEvents.MinimumLevel") && AutoBuyDone == false)
	{


		//Others.AutoEvents.MinimumLevel

		AutoTrinket(itemID);
	}

	lastCheck = GetTickCount();
}

int AutoEvents::GetRLevel(int currentRLevel)
{

	auto playerChampName = Player.GetCharName();


	if (_stricmp(playerChampName, "Karma") == 0)
	{
		return currentRLevel - 1;
	}
	/*
	if (_stricmp(playerChampName, "Jayce") == 0)
	{
		return currentRLevel - 1;
	}
	*/
	if (_stricmp(playerChampName, "Nidalee") == 0)
	{
		return currentRLevel - 1;
	}
	if (_stricmp(playerChampName, "Elise") == 0)
	{
		return currentRLevel - 1;
	}

	return currentRLevel;
}

void AutoEvents::AutoLeveler()
{
	if (Player.GetLevel() >= 6)
	{
		//SdkUiConsoleWrite("1");
		int levelQ = LevelerQ.Level();
		int levelW = LevelerW.Level();
		int levelE = LevelerE.Level();
		int levelR = LevelerR.Level();

		if (!LevelerQ.IsLearned())
		{
			levelQ -= 1;
		}

		if (!LevelerW.IsLearned())
		{
			levelW -= 1;
		}

		if (!LevelerE.IsLearned())
		{
			levelE -= 1;
		}

		if (!LevelerR.IsLearned())
		{
			levelR -= 1;
		}

		if (GetRLevel(levelR) >= 3 || Player.GetLevel() < 11 && GetRLevel(levelR) == 1 || Player.GetLevel() < 16 && GetRLevel(levelR) == 2)
		{
			return;
		}
		int availablePoints = Player.GetLevel() - (levelQ + levelW + levelE + GetRLevel(levelR));

		//	SdkUiConsoleWrite("1 p %d lq %d lw %d le %d lr%d ", Player.GetLevel(), LevelerQ.Level(), LevelerW.Level(), LevelerE.Level(), GetRLevel());
		if (availablePoints > 0)
		{
			if (GetRLevel(levelR) >= 0)
			{
				bool done = false;
				SdkLevelSpellLocalPlayer((unsigned char)SpellSlot::R, &done);
				
				
			}
		}
	}
}

void AutoEvents::AutoTrinket(int itemID)
{
	int selected = Menu::Get<int>("Others.AutoEvents.BuyTrinketType");


	if (selected == 0 && itemID != 3364 || selected == 1 && itemID != int(ItemID::FarsightAlteration) && Player.GetLevel() >= 9)
	{
		int targetItem = 0;

		if (selected == 0)
		{
			targetItem = 3364;
		}
		else if (selected == 1)
		{
			targetItem = int(ItemID::FarsightAlteration);
		}


		pSDK->EventHandler->DelayedAction([targetItem]()
		{
			bool done = false;
			SdkBuyItemLocalPlayer(targetItem, (int)SpellSlot::Trinket - 6, &done);
			AutoBuyDone = done;

		}, float(Menu::Get<int>("Others.AutoEvents.ButTrinketDelay")));
	}





}

void AutoEvents::Level1Trinket(int itemID)
{
	if (itemID == 0)
	{
		pSDK->EventHandler->DelayedAction([]()
		{
			bool done = false;
			SdkBuyItemLocalPlayer((int)ItemID::WardingTotemTrinket, (int)SpellSlot::Trinket - 6, &done);
			Level1Bought = done;

		}, float(Menu::Get<int>("Others.AutoEvents.YellowDelay")));


	}
}

//Level1Trinket