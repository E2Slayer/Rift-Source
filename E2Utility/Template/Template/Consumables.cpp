#include "stdafx.h"
#include "Consumables.h"


//const enum ItemTypes { Health, Mana, HealthMana, Instant};

const char* subCategory = "Consumables";

std::vector<ItemStruct> ItemList;

Vector3 fountainLocation;


/*
	{ItemID::RefillablePotion, "Refillable Potion"},
	{ItemID::HuntersPotion, "Hunters Potion"},
*/


/*
2004 - Mana Potion
2056 - Pilfered Stealth Ward
2061 - Pilfered Health Potion
2062 - pilfered potion of rouge
2319 - Sly Sack of Gold
2422 - Slightly Magical Boots

2057 - Peering Farsight Ward
2058 - Travel Size Elixir of Iron
2059 - Travel Size Elixir of Sorcery
2060 - Travel Size Elixir of Wrath

2010 - Total Biscuit of Everlasting Will
2011 - Elixir Of Skill
2013 - Looted Oracle's Extract

3400 - Yourcut (Pyke)

*/

//Consumables
void Consumables::Init()
{
	//std::fill_n(expandedArray, 50, false);

	ItemList.clear();
	fountainLocation = Vector3(0.0f, 0.0f, 0.0f);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick, Consumables::Tick);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, Summoners::Update);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, Consumables::DrawMenu);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::InventoryUpdate, Consumables::InventoryUpdate);
	

	ItemList.emplace_back(ItemStruct((int)ItemID::HealthPotion, "Health Potion", "HealthPotion", subCategory, MenuTypes::MyHealth, SpellTypes::Active, 700.0f));
	ItemList.emplace_back(ItemStruct((int)ItemID::RefillablePotion, "Refillable Potion", "RefillablePotion", subCategory, MenuTypes::MyHealth, SpellTypes::Active, 700.0f));
	ItemList.emplace_back(ItemStruct((int)2056, "Pilfered Health Potion (Kleptomancy)", "PilferedHealthPotion", subCategory, MenuTypes::MyHealth, SpellTypes::Active, 700.0f));
	ItemList.emplace_back(ItemStruct((int)2062, "Pilfered Potion of Rouge (Kleptomancy)", "PilferedPotionofRouge", subCategory, MenuTypes::MyHealth, SpellTypes::Active, 700.0f));

	ItemList.emplace_back(ItemStruct((int)ItemID::ManaPotion, "Mana Potion", "ManaPotion", subCategory, MenuTypes::MyMana, SpellTypes::Active, 700.0f));

	ItemList.emplace_back(ItemStruct((int)ItemID::HuntersPotion, "Hunter's Potion", "HuntersPotion", subCategory, MenuTypes::MyHealth | MenuTypes::MyMana, SpellTypes::Active, 700.0f));
	ItemList.emplace_back(ItemStruct((int)ItemID::CorruptingPotion, "Corrupting Potion", "CorruptingPotion", subCategory, MenuTypes::MyHealth | MenuTypes::MyMana, SpellTypes::Active, 700.0f));
	ItemList.emplace_back(ItemStruct((int)ItemID::TotalBiscuitofRejuvenation, "Total Biscuit Of Rejuvenation", "TotalBiscuitofRejuvenation", subCategory, MenuTypes::MyHealth | MenuTypes::MyMana, SpellTypes::Active, 700.0f));

	ItemList.emplace_back(ItemStruct((int)ItemID::SlySackOfGold, "Sly Sack of Gold  (Kleptomancy)", "SlySackOfGold", subCategory, MenuTypes::Instant, SpellTypes::Active, 700.0f));
	ItemList.emplace_back(ItemStruct((int)2011, "Elixir Of Skill (Kleptomancy)", "ElixirOfSkill", subCategory, MenuTypes::Instant, SpellTypes::Active, 700.0f));
	ItemList.emplace_back(ItemStruct((int)2013, "Looted Oracle's Extract (Kleptomancy)", "PilferedPotion ofRouge", subCategory, MenuTypes::Instant, SpellTypes::Active, 700.0f));
	ItemList.emplace_back(ItemStruct((int)3400, "Your Cut (Pyke's Ult)", "YourCut", subCategory, MenuTypes::Instant, SpellTypes::Active, 700.0f));

	ItemList.emplace_back(ItemStruct((int)ItemID::ElixirofIron, "Elixir Of Iron", "ElixirofIron", subCategory, MenuTypes::MyHealth, SpellTypes::Active, 700.0f));
	ItemList.emplace_back(ItemStruct((int)ItemID::ElixirofSorcery, "Elixir Of Sorcery", "ElixirofSorcery", subCategory, MenuTypes::Instant, SpellTypes::Active, 700.0f));
	ItemList.emplace_back(ItemStruct((int)ItemID::ElixirofWrath, "Elixir Of Wrath", "ElixirofWrath", subCategory, MenuTypes::MyHealth, SpellTypes::Active, 700.0f));
	
	ItemList.emplace_back(ItemStruct((int)2058, "Travel Size Elixir of Iron (Kleptomancy)", "TravelSizeElixirofIron", subCategory, MenuTypes::MyHealth, SpellTypes::Active, 700.0f));
	ItemList.emplace_back(ItemStruct((int)2059, "Travel Size Elixir of Sorcery (Kleptomancy)", "TravelSizeElixirofSorcery", subCategory, MenuTypes::Instant, SpellTypes::Active, 700.0f));
	ItemList.emplace_back(ItemStruct((int)2060, "Travel Size Elixir of Wrath (Kleptomancy)", "TravelSizeElixirofWrath", subCategory, MenuTypes::MyHealth, SpellTypes::Active, 700.0f));



	//ItemList.emplace_back(ItemStruct((int)ItemID::HealthPotion, "Health Potion", "HealthPotion", subCategory, ItemStruct::MenuTypes::MyHealth));
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

			if (strstr(turret->GetName(), "Turret_OrderTurretShrine_A"))
			{


				if (turret->GetPosition().IsValid() )
				{
					fountainLocation = turret->GetPosition();
				}
			}
		}

	}



	/*,
	{(int)ItemID::ElixirofIron, "Elixir Of Iron"},
	{(int)ItemID::ElixirofSorcery, "Elixir Of Sorcery"},
	{(int)ItemID::ElixirofWrath, "Elixir Of Wrath"},
	{(int)ItemID::ElixirofFortitude, "Elixir Of Fortitude"},
	{2058, "Travel Size Elixir of Iron"},
	{2059, "Travel Size Elixir of Sorcery"},
	{2060, "Travel Size Elixir of Wrath"}*/


}




