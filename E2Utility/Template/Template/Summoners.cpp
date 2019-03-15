#include "stdafx.h"
#include "Summoners.h"


//Summoners




std::map<unsigned int, AIHeroClient*> AllyChampions;
std::map<unsigned int, AIHeroClient*> EnemyChampions;



void Summoners::Init()
{


	pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick, Summoners::Tick);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, Summoners::Update);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, Summoners::DrawMenu);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, Summoners::Draw);

	AllyChampions.clear();
	EnemyChampions.clear();

	AllyChampions = pSDK->EntityManager->GetAllyHeroes();

	EnemyChampions = pSDK->EntityManager->GetEnemyHeroes();

}


void Summoners::Tick(void * UserData)
{
	if (Player.IsAlive() && !Player.IsRecalling())
	{
		IgniteCheck();
		ExhaustCheck();
		HealCheck();
		BarrierCheck();
	}
}


///This gets called X times per second, where X is your league fps.
///Put any drawings you need here
void Summoners::Draw(void * UserData)
{


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


bool Summoners::bSummonersExpanded = false;
bool Summoners::bSmiteExpanded = false;
bool Summoners::bIgniteExpanded = false;
bool Summoners::bIgniteExpanded2 = false;
bool Summoners::bExhaustExpanded = false;
bool Summoners::bExhaustExpanded2 = false;
bool Summoners::bHealExpanded = false;
bool Summoners::bHealAllyExpanded = false;
bool Summoners::bBarrierExpanded = false;

///Your menu settings go here
void Summoners::DrawMenu(void * UserData)
{
	UNREFERENCED_PARAMETER(UserData);

	Menu::Tree("Summoners", "Activator.Summoners", false, []()
	{

		//SdkUiText("For Smite, Use the Rift Essential one :^)");
		
		
		Menu::Tree("Smite", "Activator.Summoners.Smite", false, []()
		{
			SdkUiText("For Smite, Use the Rift Essential one :^)");
			SdkUiText("Because I want you to feel the power of RMAN");


			//const std::vector<std::string> comboKillsteal{ "Combo", "KillSteal" };

			/*
			Menu::Checkbox("Use Smite", "Activator.Summoners.SmiteUse", true);
			Menu::HotkeyToggle("test hoeky", "howowo", 0, 0);
			Menu::DropList("Smite Style on Enemy", "Activator.Summoners.SmiteStyle", std::vector<std::string>{ "Combo", "KillSteal", "No Use" }, 0);

			bool bSmiteExpanded2 = true;
			Menu::Tree("Use Smite on Enemy", "Activator.Summoners.SmiteUseTo", &bSmiteExpanded2, []()
			{
				
				if (!EnemyChampions.empty())
				{
					for (auto const &ent : EnemyChampions)
					{
						std::string menuID = "Activator.Summoners.IgniteUseFor";
						menuID += (&ent)->second->GetCharName();
						Menu::Checkbox((&ent)->second->GetCharName(), menuID, true);
					}
				}
			});


			bool bSmiteEpicExpanded = true;
			Menu::Tree("Use Smite on Epic Monsters", "Activator.Summoners.SmiteUseToEpic", &bSmiteEpicExpanded, []()
			{
				Menu::Checkbox("Baron", "Activator.Summoners.SmiteBaron", true);
				Menu::Checkbox("Dragon", "Activator.Summoners.SmiteDragon", true);
				Menu::Checkbox("Rift Herald", "Activator.Summoners.SmiteRift", true);
			});

			bool bSmiteLargeExpanded = true;
			Menu::Tree("Use Smite on Large Monsters", "Activator.Summoners.SmiteUseToLarge", &bSmiteLargeExpanded, []()
			{
				Menu::Checkbox("Blue", "Activator.Summoners.SmiteBlue", true);
				Menu::Checkbox("Red", "Activator.Summoners.SmiteRed", true);
			});

			bool bSmiteSmallExpanded = true;
			Menu::Tree("Use Smite on Small Monsters", "Activator.Summoners.SmiteUseToSmall", &bSmiteSmallExpanded, []()
			{
				Menu::Checkbox("Wolf", "Activator.Summoners.SmiteWolf", false);
				Menu::Checkbox("Razorbeak", "Activator.Summoners.SmiteRazorbeak", false);
				Menu::Checkbox("Krug", "Activator.Summoners.SmiteKrug", false);
				Menu::Checkbox("Gromp", "Activator.Summoners.SmiteGromp", false);
				Menu::Checkbox("Rift Scuttler", "Activator.Summoners.SmiteCrab", true);
			});*/

		});
		


		Menu::Tree("Ignite", "Activator.Summoners.Ignite", false, []()
		{
			//const std::vector<std::string> comboKillsteal{ "Combo", "KillSteal" };


			Menu::Checkbox("Use Ignite", "Activator.Summoners.IgniteUse", true);
			Menu::DropList("Ignite Style", "Activator.Summoners.IgniteStyle", std::vector<std::string>{ "KillSteal", "Combo" }, 0);

			
			Menu::Tree("Use Ignite For", "Activator.Summoners.IgniteUseTo", &bIgniteExpanded2, []()
			{
				if (!EnemyChampions.empty())
				{
					for (auto const &ent : EnemyChampions)
					{
						std::string menuID = "Activator.Summoners.IgniteUseFor";
						menuID += (&ent)->second->GetCharName();
						Menu::Checkbox((&ent)->second->GetCharName(), menuID, true);
					}
				}
			});


		});




		Menu::Tree("Exhaust", "Activator.Summoners.Exhaust", false, []()
		{
			Menu::Checkbox("Use Exhaust", "Activator.Summoners.ExhaustUse", true);

			Menu::SliderInt("Exhaust on ally HP %", "Activator.Summoners.ExhaustAllyPct", 30, 0, 100);

			Menu::SliderInt("Exhaust on enemy HP %", "Activator.Summoners.ExhaustEnemyPct", 30, 0, 100);


			Menu::DropList("Exhaust Style", "Activator.Summoners.ExhaustStyle", std::vector<std::string>{ "Always", "Combo" }, 0);

			Menu::Tree("Use Exhaust For", "Activator.Summoners.ExhaustUseTo", &bExhaustExpanded2, []()
			{
				if (!EnemyChampions.empty())
				{
					for (auto const &ent : EnemyChampions)
					{
						std::string menuID = "Activator.Summoners.ExhaustUseFor";
						menuID += (&ent)->second->GetCharName();
						Menu::Checkbox((&ent)->second->GetCharName(), menuID, true);
					}
				}
			});


		});


		Menu::Tree("Heal", "Activator.Summoners.Heal", false, []()
		{
			Menu::Checkbox("Use Heal", "Activator.Summoners.HealUse", true);

			Menu::SliderInt("Heal Myself - min HP %", "Activator.Summoners.HealMeMinHP", 30, 0, 100);

			//Menu::SliderInt("Heal Myself - Min Incoming Dmg %", "Activator.Summoners.HealMeMinIncDmg", 20, 1, 50);

			Menu::SliderInt("Heal Ally - Min HP %", "Activator.Summoners.HealAllyMinHP", 20, 1, 50);



			Menu::DropList("Heal Style", "Activator.Summoners.HealStyle", std::vector<std::string>{ "Myself Only", "Ally and Me" }, 0);



			Menu::Tree("Use Heal For", "Activator.Summoners.HealUseTo", &bHealAllyExpanded, []()
			{
				if (!AllyChampions.empty())
				{
					for (auto const &ent : AllyChampions)
					{
						if ((&ent)->first != Player.GetNetworkID())
						{
							std::string menuID = "Activator.Summoners.HealUseFor";
							menuID += (&ent)->second->GetCharName();
							Menu::Checkbox((&ent)->second->GetCharName(), menuID, true);
						}
					}
				}
			});


		});

		Menu::Tree("Barrier", "Activator.Summoners.Barrier", false, []()
		{
			Menu::Checkbox("Use Barrier", "Activator.Summoners.BarrierUse", true);

			Menu::SliderInt("Min HP %", "Activator.Summoners.BarrierMinHP", 30, 0, 100);

			//Menu::SliderInt("Min Incoming Dmg %", "Activator.Summoners.BarrierMinIncDmg", 20, 1, 50);

			Menu::Checkbox("Use Barrier on Ignite", "Activator.Summoners.BarrierUseIgnite", true);



		});

	});




}

void Summoners::IgniteCheck()
{
	if (!Menu::Get<bool>("Activator.Summoners.IgniteUse"))
	{
		return;
	}

	unsigned char ignite = Player.GetSpellSlotFromName("SummonerDot");

	if (ignite == (unsigned char)SpellSlot::Unknown || ignite == NULL)
	{
		return;
		//SdkUiConsoleWrite("You have Ignite");
	}

	if (ignite != (unsigned char)SpellSlot::Summoner1 && ignite != (unsigned char)SpellSlot::Summoner2)
	{
		return;
	}


	Spell::Targeted igniteSpell = Spell::Targeted(ignite, 600.0f, DamageType::True);
	
	if (igniteSpell.LastCast + 0.1f >= Game::Time()) //0.1f second
	{
		return;
	}

	if (!igniteSpell.IsReady())
	{
		return;
	}

	int list = Menu::Get<int>("Activator.Summoners.IgniteStyle"); // "KillSteal", "Combo"

	if (list < 0 || list > 1)
	{
		return;
	}

	if (!pSDK->EntityManager->GetLocalPlayer().GetPosition().IsValid())
	{
		return;
	}


	float igniteDamage = 50.0f + (20.0f * ((float)Player.GetLevel() - 1.0f));

	
	auto heroes_ptr
	{
		pSDK->EntityManager->GetEnemyHeroes(igniteSpell.Range, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
	};

	if (heroes_ptr.empty())
	{
		return;
	}

	for (auto &[netID, heroes] : heroes_ptr)
	{
		if (heroes != nullptr && heroes != NULL)
		{
			if (heroes->GetHealthPercent() <= 50.0f)
			{

				if (heroes->IsAlive() && heroes->IsVisible() )
				{
					if (!heroes->IsZombie())
					{
						if (!heroes->HasBuff("SummonerDot"))
						{
							std::string menuID = "Activator.Summoners.IgniteUseFor";
							menuID += heroes->GetCharName();
							if (Menu::Get<bool>(menuID))
							{
								if (list == 0) //KillSteal
								{
									if ((heroes->GetHealth().Current + heroes->GetHealth().AllShield) <= igniteDamage)
									{
										igniteSpell.Cast(heroes);
									}
								}
								else if (list == 1 && heroes->GetHealthPercent() <= 40.0f) // combo
								{
									if (pCore->Orbwalker->IsModeActive(OrbwalkingMode::Combo))
									{
										auto OrbTarget = pCore->Orbwalker->GetTarget();
										if (pCore->TS->IsValidTarget(OrbTarget) && OrbTarget->GetNetworkID() == heroes->GetNetworkID())
										{
											igniteSpell.Cast(heroes);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	

	//SpellSlot::Summoner1;
	//auto sum1 = Player.GetSpell((unsigned char)SpellSlot::Summoner1).ScriptName;
	//auto sum2 = Player.GetSpell((unsigned char)SpellSlot::Summoner2).ScriptName;


}

void Summoners::ExhaustCheck()
{
	/*
			Menu::Tree("Exhaust", "Activator.Summoners.Exhaust", &bExhaustExpanded, []()
		{
			Menu::Checkbox("Use Exhaust", "Activator.Summoners.ExhaustUse", true);

			Menu::SliderInt("Exhaust on ally HP %", "Activator.Summoners.ExhaustAllyPct", 30, 0, 100);

			Menu::SliderInt("Exhaust on enemy HP %", "Activator.Summoners.ExhaustEnemyPct", 30, 0, 100);


			Menu::DropList("Exhaust Style", "Activator.Summoners.ExhaustStyle", std::vector<std::string>{ "Always", "Combo" }, 0);

			bool bExhaustExpanded = true;
			Menu::Tree("Use Exhaust For", "Activator.Summoners.ExhaustUseTo", &bExhaustExpanded, []()
			{
				if (!EnemyChampions.empty())
				{
					for (auto const &ent : EnemyChampions)
					{
						std::string menuID = "Activator.Summoners.ExhaustUseFor";
						menuID += (&ent)->second->GetCharName();
						Menu::Checkbox((&ent)->second->GetCharName(), menuID, true);
					}
				}
			});


		});

	*/

	if (!Menu::Get<bool>("Activator.Summoners.ExhaustUse"))
	{
		return;
	}

	unsigned char exhuast = Player.GetSpellSlotFromName("SummonerExhaust");

	if (exhuast == (unsigned char)SpellSlot::Unknown || exhuast == NULL)
	{
		return;
		//SdkUiConsoleWrite("You have Ignite");
	}

	if (exhuast != (unsigned char)SpellSlot::Summoner1 && exhuast != (unsigned char)SpellSlot::Summoner2)
	{
		return;
	}

	//Player.GetSpell(exhuast); //50 +(20*level) dmg

	Spell::Targeted exhuastSpell = Spell::Targeted(exhuast, 650.0f, DamageType::Mixed);

	if (exhuastSpell.LastCast + 0.1f >= Game::Time()) //0.1f second
	{
		return;
	}


	if (!exhuastSpell.IsReady())
	{
		return;
	}

	int list = Menu::Get<int>("Activator.Summoners.ExhaustStyle"); // "Always", "Combo"
	

	if (list < 0 || list > 1)
	{
		return;
	}
	
	
	if (!pSDK->EntityManager->GetLocalPlayer().GetPosition().IsValid())
	{
		return;
	}



	auto heroes_ptr
	{
		pSDK->EntityManager->GetEnemyHeroes(exhuastSpell.Range, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
	};

	auto Allyheroes_ptr
	{
		pSDK->EntityManager->GetAllyHeroes(1000.0f, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
	};

	//SdkUiConsoleWrite("are");
	if (heroes_ptr.empty() || Allyheroes_ptr.empty())
	{
		return;
	}

	for (auto &[netID, heroes] : heroes_ptr)
	{
		if (heroes != nullptr && heroes != NULL)
		{

				if (heroes->IsAlive() && heroes->IsVisible() )
				{

					std::string menuID = "Activator.Summoners.ExhaustUseFor";
					menuID += heroes->GetCharName();
					if (Menu::Get<bool>(menuID))
					{
						if (list == 0 || (list == 1 && pCore->Orbwalker->IsModeActive(OrbwalkingMode::Combo))) // "Always", "Combo"
						{
							float enemyHealthMinimum = (float) Menu::Get<int>("Activator.Summoners.ExhaustEnemyPct");
							float allyHealthMinimum = (float)Menu::Get<int>("Activator.Summoners.ExhaustAllyPct");
							if (enemyHealthMinimum == 0 || enemyHealthMinimum == NULL || allyHealthMinimum == 0 || allyHealthMinimum == NULL)
							{
								continue;
							}

							
							for (auto &[allynetID, Ally] : Allyheroes_ptr)
							{
								if (Ally != nullptr && Ally != NULL)
								{
									if (!heroes->IsZombie())
									{
										if (!heroes->HasBuff("SummonerExhaust"))
										{
											if (heroes->GetHealthPercent() <= enemyHealthMinimum)
											{
												if (!heroes->IsFacing(Ally))
												{
													exhuastSpell.Cast(heroes);
												}

											}
											else if (Ally->GetHealthPercent() <= allyHealthMinimum)
											{
												if (heroes->IsFacing(Ally))
												{
													exhuastSpell.Cast(heroes);
												}

											}
										}
									}
								}
							}
						}
					}
				}
			
		}
	}
}

void Summoners::HealCheck()
{

	/*
	Menu::Tree("Heal", "Activator.Summoners.Heal", &bHealExpanded, []()
		{
			Menu::Checkbox("Use Heal", "Activator.Summoners.HealUse", true);

			Menu::SliderInt("Heal Myself - min HP %", "Activator.Summoners.HealMeMinHP", 30, 0, 100);

			Menu::SliderInt("Heal Myself - Min Incoming Dmg %", "Activator.Summoners.HealMeMinIncDmg", 20, 1, 50);


			Menu::DropList("Heal Style", "Activator.Summoners.HealStyle", std::vector<std::string>{ "Myself Only", "Ally and Me" }, 0);

			bool bHealAllyExpanded = true;
			Menu::Tree("Use Heal For", "Activator.Summoners.HealUseTo", &bHealAllyExpanded, []()
			{
				if (!AllyChampions.empty())
				{
					for (auto const &ent : AllyChampions)
					{
						if ((&ent)->first != Player.GetNetworkID())
						{
							std::string menuID = "Activator.Summoners.HealUseFor";
							menuID += (&ent)->second->GetCharName();
							Menu::Checkbox((&ent)->second->GetCharName(), menuID, true);
						}
					}
				}
			});


		});
	*/

	if (!Menu::Get<bool>("Activator.Summoners.HealUse"))
	{
		return;
	}

	unsigned char heal = Player.GetSpellSlotFromName("SummonerHeal");

	if (heal == (unsigned char)SpellSlot::Unknown || heal == NULL)
	{
		return;
		//SdkUiConsoleWrite("You have Ignite");
	}

	if (heal != (unsigned char)SpellSlot::Summoner1 && heal != (unsigned char)SpellSlot::Summoner2)
	{
		return;
	}


	Spell::Active healSpell = Spell::Active(heal, 850.0f, DamageType::True);


	if (!healSpell.IsReady())
	{
		return;
	}

	//SdkUiConsoleWrite("here123");


	int list = Menu::Get<int>("Activator.Summoners.HealStyle"); // "Myself Only", "Ally and Me"

	if (list < 0 || list > 1)
	{
		return;
	}

	if (!pSDK->EntityManager->GetLocalPlayer().GetPosition().IsValid())
	{
		return;
	}


	float healAmount = 75.0f + (15.0f * (float)Player.GetLevel());


	//SdkUiConsoleWrite("here1");
	
	if (Player.CountEnemiesInRange(healSpell.Range) >= 1)
	{
		if (Player.GetHealthPercent() <= (float)Menu::Get<int>("Activator.Summoners.HealMeMinHP"))
		{
			//SdkUiConsoleWrite("gt2");
			healSpell.Cast();
		}
	}
	
	if (list == 1)
	{

		auto heroes_ptr
		{
			pSDK->EntityManager->GetAllyHeroes(healSpell.Range, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
		};

		if (heroes_ptr.empty())
		{
			return;
		}

		for (auto &[netID, heroes] : heroes_ptr)
		{
			if (heroes != nullptr && heroes != NULL)
			{
				if (heroes->GetHealthPercent() <= 50.0f)
				{
					if (heroes->IsAlive() && !heroes->IsZombie())
					{
						std::string menuID = "Activator.Summoners.HealUseFor";
						menuID += heroes->GetCharName();
						if (Menu::Get<bool>(menuID))
						{
							if (Player.CountEnemiesInRange(healSpell.Range) >= 1)
							{
								if (heroes->GetHealthPercent() <= (float)Menu::Get<int>("Activator.Summoners.HealAllyMinHP"))
								{
									healSpell.Cast();
								}
							}
						}
					}
				}
			}
		}
	}

}

void Summoners::BarrierCheck()
{
	/*
		bool bBarrierExpanded = true;
		Menu::Tree("Barrier", "Activator.Summoners.Barrier", &bBarrierExpanded, []()
		{
			Menu::Checkbox("Use Barrier", "Activator.Summoners.BarrierUse", true);

			Menu::SliderInt("Min HP %", "Activator.Summoners.BarrierMinHP", 30, 0, 100);

			//Menu::SliderInt("Min Incoming Dmg %", "Activator.Summoners.BarrierMinIncDmg", 20, 1, 50);

			Menu::Checkbox("Use Barrier on Ignite", "Activator.Summoners.BarrierUseIgnite", true);



		});
	*/

	if (!Menu::Get<bool>("Activator.Summoners.BarrierUse"))
	{
		return;
	}

	unsigned char barrier = Player.GetSpellSlotFromName("SummonerBarrier");

	if (barrier == (unsigned char)SpellSlot::Unknown || barrier == NULL)
	{
		return;
		//SdkUiConsoleWrite("You have Ignite");
	}

	if (barrier != (unsigned char)SpellSlot::Summoner1 && barrier != (unsigned char)SpellSlot::Summoner2)
	{
		return;
	}


	Spell::Active barrierSpell = Spell::Active(barrier, 750.0f , DamageType::True);


	if (!barrierSpell.IsReady())
	{
		return;
	}

	//SdkUiConsoleWrite("here123");




	if (!pSDK->EntityManager->GetLocalPlayer().GetPosition().IsValid())
	{
		return;
	}


	//float healAmount = 75.0f + (15.0f * (float)Player.GetLevel());


	//SdkUiConsoleWrite("here1");


	

	if (Player.CountEnemiesInRange(barrierSpell.Range) >= 1)
	{
		if (Player.GetHealthPercent() <= (float)Menu::Get<int>("Activator.Summoners.BarrierMinHP"))
		{
			//SdkUiConsoleWrite("gt2");
			barrierSpell.Cast();
		}
	}

	if (Player.HasBuff("SummonerDot") && Menu::Get<bool>("Activator.Summoners.BarrierUseIgnite"))
	{
		barrierSpell.Cast();
	}


}



/*
void Summoners::Tick(_In_ void* UserData)
{

}

void Summoners::Draw(_In_ void* UserData)
{

}

void Summoners::DrawMenu(_In_ void* UserData)
{

}*/