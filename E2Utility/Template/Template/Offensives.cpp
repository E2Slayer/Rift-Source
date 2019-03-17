#include "stdafx.h"
#include "Offensives.h"

const enum ItemTypes { Active, Timats, Target, HextTechs };


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
};

void Offensives::Init()
{
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick, Offensives::Tick);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, Summoners::Update);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, Offensives::DrawMenu);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::PostAttack, Offensives::PostAttack);

}


void Offensives::Tick(void * UserData)
{
	if (Player.IsAlive() && !Player.IsRecalling())
	{

		auto playerItems{ Player.GetItems() };
		if (playerItems.empty())
		{
			return;
		}

		for (auto const&[key, val] : playerItems)
		{

			//Activator.Consumables.Level1
			//if (!Menu::Get<bool>("Activator.Summoners.HealUse"))



			UseItems(ActiveItems, ItemTypes::Active, key, val);
			UseItems(TimatsItems, ItemTypes::Timats, key, val);
			UseItems(TargetItems, ItemTypes::Target, key, val);
			UseItems(HextTechsItems, ItemTypes::HextTechs, key, val);


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

		auto playerItems{ Player.GetItems() };
		if (playerItems.empty())
		{
			return;
		}

		for (auto const&[key, val] : playerItems)
		{

			if (key != (int)ItemID::Tiamat && key != (int)ItemID::RavenousHydra && key != (int)ItemID::TitanicHydra)
			{
				continue;
			}

			
			if (TimatsItems.empty())
			{
				return;
			}

			for (auto const&[key2, val] : TimatsItems)
			{
				SdkUiConsoleWrite("Inside of AA");
				UseTimats(key2, Player.GetItemSlot(key + 6), val, true);
			}


			/*
			if ((Menu::Get<bool>("Activator.Offensives.UseTiamatAfterAA") && Player.HasItem((int)ItemID::Tiamat)) ||
				(Menu::Get<bool>("Activator.Offensives.UseRavenousHydraAfterAA") && Player.HasItem((int)ItemID::RavenousHydra)))
			{
				if (Player.GetItemSlot(key + 6) == (unsigned char)SpellSlot::Unknown)
				{
					continue;
				}

				int spellSlot = Player.GetItemSlot(key) + 6;

				Spell::Active item = Spell::Active(spellSlot, 350.0f, DamageType::Physical);

				if (item.IsReady())
				{
					if (Target->IsAlive() && !Target->IsZombie())
					{
						//SdkUiConsoleWrite("USE123");
						if (Target->IsValidTarget() && Target->GetPosition().IsValid() && item.IsInRange(&Target->GetServerPosition()))
						{
							item.Cast();
						}
					}

				}

			}
			else if ((Menu::Get<bool>("Activator.Offensives.UseTitanicHydraAfterAA") && Player.HasItem((int)ItemID::TitanicHydra)))
			{
				if (Player.GetItemSlot(key + 6) == (unsigned char)SpellSlot::Unknown)
				{
					continue;
				}

				int spellSlot = Player.GetItemSlot(key) + 6;

				Spell::Active item = Spell::Active(spellSlot);


				if (item.IsReady())
				{
					if (Target->IsAlive() && !Target->IsZombie())
					{
						//SdkUiConsoleWrite("USE123");
						if (Target->IsValidTarget() && Target->GetPosition().IsValid() && item.IsInRange(&Target->GetServerPosition()))
						{
							item.Cast();
						}
					}

				}
			}*/


		}

	}




	//Activator.Offensives.UseTiamatAfterAA
	//Activator.Offensives.UseRavenousHydraAfterAA
	//Activator.Offensives.UseTitanicHydraAfterAA

}



///Your menu settings go here
void Offensives::DrawMenu(void * UserData)
{
	Menu::Tree("Offensives", "Activator.Offensives", false, []()
	{
		ItemMenuGenerator(ActiveItems, ItemTypes::Active);
		ItemMenuGenerator(TimatsItems, ItemTypes::Timats);
		ItemMenuGenerator(TargetItems, ItemTypes::Target);
		ItemMenuGenerator(HextTechsItems, ItemTypes::HextTechs);


		//const enum ItemTypes { ActiveItems, Timats, TargetItems, HextTechs};

		/*
		Menu::Checkbox("Disable At Level1", "Activator.Consumables.Level1", true);
		Menu::BulletText("^-> Rechargable Potion Still Will Be Used");
		Menu::BulletText("^-> Refillable Potion, Hunter's Potion, And Corruptiong Potion");
		*/
	});
}


void Offensives::UseTimats(int targetID, int spellSlot, std::string menuNameOrigninal, bool fromPostAttack)
{
	Spell::Active item = Spell::Active(spellSlot);
	if (targetID != (int)ItemID::Tiamat)
	{
		item = Spell::Active(spellSlot, 350.0f, DamageType::Physical);

	}
	else if (targetID != (int)ItemID::RavenousHydra)
	{
		item = Spell::Active(spellSlot, 350.0f, DamageType::Physical);

	}
	else if (targetID != (int)ItemID::TitanicHydra)
	{
		item = Spell::Active(spellSlot);

	}



	if (!item.IsValid())
	{
		return;
	}



	//SdkUiConsoleWrite("USE142");
	//Activator.Offensives.UseAfterAA
	std::string menuID = "Activator.Offensives.";
	std::string tempName = menuNameOrigninal;
	tempName.erase(remove_if(tempName.begin(), tempName.end(), std::isspace), tempName.end());
	menuID += tempName;

	std::string menuIDSecond = menuID;

	menuID += "Use";


	//SdkUiConsoleWrite("USE14 %s", menuID);

	//SdkUiConsoleWrite("01234");

	if (!item.IsReady())
	{
		//SdkUiConsoleWrite("1cant't cast %s", menuID);
		return;
	}

	if (!Menu::Get<bool>(menuID))
	{
		//SdkUiConsoleWrite("rere %s", menuID);
		return;
	}

	/*
	menuID += "AfterAA";

	if (Menu::Get<bool>(menuID) && !fromPostAttack)
	{
		//SdkUiConsoleWrite("rere %s", menuID);
		return;
	}*/

	/*
	if (item.LastCast != NULL && item.LastCastTick + 10000000.0f > GetTickCount()) //0.1f second
	{
		//SdkUiConsoleWrite("Here??");
		continue;
	}*/
	//SdkUiConsoleWrite("before Pointer");

	auto heroes_ptr
	{
		pSDK->EntityManager->GetEnemyHeroes(item.Range + 150.0f, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
	};

	if (heroes_ptr.empty())
	{
		return;
	}

	for (auto &[netID, heroes] : heroes_ptr)
	{
		if (heroes != nullptr && heroes != NULL)
		{

			if (heroes->IsAlive() && !heroes->IsZombie())
			{
				//SdkUiConsoleWrite("USE123");
				if (heroes->IsValidTarget() && heroes->GetPosition().IsValid() && item.IsInRange(heroes))
				{
					//SdkUiConsoleWrite("USE14");
					menuIDSecond += "MinHealth";

					float enemyPct = (float)Menu::Get<int>(menuIDSecond);
					if (enemyPct == 0)
					{
						//SdkUiConsoleWrite("01");
						continue;
					}

					if (heroes->GetHealthPercent() <= enemyPct)
					{
						//SdkUiConsoleWrite("cast1 %s", menuID);
						//SdkUiConsoleWrite("USE1");
						//item.Cast(heroes->GetServerPosition());
						item.Cast();
						continue;

					}

					menuIDSecond += "Me";

					float MyhealthPct = (float)Menu::Get<int>(menuIDSecond);
					if (MyhealthPct == 0)
					{
						//SdkUiConsoleWrite("01");
						continue;
					}

					if (Player.GetHealthPercent() <= MyhealthPct)
					{
						//SdkUiConsoleWrite("cast2 %s", menuID);
						//SdkUiConsoleWrite("USE1");
						//item.Cast(heroes->GetServerPosition());
						item.Cast();
						continue;

					}
				}


			}

		}
	}
}

void Offensives::UseItems(std::map<int, std::string> inputMap, ItemTypes itemType, int targetID, SDK_ITEM targetItem)
{
	if (inputMap.empty())
	{
		return;
	}

	//SdkUiConsoleWrite("USE123333333");



	for (auto const&[key, val] : inputMap)
	{
		if (targetID != key || key == NULL)
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
		if (!Player.HasItem(key) || Player.IsRecalling() )
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

		int spellSlot = Player.GetItemSlot(targetID) + 6;
		//SdkUiConsoleWrite("0666");

		//SdkUiConsoleWrite("USE14");
		//Spell::Active item = Spell::Active(spellSlot);

		if (itemType == ItemTypes::Timats )
		{
			
			UseTimats(targetID, spellSlot, val , false);


		}
		else
		{

			Spell::Targeted item = Spell::Targeted(spellSlot, 700.0f, DamageType::Magical);


			if (itemType == ItemTypes::Target)
			{
				if (targetID == (int)ItemID::HextechGunblade)
				{
					item = Spell::Targeted(spellSlot, 550.0f, DamageType::Magical);
				}
				else
				{
					item = Spell::Targeted(spellSlot, 700.0f, DamageType::Magical);
				}
			}
			else if (itemType == ItemTypes::HextTechs)
			{
				if (targetID == (int)ItemID::HextechGLP800)
				{
					item = Spell::Targeted(spellSlot, 1100.0f, DamageType::Magical);
					//item = Spell::Skillshot(spellSlot, 1200.0f, SkillshotType::Cone, 0.25f, 1600.0f, 80.0f, DamageType::Magical, true, CollisionFlags::BraumWall | CollisionFlags::Minions | CollisionFlags::YasuoWall );
				}
				else if (targetID == (int)ItemID::HextechProtobelt01)
				{
					item = Spell::Targeted(spellSlot, 1100.0f, DamageType::Magical);
				}
			}
			//SdkUiConsoleWrite("012");



			if (!item.IsValid())
			{
				continue;
			}



			//SdkUiConsoleWrite("USE142");

			std::string menuID = "Activator.Offensives.";
			std::string tempName = val;
			tempName.erase(remove_if(tempName.begin(), tempName.end(), std::isspace), tempName.end());
			menuID += tempName;

			std::string menuIDSecond = menuID;

			menuID += "Use";

			//SdkUiConsoleWrite("USE14 %s", menuID);

			//SdkUiConsoleWrite("01234");

			if (!item.IsReady())
			{
				SdkUiConsoleWrite("cant't cast %s", menuID);
				continue;
			}

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

			auto heroes_ptr
			{
				pSDK->EntityManager->GetEnemyHeroes(item.Range + 150.0f, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
			};

			if (heroes_ptr.empty())
			{
				return;
			}

			for (auto &[netID, heroes] : heroes_ptr)
			{
				if (heroes != nullptr && heroes != NULL)
				{

					if (heroes->IsAlive() && !heroes->IsZombie())
					{
						//SdkUiConsoleWrite("USE123");
						if (heroes->IsValidTarget() && heroes->GetPosition().IsValid() && item.IsInRange(heroes))
						{
							//SdkUiConsoleWrite("USE14");
							menuIDSecond += "MinHealth";

							float enemyPct = (float)Menu::Get<int>(menuIDSecond);
							if (enemyPct == 0)
							{
								//SdkUiConsoleWrite("01");
								continue;
							}

							if (heroes->GetHealthPercent() <= enemyPct)
							{
								SdkUiConsoleWrite("cast1 %s", menuID);
								//SdkUiConsoleWrite("USE1");
								//item.Cast(heroes->GetServerPosition());
								item.Cast(&heroes->GetServerPosition());
								continue;

							}

							menuIDSecond += "Me";

							float MyhealthPct = (float)Menu::Get<int>(menuIDSecond);
							if (MyhealthPct == 0)
							{
								//SdkUiConsoleWrite("01");
								continue;
							}

							if (Player.GetHealthPercent() <= MyhealthPct)
							{
								SdkUiConsoleWrite("cast2 %s", menuID);
								//SdkUiConsoleWrite("USE1");
								//item.Cast(heroes->GetServerPosition());
								item.Cast(&heroes->GetServerPosition());
								continue;

							}
						}


					}

				}
			}







		}
	}
}


void Offensives::ItemMenuGenerator(std::map<int, std::string> inputMap, ItemTypes itemType)
{
	if (inputMap.empty())
	{
		return;
	}

	for (auto const&[key, val] : inputMap)
	{

		std::string menuID = "Activator.Offensives.";
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


			displayName = val;
			displayName += " Target Minimum Health %";

			menuIDSecond += "MinHealth";

			Menu::SliderInt(displayName.c_str(), menuIDSecond, 40, 1, 100);


			displayName = val;
			displayName += " My Minimum Health %";

			menuIDSecond += "Me";

			Menu::SliderInt(displayName.c_str(), menuIDSecond, 35, 1, 100);

			/*
			if (itemType == ItemTypes::Timats)
			{
					
				menuID += "AfterAA";

				std::string aaDisplayName = val;
				aaDisplayName += " Use Only After AA";

				Menu::Checkbox(aaDisplayName.c_str(), menuID, true);
			}*/


			

			/*
			if (itemType == ItemTypes::Mana || itemType == ItemTypes::HealthMana)
			{
				std::string displayNameSecond = val;

				displayNameSecond += " Minimum Mana %";

				menuIDSecond += "Mana";
				Menu::SliderInt(displayName.c_str(), menuIDSecond, 25, 1, 100);
			}*/

		});

	}
}