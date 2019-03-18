#include "stdafx.h"
#include "Defensives.h"

std::vector<ItemStruct> DefensivesItemList;
const char* subCategoryDef = "Defensives";

DWORD LastTimeTickCountDef = 0;

//const enum ItemTypes { AffectEnemy, AffectAlly, AllyTarget, SpeedUp, LowMyHealth};



void Defensives::Init()
{
	LastTimeTickCountDef = 0;
	DefensivesItemList.clear();
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick, Defensives::Tick);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, Summoners::Update);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, Defensives::DrawMenu);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::PostAttack, Offensives::PostAttack);

	DefensivesItemList.emplace_back(ItemStruct((int)ItemID::RanduinsOmen, "Randuins Omen", "RanduinsOmen", subCategoryDef, MenuTypes::EnemyNumber, SpellTypes::Active, 500.0f));
	DefensivesItemList.emplace_back(ItemStruct((int)ItemID::LocketoftheIronSolari, "Locket of the Iron Solari", "LocketoftheIronSolari", subCategoryDef, MenuTypes::MyHealth | MenuTypes::AllyHealth, SpellTypes::Active, 600.0f));
	DefensivesItemList.emplace_back(ItemStruct((int)ItemID::Redemption, "Redemption", "Redemption", subCategoryDef, MenuTypes::AllyHealth, SpellTypes::Targeted, 5500.0f));
	DefensivesItemList.emplace_back(ItemStruct((int)ItemID::ShurelyasReverie, "Shurelyas Reverie", "ShurelyasReverie", subCategoryDef, MenuTypes::AllyNumber | MenuTypes::EnemyHealth, SpellTypes::Active, 700.0f));
	DefensivesItemList.emplace_back(ItemStruct((int)ItemID::RighteousGlory, "Righteous Glory", "RighteousGlory", subCategoryDef, MenuTypes::EnemyHealth, SpellTypes::Active, 1000.0f));
	DefensivesItemList.emplace_back(ItemStruct((int)ItemID::GargoyleStoneplate, "Gargoyle Stoneplate", "GargoyleStoneplate", subCategoryDef, MenuTypes::MyHealth, SpellTypes::Active, 700.0f));
	DefensivesItemList.emplace_back(ItemStruct((int)ItemID::SeraphsEmbrace, "Seraphs Embrace", "SeraphsEmbrace", subCategoryDef, MenuTypes::MyHealth, SpellTypes::Active, 700.0f));
	DefensivesItemList.emplace_back(ItemStruct((int)ItemID::Stopwatch, "Stopwatch", "Stopwatch", subCategoryDef, MenuTypes::MyHealth, SpellTypes::Active, 700.0f));
	DefensivesItemList.emplace_back(ItemStruct((int)ItemID::ZhonyasHourglass, "Zhonyas Hourglass", "ZhonyasHourglass", subCategoryDef, MenuTypes::MyHealth, SpellTypes::Active, 700.0f));
	DefensivesItemList.emplace_back(ItemStruct((int)ItemID::Ohmwrecker, "Ohmwrecker", "Ohmwrecker", subCategoryDef, MenuTypes::AllyHealth | MenuTypes::AllyNumber, SpellTypes::Active, 1100.0f));
}



void Defensives::MenuLoader()
{
	Menu::Tree("Defensives", "Activator.Defensives", false, []()
	{

		for (auto& value : DefensivesItemList)
		{
			value.MenuGenerator();
		}

		Menu::DropList("Defensives Style", "Activator.Defensives.Style", std::vector<std::string>{ "Always", "Combo" }, 0);


	});
}

void Defensives::TickLoader(ItemStruct currentItem)
{
	if (Menu::Get<int>("Activator.Defensives.Style") == 0 || (Menu::Get<int>("Activator.Defensives.Style") == 1 && Menu::Get<Hotkey>("Activator.Config.ComboKey").Active))
	{
		if (currentItem.GetItemID() == 0 || (LastTimeTickCountDef + (DWORD) Menu::Get<int>("Activator.Config.HumanizerDelay") >= GetTickCount()))
		{
			
			return;
		}

		for (auto const &value : DefensivesItemList)
		{
			if (currentItem.GetItemID() == value.GetItemID())
			{
				ItemStruct caster = ItemStruct(currentItem.GetItemID(), value.GetSDKItem(), (unsigned char)currentItem.GetItemSlot() - 6, value.GetDisplayName(), value.GetMenuID(), subCategoryDef, value.GetMenuTypes(), value.GetSpellTypes(), value.GetSpellRange());
				caster.CastItem();
				LastTimeTickCountDef = GetTickCount();
				caster.~ItemStruct();
			}
		}
	}

}


