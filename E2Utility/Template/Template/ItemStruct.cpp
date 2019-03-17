#include "stdafx.h"
#include "ItemStruct.h"
#include "Activator.h"



unsigned int itemID;
_SDK_ITEM sdkItem;
SpellSlot itemSlot;
const char* displayName;
const char* menuID;
const char* menuSubCategory;
MenuTypes enumState;
SpellTypes spellType;
float spellRange;


ItemStruct::ItemStruct()
{
	this->itemID = 0;
	//this->sdkItem = NULL;
	this->itemSlot = SpellSlot::Unknown;
	this->displayName = "None";
	this->menuID = "None";
	this->menuSubCategory = "None";
	this->enumState = MenuTypes::None;
	this->spellType = SpellTypes::SkillBase;
	this->spellRange = 0.0f;
}


ItemStruct::~ItemStruct()
{
}


ItemStruct::ItemStruct(int _itemID, _SDK_ITEM _sdkItem, unsigned char _itemSlot) //for Player Items
{
	if (_itemID != 0)
	{
		this->itemID = _itemID;
	}

	this->sdkItem = _sdkItem;
	this->itemSlot = ItemSlotAdjust(_itemSlot);

}



ItemStruct::ItemStruct(int _itemID, _SDK_ITEM _sdkItem, unsigned char _itemSlot, const char* _displayName, const char* _menuID, const char* _menuSubCategory, MenuTypes _enumState, SpellTypes _spellType, float _spellRange)
{

	//this->itemSlot = ItemSlotAdjust(_itemSlot);

	
	if (_itemID != 0)
	{
		this->itemID = _itemID;
	}

	this->sdkItem = _sdkItem;
	this->itemSlot = ItemSlotAdjust(_itemSlot);
	this->menuID = _menuID;
	this->menuSubCategory = _menuSubCategory;
	this->enumState = _enumState;
	this->spellType = _spellType;
	this->spellRange = _spellRange;
	
}


ItemStruct::ItemStruct(int _itemID, const char* _displayName, const char* _menuID, const char* _menuSubCategory, MenuTypes _enumState, SpellTypes _spellType, float _spellRange) //for menu generator
{
	if (_itemID != 0)
	{
		this->itemID = _itemID;
	}

	this->displayName = _displayName;
	this->menuID = _menuID;
	this->menuSubCategory = _menuSubCategory;
	this->enumState = _enumState;
	this->spellType = _spellType;
	this->spellRange = _spellRange;
}

