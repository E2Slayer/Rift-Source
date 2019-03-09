#include "stdafx.h"
#include "Kalista.h"

AttackableUnit  Kalista::CurrentTarget;
AttackableUnit* Kalista::OrbTarget;

static float lastaa = 0.0f;

auto spellQ = Spell::Skillshot(SpellSlot::Q, 1150.0f, SkillshotType::Line, 0.25f, 2100.0f, 40.0f, DamageType::Physical, true, CollisionFlags::YasuoWall | CollisionFlags::Minions | CollisionFlags::BraumWall );
auto spellW = Spell::Active(SpellSlot::W, 5000.0f, DamageType::Physical);
auto spellE = Spell::Active(SpellSlot::E, 1000.0f, DamageType::Physical);
auto spellR = Spell::Active(SpellSlot::R, 1200.0f, DamageType::Physical);


std::vector<std::string> hitchanceList { "Low", "Medium", "High", "Very High" };

float rawPierceMana[] = { 50.0f , 55.0f , 60.0f , 65.0f , 70.0f };
float rawFatesCallMana = 100.0f;

///This gets called once, at load
void Kalista::Init()
{
	//SdkUiConsoleWrite("c2 %s", Player.GetCharName());

	if(strcmp(Player.GetCharName(), "Kalista") != 0)
	{
		return; //since player is not playing Kalista
	}

	CurrentTarget = AttackableUnit();
	OrbTarget = NULL;

#pragma region RegisterCallbacks
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick,	Kalista::Tick);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Update,	Kalista::Update);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, Kalista::DrawMenu);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Update,	Kalista::Draw);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::SpellCastStart, Kalista::SpellCastStart);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::SpellCastEnd, Kalista::SpellCastEnd);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Attack, Kalista::Attack);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::UnkillableMinion, Kalista::UnkillableMinion);

#pragma endregion
}


void Kalista::Attack(void* AI, void* TargetObject, bool StartAttack, bool StopAttack, void* UserData)
{
	if (AI == NULL || AI == nullptr || TargetObject == NULL || TargetObject == nullptr)
	{
		return;
	}



	if (pSDK->EntityManager->GetObjectFromPTR(AI)->GetNetworkID() == Player.GetNetworkID())
	{

		//SdkUiConsoleWrite("Delay - %f", Player.GetAttackDelay()*1000);
		//SdkUiConsoleWrite("GameTime - %f", float(Game::Time() * 1000));
		//SdkUiConsoleWrite("GetLastAA - %d", pCore->Orbwalker->GetLastAutoAttack());
		//Player.GetAttackDelay()pCore->Orbwalker->GetLastAutoAttack()

		float calc = lastaa - float(Game::Time() * 1000);

		SdkUiConsoleWrite("Difference - %f", Player.GetAttackSpeed());
		//SdkUiConsoleWrite("Difference2 - %f", P);
		lastaa = float(Game::Time()*1000);
	}

}

void Kalista::UnkillableMinion(AIMinionClient* Target)
{
	if (Menu::Get<bool>("Kalista.miscEMinion") && spellE.IsReady())
	{
		if(Target->IsValid() && Target->IsAlive() && IsRendKillable(Target->AsAIBaseClient()))
		{
			spellE.Cast();
		}

	}
}