void Consumables::InventoryUpdate(int Event, int Slot, int Extra, void* UserData)
{
	//SdkUiConsoleWrite("Event : %d Slot : %d Extra : %d", Event, Slot, Extra);

}


/*
SummonerBoost - Cleanse
SummonerExhaust - Exhaust
SummonerHaste - Ghost
SummonerHeal - Heal
SummonerSmite - Smite
SummonerTeleport - TP
SummonerBarrier - Barrier
SummonerFlash - Flash
*/

bool bHealthItemsExpanded;
bool bHarassExpanded;
bool bLaneClearExpanded;



void Consumables::MenuLoader()
{
	Menu::Tree("Consumables", "Activator.Consumables", false, []()
	{

		for (auto& value : ItemList)
		{
			value.MenuGenerator();
		}
		Menu::DropList("Consumables Style", "Activator.Consumables.Style", std::vector<std::string>{ "Always", "Combo" }, 0);
		Menu::Checkbox("Disable Consumables Usage at Level 1", "Activator.Consumables.Level1", false);
		SdkUiText("^-> Rechargeable Potions Will Still Be Used With the function above is enabled");
	});
}

void Consumables::TickLoader(ItemStruct currentItem)
{
	if (Player.Distance(&fountainLocation) <= 1500.0f)
	{
		return; 
	}


	if (Player.GetLevel() == 1 && Menu::Get<bool>("Activator.Consumables.Level1"))
	{
		if (currentItem.GetItemID() != (int)ItemID::RefillablePotion && currentItem.GetItemID() != (int)ItemID::HuntersPotion && currentItem.GetItemID() != (int)ItemID::CorruptingPotion)
		{
			return;
		}
	}

	if (Menu::Get<int>("Activator.Consumables.Style") == 0 || (Menu::Get<int>("Activator.Consumables.Style") == 1 && Menu::Get<Hotkey>("Activator.Config.ComboKey").Active))
	{
		if (currentItem.GetItemID() == 0 || (LastTimeTickCount + (DWORD)Menu::Get<int>("Activator.Config.HumanizerDelay") >= GetTickCount()))
		{
			//return;
		}

		for (auto const &value : ItemList)
		{
			if (currentItem.GetItemID() == value.GetItemID())
			{
				ItemStruct caster = ItemStruct(currentItem.GetItemID(), value.GetSDKItem(), (unsigned char)currentItem.GetItemSlot() - 6, value.GetDisplayName(), value.GetMenuID(), subCategory, value.GetMenuTypes(), value.GetSpellTypes(), value.GetSpellRange());
				caster.CastItem();
				caster.~ItemStruct();
			}
		}
	}
}