void ItemStruct::MenuGenerator()
{

	if (this->menuSubCategory == NULL || this->menuID == NULL)
	{
		return;
	}
	std::string menuIDInside0 = "Activator.";
	menuIDInside0 += this->menuSubCategory;
	menuIDInside0 += ".";
	menuIDInside0 += this->menuID;
	//menuIDInside0 += ".";

	Menu::Tree(this->displayName, menuIDInside0, false, [this, menuIDInside0]()
	{
		std::string menuIDInside = menuIDInside0;
		menuIDInside += "Use";
		std::string displayMenu = this->displayName;
		displayMenu += " Use";

		Menu::Checkbox(displayMenu.c_str(), menuIDInside, true);
		//SpellSlot::Trinket;

		if (this->enumState & MenuTypes::MyHealth)
		{
			std::string menuIDInside = menuIDInside0;
			menuIDInside += "MyHealth";
			std::string displayMenu = this->displayName;
			displayMenu += " My Minimum Health # %";

			Menu::SliderInt(displayMenu.c_str(), menuIDInside, 40, 1, 100);
		}
		

		if (this->enumState & MenuTypes::MyMana)
		{
			std::string menuIDInside = menuIDInside0;
			menuIDInside += "MyMana";
			std::string displayMenu = this->displayName;
			displayMenu += " My Minimum Mana # %";

			Menu::SliderInt(displayMenu.c_str(), menuIDInside, 25, 1, 100);
			//Menu::SliderInt(DisplayString(this->displayName, "My Minimum Mana # %"), MenuString("MyMana"), 25, 1, 100);
		}

		if (this->enumState & MenuTypes::AllyHealth)
		{
			std::string menuIDInside = menuIDInside0;
			menuIDInside += "AllyHealth";
			std::string displayMenu = this->displayName;
			displayMenu += " Ally Minimum Health # %";

			Menu::SliderInt(displayMenu.c_str(), menuIDInside, 35, 1, 100);
			//Menu::SliderInt(DisplayString(this->displayName, "Ally Minimum Health # %"), MenuString("AllyHealth"), 35, 1, 100);
		}

		if (this->enumState & MenuTypes::AllyNumber)
		{
			std::string menuIDInside = menuIDInside0;
			menuIDInside += "AllyNumber";
			std::string displayMenu = this->displayName;
			displayMenu += " Ally Minimum Number #";

			Menu::SliderInt(displayMenu.c_str(), menuIDInside, 2, 1, 5);
			//Menu::SliderInt(DisplayString(this->displayName, "Ally Minimum Number #"), MenuString("AllyNumber"), 2, 1, 5);
		}

		if (this->enumState & MenuTypes::EnemyHealth)
		{
			std::string menuIDInside = menuIDInside0;
			menuIDInside += "EnemyHealth";
			std::string displayMenu = this->displayName;
			displayMenu += " Enemy Minimum Health # %";

			Menu::SliderInt(displayMenu.c_str(), menuIDInside, 40, 1, 100);
			//Menu::SliderInt(DisplayString(this->displayName, "Enemy Minimum Health # %"), MenuString("EnemyHealth"), 35, 1, 100);
		}

		if (this->enumState & MenuTypes::EnemyNumber)
		{
			std::string menuIDInside = menuIDInside0;
			menuIDInside += "EnemyNumber";
			std::string displayMenu = this->displayName;
			displayMenu += " Enemy Minimum Number #";

			Menu::SliderInt(displayMenu.c_str(), menuIDInside, 2, 1, 5);
			//Menu::SliderInt(DisplayString(this->displayName, "Enemy Minimum Number #"), MenuString("EnemyNumber"), 2, 1, 5);
		}


		if (this->enumState & MenuTypes::AfterAA)
		{
			std::string menuIDInside = menuIDInside0;
			menuIDInside += "AfterAA";
			std::string displayMenu = this->displayName;
			displayMenu += " Use After Auto Attack Only";

			Menu::Checkbox(displayMenu.c_str(), menuIDInside, true);
			//Menu::Checkbox(DisplayString(this->displayName, "Use After Auto Attack Only"), MenuString("AfterAA"), true);
		}



	});
}

