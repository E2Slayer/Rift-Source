#include "stdafx.h"
#include "Defensives.h"

std::vector<ItemStruct> DefensivesItemList;
const char* subCategoryDef = "Defensives";


//const enum ItemTypes { AffectEnemy, AffectAlly, AllyTarget, SpeedUp, LowMyHealth};



std::map<int, std::string> AffectEnemyItems
{
	{(int)ItemID::RanduinsOmen, "Randuins Omen"}
};

std::map<int, std::string> AffectAllyItems
{
	{(int)ItemID::LocketoftheIronSolari, "Locket of the Iron Solari"},
	{(int)ItemID::Redemption, "Redemption"},
	{(int)ItemID::Ohmwrecker, "Ohmwrecker"}
};

std::map<int, std::string> AllyTargetItems
{
	{(int)ItemID::KnightsVow, "Knights Vow"}, 
	{(int)ItemID::ZekeConvergence, "Zeke Convergence"}
};


std::map<int, std::string> SpeedUpItems
{
	{(int)ItemID::ShurelyasReverie, "Shurelyas Reverie"},
	{(int)ItemID::RighteousGlory, "Righteous Glory"}
};

std::map<int, std::string> LowMyHealthItems
{
	{(int)ItemID::GargoyleStoneplate, "GargoyleStoneplate"},
	{(int)ItemID::SeraphsEmbrace, "Seraphs Embrace"},
	{(int)ItemID::Stopwatch, "Stopwatch"},
	{(int)ItemID::ZhonyasHourglass, "Zhonyas Hourglass"}
};



void Defensives::Init()
{

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


void Defensives::Tick(void * UserData)
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

			for (auto const &value : DefensivesItemList)
			{
				if (currentItems[i].GetItemID() == value.GetItemID())
				{
					ItemStruct caster = ItemStruct(currentItems[i].GetItemID(), value.GetSDKItem(), (unsigned char)currentItems[i].GetItemSlot() - 6, value.GetDisplayName(), value.GetMenuID(), subCategoryDef, value.GetMenuTypes(), value.GetSpellTypes(), value.GetSpellRange());
					caster.CastItem();
					caster.~ItemStruct();
				}
			}

		}

	}

}