///This gets called 30 times per second, put your logics in here instead of
///on OnUpdate so it doesnt drop too many fps
void Kalista::Tick(void * UserData)
{
	auto target_ptr
	{ 
		pCore->TS->GetTarget(1500.0f, DamageType::Magical) 
	};


	if (target_ptr) 
	{
		CurrentTarget = *(target_ptr);
	}



	auto minions_ptr
	{
		pSDK->EntityManager->GetEnemyMinions(4000.0f, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
	};

	




}

///This gets called X times per second, where X is your league fps.
///Use this only to update stuff you need to validade on every frame.
void Kalista::Update(void * UserData)
{
	OrbTarget = pCore->Orbwalker->GetTarget();

	SoulBoundSaver();

	KillSteal();

	AlwaysJungleE();

	if (pCore->Orbwalker->IsModeActive(OrbwalkingMode::Combo)) // I will get you
	{

		Exploit();
		/*
		if (Menu::Get<bool>("Kalista.ComboChase"))
		{
			Chase();
		}

		//int hi = Menu::Get<int>("Kalista.ComboQHit");

		//SdkUiConsoleWrite("ayy I'm 1.7 - %d", hi); 
		Combo();*/
	}

	if(pCore->Orbwalker->IsModeActive(OrbwalkingMode::Mixed))
	{
		//Combo(); //change it to harass later at this point no need to use harass function
		Harass();
	}

	if (pCore->Orbwalker->IsModeActive(OrbwalkingMode::LaneClear))
	{
		LaneClear();
	}

	if (pCore->Orbwalker->IsModeActive(OrbwalkingMode::JungleClear))
	{
		JungleClear();
		Exploit();
	}

}

void Kalista::Combo()
{

	//delay, speed, width 

	auto heroes_ptr
	{
		pSDK->EntityManager->GetEnemyHeroes(4000.0f, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
	};



	// collision I gotta add it later


	

	if (CurrentTarget.IsValid() && spellQ.IsLearned() && spellQ.IsReady() && CurrentTarget.IsAlive() && Menu::Get<bool>("Kalista.ComboQ")) // Q usage
	{
		if ( ((&Player)->GetResource().Current - rawPierceMana[Player.GetSpell((char)SpellSlot::Q).Level - 1] >= 30.0f && Menu::Get<bool>("Kalista.miscSaveManaE")) || !Menu::Get<bool>("Kalista.miscSaveManaE"))
		{
			auto prediction{ pSDK->Prediction->GetPrediction(CurrentTarget.AsAIHeroClient()) };
			if (prediction->Hitchance >= GetHitChanceFromDropList(Menu::Get<int>("Kalista.ComboQHit")) && spellQ.Range >= Player.Distance(&CurrentTarget) && CurrentTarget.IsAlive())
			{

				Vector3 playerPos{ (&Player)->GetServerPosition() };


				if (!pSDK->Collision->CheckCollisions(playerPos, prediction->CastPosition, spellQ.Width, spellQ.Delay, spellQ.Speed, false, CollisionFlags::Minions | CollisionFlags::YasuoWall | CollisionFlags::BraumWall, false))
				{
					spellQ.Cast(&prediction->CastPosition);
				}


			}
		}
	}



	if (Menu::Get<bool>("Kalista.ComboE"))
	{

		for (auto &[netID, heroes] : heroes_ptr)
		{
			if (heroes->IsValid() && heroes->IsAlive() && heroes->HasBuff("kalistaexpungemarker", false))
			{
				if (IsRendKillable(heroes->AsAIBaseClient()) && !heroes->HasUndyingBuff())
				{
					if (spellE.IsInRange(heroes) && spellE.IsReady())
					{
						spellE.Cast();
					}
				}

				//Kalista.ComboEMinion
				if (Menu::Get<bool>("Kalista.ComboEMinion"))
				{
					auto minions_ptr // Get enemy minions within 1000 range to kite
					{
						pSDK->EntityManager->GetEnemyMinions(600.0f, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
					};

					for (auto &[netID, minions] : minions_ptr) // minion loop
					{

						if (minions->IsAlive() && minions->HasBuff("kalistaexpungemarker", false) && (minions->IsLaneMinion() || minions->IsSiegeMinion() || minions->IsSuperMinion()))
						{
							if (heroes->HasBuff("kalistaexpungemarker", false))
							{
								if (IsRendKillable(minions->AsAIBaseClient()))
								{
									if (spellE.IsInRange(minions) && spellE.IsReady())
									{
										spellE.Cast();
									}
								}
							}
						}

					}
				}

			}
		}
	}

	 // ======================= Exploit but need to work on somehow doesn't work yet.


	/*
	//if (Player.GetAttackSpeed() > 1.70f)
	{
		auto time = Game::Time()*1000.0f;

		//SdkGetGameTime()

		//auto time2 = time * 1000.0f;
		//SdkUiConsoleWrite("c1 : ");
		auto last = (float)pCore->Orbwalker->GetLastAutoAttack();

		//	SdkUiConsoleWrite("ayy I'm 1.7 - %f", time);1

			
			if(time >= lastaa + 1.0f)
			{

				auto mousepos{ Renderer::MousePos() };
				SdkMoveLocalPlayer(&mousepos, false);
			}

			if(time > lastaa + 1.0f + Player.GetAttackDelay()*1000.0f - 150.0f)
			{
				SdkUiConsoleWrite("c2");
				SdkAttackTargetLocalPlayer(&CurrentTarget, false);
				//pSDK->Control->Attack(&CurrentTarget);
			}
			
	}*/
	
}

void Kalista::Harass()
{
	//delay, speed, width 

	auto heroes_ptr
	{
		pSDK->EntityManager->GetEnemyHeroes(4000.0f, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
	};



	// collision I gotta add it later

	if (CurrentTarget.IsValid() && spellQ.IsLearned() && spellQ.IsReady() && Menu::Get<bool>("Kalista.HarassQ")) // Q usage
	{

		if ((float)Menu::Get<int>("Kalista.HarassQMana") <= Player.GetManaPercent())
		{
			if ( ((&Player)->GetResource().Current - rawPierceMana[Player.GetSpell((char)SpellSlot::Q).Level - 1] >= 30.0f && Menu::Get<bool>("Kalista.miscSaveManaE")) || !Menu::Get<bool>("Kalista.miscSaveManaE"))
			{
				auto prediction{ pSDK->Prediction->GetPrediction(CurrentTarget.AsAIHeroClient()) };
				if (prediction->Hitchance >= GetHitChanceFromDropList(Menu::Get<int>("Kalista.HarassQHit")) && spellQ.Range >= Player.Distance(&CurrentTarget) && CurrentTarget.IsAlive() && spellQ.IsReady())
				{

					Vector3 playerPos{ (&Player)->GetServerPosition() };

					if (!pSDK->Collision->CheckCollisions(playerPos, prediction->CastPosition, spellQ.Width, spellQ.Delay, spellQ.Speed, false, CollisionFlags::Minions | CollisionFlags::YasuoWall | CollisionFlags::BraumWall, false))
					{
						spellQ.Cast(&prediction->CastPosition);
					}

				}
			}
		}
	}



	if (Menu::Get<bool>("Kalista.HarassE"))
	{
		for (auto &[netID, heroes] : heroes_ptr)
		{
			if (heroes->IsValid() && heroes->IsAlive() && heroes->HasBuff("kalistaexpungemarker", false))
			{
				if (IsRendKillable(heroes->AsAIBaseClient()) && !heroes->HasUndyingBuff())
				{
					if (spellE.IsInRange(heroes) && spellE.IsReady())
					{
						spellE.Cast();
					}
				}

				//Kalista.ComboEMinion
				if (Menu::Get<bool>("Kalista.HarassEMinion"))
				{
					auto minions_ptr // Get enemy minions within 1000 range to kite
					{
						pSDK->EntityManager->GetEnemyMinions(600.0f, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
					};

					for (auto &[netID, minions] : minions_ptr) // minion loop
					{

						if (minions->IsAlive() && minions->HasBuff("kalistaexpungemarker", false) && (minions->IsLaneMinion() || minions->IsSiegeMinion() || minions->IsSuperMinion()))
						{
							if (heroes->HasBuff("kalistaexpungemarker", false))
							{
								if (IsRendKillable(minions->AsAIBaseClient()))
								{
									if (spellE.IsInRange(minions) && spellE.IsReady())
									{
										spellE.Cast();
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

void Kalista::LaneClear()
{
	auto minions_ptr
	{
		pSDK->EntityManager->GetEnemyMinions(1500.0f, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
	};


	int hitNumber = Menu::Get<int>("Kalista.LaneClearEMinimum");
	int killableNum = 0;
	for (auto &[netID, minions] : minions_ptr)
	{
		
		if (minions->IsAlive() && minions->IsValid() && minions->HasBuff("kalistaexpungemarker", false) && Menu::Get<bool>("Kalista.LaneClearE") && (minions->IsLaneMinion() || minions->IsSiegeMinion() || minions->IsSuperMinion()))
		{

			if (IsRendKillable(minions->AsAIBaseClient()))
			{
				if (spellE.IsInRange(minions) && spellE.IsReady())
				{
					killableNum++;
					if (killableNum >= hitNumber)
					{
						spellE.Cast();
					}
				}
			}
		}
	}
}

void Kalista::JungleClear()
{
	if (!Menu::Get<bool>("Kalista.JungleClearQ") && !Menu::Get<bool>("Kalista.JungleClearE"))
	{
		return;
	}

	auto jungle_ptr
	{
		pSDK->EntityManager->GetJungleMonsters(spellQ.Range, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
	};


	
	for (auto &[netID, jungle] : jungle_ptr)
	{


		if (jungle->IsValid() && jungle->IsAlive() && jungle->GetCampNumber() != 0) // campnumber 0 is plant include all kind of plants
		{
			if ( ((&Player)->GetResource().Current - rawPierceMana[Player.GetSpell((char)SpellSlot::Q).Level - 1] >= 30.0f && Menu::Get<bool>("Kalista.miscSaveManaE")) || !Menu::Get<bool>("Kalista.miscSaveManaE"))
			{
				if ((float)Menu::Get<int>("Kalista.JungleClearQMana") <= Player.GetManaPercent())
				{
					if (Menu::Get<bool>("Kalista.JungleClearQ") && spellQ.IsInRange(jungle) && jungle->IsVisible() && Player.IsFacing(jungle))
					{
						Vector3 junglePos{ jungle->GetServerPosition() };
						if (spellQ.IsLearned() && spellQ.IsReady() && junglePos.IsValid() && spellQ.IsReady()) // Q usage
						{
							spellQ.Cast(&junglePos);

						}
					}
				}

			}

			if (jungle->HasBuff("kalistaexpungemarker", false) && Menu::Get<bool>("Kalista.JungleClearE"))
			{
				if (IsRendKillable(jungle->AsAIBaseClient()))
				{
					if (spellE.IsInRange(jungle) && spellE.IsReady())
					{
						spellE.Cast();
					}
				}
			}
			
		}
	}

}

void Kalista::Chase()
{
	if (pCore->Orbwalker->IsModeActive(OrbwalkingMode::Combo) ) // When combo is active
	{
		if(Player.CountEnemiesInRange(2000.0f) <= 0)
		{
			return;
		}

		auto Enemies{ pSDK->EntityManager->GetEnemyHeroes(2000.0f, &pSDK->EntityManager->GetLocalPlayer().GetPosition()) }; // Get enemies

		
		
		for (auto &[netID, Enemy] : Enemies) // enemy loop
		{
			if (Enemy->IsValid() && Enemy != nullptr && Enemy != NULL)
			{
				if (Player.GetLevel() >= 6 || Enemy->IsRunningFrom(&Player))
				{
					if (pCore->Orbwalker->GetTrueAutoAttackRange(&Player, Enemy) < Player.Distance(Enemy) && Enemy->IsValid() && Enemy->IsAlive()) // if enemy location is further than player's aa range
					{
						Vector3 pos{ Enemy->GetPosition() }; // get enemy location

						if (pos.IsValid()) // if the location is valid 
						{

							auto minions_ptr // Get enemy minions within 1000 range to kite
							{
								pSDK->EntityManager->GetEnemyMinions(600.0f, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
							};

							for (auto &[netID, minions] : minions_ptr) // minion loop
							{

								if (minions->IsAlive() && pCore->Orbwalker->CanAttack() && (pCore->Orbwalker->GetTrueAutoAttackRange(&Player, minions) > Player.Distance(minions)))
								{
									pSDK->Control->Attack(minions); // attack the proper minions
								}

							}

						}
					}
				}
			}
		}




	}
}

void Kalista::Exploit()
{
	/*
	 * var target = TargetSelector.GetTarget(ObjectManager.Player.AttackRange+ObjectManager.Player.BoundingRadius+65,DamageType.Physical);
                    if (target.IsValidTarget())
                    {
                        if (Game.Time*1000 >= lastaa + 1)
                        {
                            Player.IssueOrder(GameObjectOrder.MoveTo, Game.CursorPos);
                        }
                        if (Game.Time*1000 > lastaa + ObjectManager.Player.AttackDelay*1000-180)
                        {
                            Player.IssueOrder(GameObjectOrder.AttackUnit, target);
                        }
                    }
                    else
                    {
                        Player.IssueOrder(GameObjectOrder.MoveTo, Game.CursorPos);
                    }
	 *
	 */


	auto target = pCore->TS->GetTarget(Player.GetTrueAttackRange()+100.0f, DamageType::Magical);

	if(target != NULL && target != nullptr)
	{
		
		float time = float(Game::Time() * 1000);

		if(time >= lastaa +1.0f)
		{
			auto mousepos{ Renderer::MousePos() };
			SdkMoveLocalPlayer(&mousepos, false);
			//pSDK->Control->Attack(&CurrentTarget);
			//SdkUiConsoleWrite("Move");
		}
		
		
		if(time > lastaa + Player.GetAttackDelay()*1000.0f - (float)(Menu::Get<int>("Kalista.ExploitAdjustment")))
		//if (time > lastaa + 350.0f )
		{
			//SdkAttackTargetLocalPlayer(OrbTarget, false);


			auto mousepos{ Renderer::MousePos() };
			//SdkMoveLocalPlayer(&mousepos, false);
			pSDK->Control->Attack(target);
			//SdkUiConsoleWrite("Attack");
			
		}

	}
	else
	{
		auto mousepos{ Renderer::MousePos() };
		SdkMoveLocalPlayer(&mousepos, false);
	}


	
}

void Kalista::SoulBoundSaver()
{

	/*
	 * Menu::Checkbox("Use R to Save Ally", "Kalista.miscR", true);

			Menu::SliderInt("^-> if Ally HP below # %", "Kalista.miscRHP", 20, 1, 100);
	 */

	
	if (!Menu::Get<bool>("Kalista.miscR") || !spellR.IsLearned())
	{
		return;
	}
	


	auto allies_ptr
	{
		pSDK->EntityManager->GetAllyHeroes(spellR.Range, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
	};


	if ( ((&Player)->GetResource().Current - rawFatesCallMana >= 30.0f && Menu::Get<bool>("Kalista.miscSaveManaE")) || !Menu::Get<bool>("Kalista.miscSaveManaE"))
	{
		for (auto &[netID, allies] : allies_ptr)
		{
			if (allies->HasBuff("kalistacoopstrikeally") && allies->CountEnemiesInRange(500.0f) >= 1)
			{
				Vector3 pos{ allies->GetPosition() };


				float targetHealth = (float)(Menu::Get<int>("Kalista.miscRHP"));
				if (allies->GetHealthPercent() <= targetHealth)
				{
					if (!allies->IsChannelingImportantSpell() && spellR.IsReady())
					{
						spellR.Cast();

						/*
						if (pos.IsValid() && pos.IsOnScreen()) {
							//We get the screen position and offset it a little so it doesnt draw over the above text
							Vector2 screenPos{ Renderer::WorldToScreen(pos) };
							screenPos.y -= 20.0f;

							Draw::Text(NULL, &screenPos, std::to_string(targetHealth), "Arial Narrow", &Color::White, 20, 6);
						}*/
					}
				}


			}
		}
	}


}


void Kalista::KillSteal()
{
	auto heroes_ptr
	{
		pSDK->EntityManager->GetEnemyHeroes(1500.0f, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
	};



	// collision I gotta add it later

	for (auto &[netID, heroes] : heroes_ptr)
	{
		if (heroes->IsValid() && spellQ.IsLearned() && spellQ.IsReady() && heroes->IsAlive() && Menu::Get<bool>("Kalista.KillStealQ"))
		{
			if(spellQ.GetSpellDamage(heroes) >= (heroes->GetHealth().Current + heroes->GetHealth().AllShield))
			{
				Vector3 playerPos{ (&Player)->GetServerPosition() };

				Vector3 targetPos{ (heroes)->GetPosition() };

				if (!pSDK->Collision->CheckCollisions(playerPos, targetPos, spellQ.Width, spellQ.Delay, spellQ.Speed, false, CollisionFlags::Minions | CollisionFlags::YasuoWall | CollisionFlags::BraumWall, false))
				{
					spellQ.Cast(&targetPos);
				}
			}
		}
		
		if(heroes->IsValid() && heroes->IsAlive() && heroes->HasBuff("kalistaexpungemarker", false) && Menu::Get<bool>("Kalista.KillStealE"))
		{
			if (IsRendKillable(heroes->AsAIBaseClient()) && !heroes->HasUndyingBuff())
			{
				if (spellE.IsInRange(heroes) && spellE.IsReady())
				{
					spellE.Cast();
				}
			}
		}
	
	}
}

void Kalista::AlwaysJungleE()
{
	auto jungle_ptr
	{
		pSDK->EntityManager->GetJungleMonsters(spellQ.Range, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
	};



	/*
	 *
	 * 		Menu::Checkbox("Always Use E on Dragon", "Kalista.JungleEDragon", true);
			Menu::Checkbox("Always Use E on Baron", "Kalista.JungleEBaron", true);
			Menu::Checkbox("Always Use E on Rift Herald", "Kalista.JungleERift", true);
			Menu::Checkbox("Always Use E on Blue", "Kalista.JungleEBlue", true);
			Menu::Checkbox("Always Use E on Red", "Kalista.JungleERed", true);
			Menu::Checkbox("Always Use E on Scuttler Crab", "Kalista.JungleECrab", true);
	 */

	for (auto &[netID, jungle] : jungle_ptr)
	{


		if (jungle->IsValid() && jungle->IsAlive() && jungle->GetCampNumber() != 0 && jungle != NULL && jungle != nullptr) // campnumber 0 is plant include all kind of plants
		{
			
			if (jungle->HasBuff("kalistaexpungemarker", false))
			{
				if (IsRendKillable(jungle->AsAIBaseClient()) && spellE.IsInRange(jungle) && spellE.IsReady())
				{


					std::string s1(jungle->GetName());

						if (strstr(s1.c_str(), "SRU_Dragon") && Menu::Get<bool>("Kalista.JungleEDragon") ||
							strstr(s1.c_str(), "SRU_Baron") && Menu::Get<bool>("Kalista.JungleEBaron") ||
							strstr(s1.c_str(), "SRU_RiftHerald") && Menu::Get<bool>("Kalista.JungleERift") ||
							strstr(s1.c_str(), "SRU_Blue") && Menu::Get<bool>("Kalista.JungleEBlue") ||
							strstr(s1.c_str(), "SRU_Red") && Menu::Get<bool>("Kalista.JungleERed") ||
							strstr(s1.c_str(), "Sru_Crab") && Menu::Get<bool>("Kalista.JungleECrab"))
						{
							spellE.Cast();
						}


				}
			}

		}
	}
}

void temp()
{
	Vector3 mousePos
	{
		Renderer::MousePos()
		//CurrentTarget.GetPosition()
	};


	//pSDK->EventHandler->DelayedAction(pSDK->Control->Move(&mousePos), 300);
	pSDK->Control->Move(&mousePos);

	pCore->Orbwalker->OrbwalkTo(mousePos);
}



void Kalista::SpellCastStart(void* AI, PSDK_SPELL_CAST SpellCast, void* UserData)
{
	/*
	SdkUiConsoleWrite("Game: %f", Game::Time());

	SdkUiConsoleWrite("Spell: %f", (float)pCore->Orbwalker->GetLastAutoAttack());

	SdkUiConsoleWrite("delay: %f", Player.GetAttackSpeed());

	*/
	//Player.GetAttackDelay()


	if(AI == NULL || AI == nullptr || SpellCast == NULL || SpellCast == nullptr)
	{
		return;
	}


	if(strcmp(SpellCast->Spell.Name, "KalistaMysticShot") == 0)
	{
		
		if (pSDK->EntityManager->GetObjectFromPTR(AI)->GetNetworkID() == Player.GetNetworkID()) 
		{

			if (pCore->Orbwalker->IsModeActive(OrbwalkingMode::Combo) || pCore->Orbwalker->IsModeActive(OrbwalkingMode::Mixed) || pCore->Orbwalker->IsModeActive(OrbwalkingMode::LaneClear) || pCore->Orbwalker->IsModeActive(OrbwalkingMode::JungleClear))
			{
				auto mousepos{ Renderer::MousePos() };
				SdkMoveLocalPlayer(&mousepos, false);
				pCore->Orbwalker->ResetAttackTimer();
			}
		}
		

	}
		
}

void Kalista::SpellCastEnd(void* AI, PSDK_SPELL_CAST SpellCast, void* UserData)
{
	return;
	//SdkUiConsoleWrite("2: %s", SpellCast->Spell.Name);

	/*

	auto hi = SpellCast->Spell.Name;

	if (strcmp(hi, "KalistaMysticShot") == 0)
	{

		Vector3 mousePos
		{
			//Renderer::MousePos()
			CurrentTarget.GetPosition()
		};


		//pSDK->EventHandler->DelayedAction(pSDK->Control->Move(&mousePos), 300);
		pSDK->Control->Move(&mousePos);

		pCore->Orbwalker->OrbwalkTo(mousePos);
		pCore->Orbwalker->ResetAttackTimer();


		//pCore->Orbwalker->OrbwalkTo(mousePos);
	}*/

}



///This gets called X times per second, where X is your league fps.
///Put any drawings you need here
void Kalista::Draw(void * UserData)
{


	/*
	auto jungle_ptr2
	{
		pSDK->EntityManager->GetJungleMonsters(spellQ.Range, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
	};

	for (auto &[netID, jungle] : jungle_ptr2)
	{


		if (jungle->IsValid() && jungle->IsAlive() && jungle != NULL && jungle != nullptr) // campnumber 0 is plant include all kind of plants
		{
			Vector3 pos{ jungle->GetPosition() };
			Draw::Text(&pos, NULL, jungle->GetName(), "Arial Narrow", &Color::Red, 20, 6);
		}
	}*/

	/*
	//CurrentTarget is a copy of an object, so we check if its still valid.
	if (pCore->TS->IsValidTarget(&CurrentTarget)) {
		Vector3 pos{ CurrentTarget.GetPosition() };
		
		if (pos.IsValid() && pos.IsOnScreen()) {
			//Draw::Text(&pos, NULL, "This is CurrentTarget", "Arial Narrow", &Color::Red, 20, 6);
		}


	}
	
	if (pCore->TS->IsValidTarget(OrbTarget)) {
		Vector3 pos{ OrbTarget->GetPosition() };
		
		if (pos.IsValid() && pos.IsOnScreen()) {
		
			//We get the screen position and offset it a little so it doesnt draw over the above text
			Vector2 screenPos{ Renderer::WorldToScreen(pos) };			
			screenPos.y += 20.0f;
		
			//Draw::Text(NULL, &screenPos, "This is OrbTarget", "Arial Narrow", &Color::White, 20, 6);
		}		
	}

	auto Enemies{ pSDK->EntityManager->GetEnemyHeroes() };
	for (auto &[netID, Enemy] : Enemies) {
		if (pCore->Orbwalker->GetTrueAutoAttackRange(&Player, Enemy) > Player.Distance(Enemy)) {
			Vector3 pos{ Enemy->GetPosition() };

			if (pos.IsValid() && pos.IsOnScreen()) {
				//We get the screen position and offset it a little so it doesnt draw over the above text
				Vector2 screenPos{ Renderer::WorldToScreen(pos) };
				screenPos.y -= 20.0f;

				//Draw::Text(NULL, &screenPos, "In AA Range", "Arial Narrow", &Color::White, 20, 6);

				
			}
		}
	}*/



	SDKVECTOR PlayerLocation = Player.GetPosition();
	SDKVECTOR direction{ 100.f, 100.f, 100.f };

	if (PlayerLocation.IsValid() )
	{
		if (spellQ.IsLearned() && spellQ.IsReady() && Menu::Get<bool>("Kalista.drawQ"))
		{
			Draw::Circle(&PlayerLocation, spellQ.Range, &Color::White, 2, &direction);
		}

		if (spellW.IsLearned() && spellW.IsReady() && Menu::Get<bool>("Kalista.drawW"))
		{
			Draw::Circle(&PlayerLocation, spellW.Range, &Color::Purple, 2, &direction);
		}

		if (spellE.IsLearned() && spellE.IsReady() && Menu::Get<bool>("Kalista.drawE"))
		{
			Draw::Circle(&PlayerLocation, spellE.Range, &Color::Green, 2, &direction);
		}

		if (spellR.IsLearned() && spellR.IsReady() && Menu::Get<bool>("Kalista.drawR"))
		{
			Draw::Circle(&PlayerLocation, spellR.Range, &Color::DarkBlue, 2, &direction);
		}

		
	}


	/*

	auto minions_ptr
	{
		pSDK->EntityManager->GetEnemyMinions(4000.0f, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
	};

	for (auto &[netID, minions] : minions_ptr)
	{
		if (minions->IsAlive() && minions->HasBuff("kalistaexpungemarker", false) && (minions->IsLaneMinion() || minions->IsSiegeMinion()))
		{
			Vector3 pos{ minions->GetPosition() };
			//kalistaexpungemarker

			auto hi2 = pSDK->BuffManager->GetBuffStacks(netID, "kalistaexpungemarker", false);

			//if()

			Vector3 posHP{ minions->GetHealthBarScreenPos() };





			float dmg = GetRendDamage(minions, hi2);


			auto health = minions->GetHealth();

			float totalHealth = health.Current + health.AllShield;


			float calc = (dmg / totalHealth) * 100.0f;


		//	for (auto &yop : hi)
			{
				if (pos.IsValid() && pos.IsOnScreen())
				{
					//We get the screen position and offset it a little so it doesnt draw over the above text
					Vector2 screenPos{ Renderer::WorldToScreen(pos) };
					screenPos.y -= 20.0f;

					Draw::Text(NULL, &screenPos, std::to_string(calc), "Arial Narrow", &Color::White, 20, 6);
				}
			}

		}
	}
	*/
	if (Menu::Get<bool>("Kalista.drawEDmgPctJungle"))
	{
		auto jungle_ptr
		{
			pSDK->EntityManager->GetJungleMonsters(4000.0f, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
		};


		for (auto &[netID, jungle] : jungle_ptr)
		{
			//if (pCore->Orbwalker->GetTrueAutoAttackRange(&Player, Enemy) > Player.Distance(Enemy)) 
			if (jungle->IsAlive() && jungle->HasBuff("kalistaexpungemarker", false))
			{




				float calc = RendDamageToHealth(jungle, false);

				Vector2 posHP{ jungle->GetHealthBarScreenPos() };

				if (posHP.IsValid())
				{


					std::stringstream ss;
					ss.precision(0); //for decimal
					ss.setf(std::ios_base::fixed, std::ios_base::floatfield);

					ss << calc << "%";

					posHP.x += 25;
					posHP.y += 10;

					Draw::Text(NULL, &posHP, ss.str().c_str(), "Calibri Bold", &Color::White, 24, 8);


				}


			}
		}
	}





	if (Menu::Get<bool>("Kalista.drawEDmgHPBar") || Menu::Get<bool>("Kalista.drawEDmgPctHero"))
	{
		auto heroes_ptr
		{
			pSDK->EntityManager->GetEnemyHeroes(3000.0f, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
		};

		for (auto &[netID, heroes] : heroes_ptr)
		{


			if (heroes->IsAlive() && heroes->HasBuff("kalistaexpungemarker", false))
			{
				if (Menu::Get<bool>("Kalista.drawEDmgHPBar")) //Kalista.drawEDmgHPBar
				{
					heroes->DrawDamageOnHP(RendDamageToHealth(heroes, true));
				}

				if (Menu::Get<bool>("Kalista.drawEDmgPctHero"))
				{
					float calc = RendDamageToHealth(heroes, false);

					Vector2 posHP{ heroes->GetHealthBarScreenPos() };

					if (posHP.IsValid())
					{


						std::stringstream ss;
						ss.precision(0); //for decimal
						ss.setf(std::ios_base::fixed, std::ios_base::floatfield);

						ss << calc << "%";

						posHP.x += 25;
						posHP.y += 10;

						Draw::Text(NULL, &posHP, ss.str().c_str(), "Calibri Bold", &Color::White, 24, 8);


					}
				}


			}
		}
	}


}

float GetRendDamage(AIBaseClient* target, int StackCount)
{
	if(StackCount <= 0) // no need to calculate if there is no rend stack
	{
		return 0;
	}
	else if (!target->IsValid() || target == nullptr || target == NULL)
	{
		return 0;
	}


	float rawRendDamage [] = { 20, 30, 40, 50, 60 };
	float rawRendDamageMultiplier [] = { 0.6f, 0.6f, 0.6f, 0.6f, 0.6f };
	float rawRendDamagePerSpear [] = { 5, 9, 14, 25, 32 };
	float rawRendDamagePerSpearMultiplier [] = { 0.2f, 0.2375f, 0.275f, 0.315f, 0.35f };


	// 10, 14, 19, 

	int CurrentELevel = Player.GetSpell((char)SpellSlot::E).Level -1;
	float attackDmg = Player.GetAttackDamage();


	float rawDmg = (rawRendDamage[CurrentELevel] + rawRendDamageMultiplier[CurrentELevel] * attackDmg) + // Base damage
		(static_cast<float>(StackCount-1) * (rawRendDamagePerSpear[CurrentELevel] + rawRendDamagePerSpearMultiplier[CurrentELevel] * attackDmg)); // Damage per spear

	auto done = pSDK->DamageLib->CalculatePhysicalDamage(Player.AsAIBaseClient(), target, rawDmg);



	std::string s1(target->GetName());

	if (strstr(s1.c_str(), "SRU_Dragon") || strstr(s1.c_str(), "SRU_Baron") || strstr(s1.c_str(), "SRU_RiftHerald")) //SRU_RiftHerald
	{
		done = done / 2.0f;
	}

	//Player.getattack
	//SdkUiConsoleWrite("1: %f", (rawRendDamage[CurrentELevel] + (rawRendDamageMultiplier[CurrentELevel] * attackDmg)));
	//SdkUiConsoleWrite("2: %d", StackCount);
	//SdkUiConsoleWrite("3: %f", (rawRendDamagePerSpear[CurrentELevel] + (rawRendDamagePerSpearMultiplier[CurrentELevel] * attackDmg)));


	return done;

}


///Your menu settings go here
void Kalista::DrawMenu(void * UserData) 
{
	

	UNREFERENCED_PARAMETER(UserData);

	bool bKalistaExpanded = true;
	Menu::Tree("Kalista", "Kalista.Main", &bKalistaExpanded, []()
	{
		//SdkUiText("MataView 1.1 [Beta] - E2Slayer");
		SdkUiColoredText(&Color::Green, "Kalista 1.1 (Beta)");

		bool bComboExpanded = false;
		Menu::Tree("Combo", "Kalista.Combo", &bComboExpanded, []()
		{
			
			Menu::Checkbox("Use Q", "Kalista.ComboQ", true);

			Menu::DropList("Q HitChance", "Kalista.ComboQHit", hitchanceList, 2);

			

			Menu::Checkbox("Use E", "Kalista.ComboE", true);

			Menu::Checkbox("Reset E when minion is Killable and Enemy has spear(s)", "Kalista.ComboEMinion", true);


			Menu::Checkbox("Chasing Enemy by Using Minions", "Kalista.ComboChase", true);
			//Menu::SliderInt("Chase Range", "Kalista.ComboChaseRange", 1500, 1000, 2000);
			//Menu::Checkbox("Use E on Minions While Chasing", "Kalista.ComboE", true);


		});


		bool bHarassExpanded = false;
		Menu::Tree("Harass", "Kalista.Harass", &bHarassExpanded, []()
		{

			Menu::Checkbox("Use Q", "Kalista.HarassQ", true);

			Menu::DropList("Q HitChance", "Kalista.HarassQHit", hitchanceList, 2);

			Menu::SliderInt("Q Minimum Mana", "Kalista.HarassQMana", 40, 0, 100);

			


			Menu::Checkbox("Use E", "Kalista.HarassE", true);

			Menu::Checkbox("Reset E when minion is Killable and Enemy has spear(s)", "Kalista.HarassEMinion", true);
	
			/*
			Menu::Checkbox("Use E when Enemy is leaving E Range", "Kalista.HarassEleaving", true);

			Menu::SliderInt("^-> Minimum Spear to use", "Kalista.ComboChase", 3, 1, 10);
			*/

		});

		bool bLaneClearExpanded = false;
		Menu::Tree("LaneClear", "Kalista.LaneClear", &bLaneClearExpanded, []()
		{

			//Menu::Checkbox("Use Q", "Kalista.LaneClearQ", true);
			SdkUiText("LaneClear Q will be added when Collision is implemented in SDK");
			Menu::Checkbox("Use E", "Kalista.LaneClearE", true);
			Menu::SliderInt("^-> Minimum Killable Minions ", "Kalista.LaneClearEMinimum", 2, 1, 10);


		});



		bool bJungleClearExpanded = false;
		Menu::Tree("JungleClear", "Kalista.JungleClear", &bJungleClearExpanded, []()
		{

			Menu::Checkbox("Use Q", "Kalista.JungleClearQ", true);
			Menu::SliderInt("^-> Minimum Mana", "Kalista.JungleClearQMana", 40, 0, 100);

			Menu::Checkbox("Use E", "Kalista.JungleClearE", true);



		});


		bool bJungleEExpanded = false;
		Menu::Tree("Always E on Jungle Mobs", "Kalista.JungleE", &bJungleEExpanded, []()
		{
			Menu::Checkbox("Always Use E on Dragon", "Kalista.JungleEDragon", true);
			Menu::Checkbox("Always Use E on Baron", "Kalista.JungleEBaron", true);
			Menu::Checkbox("Always Use E on Rift Herald", "Kalista.JungleERift", true);
			Menu::Checkbox("Always Use E on Blue", "Kalista.JungleEBlue", true);
			Menu::Checkbox("Always Use E on Red", "Kalista.JungleERed", true);
			Menu::Checkbox("Always Use E on Scuttler Crab", "Kalista.JungleECrab", true);

		
		});



		bool bSettingsExpanded = false;
		Menu::Tree("Misc", "Kalista.misc", &bSettingsExpanded, []()
		{
			//Menu::Checkbox("Use ", "Kalista.MiscE", true);


			Menu::Checkbox("Use R to Save Ally", "Kalista.miscR", true);

			Menu::SliderInt("^-> if Ally HP below # %", "Kalista.miscRHP", 20, 1, 100);

			Menu::Checkbox("Auto E when you can't kill a minion with AA", "Kalista.miscEMinion", true);

			Menu::Checkbox("Always Save Mana For E", "Kalista.miscSaveManaE", true);


			Menu::Checkbox("Use Fly Exploit", "Kalista.Exploit", true);

			Menu::SliderInt("^-> Fly Adjustment", "Kalista.ExploitAdjustment", 150, 100, 250);
			SdkUiColoredText(&Color::Purple, "^-> Higher number = Faster jumps but More Buggy jumps");
			SdkUiColoredText(&Color::Purple, "^-> Ideal Range is around 150-180");




			//Menu::Checkbox("Use E", "Kalista.JungleClearE", true);



		//	Menu::SliderInt("Font Size", "Ability.FontSize", 28, 21, 50); //&_g_CheckTeamAbilities.iFontSize, 21.0f, 50.0f, "%.0f", NULL);
		//	SDKVECTOR defaultColor = { 255, 255, 255 };
		//	Menu::ColorPicker("Color Picker", "Ability.Color", defaultColor);

			//MakeMenu::ColorPicker()
			//SdkUiColorPicker("Color Pick", &_g_CheckTeamAbilities.m_CurrentColor, NULL);
			//SdkUiText("Note: The settings work after pressed the Save Settings");
			//SdkUiColoredText(&_g_ColorGreen, "Note: The settings will apply \nAfter pressed the Save Settings");


		});

		bool bKillStealExpanded = false;
		Menu::Tree("Misc", "Kalista.KillSteal", &bKillStealExpanded, []()
		{
			Menu::Checkbox("Use Q to Killsteal", "Kalista.KillStealQ", true);
			Menu::Checkbox("Use E to Killsteal", "Kalista.KillStealE", true);
		});




		bool bDrawExpanded = false;
		Menu::Tree("Drawings", "Kalista.draw", &bDrawExpanded, []()
		{
			//Menu::Checkbox("Use ", "Kalista.MiscE", true);


			Menu::Checkbox("Draw Q Range", "Kalista.drawQ", true);
			Menu::Checkbox("Draw W Range", "Kalista.drawW", true);
			Menu::Checkbox("Draw E Range", "Kalista.drawE", true);
			Menu::Checkbox("Draw R Range", "Kalista.drawR", true);

			Menu::Checkbox("Show E Damage on Champion - Percentage", "Kalista.drawEDmgPctHero", true);
			Menu::Checkbox("Show E Damage on Jungle Mobs- Percentage", "Kalista.drawEDmgPctJungle", true);



			Menu::Checkbox("Show E Damage - Draw on HPbar (Only Works Champions)", "Kalista.drawEDmgHPBar", true);

			//Menu::Checkbox("Draw R Range", "Kalista.drawR", true);



			//Menu::DropList("Draw E Damage", "Kalista.drawEDMG", )

			//MakeMenu::ColorPicker()
			//SdkUiColorPicker("Color Pick", &_g_CheckTeamAbilities.m_CurrentColor, NULL);
			//SdkUiText("Note: The settings work after pressed the Save Settings");
			//SdkUiColoredText(&_g_ColorGreen, "Note: The settings will apply \nAfter pressed the Save Settings");


		});




	});




}




int NumberOfRendBuff(AIBaseClient* target)
{
	return pSDK->BuffManager->GetBuffCount(target->GetNetworkID(), "kalistaexpungemarker", false);
}

float RendDamageToHealth(AIBaseClient* target, bool rawDamage)
{

	auto hi2 = pSDK->BuffManager->GetBuffStacks(target->GetNetworkID(), "kalistaexpungemarker", false);

	float dmg = GetRendDamage(target, hi2);

	if(rawDamage)
	{
		return dmg;
	}


	auto health = target->GetHealth();

	float totalHealth = health.Current + health.AllShield;

	float calc = ((dmg / totalHealth) * 100.0f);
	return calc;
}


bool IsRendKillable(AIBaseClient* target)
{
	/*
	auto id = pSDK->BuffManager->GetBuffCount(netID, "kalistaexpungemarker", false);

	float dmg = GetRendDamage(target, id);

	float totalHealth = target->GetHealth().Current + target->GetHealth().AllShield;

	float calc = (dmg / totalHealth) * 100.0f;

	*/




	auto hi2 = pSDK->BuffManager->GetBuffStacks(target->GetNetworkID(), "kalistaexpungemarker", false);

	float dmg = GetRendDamage(target, hi2);

	auto health = target->GetHealth();

	float totalHealth = health.Current + health.AllShield;

	float calc = ((dmg / totalHealth) * 100.0f);

	//SdkUiConsoleWrite("Im attacking: %s", target->GetName());
	//SdkUiConsoleWrite("2: %f", dmg);
	//SdkUiConsoleWrite("3: %f", calc);
	return calc >= 100.0f;
}

HitChance GetHitChanceFromDropList(int list)
{
	//int hi = Menu::Get<int>("Kalista.ComboQHit");

	
	//std::vector<std::string> hitchanceList{ "Low", "Medium", "High", "Very High" };
	switch(list)
	{
	case 0:
		return HitChance::Low;
	case 1:
		return HitChance::Medium;
	case 2:
		return HitChance::High;
	case 3:
		return HitChance::VeryHigh;
	default:
		
		break;
	}

}