void ItemStruct::CastItem()
{
	if (this->menuSubCategory == NULL || this->menuID == NULL)
	{
		return;
	}

	std::string menuIDInside0 = "Activator.";
	menuIDInside0 += this->menuSubCategory;
	menuIDInside0 += ".";
	menuIDInside0 += this->menuID;
	//menuIDInside0 += ".";
	std::string menuIDInside = menuIDInside0;
	menuIDInside0 += "Use";

	if (!Menu::Get<bool>(menuIDInside0))
	{
		return;
	}


	if (this->itemID == 0 || this->itemSlot == SpellSlot::Unknown)
	{
		return;
	}

	const char* buffNameOfItem = GetItemBuffName(this->itemID);

	if (!Player.HasItem(this->itemID) || Player.IsRecalling() || Player.HasBuff((buffNameOfItem), true))
	{
		return;
	}

	
	Spell::Active item = Spell::Active(this->itemSlot); //temporary

	//SdkUiConsoleWrite("are we here");
	if (!item.IsValid() || !item.IsReady())
	{
		return;
	}


	if (this->enumState & MenuTypes::Instant)
	{
		Spell::Active item2 = Spell::Active(this->itemSlot); //temporary
		item2.Cast();
		SdkUiConsoleWrite("Instant Cast");
		// cast right away
	}

	//SpellSlot::Trinket;

	if (this->enumState & MenuTypes::MyHealth)
	{
		std::string menuIDInside0 = menuIDInside;
		menuIDInside0 += "MyHealth";
		float myHealthPct = (float)Menu::Get<int>(menuIDInside0);
		//Menu::SliderInt(name.c_str(), oss.str(), 40, 1, 100);

		if (myHealthPct == 0)
		{
			return;
		}

		if (Player.GetHealthPercent() <= myHealthPct)
		{
			//SdkUiConsoleWrite("Cast1");
			SpellCaster(this->spellType, this->spellRange);
		}

	}


	if (this->enumState & MenuTypes::MyMana)
	{
		std::string menuIDInside0 = menuIDInside;
		menuIDInside0 += "MyMana";
		float myManaPct = (float)Menu::Get<int>(menuIDInside0);
		//Menu::SliderInt(name.c_str(), oss.str(), 25, 1, 100);


		if (myManaPct == 0)
		{
			return;
		}

		if (Player.GetManaPercent() <= myManaPct)
		{
			//SdkUiConsoleWrite("Cast12");
			SpellCaster(this->spellType, this->spellRange);
		}
	}

	if (this->enumState & MenuTypes::AllyHealth)
	{
		std::string menuIDInside0 = menuIDInside;
		menuIDInside0 += "AllyHealth";
		float allyHealthPct = (float)Menu::Get<int>(menuIDInside0);
		//Menu::SliderInt(name.c_str(), oss.str(), 35, 1, 100);
		if (Player.CountEnemiesInRange(1500.0f) >= 1 || this->itemID == (int)ItemID::Redemption)
		{
			if ((Player.CountAlliesInRange(this->spellRange) - 1) >= 1) // -1 because it counts player itself too
			{

				auto allies{ pSDK->EntityManager->GetAllyHeroes(this->spellRange, &pSDK->EntityManager->GetLocalPlayer().GetPosition()) }; // Get enemies

				if (allies.empty())
				{
					return;
				}

				for (auto &[netID, Ally] : allies) // enemy loop
				{
					if (Ally != nullptr && Ally != NULL)
					{
						if (Ally->GetNetworkID() == NULL )
						{
							return;
						}

						if (Ally->IsAlive() && !Ally->IsZombie() && Ally->GetNetworkID() != Player.GetNetworkID())
						{
							if (Ally->GetHealthPercent() <= allyHealthPct)
							{
								if (this->itemID == (int)ItemID::Redemption)
								{
									if (Ally->CountEnemiesInRange(1500.0f) >= 1)
									{
										SpellAllyCaster(this->spellType, this->spellRange, Ally);
									}
								}
								else
								{
									SpellCaster(this->spellType, this->spellRange);
								}

								return;
							}
						}
					}
				}
			}
		}

	}

	if (this->enumState & MenuTypes::AllyNumber)
	{
		std::string menuIDInside0 = menuIDInside;
		menuIDInside0 += "AllyNumber";
		int allyNumbers = Menu::Get<int>(menuIDInside0);
		//Menu::SliderInt(name.c_str(), oss.str(), 2, 1, 5);
		if (Player.CountEnemiesInRange(1500.0f) >= 1)
		{
			if ((Player.CountAlliesInRange(this->spellRange) - 1) >= allyNumbers) // -1 because it counts player itself too
			{
				SpellCaster(this->spellType, this->spellRange);
			}
		}
	}

	if (this->enumState & MenuTypes::EnemyHealth)
	{
		std::string menuIDInside0 = menuIDInside;
		menuIDInside0 += "EnemyHealth";
		float enemyHealthPct = (float)Menu::Get<int>(menuIDInside0);

		if (Player.CountEnemiesInRange(1500.0f) >= 1)
		{
			//Menu::SliderInt(name.c_str(), oss.str(), 35, 1, 100);
			auto target = pCore->TS->GetTarget(this->spellRange);
			if (target != NULL && target != nullptr)
			{
				if (target->IsValidTarget() && target->GetServerPosition().IsValid())
				{
					if (target->GetHealthPercent() <= enemyHealthPct)
					{
						//SdkUiConsoleWrite("Cast1234");
						SpellCaster(this->spellType, this->spellRange);
					}
				}
			}
		}
	}

	if (this->enumState & MenuTypes::EnemyNumber)
	{
		std::string menuIDInside0 = menuIDInside;
		menuIDInside0 += "EnemyNumber";
		int enemyNumbers = Menu::Get<int>(menuIDInside0);
		//Menu::SliderInt(name.c_str(), oss.str(), 2, 1, 5);

		if (Player.CountEnemiesInRange(this->spellRange) >= enemyNumbers)
		{
			SpellCaster(this->spellType, this->spellRange);
		}
	}


	if (this->enumState & MenuTypes::AfterAA)
	{
		std::string menuIDInside0 = menuIDInside;
		menuIDInside0 += "AfterAA";
		bool aaChecker = Menu::Get<bool>(menuIDInside0);
		//Menu::Checkbox(name.c_str(), oss.str(), true);
	}

	if (this->enumState & MenuTypes::Custom)
	{

	}


}

/*
const char* ItemStruct::MenuString(const char* targetName)
{

	std::string menuID = "Activator.";
	menuID += this->menuSubCategory;
	menuID += ".";
	menuID += this->menuID;
	menuID += ".";
	menuID += targetName;

	return menuID.c_str();

}

const char* ItemStruct::DisplayString(const char* displayName, const char* targetName)
{

	std::string menuID2 = displayName;
	menuID2 += " ";
	menuID2 += targetName;

	return menuID2.c_str();
}
*/

