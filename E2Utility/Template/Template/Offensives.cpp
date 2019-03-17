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
	//OffensiveItemList.emplace_back(ItemStruct((int)ItemID::EdgeofNight, "Edge of Night", "EdgeofNight", subCategoryOff, MenuTypes::EnemyNumber, SpellTypes::Active, 1500.0f));
	OffensiveItemList.emplace_back(ItemStruct((int)ItemID::TwinShadows, "Twin Shadows", "TwinShadows", subCategoryOff, MenuTypes::EnemyHealth, SpellTypes::Active, 1000.0f));



	OffensiveItemList.emplace_back(ItemStruct((int)ItemID::BilgewaterCutlass, "Bilgewater Cutlass", "BilgewaterCutlass", subCategoryOff, MenuTypes::EnemyHealth, SpellTypes::Targeted, 700.0f));
	OffensiveItemList.emplace_back(ItemStruct((int)ItemID::BladeoftheRuinedKing, "Blade of the Ruined King", "BladeoftheRuinedKing", subCategoryOff, MenuTypes::EnemyHealth, SpellTypes::Targeted, 700.0f));
	OffensiveItemList.emplace_back(ItemStruct((int)ItemID::HextechGunblade, "Hextech Gunblade", "HextechGunblade", subCategoryOff, MenuTypes::EnemyHealth, SpellTypes::Targeted, 550.0f));

	OffensiveItemList.emplace_back(ItemStruct((int)ItemID::HextechGLP800, "Hextech GLP-800", "HextechGLP800", subCategoryOff, MenuTypes::EnemyHealth, SpellTypes::Targeted, 1100.0f));
	OffensiveItemList.emplace_back(ItemStruct((int)ItemID::HextechProtobelt01, "Hextech Protobelt-01", "HextechProtobelt01", subCategoryOff, MenuTypes::EnemyHealth, SpellTypes::Targeted, 1100.0f));

	OffensiveItemList.emplace_back(ItemStruct((int)ItemID::Tiamat, "Tiamat", "Tiamat", subCategoryOff, MenuTypes::EnemyHealth | MenuTypes::AfterAA, SpellTypes::Active, 350.0f));
	OffensiveItemList.emplace_back(ItemStruct((int)ItemID::RavenousHydra, "Ravenous Hydra", "RavenousHydra", subCategoryOff, MenuTypes::EnemyHealth | MenuTypes::AfterAA, SpellTypes::Active, 350.0f));
	OffensiveItemList.emplace_back(ItemStruct((int)ItemID::TitanicHydra, "Titanic Hydra", "TitanicHydra", subCategoryOff, MenuTypes::EnemyHealth | MenuTypes::AfterAA, SpellTypes::Active, 350.0f));

}


void Offensives::Tick(void * UserData)
{
	if (Player.IsAlive() && !Player.IsRecalling())
	{

		ItemStruct* currentItems = ItemRetriever::GetAllPlayerItems();


		for (int i = 0; i < 7; i++)
		{
			if (currentItems[i].GetItemID() == 0)
			{
				continue;
			}

			for (auto const &value : OffensiveItemList)
			{
				if (currentItems[i].GetItemID() == value.GetItemID())
				{
					ItemStruct caster = ItemStruct(currentItems[i].GetItemID(), value.GetSDKItem(), (unsigned char)currentItems[i].GetItemSlot() - 6, value.GetDisplayName(), value.GetMenuID(), subCategoryOff, value.GetMenuTypes(), value.GetSpellTypes(), value.GetSpellRange());
					caster.CastItem();
					caster.~ItemStruct();
				}
			}

		}

	}

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
	});
}

void Offensives::TickLoader(ItemStruct currentItem)
{
	if (currentItem.GetItemID() == 0)
	{
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



///Your menu settings go here
void Offensives::DrawMenu(void * UserData)
{
	/*
	Menu::Tree("Offensives", "Activator.Offensives", false, []()
	{
		for (auto& value : OffensiveItemList)
		{
			value.MenuGenerator();
		}
	});
	*/
}

