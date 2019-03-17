#include "stdafx.h"
#include "Consumables.h"


//const enum ItemTypes { Health, Mana, HealthMana, Instant};
bool Consumables::expandedArray[50];
const char* subCategory = "Consumables";

std::vector<ItemStruct> ItemList;


std::map<int, std::string> HealthItems  
{ 
	{(int) ItemID::HealthPotion, "Health Potion"},
	{(int)ItemID::RefillablePotion, "Refillable Potion"},
	{2056, "Pilfered Health Potion"},
	{2062, "Pilfered Potion of Rouge"}
	

	
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
	{(int)ItemID::SlySackOfGold, "Sly Sack of Gold"},
	{2011, "Elixir Of Skill"},
	{2013, "Looted Oracle's Extract"},
	{3400, "Your Cut"}
};



std::map<int, std::string> RechargebaleItems
{
	{(int)ItemID::HuntersPotion, "Hunter's Potion"},
	{(int)ItemID::RefillablePotion, "Refillable Potion"},
	{(int)ItemID::CorruptingPotion, "Corrupting Potion"}
};



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

	pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick, Consumables::Tick);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, Summoners::Update);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, Consumables::DrawMenu);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::InventoryUpdate, Consumables::InventoryUpdate);
	

	ItemList.emplace_back(ItemStruct((int)ItemID::HealthPotion, "Health Potion", "HealthPotion", subCategory, MenuTypes::MyHealth, SpellTypes::Active));
	ItemList.emplace_back(ItemStruct((int)ItemID::RefillablePotion, "Refillable Potion", "RefillablePotion", subCategory, MenuTypes::MyHealth, SpellTypes::Active));
	ItemList.emplace_back(ItemStruct((int)2056, "Pilfered Health Potion (Kleptomancy)", "PilferedHealthPotion", subCategory, MenuTypes::MyHealth, SpellTypes::Active));
	ItemList.emplace_back(ItemStruct((int)2062, "Pilfered Potion of Rouge (Kleptomancy)", "PilferedPotionofRouge", subCategory, MenuTypes::MyHealth, SpellTypes::Active));

	ItemList.emplace_back(ItemStruct((int)ItemID::ManaPotion, "Mana Potion", "ManaPotion", subCategory, MenuTypes::MyMana, SpellTypes::Active));

	ItemList.emplace_back(ItemStruct((int)ItemID::HuntersPotion, "Hunter's Potion", "HuntersPotion", subCategory, MenuTypes::MyHealth | MenuTypes::MyMana, SpellTypes::Active));
	ItemList.emplace_back(ItemStruct((int)ItemID::CorruptingPotion, "Corrupting Potion", "CorruptingPotion", subCategory, MenuTypes::MyHealth | MenuTypes::MyMana, SpellTypes::Active));
	ItemList.emplace_back(ItemStruct((int)ItemID::TotalBiscuitofRejuvenation, "Total Biscuit Of Rejuvenation", "TotalBiscuitofRejuvenation", subCategory, MenuTypes::MyHealth | MenuTypes::MyMana, SpellTypes::Active));

	ItemList.emplace_back(ItemStruct((int)ItemID::SlySackOfGold, "Sly Sack of Gold  (Kleptomancy)", "SlySackOfGold", subCategory, MenuTypes::Instant, SpellTypes::Active));
	ItemList.emplace_back(ItemStruct((int)2011, "Elixir Of Skill (Kleptomancy)", "PilferedHealthPotion", subCategory, MenuTypes::Instant, SpellTypes::Active));
	ItemList.emplace_back(ItemStruct((int)2013, "Looted Oracle's Extract (Kleptomancy)", "PilferedPotion ofRouge", subCategory, MenuTypes::Instant, SpellTypes::Active));
	ItemList.emplace_back(ItemStruct((int)3400, "Your Cut (Pyke's Ult)", "YourCut", subCategory, MenuTypes::Instant, SpellTypes::Active));

	ItemList.emplace_back(ItemStruct((int)ItemID::ElixirofIron, "Elixir Of Iron", "ElixirofIron", subCategory, MenuTypes::MyHealth, SpellTypes::Active));
	ItemList.emplace_back(ItemStruct((int)ItemID::ElixirofSorcery, "Elixir Of Sorcery", "ElixirofSorcery", subCategory, MenuTypes::Instant, SpellTypes::Active));
	ItemList.emplace_back(ItemStruct((int)ItemID::ElixirofWrath, "Elixir Of Wrath", "ElixirofWrath", subCategory, MenuTypes::MyHealth, SpellTypes::Active));
	
	ItemList.emplace_back(ItemStruct((int)2058, "Travel Size Elixir of Iron (Kleptomancy)", "TravelSizeElixirofIron", subCategory, MenuTypes::MyHealth, SpellTypes::Active));
	ItemList.emplace_back(ItemStruct((int)2059, "Travel Size Elixir of Sorcery (Kleptomancy)", "TravelSizeElixirofSorcery", subCategory, MenuTypes::Instant, SpellTypes::Active));
	ItemList.emplace_back(ItemStruct((int)2060, "Travel Size Elixir of Wrath (Kleptomancy)", "TravelSizeElixirofWrath", subCategory, MenuTypes::MyHealth, SpellTypes::Active));



	//ItemList.emplace_back(ItemStruct((int)ItemID::HealthPotion, "Health Potion", "HealthPotion", subCategory, ItemStruct::MenuTypes::MyHealth));






	/*,
	{(int)ItemID::ElixirofIron, "Elixir Of Iron"},
	{(int)ItemID::ElixirofSorcery, "Elixir Of Sorcery"},
	{(int)ItemID::ElixirofWrath, "Elixir Of Wrath"},
	{(int)ItemID::ElixirofFortitude, "Elixir Of Fortitude"},
	{2058, "Travel Size Elixir of Iron"},
	{2059, "Travel Size Elixir of Sorcery"},
	{2060, "Travel Size Elixir of Wrath"}*/


}


