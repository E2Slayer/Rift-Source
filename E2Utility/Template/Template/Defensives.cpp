#include "stdafx.h"
#include "Defensives.h"



const enum ItemTypes { AffectEnemy, AffectAlly, AllyTarget, SpeedUp, LowMyHealth};



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

	pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick, Defensives::Tick);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, Summoners::Update);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, Defensives::DrawMenu);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::PostAttack, Offensives::PostAttack);

}


void Defensives::Tick(void * UserData)
{
	if (Player.IsAlive())
	{

		auto playerItems{ Player.GetItems() };
		if (playerItems.empty())
		{
			return;
		}

		for (auto const&[key, val] : playerItems)
		{
			unsigned char temp = 0;

			if (val.Slot == NULL)
			{
				temp = 0;
			}
			else
			{
				temp = val.Slot;
			}
 
			if (temp >= 0 && temp < 66)
			{
				//UseItems(AffectEnemyItems, ItemTypes::AffectEnemy, key, val);
				//Activator.Consumables.Level1
				//if (!Menu::Get<bool>("Activator.Summoners.HealUse"))

				//const enum ItemTypes { AffectEnemy, AffectAlly, AllyTarget, SpeedUp, LowMyHealth };
				//SdkUiConsoleWrite("slot : %d Name: %s", val.Slot, val.DisplayName);


				UseItems(AffectEnemyItems, ItemTypes::AffectEnemy, key, temp);
				UseItems(AffectAllyItems, ItemTypes::AffectAlly, key, temp);
				UseItems(AllyTargetItems, ItemTypes::AllyTarget, key, temp);
				UseItems(SpeedUpItems, ItemTypes::SpeedUp, key, temp);
				UseItems(LowMyHealthItems, ItemTypes::LowMyHealth, key, temp);
			}
			
		}

	}

}


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


		/*
		if (strcmp(buffNameOfItem, "None") == 0)
		{
			continue;
		}*/

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

			/*
			if (item.LastCast != NULL && item.LastCastTick + 10000000.0f > GetTickCount()) //0.1f second
			{
				//SdkUiConsoleWrite("Here??");
				continue;
			}*/
			//SdkUiConsoleWrite("before Pointer");

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

				/*
				for (auto &[netID, heroes] : heroes_ptr)
				{
					if (heroes != nullptr && heroes != NULL)
					{

						if (heroes->IsAlive())
						{
							//SdkUiConsoleWrite("USE123");
							if (heroes->IsValidTarget() && heroes->GetPosition().IsValid() && item.IsInRange(heroes))
							{
								targetEnemyNumber++;
								//SdkUiConsoleWrite("USE14");

								//if (countTarget >= targetEnemyNumber)
								{
									SdkUiConsoleWrite("cast1 %s", menuID);
									item.Cast(&heroes->GetServerPosition());
									continue;

								}

							}


						}

					}
				}*/
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

				/*
				menuIDThird += "AllyTarget";
				Menu::DropList(displayName.c_str(), menuIDThird, std::vector<std::string>{ "Most AD", "Most AP", "Anyone" }, 0);
				*/

				
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




///Your menu settings go here
void Defensives::DrawMenu(void * UserData)
{

	Menu::Tree("Defensives", "Activator.Defensives", false, []()
	{
		ItemMenuGenerator(AffectEnemyItems, ItemTypes::AffectEnemy);
		ItemMenuGenerator(AffectAllyItems, ItemTypes::AffectAlly);
		ItemMenuGenerator(AllyTargetItems, ItemTypes::AllyTarget);
		ItemMenuGenerator(SpeedUpItems, ItemTypes::SpeedUp);
		ItemMenuGenerator(LowMyHealthItems, ItemTypes::LowMyHealth);


		//const enum ItemTypes { ActiveItems, Timats, TargetItems, HextTechs};

		/*
		Menu::Checkbox("Disable At Level1", "Activator.Consumables.Level1", true);
		Menu::BulletText("^-> Rechargable Potion Still Will Be Used");
		Menu::BulletText("^-> Refillable Potion, Hunter's Potion, And Corruptiong Potion");
		*/
	});
}


void Defensives::ItemMenuGenerator(std::map<int, std::string> inputMap, ItemTypes itemType)
{
	if (inputMap.empty())
	{
		return;
	}

	for (auto const&[key, val] : inputMap)
	{

		std::string menuID = "Activator.Defensives.";
		std::string tempName = val;
		tempName.erase(remove_if(tempName.begin(), tempName.end(), std::isspace), tempName.end());


		menuID += tempName;

		//const char* menuDispaly = std::to_wstring(val);

		Menu::Tree(val.c_str(), menuID, false, [key, val, itemType, menuID]() mutable
		{
			std::string menuIDSecond = menuID;

			std::string menuIDThird = menuID;


			menuID += "Use";
			std::string displayName = "Use ";
			displayName += val;

			Menu::Checkbox(displayName.c_str(), menuID, true);


			if (itemType == ItemTypes::LowMyHealth || itemType == ItemTypes::AffectAlly)
			{
				if (key != (int)ItemID::Ohmwrecker)
				{
					displayName = val;
					displayName += " My Minimum Health %";

					menuIDSecond += "Me";

					Menu::SliderInt(displayName.c_str(), menuIDSecond, 35, 1, 100);
				}

				displayName = val;
				if (key == (int)ItemID::LocketoftheIronSolari)
				{
					displayName += " Ally Minimum Health %";

					menuIDSecond += "MinHealth";

					Menu::SliderInt(displayName.c_str(), menuIDSecond, 30, 1, 100);
				}
				else if (key == (int)ItemID::Ohmwrecker)
				{
					displayName += " Minimum Ally Number # Under Tower";

					menuIDThird += "MinAllyTower";

					Menu::SliderInt(displayName.c_str(), menuIDThird, 2, 1, 5);
				}

			}
			else if (itemType == ItemTypes::SpeedUp && key == (int)ItemID::ShurelyasReverie)
			{
				displayName = val;
				displayName += " Minimum Ally Number # Nearyby Me";

				menuIDThird += "MinAlly";

				Menu::SliderInt(displayName.c_str(), menuIDThird, 2, 1, 5);
			}
			else if (itemType == ItemTypes::AffectEnemy)
			{
				displayName = val;
				if (key == (int)ItemID::RanduinsOmen)
				{
					displayName += " Minimum Enemy Number #";

					menuIDThird += "MinEnemy";

					Menu::SliderInt(displayName.c_str(), menuIDThird, 2, 1, 5);
				}

			}
			else if (itemType == ItemTypes::AllyTarget)
			{
				displayName = val;
				displayName += " on Ally Who";

				menuIDThird += "AllyTarget";
				Menu::DropList(displayName.c_str(), menuIDThird, std::vector<std::string>{ "Most AD", "Most AP", "Anyone" }, 0);
			}
		

		});

	}
}