SpellSlot ItemStruct::ItemSlotAdjust(unsigned char slot)
{
	switch (slot)
	{
		case NULL: //strangly slot 0 returns as NUL
			return SpellSlot::Item1;
		case 1:
			return SpellSlot::Item2;
		case 2:
			return SpellSlot::Item3;
		case 3:
			return SpellSlot::Item4;
		case 4:
			return SpellSlot::Item5;
		case 5:
			return SpellSlot::Item6;
		case 6:
			return SpellSlot::Trinket;
		default:
			return SpellSlot::Unknown;
	}
	return SpellSlot::Unknown;
}

const char* ItemStruct::GetItemBuffName(int itemID)
{
	switch (itemID)
	{
	case (int)ItemID::HealthPotion: //Health Potion
		return "RegenerationPotion";

	case (int)ItemID::RefillablePotion: //Health Potion
		return "ItemCrystalFlask";

	case 2056: //Pilfered Health Potion
		return "LootedRegenerationPotion";

	case 2062: //Pilfered Potion of Rouge
		return "LootedPotionOfGiantStrength";

	case (int)ItemID::HuntersPotion:
		return "ItemCrystalFlaskJungle";

	case (int)ItemID::CorruptingPotion:
		return "ItemDarkCrystalFlask";

	case (int)ItemID::TotalBiscuitofRejuvenation:
		return "Item2010";

	case (int)ItemID::ManaPotion:
		return "FlaskOfCrystalWater";

	case 2013: //Looted Oracle's Extract
		return "LootedOracleExtractSight";
	default:
		return "None";

	}
	return "None";
}


void ItemStruct::SpellCaster(SpellTypes type, float range)
{
	if (type == SpellTypes::Active)
	{
		Spell::Active item = Spell::Active(this->itemSlot); //temporary
		item.Cast();

	}
	else if (type == SpellTypes::Targeted)
	{
		Spell::Targeted item = Spell::Targeted(this->itemSlot, range , DamageType::Magical); //temporary

			auto target = pCore->TS->GetTarget(range);
			if (target != NULL && target != nullptr)
			{
				if (target->IsValidTarget() && target->GetServerPosition().IsValid())
				{
					item.Cast(target);
					item.Cast(&target->GetServerPosition());
				}
			}
	
	}
	else if (type == SpellTypes::SkillShot)
	{

	}
	else if (type == SpellTypes::Ranged)
	{

	}
}

void ItemStruct::SpellAllyCaster(SpellTypes type, float range, AIHeroClient* allyTarget)
{
	//SdkUiConsoleWrite("Did you come here %d", allyTarget->CountEnemiesInRange(1500.0f));
	if (allyTarget->GetNetworkID() == Player.GetNetworkID() || !allyTarget->IsHero() || allyTarget->IsEnemy())
	{
		return;
	}
	//SdkUiConsoleWrite("Did you come here2");
	if (type == SpellTypes::Active)
	{
		Spell::Active item = Spell::Active(this->itemSlot); //temporary
		item.Cast();

	}
	else if (type == SpellTypes::Targeted)
	{
		Spell::Targeted item = Spell::Targeted(this->itemSlot, range, DamageType::Magical); //temporary

		if (allyTarget != NULL && allyTarget != nullptr)
		{
			//SdkUiConsoleWrite("Did you come here3");
			if (allyTarget->IsValidTarget() && allyTarget->GetServerPosition().IsValid())
			{
				//SdkUiConsoleWrite("Did you come here4");
				item.Cast(allyTarget);
				item.Cast(&allyTarget->GetServerPosition());
			}
		}

	}
	else if (type == SpellTypes::SkillShot)
	{

	}
	else if (type == SpellTypes::Ranged)
	{

	}
}

unsigned int ItemStruct::GetItemID() const
{
	return this->itemID;
}

_SDK_ITEM ItemStruct::GetSDKItem() const
{
	return this->sdkItem;
}

SpellSlot ItemStruct::GetItemSlot() const
{
	return this->itemSlot;
}

const char* ItemStruct::GetDisplayName() const
{
	return this->displayName;
}

const char* ItemStruct::GetMenuID() const
{
	return this->menuID;
}

MenuTypes ItemStruct::GetMenuTypes() const
{
	return this->enumState;
}

SpellTypes ItemStruct::GetSpellTypes() const
{
	return this->spellType;
}

float ItemStruct::GetSpellRange() const
{
	return this->spellRange;
}