void Consumables::Tick(void * UserData)
{
	if (Player.IsAlive() && !Player.IsRecalling())
	{

		//SdkUiConsoleWrite("ID : %d ", ItemList.size());

		/*
		auto items = Player.GetItems();

		//Player.GetItem((SpellSlot::Item1);


		for (auto &[netID, itemsz] : items)
		{

			//itemsz.Slot 
			SdkUiConsoleWrite("ID : %d Name : %s Slot : %d", netID, itemsz.DisplayName, itemsz.Slot );

		}
		*/

		ItemStruct* currentItems = ItemRetriever::GetAllPlayerItems();

		
		for (int i = 0; i < 7; i++)
		{
			if (currentItems[i].GetItemID() == 0)
			{
				continue;
			}

			for (auto const &value : ItemList)
			{
				if (currentItems[i].GetItemID() == value.GetItemID())
				{
					ItemStruct caster = ItemStruct(currentItems[i].GetItemID(), value.GetSDKItem(), (unsigned char)currentItems[i].GetItemSlot() - 6, value.GetDisplayName(), value.GetMenuID(), subCategory, value.GetMenuTypes(), value.GetSpellTypes());
					caster.CastItem();
					caster.~ItemStruct();
				}
			}
			
		}





		/*
		for (int i = 0; i < 7; i++)
		{
			//(hi[i]).GetItemID()
			//if ((int)ItemID::HealthPotion == (currentItems[i]).GetItemID())
			{
				//menuGenerators = ItemStruct((int)ItemID::HealthPotion, (currentItems[i]).GetSDKItem(), (currentItems[i]).GetItemSlot(), "Health Potion", "HealthPotion", subCategory, ItemStruct::MenuTypes::MyHealth);
			}
		}*/

		/*
		auto playerItems{ Player.GetItems() };
		if (playerItems.empty())
		{
			return;
		}

		for (auto const&[key, val] : playerItems)
		{


			

			if (Menu::Get<bool>("Activator.Consumables.Level1") && Player.GetLevel() == 1)
			{
				if (((key != (int)ItemID::RefillablePotion) && (key != (int)ItemID::HuntersPotion) && (key != (int)ItemID::CorruptingPotion)))
				{
					continue;
				}
			}
			
			UseItems(HealthItems, ItemTypes::Health, key, val);
			UseItems(HealthManaItems, ItemTypes::HealthMana, key, val);
			UseItems(ManaItems, ItemTypes::Mana, key, val);
			UseItems(InstantItems, ItemTypes::Instant, key, val);
			
		
		}*/

		
	}
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

///Your menu settings go here
void Consumables::DrawMenu(void * UserData)
{
	Menu::Tree("Consumables", "Activator.Consumables", false, []()
	{
		//ItemStruct menuGenerators = ItemStruct((int)ItemID::HealthPotion, "Health Potion", "HealthPotion", subCategory, ItemStruct::MenuTypes::MyHealth);

		//ItemList.emplace_back(ItemStruct((int)ItemID::HealthPotion, "Health Potion", "HealthPotion", subCategory, ItemStruct::MenuTypes::MyHealth));
		for (auto& value : ItemList)
		{
			value.MenuGenerator();
		}
		/*
		int arraySize = 0;
		ItemMenuGenerator(HealthItems, ItemTypes::Health, arraySize);
		arraySize += HealthItems.size();

		ItemMenuGenerator(HealthManaItems, ItemTypes::HealthMana, arraySize);
		arraySize += HealthManaItems.size();
		ItemMenuGenerator(ManaItems, ItemTypes::Mana, arraySize);
		arraySize += ManaItems.size();
		ItemMenuGenerator(InstantItems, ItemTypes::Instant, arraySize);
		arraySize += InstantItems.size();

		Menu::Checkbox("Disable At Level1", "Activator.Consumables.Level1", true);
		Menu::BulletText("^-> Rechargable Potion Still Will Be Used");
		Menu::BulletText("^-> Refillable Potion, Hunter's Potion, And Corruptiong Potion");	
		*/
	});

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