/*
RegenerationPotion - Health Potion
ItemCrystalFlask - Refill
ItemCrystalFlaskJungle - Hunter
ItemDarkCrystalFlask - Corrupting

*/

/*
void Consumables::UseItems(std::map<int, std::string> inputMap, ItemTypes itemType, int targetID, SDK_ITEM targetItem)
{
	if (inputMap.empty())
	{
		return;
	}

	//SdkUiConsoleWrite("USE123333333");



	for (auto const&[key, val] : inputMap)
	{
		if (targetID != key || key == NULL )
		{
			continue;
		}

		//SdkUiConsoleWrite("USE125");


		const char* buffNameOfItem = GetItemBuffName(key);


		//SdkUiConsoleWrite("USE12");
		if (!Player.HasItem(key) || Player.IsRecalling() || Player.HasBuff((buffNameOfItem), false))
		{
			continue;
		}

		//Player.GetItemSlot(targetID);
	//	SdkUiConsoleWrite("target %d", Player.GetItemSlot(targetID));
	//	SdkUiConsoleWrite("targgg %d", targetItem.Slot+5);

		if (Player.GetItemSlot(targetID + 6) == (unsigned char)SpellSlot::Unknown)
		{
			continue;
		}


		
		//SdkUiConsoleWrite("USE14");
		Spell::Active item = Spell::Active(Player.GetItemSlot(targetID) + 6);

		
		if (!item.IsValid() || !item.IsReady())
		{
			continue;
		}

		//SdkUiConsoleWrite("USE142");

		std::string menuID = "Activator.Consumables.";
		std::string tempName = val;
		tempName.erase(remove_if(tempName.begin(), tempName.end(), std::isspace), tempName.end());
		menuID += tempName;

		std::string menuIDSecond = menuID;

		menuID += "Use";

		//SdkUiConsoleWrite("USE14 %s", menuID);

		if (!Menu::Get<bool>(menuID))
		{
			continue;
		}

		
		if (item.LastCast != NULL && item.LastCast + 0.1f >= Game::Time()) //0.1f second
		{
			continue;
		}

		if (itemType == ItemTypes::Instant)
		{
			//SdkUiConsoleWrite("USE125 ");
			item.Cast();
			continue;
		}

		//Player.GetItems();

		//SdkUiConsoleWrite("Passed");

		if (itemType == ItemTypes::Health || itemType == ItemTypes::HealthMana)
		{

			menuIDSecond += "MinHealth";

			float healthPct = (float) Menu::Get<int>(menuIDSecond);
			if (healthPct == 0)
			{
				//SdkUiConsoleWrite("01");
				continue;
			}

			if (Player.GetHealthPercent() <= healthPct )
			{
				//SdkUiConsoleWrite("USE1");
				item.Cast();
				continue;
				
			}


		}


		if (itemType == ItemTypes::Mana || itemType == ItemTypes::HealthMana)
		{

			menuIDSecond += "Mana";

			float manaPct = (float)Menu::Get<int>(menuIDSecond);
			if (manaPct == 0)
			{
				//SdkUiConsoleWrite("02");
				continue;
			}

			if (Player.GetManaPercent() <= manaPct)
			{
				//SdkUiConsoleWrite("USE2");
				item.Cast();
				continue;
			}

			//Menu::SliderInt(displayName.c_str(), menuIDSecond, 25, 1, 100);

		}

	}


}

void Consumables::ItemMenuGenerator(std::map<int, std::string> inputMap, ItemTypes itemType, int arrayStart)
{

	if (inputMap.empty() )
	{
		return;
	}

	int arrayStartTemp = arrayStart;

	for (auto const&[key, val] : inputMap)
	{

		std::string menuID = "Activator.Consumables.";
		std::string tempName = val;
		tempName.erase(remove_if(tempName.begin(), tempName.end(), std::isspace), tempName.end());


		menuID += tempName;

		//const char* menuDispaly = std::to_wstring(val);

		Menu::Tree(val.c_str(), menuID, false, [key, val, itemType, menuID]() mutable
		{
			std::string menuIDSecond = menuID;

			menuID += "Use";
			std::string displayName = "Use ";
			displayName += val;

			Menu::Checkbox(displayName.c_str(), menuID, true);


			if (itemType == ItemTypes::Health || itemType == ItemTypes::HealthMana)
			{
				displayName = val;
				displayName += " Minimum Health %";

				menuIDSecond += "MinHealth";

				Menu::SliderInt(displayName.c_str(), menuIDSecond, 40, 1, 100);



			}

			if (itemType == ItemTypes::Mana || itemType == ItemTypes::HealthMana)
			{
				std::string displayNameSecond = val;

				displayNameSecond += " Minimum Mana %";

				menuIDSecond += "Mana";
				Menu::SliderInt(displayName.c_str(), menuIDSecond, 25, 1, 100);
			}

		});







	}

}*/

