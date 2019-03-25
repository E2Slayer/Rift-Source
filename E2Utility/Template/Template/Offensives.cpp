#include "stdafx.h"
#include "Offensives.h"

std::vector<ItemStruct> OffensiveItemList;
const char* subCategoryOff = "Offensives";

//const enum ItemTypes { Active, Timats, Target, HextTechs };

/*
std::map<int, std::string> ActiveItems
{
	{(int)ItemID::YoumuusGhostblade, "Youmuus Ghostblade"},
	{(int)ItemID::EdgeofNight, "Edge of Night"},
	{(int)ItemID::TwinShadows, "Twin Shadows"}
};

std::map<int, std::string> TimatsItems
{
	{(int)ItemID::Tiamat, "Tiamat"}, //Activator.Offensives.UseTiamatAfterAA
	{(int)ItemID::RavenousHydra, "Ravenous Hydra"},//Activator.Offensives.UseRavenousHydraAfterAA
	{(int)ItemID::TitanicHydra, "Titanic Hydra"}//Activator.Offensives.UseTitanicHydraAfterAA
};


std::map<int, std::string> TargetItems
{
	{(int)ItemID::BilgewaterCutlass, "Bilgewater Cutlass"},
	{(int)ItemID::BladeoftheRuinedKing, "Blade of the Ruined King"},
	{(int)ItemID::HextechGunblade, "Hextech Gunblade"}
};

std::map<int, std::string> HextTechsItems
{
	{(int)ItemID::HextechGLP800, "Hextech GLP 800"},
	{(int)ItemID::HextechProtobelt01, "Hextech Protobelt 01"}
};*/

void Offensives::Init()
{

	OffensiveItemList.clear();
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick, Offensives::Tick);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, Summoners::Update);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, Offensives::DrawMenu);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::PostAttack, Offensives::PostAttack);


	OffensiveItemList.emplace_back(ItemStruct((int)ItemID::YoumuusGhostblade, "Youmuus Ghostblade", "YoumuusGhostblade", subCategoryOff, MenuTypes::EnemyHealth, SpellTypes::Active, 700.0f));
	OffensiveItemList.emplace_back(ItemStruct((int)ItemID::Spellbinder, "Spell Binder", "Spellbinder", subCategoryOff, MenuTypes::EnemyHealth, SpellTypes::Active, 700.0f));
	//OffensiveItemList.emplace_back(ItemStruct((int)ItemID::EdgeofNight, "Edge of Night", "EdgeofNight", subCategoryOff, MenuTypes::EnemyNumber, SpellTypes::Active, 1500.0f));
	OffensiveItemList.emplace_back(ItemStruct((int)ItemID::TwinShadows, "Twin Shadows", "TwinShadows", subCategoryOff, MenuTypes::EnemyHealth, SpellTypes::Active, 1000.0f));



	OffensiveItemList.emplace_back(ItemStruct((int)ItemID::BilgewaterCutlass, "Bilgewater Cutlass", "BilgewaterCutlass", subCategoryOff, MenuTypes::EnemyHealth, SpellTypes::Targeted, 700.0f));
	OffensiveItemList.emplace_back(ItemStruct((int)ItemID::BladeoftheRuinedKing, "Blade of the Ruined King", "BladeoftheRuinedKing", subCategoryOff, MenuTypes::EnemyHealth, SpellTypes::Targeted, 700.0f));
	OffensiveItemList.emplace_back(ItemStruct((int)ItemID::HextechGunblade, "Hextech Gunblade", "HextechGunblade", subCategoryOff, MenuTypes::EnemyHealth, SpellTypes::Targeted, 550.0f));

	OffensiveItemList.emplace_back(ItemStruct((int)ItemID::HextechGLP800, "Hextech GLP-800", "HextechGLP800", subCategoryOff, MenuTypes::EnemyHealth, SpellTypes::Targeted, 1100.0f));
	OffensiveItemList.emplace_back(ItemStruct((int)ItemID::HextechProtobelt01, "Hextech Protobelt-01", "HextechProtobelt01", subCategoryOff, MenuTypes::EnemyHealth, SpellTypes::Targeted, 1100.0f));

	OffensiveItemList.emplace_back(ItemStruct((int)ItemID::Tiamat, "Tiamat", "Tiamat", subCategoryOff, MenuTypes::EnemyHealth , SpellTypes::Active, 350.0f));
	OffensiveItemList.emplace_back(ItemStruct((int)ItemID::RavenousHydra, "Ravenous Hydra", "RavenousHydra", subCategoryOff, MenuTypes::EnemyHealth , SpellTypes::Active, 350.0f));
	OffensiveItemList.emplace_back(ItemStruct((int)ItemID::TitanicHydra, "Titanic Hydra", "TitanicHydra", subCategoryOff, MenuTypes::EnemyHealth , SpellTypes::Active, 350.0f));

}




void Offensives::PostAttack(AttackableUnit* Target)
{
	return;
	//test++;



	//SdkUiConsoleWrite("Ins33");
	//Game::PrintChat("atack");
	if (Target == NULL || Target == nullptr)
	{
		return;
	}

	if (!Target->IsValidTarget() || !Target->IsHero() || Target->IsAlly() ||  !Target->IsTargetable())
	{
		//return;
	}





	if (Player.IsAlive() && !Player.IsRecalling())
	{

		

	}




	//Activator.Offensives.UseTiamatAfterAA
	//Activator.Offensives.UseRavenousHydraAfterAA
	//Activator.Offensives.UseTitanicHydraAfterAA

}

void Offensives::MenuLoader()
{
	Menu::Tree("Offensives", "Activator.Offensives", false, []()
	{
		for (auto& value : OffensiveItemList)
		{
			value.MenuGenerator();
		}
		Menu::DropList("Offensives Style", "Activator.Offensives.Style", std::vector<std::string>{ "Always", "Combo" }, 0);

	});
}

void Offensives::TickLoader(ItemStruct currentItem)
{
	if (Menu::Get<int>("Activator.Offensives.Style") == 0 || (Menu::Get<int>("Activator.Offensives.Style") == 1 && Menu::Get<Hotkey>("Activator.Config.ComboKey").Active))
	{
		if (currentItem.GetItemID() == 0 )
		{
			//SdkUiConsoleWrite("waht?");
			return;
		}

		if ((LastTimeTickCount + Menu::Get<int>("Activator.Config.HumanizerDelay") >= GetTickCount()))
		{
			//SdkUiConsoleWrite("waht?22");
			return;
		}

		for (auto const &value : OffensiveItemList)
		{
			if (currentItem.GetItemID() == value.GetItemID())
			{
				ItemStruct caster = ItemStruct(currentItem.GetItemID(), value.GetSDKItem(), (unsigned char)currentItem.GetItemSlot() - 6, value.GetDisplayName(), value.GetMenuID(), subCategoryOff, value.GetMenuTypes(), value.GetSpellTypes(), value.GetSpellRange());
				caster.CastItem();
				caster.~ItemStruct();
			}
		}
	}

}