/*
void Defensives::UseItems(std::map<int, std::string> inputMap, ItemTypes itemType, int targetID, unsigned char targetslot)
{
	if (inputMap.empty())
	{
		return;
	}

	//SdkUiConsoleWrite("USE123333333");



	for (auto const&[key, val] : inputMap)
	{
		if (targetID != key )
		{
			continue;
		}

		//SdkUiConsoleWrite("USE125");




		//SdkUiConsoleWrite("USE12");
		if (!Player.HasItem(key) || (Player.IsRecalling() && key != (int) ItemID::Redemption)) //since redemption can be used while dead
		{
			continue;
		}

		//Player.GetItemSlot(targetID);
	//	SdkUiConsoleWrite("target %d", Player.GetItemSlot(targetID));
	//	SdkUiConsoleWrite("targgg %d", targetItem.Slot+5);
		unsigned char spellSlot = Player.GetItemSlot(targetID) + 6;

		if (spellSlot == (unsigned char)SpellSlot::Unknown)
		{
			continue;
		}

		//SdkUiConsoleWrite("spellSlot %d", spellSlot);

		//SdkUiConsoleWrite("USE14");
		//Spell::Active item = Spell::Active(spellSlot);

	

			Spell::Targeted item = Spell::Targeted(spellSlot, 700.0f, DamageType::Magical);

			 if (itemType == ItemTypes::AffectAlly)
			{
				if (targetID == (int)ItemID::LocketoftheIronSolari)
				{
					item = Spell::Targeted(spellSlot, 600.0f, DamageType::Magical);
					//item = Spell::Skillshot(spellSlot, 1200.0f, SkillshotType::Cone, 0.25f, 1600.0f, 80.0f, DamageType::Magical, true, CollisionFlags::BraumWall | CollisionFlags::Minions | CollisionFlags::YasuoWall );
				}
				else if (targetID == (int)ItemID::Ohmwrecker)
				{
					item = Spell::Targeted(spellSlot, 1100.0f, DamageType::Magical); //range 1000 is not sure
				}
				else if (targetID == (int)ItemID::Redemption)
				{
					item = Spell::Targeted(spellSlot, 5500.0f, DamageType::Magical); //range 1000 is not sure
				}

			}
			else if (itemType == ItemTypes::AllyTarget)
			{
				if (targetID == (int)ItemID::KnightsVow)
				{
					item = Spell::Targeted(spellSlot, 1000.0f, DamageType::Magical);
				}
				else if (targetID == (int)ItemID::ZekeConvergence)
				{
					item = Spell::Targeted(spellSlot, 1000.0f, DamageType::Magical); //range 1000 is not sure
				}
			}


			//SdkUiConsoleWrite("012");

			//spellSlot = Player.GetItemSlot(targetID) + 6;
			spellSlot = targetslot + (unsigned char)6;
		// SdkUiConsoleWrite("1Slot %d", spellSlot);
			if (!item.IsValid())
			{
				continue;
			}



			//SdkUiConsoleWrite("USE142");

			std::string menuID = "Activator.Defensives.";
			std::string tempName = val;
			tempName.erase(remove_if(tempName.begin(), tempName.end(), std::isspace), tempName.end());
			menuID += tempName;

			std::string menuIDSecond = menuID;

			menuID += "Use";

			//SdkUiConsoleWrite("USE14 %s", menuID);

			//SdkUiConsoleWrite("01234");



			if (!Menu::Get<bool>(menuID))
			{
				//SdkUiConsoleWrite("rere %s", menuID);
				continue;
			}



			//SdkUiConsoleWrite("2Slot %d", spellSlot);
			if (targetID == (int)ItemID::RanduinsOmen)
			{
				item = Spell::Targeted(spellSlot, 500.0f, DamageType::Magical); //Randunin
				if (!item.IsReady())
				{
					SdkUiConsoleWrite("12cant't cast %s %d", menuID, spellSlot);
					continue;
				}
				menuIDSecond += "MinEnemy";

				int targetEnemyNumber = Menu::Get<int>(menuIDSecond);

				if (targetEnemyNumber == 0)
				{
					continue;
				}

				if (Player.CountEnemiesInRange(item.Range) <= targetEnemyNumber)
				{
					SdkUiConsoleWrite("cast123 %s", menuID);
					item.Cast(&Player.GetServerPosition());
				}

			}
			else if (targetID == (int)ItemID::ShurelyasReverie)
			{
				item = Spell::Targeted(spellSlot, 700.0f, DamageType::Magical);
				if (!item.IsReady())
				{
					//SdkUiConsoleWrite("1cant't cast %s %d", menuID, spellSlot);
					continue;
				}
				const auto target = pCore->TS->GetTarget(1500.0f);

				if (target == NULL || target == nullptr)
				{
					continue;
				}

				if ((Player.IsRunningFrom(target) && Player.GetHealthPercent() <= 40.0f) || (target->IsRunningFrom(&Player) && target->GetHealthPercent() <= 40.0f))
				{

					menuIDSecond += "MinAlly";

					int targetEnemyNumber = Menu::Get<int>(menuIDSecond);

					if (targetEnemyNumber == 0)
					{
						continue;
					}

					if (Player.CountAlliesInRange(item.Range) -1 >= targetEnemyNumber) //-1 because it counts localplayer too
					{
						SdkUiConsoleWrite("cast123 %s", menuID);
						item.Cast(&Player.GetServerPosition());
					}
				}
			}
			else if (targetID == (int)ItemID::RighteousGlory)
			{
				item = Spell::Targeted(spellSlot, 1000.0f, DamageType::Magical); //range 1000 is not sure
				if (!item.IsReady())
				{
					//SdkUiConsoleWrite("1cant't cast %s %d", menuID, spellSlot);
					continue;
				}
				const auto target = pCore->TS->GetTarget(1500.0f);

				if (target == NULL || target == nullptr)
				{
					continue;
				}

				if ((Player.IsRunningFrom(target) && Player.GetHealthPercent() <= 40.0f) || (target->IsRunningFrom(&Player) && target->GetHealthPercent() <= 40.0f))
				{
					SdkUiConsoleWrite("cast123 %s", menuID);
					item.Cast(&Player.GetServerPosition());
				}
			}
			else if (itemType == ItemTypes::AllyTarget)
			{
				menuIDSecond += "AllyTarget";


				item = Spell::Targeted(spellSlot, 1000.0f, DamageType::Magical);
				if (!item.IsReady())
				{
					//SdkUiConsoleWrite("1cant't cast %s %d", menuID, spellSlot);
					continue;
				}

				int list = Menu::Get<int>(menuIDSecond); // "Most AD", "Most AP", "Anyone"


				if (list < 0 || list > 2)
				{
					continue;
				}

	

				
			}
			else if (itemType == ItemTypes::LowMyHealth)
			{
				item = Spell::Targeted(spellSlot, 700.0f, DamageType::Magical);

				//Spell::Active test = Spell::Active((unsigned char) (targetslot + 6));

			//Spell::Targeted test = Spell::Targeted((unsigned char)(targetslot + 6));
				//SdkUiConsoleWrite("3Slot %d", spellSlot);
				const auto target = pCore->TS->GetTarget(700.0f);

				if (target == NULL || target == nullptr || Player.CountAlliesInRange(item.Range) <= 0 || !Player.IsAlive())
				{
					continue;
				}

				if (!item.IsReady())
				{
					SdkUiConsoleWrite("1cant't cast %s Slot: %d", menuID, spellSlot);
					//SdkUiConsoleWrite("4Slot %d", spellSlot);
					continue;
				}


				
				menuIDSecond += "Me";
				float myHealth = (float) Menu::Get<int>(menuIDSecond);

				if (myHealth <= Player.GetHealthPercent() && !Player.IsChannelingImportantSpell() ) //&& !target->IsRunningFrom(&Player)
				{
					SdkUiConsoleWrite("cast123 %s", menuID);
					item.Cast(&Player.GetServerPosition());
				}
				

				//Menu::SliderInt(displayName.c_str(), menuIDSecond, 35, 1, 100);
			}

			







		
	}
}

*/


///Your menu settings go here
void Defensives::DrawMenu(void * UserData)
{

	Menu::Tree("Defensives", "Activator.Defensives", false, []()
	{
		
		for (auto& value : DefensivesItemList)
		{
			value.MenuGenerator();
		}

	});
}

void Defensives::MenuLoader()
{
	Menu::Tree("Defensives", "Activator.Defensives", false, []()
	{

		for (auto& value : DefensivesItemList)
		{
			value.MenuGenerator();
		}

	});
}

void Defensives::TickLoader(ItemStruct currentItem)
{
	if (currentItem.GetItemID() == 0)
	{
		return;
	}

	for (auto const &value : DefensivesItemList)
	{
		if (currentItem.GetItemID() == value.GetItemID())
		{
			ItemStruct caster = ItemStruct(currentItem.GetItemID(), value.GetSDKItem(), (unsigned char)currentItem.GetItemSlot() - 6, value.GetDisplayName(), value.GetMenuID(), subCategoryDef, value.GetMenuTypes(), value.GetSpellTypes(), value.GetSpellRange());
			caster.CastItem();
			caster.~ItemStruct();
		}
	}

}