/*

std::map<int, std::string> HealthItems
{
	{(int) ItemID::HealthPotion, "Health Potion"},
	{2056, "Pilfered Health Potion"},
	{2062, "Pilfered Potion of Rouge"}
	{(int)ItemID::RefillablePotion, "Refillable Potion"},

};

std::map<int, std::string> HealthManaItems
{
	{(int)ItemID::HuntersPotion, "Hunter's Potion"},
	{(int)ItemID::CorruptingPotion, "Corrupting Potion"},
	{(int)ItemID::TotalBiscuitofRejuvenation, "Total Biscuit Of Rejuvenation"},
};


std::map<int, std::string> ManaItems
{
	{(int)ItemID::ManaPotion, "Mana Potion"}
};


std::map<int, std::string> InstantItems
{
	{(int)ItemID::SlySackOfGold, "Sly Sack of Gold (Kleptomancy)"},
	{2011, "Elixir Of Skill"},
	{2013, "Looted Oracle's Extract"},
	{3400, "Your Cut (Pyke's Ult)"}
};


RegenerationPotion - Health Potion
ItemCrystalFlask - Refill
ItemCrystalFlaskJungle - Hunter
ItemDarkCrystalFlask - Corrupting
FlaskOfCrystalWater - Mana potion
LootedRegenerationPotion -
LootedPotionOfGiantStrength
Item2010 - bisuit
TravelSizeElixirOfIron
TravelSizeElixirOfSorcery
TravelSizeElixirOfWrath
LootedOracleExtractSight

*/
