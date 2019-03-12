#include "stdafx.h"
#include "Kalista.h"
#include "KalistaSpells.h"
#include "KalistaDamage.h"
#include "KalistaMenu.h"

#include <numeric>
#include <iostream>
#include <map>
#include <string>
#include <utility>


//#include "KalistaWallJump.h"

//AttackableUnit  Kalista::CurrentTarget;
//AttackableUnit* Kalista::OrbTarget;

static float lastaa = 0.0f;

//auto spellQ = Spell::Skillshot(SpellSlot::Q, 1150.0f, SkillshotType::Line, 0.25f, 2100.0f, 40.0f, DamageType::Physical, true, CollisionFlags::YasuoWall | CollisionFlags::Minions | CollisionFlags::BraumWall );
//auto spellW = Spell::Active(SpellSlot::W, 5000.0f, DamageType::Physical);
//auto spellE = Spell::Active(SpellSlot::E, 1000.0f, DamageType::Physical);
//auto spellR = Spell::Active(SpellSlot::R, 1200.0f, DamageType::Physical);


//std::vector<std::string> ColorMenuList{ "Black", "Blue", "Cyan", "DarkBlue" };


//std::map<float, float> IncDamage;
std::map<float, float> InstDamage;




///This gets called once, at load
void Kalista::Init()
{
	//SdkUiConsoleWrite("c2 %s", Player.GetCharName());

	if(strcmp(Player.GetCharName(), "Kalista") != 0)
	{
		return; //since player is not playing Kalista
	}
	lastaa = 0.0f;
	//IncDamage.clear();
	InstDamage.clear();
	//CurrentTarget = AttackableUnit();
	//OrbTarget = NULL;


	KalistaMenu::Init();
	//WallJump::WallJumpSpotsInitializer();

#pragma region RegisterCallbacks
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick,	Kalista::Tick);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Update,	Kalista::Update);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, Kalista::DrawMenu);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, Kalista::Draw);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::SpellCastStart, Kalista::SpellCastStart);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::SpellCastEnd, Kalista::SpellCastEnd);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Attack, Kalista::Attack);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::UnkillableMinion, Kalista::UnkillableMinion);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Dash, Kalista::Dash);

#pragma endregion
}


void Kalista::Dash(AIHeroClient* Source, PSDKVECTOR StartPos, PSDKVECTOR EndPos, unsigned int StartTick, unsigned int Duration, float Speed)
{
	//SdkUiConsoleWrite("Dash");
	//SdkUiConsoleWrite("Start x %f y %f z %f", StartPos->x, StartPos->y, StartPos->z);
	//SdkUiConsoleWrite("End x %f y %f z %f", EndPos->x, EndPos->y, EndPos->z);


}


/*
void Kalista::SpellCastEnd(void* AI, PSDK_SPELL_CAST SpellCast, void* UserData)
{

}*/


void Kalista::Attack(void* AI, void* TargetObject, bool StartAttack, bool StopAttack, void* UserData)
{
	if (AI == NULL || AI == nullptr || TargetObject == NULL || TargetObject == nullptr)
	{
		return;
	}
	
	if (pSDK->EntityManager->GetObjectFromPTR(AI) == NULL || pSDK->EntityManager->GetObjectFromPTR(AI) == nullptr)
	{
		return;
	}

	auto sender{ (AIHeroClient*)pSDK->EntityManager->GetObjectFromPTR(AI) };


	if (sender->GetNetworkID()== NULL || sender->GetNetworkID() == 0)
	{
		return;
	}

	auto AIObjectNetworkID = sender->GetNetworkID();

	if (Player.GetNetworkID() == NULL || Player.GetNetworkID() == 0)
	{
		return;
	}
	
	auto PlayerNetworkID = Player.GetNetworkID();
	

	if (AIObjectNetworkID == PlayerNetworkID)
	{
		lastaa = float(Game::Time()*1000);
	}
	

	/*

	if (pSDK->EntityManager->GetObjectFromPTR(TargetObject) == NULL || pSDK->EntityManager->GetObjectFromPTR(TargetObject) == nullptr)
	{
		return;
	}
	AIBaseClient targetObject = pSDK->EntityManager->GetObjectFromPTR(TargetObject);

	if (pSDK->EntityManager->GetObjectFromPTR(TargetObject)->GetNetworkID() == NULL || pSDK->EntityManager->GetObjectFromPTR(TargetObject)->GetNetworkID() == 0)
	{
		return;
	}
	//auto targetObjectNetworkID = pSDK->EntityManager->GetObjectFromPTR(TargetObject)->GetNetworkID();
	*/
	


	//auto d2 = pSDK->DamageLib->GetAutoAttackDamage(hi2, hi, false);
	//SdkUiConsoleWrite("Helaht: %f Dmg: %f", Player.GetHealth().Current, d2);



	if (Menu::Get<bool>("Kalista.EBeforeDie"))
	{
		auto target{ (AIHeroClient*)pSDK->EntityManager->GetObjectFromPTR(TargetObject) };

		if (target != nullptr && target != NULL)
		{

			if (target->GetNetworkID() == PlayerNetworkID)
			{
				//auto sender{ (AIHeroClient*)pSDK->EntityManager->GetObjectFromPTR(AI) };

				if (sender != nullptr && sender != NULL && sender->IsHero())
				{

					float time = Game::Time();
					auto aaDmg = pSDK->DamageLib->GetAutoAttackDamage(sender, target, false);
					if (time > 0.0f && aaDmg > 0.0f)
					{

						//SdkUiConsoleWrite("are you here2 %f %f", aaDmg, time);
						InstDamage.emplace(time, aaDmg);
					}
				}
			}
		}
	}
	
}

void Kalista::UnkillableMinion(AIMinionClient* Target)
{
	/*
	if (Menu::Get<bool>("Kalista.miscEMinion") && spellE.IsReady())
	{
		if(Target->IsValid() && Target->IsAlive() && Damage::IsRendKillable(Target->AsAIBaseClient()))
		{
			spellE.Cast();
		}
		
	}*/

	
}



///This gets called 30 times per second, put your logics in here instead of
///on OnUpdate so it doesnt drop too many fps
void Kalista::Tick(void * UserData)
{

	/*
	auto target_ptr
	{ 
		pCore->TS->GetTarget(1500.0f, DamageType::Magical) 
	};


	if (target_ptr) 
	{
		CurrentTarget = *(target_ptr);
	}
	*/

	SoulBoundSaver();

	KillSteal();

	AlwaysJungleE();

	Spells::SpellWUse(OrbwalkingMode::None);
	
	UseEBeforeDeath();
	//Geometry::BestCoveringRectangle

}

///This gets called X times per second, where X is your league fps.
///Use this only to update stuff you need to validade on every frame.
void Kalista::Update(void * UserData)
{
	//OrbTarget = pCore->Orbwalker->GetTarget();

	
	if (pCore->Orbwalker->IsModeActive(OrbwalkingMode::Combo)) // I will get you
	{
		//Exploit();
		if (Menu::Get<bool>("Kalista.ComboChase"))
		{
			Chase();
		}

		//int hi = Menu::Get<int>("Kalista.ComboQHit");

		//SdkUiConsoleWrite("ayy I'm 1.7 - %d", hi); 

		ExploitExecute(OrbwalkingMode::Combo);
		Combo();
	}

	if (pCore->Orbwalker->IsModeActive(OrbwalkingMode::Mixed))
	{
		//Combo(); //change it to harass later at this point no need to use harass function
		Harass();

	}

	if (pCore->Orbwalker->IsModeActive(OrbwalkingMode::LaneClear))
	{
		LaneClear();
		ExploitExecute(OrbwalkingMode::LaneClear);
	}

	if (pCore->Orbwalker->IsModeActive(OrbwalkingMode::JungleClear))
	{
		JungleClear();
		ExploitExecute(OrbwalkingMode::JungleClear);
	}

	/* walljump
	if (pCore->Orbwalker->IsModeActive(OrbwalkingMode::Flee))
	{
		Spells::SpellQUse(OrbwalkingMode::Flee);
	}*/


}

void Kalista::UseEBeforeDeath()
{
	if (InstDamage.empty() || !Menu::Get<bool>("Kalista.EBeforeDie"))
	{
		return;
	}

	float time = Game::Time();
	if (time <= 0)
	{
		return;
	}

	for (auto it = InstDamage.begin(); it != InstDamage.end(); )
	{
		if (it->first + Menu::Get<float>("Kalista.EBeforeDieIncDuration") < time)
		{
			it = InstDamage.erase(it);
		}
		else
		{
			++it;

		}
	}

	/*
					Menu::SliderInt("^-> Minimum Health to Cast E", "Kalista.EBeforeDieMinimumHealth", 10, 1, 100);

				Menu::SliderFloat("^-> Incoming Damage Calculation Duration (Second)", "Kalista.EBeforeDieIncDuration", 1.0f, 0.1f, 2.0f);
	*/
	float sum = std::accumulate(InstDamage.begin(), InstDamage.end(), 0,
		[](float acc, std::pair<float, float> p) { return (acc + p.second); });

	if (Player.IsAlive() && sum != 0 && Player.CountEnemiesInRange(1500.0f) > 0)
	{
		if ( (((Player.GetHealth().Current + Player.GetHealth().AllShield) - sum) <= 0.0f) || (Player.GetHealthPercent() <= (float)Menu::Get<int>("Kalista.EBeforeDieMinimumHealth")))
		{
			Spells::SpellEUseSimple();
		}
	}
}

void Kalista::Combo()
{



	Spells::SpellQUse(OrbwalkingMode::Combo);
	Spells::SpellEUse(OrbwalkingMode::Combo);
	//delay, speed, width 
	/*
	auto heroes_ptr
	{
		pSDK->EntityManager->GetEnemyHeroes(4000.0f, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
	};
	*/

	//std::map<unsigned, AIBaseClient*> targetStore{ pSDK->EntityManager->GetEnemyHeroes(4000.0f, &pSDK->EntityManager->GetLocalPlayer().GetPosition()) };
	// collision I gotta add it later


	

	//Exploit(heroes_ptr);


	/*
	auto target = pCore->TS->GetTarget(Player.GetTrueAttackRange() + 100.0f, DamageType::Magical);
	Exploit(target);



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
	*/

	/*
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
	*/
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


	/*
	auto heroes_ptr
	{
		pSDK->EntityManager->GetEnemyHeroes(4000.0f, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
	};
	*/


	// collision I gotta add it later
	Spells::SpellQUse(OrbwalkingMode::Mixed);
	Spells::SpellEUse(OrbwalkingMode::Mixed);

	/*
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
	*/


}

void Kalista::LaneClear()
{

	Spells::SpellEUse(OrbwalkingMode::LaneClear);
	/*
	auto minions_ptr
	{
		pSDK->EntityManager->GetEnemyMinions(1500.0f, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
	};


	int hitNumber = Menu::Get<int>("Kalista.LaneClearEMinimum");
	int killableNum = 0;
	for (auto &[netID, minions] : minions_ptr)
	{
		
		if(minions->IsAlive() && minions->IsValid() && minions != NULL && minions != nullptr)
		{
			Exploit(minions);
		}


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
	}*/
}

void Kalista::JungleClear()
{
	Spells::SpellQUse(OrbwalkingMode::JungleClear);
	Spells::SpellEUse(OrbwalkingMode::JungleClear);

	/*
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

		if (jungle->IsAlive() && jungle->IsValid() && jungle != NULL && jungle != nullptr && jungle->GetCampNumber() != 0)
		{
			Exploit(jungle);
		}

		Spells::SpellQUse(OrbwalkingMode::JungleClear);

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
	}*/

}

void Kalista::Chase()
{
	if (pCore->Orbwalker->IsModeActive(OrbwalkingMode::Combo) ) // When combo is active
	{
		if(Player.CountEnemiesInRange(2000.0f) <= 0 || !Player.CanAttack() || !Player.CanMove())
		{
			return;
		}

		if (!pSDK->EntityManager->GetLocalPlayer().GetPosition().IsValid())
		{
			return;
		}

		auto minions_ptr // Get enemy minions within 1000 range to kite
		{
			pSDK->EntityManager->GetEnemyMinions(Player.GetTrueAttackRange(), &pSDK->EntityManager->GetLocalPlayer().GetPosition())
		};

		if (minions_ptr.empty())
		{
			return;
		}

		auto Enemies{ pSDK->EntityManager->GetEnemyHeroes(2000.0f, &pSDK->EntityManager->GetLocalPlayer().GetPosition()) }; // Get enemies

		if (Enemies.empty())
		{
			return;
		}
		
		for (auto &[netID, Enemy] : Enemies) // enemy loop
		{
			if (Enemy != nullptr && Enemy != NULL)
			{
				if (Enemy->IsRunningFrom(&Player) || Player.GetLevel() >= 6)
				{
					if (pCore->Orbwalker->GetTrueAutoAttackRange(&Player, Enemy) < Player.Distance(Enemy) && Enemy->IsValid() && Enemy->IsAlive() && pCore->TS->IsValidTarget(Enemy)) // if enemy location is further than player's aa range
					{
						Vector3 pos{ Enemy->GetPosition() }; // get enemy location
						if (pos != nullptr && pos != NULL)
						{
							if (pos.IsValid()) // if the location is valid 
							{

							

								for (auto &[netID, minions] : minions_ptr) // minion loop
								{
									if (minions->IsValid() && minions != nullptr && minions != NULL)
									{
										if (minions->IsAlive() && minions->IsTargetable() && pCore->Orbwalker->CanAttack() && (pCore->Orbwalker->GetTrueAutoAttackRange(&Player, minions) > Player.Distance(minions)))
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




	}
}


void Kalista::ExploitExecute(const OrbwalkingMode mode)
{
	if (Menu::Get<bool>("Kalista.Exploit") || Player.GetAttackSpeed() < 2.87f)
	{
		return;
	}

	if (mode == OrbwalkingMode::Combo)
	{
		
			
			if (!pSDK->EntityManager->GetLocalPlayer().GetPosition().IsValid())
			{
				return;
			}


			const auto target = pCore->TS->GetTarget(Player.GetTrueAttackRange());

			if (target != nullptr && target != NULL)
			{
				if (target->IsAlive() && target->IsVisible())
				{
					Exploit(target);
				}
			}
		
	}
	else if (mode == OrbwalkingMode::LaneClear)
	{
		
			if (!pSDK->EntityManager->GetLocalPlayer().GetPosition().IsValid())
			{
				return;
			}

			auto minions_ptr
			{
				pSDK->EntityManager->GetEnemyMinions(Player.GetTrueAttackRange(), &pSDK->EntityManager->GetLocalPlayer().GetPosition())
			};

			if (minions_ptr.empty())
			{
				return;
			}


			for (auto &[netID, minions] : minions_ptr)
			{
				if (minions != NULL && minions != nullptr)
				{
					if (minions->IsLaneMinion() || minions->IsSiegeMinion() || minions->IsSuperMinion())
					{
						if (minions->IsAlive() && minions->IsVisible() && minions->IsTargetable() )
						{
							Exploit(minions);
						}
					}
				}
			}
		
	}
	else if (mode == OrbwalkingMode::JungleClear && Menu::Get<bool>("Kalista.JungleClearE"))
	{
	
		if (!pSDK->EntityManager->GetLocalPlayer().GetPosition().IsValid())
		{
			return;
		}

		auto jungle_ptr
		{
			pSDK->EntityManager->GetJungleMonsters(Player.GetTrueAttackRange(), &pSDK->EntityManager->GetLocalPlayer().GetPosition())
		};

		if (jungle_ptr.empty())
		{
			return;
		}

		for (auto &[netID, jungle] : jungle_ptr)
		{
			if (jungle != NULL && jungle != nullptr)
			{
				if (jungle->IsAlive() && jungle->IsVisible() && jungle->GetCampNumber() != 0 && jungle->IsTargetable()) // campnumber 0 is plant include all kind of plants
				{
					Exploit(jungle);
				}
			}
		}
		

	}
}

void Kalista::Exploit(AIBaseClient* target)
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

	if(pCore->Orbwalker->IsModeActive(OrbwalkingMode::None) || !Menu::Get<bool>("Kalista.Exploit") || Player.GetAttackSpeed() < 2.87f) //2.88 is AttackSpeed 2.0
	{
		return;
	}



		if (target != NULL && target != nullptr )
		{
			if (pCore->Orbwalker->GetTrueAutoAttackRange(&Player, target) >= Player.Distance(target))
			{
				float time = float(Game::Time() * 1000);

				if (time < 0.0f && lastaa < 0.0f ) //&& lastaa - time > 10000.0f
				{
					return;
				}

				if (time >= lastaa + 1.0f)
				{
					auto mousepos{ Renderer::MousePos() };
					if (mousepos != nullptr && mousepos != NULL)
					{
						SdkMoveLocalPlayer(&mousepos, false);
					}
					//pSDK->Control->Attack(&CurrentTarget);
					//SdkUiConsoleWrite("Move");
				}


				if (time > lastaa + Player.GetAttackDelay()*1000.0f - (float)(Menu::Get<int>("Kalista.ExploitAdjustment")))
				{
					//SdkAttackTargetLocalPlayer(OrbTarget, false);


					//auto mousepos{ Renderer::MousePos() };
					//SdkMoveLocalPlayer(&mousepos, false);
					pSDK->Control->Attack(target);
					//SdkUiConsoleWrite("Attack");

				}
			}

		}
		else
		{
			auto mousepos{ Renderer::MousePos() };
			if (mousepos != nullptr && mousepos != NULL)
			{
				SdkMoveLocalPlayer(&mousepos, false);
			}
		}
		


	

	//auto target = pCore->TS->GetTarget(Player.GetTrueAttackRange()+100.0f, DamageType::Magical);

	


	
}

void Kalista::SoulBoundSaver()
{
	Spells::SpellRUse(OrbwalkingMode::None);
}


void Kalista::KillSteal()
{
	Spells::SpellQUse(OrbwalkingMode::None);
	Spells::SpellEUse(OrbwalkingMode::None);
}

void Kalista::AlwaysJungleE()
{
	Spells::SpellEUseSub(OrbwalkingMode::None);
}


void Kalista::SpellCastStart(void* AI, PSDK_SPELL_CAST SpellCast, void* UserData)
{

	//Vector3(8222.000000f, 51.648384f, 3158.000000f);
	//SdkUiConsoleWrite("Vector3(%ff, %ff, %ff)", SpellCast->StartPosition.x , SpellCast->StartPosition.y, SpellCast->StartPosition.z);
	//SdkUiConsoleWrite("End x %f y %f z %f", SpellCast->x, SpellCast->y, SpellCast->z);

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



	if (SpellCast->IsAutoAttack || SpellCast->Spell.Name == NULL || SpellCast->Spell.Name == nullptr)
	{
		return;
	}

	if (pSDK->EntityManager->GetObjectFromPTR(AI) == NULL || pSDK->EntityManager->GetObjectFromPTR(AI) == nullptr)
	{
		return;
	}

	auto sender{ (AIHeroClient*)pSDK->EntityManager->GetObjectFromPTR(AI) };

	if (sender->GetNetworkID() == NULL || sender->GetNetworkID() == 0)
	{
		return;
	}

	auto AINetworkID = sender->GetNetworkID();

	if (Player.GetNetworkID() == NULL || Player.GetNetworkID() == 0)
	{
		return;
	}


	if(strcmp(SpellCast->Spell.Name, "KalistaMysticShot") == 0 && (AINetworkID == Player.GetNetworkID()))
	{
		//SdkUiConsoleWrite("pos0 = Vector3(%ff, %ff, %ff);", SpellCast->StartPosition.x, SpellCast->StartPosition.y, SpellCast->StartPosition.z);
		if (pCore->Orbwalker->IsModeActive(OrbwalkingMode::Combo) || pCore->Orbwalker->IsModeActive(OrbwalkingMode::Mixed) || pCore->Orbwalker->IsModeActive(OrbwalkingMode::LaneClear) || pCore->Orbwalker->IsModeActive(OrbwalkingMode::JungleClear))
		{
			auto mousepos{ Renderer::MousePos() };
			if (mousepos != nullptr && mousepos != NULL)
			{
				SdkMoveLocalPlayer(&mousepos, false);
			}
			pCore->Orbwalker->ResetAttackTimer();
		}
	}
	

	if (Menu::Get<bool>("Kalista.EBeforeDie"))
	{
		auto target{ (AIHeroClient*)pSDK->EntityManager->GetObjectFromPTR(SpellCast->TargetObject) };

		if (target != nullptr && target != NULL)
		{

			if (target->GetNetworkID() == Player.GetNetworkID())
			{
				if (sender != nullptr && sender != NULL)
				{
					auto slot = SpellCast->Spell.Slot;

					if (slot >= 0 && slot < 4)
					{
						auto spellDmg = pSDK->DamageLib->GetSpellDamage(sender, target, SpellCast->Spell.Slot, SkillStage::Default);

						//float hp = Player.GetHealth().Current - d2;

						//SdkUiConsoleWrite("inc dmg : !! %f", d2);

						//SdkUiConsoleWrite("hp after : !! %f", hp);
						if (spellDmg != NULL && spellDmg != 0)
						{
							//SdkUiConsoleWrite("ScriptName: %s Slot: %d Inc Dmg %f", SpellCast->Spell.ScriptName, SpellCast->Spell.Slot, d2);
							InstDamage.emplace(Game::Time(), spellDmg);
						}
					}
				}
			}
		}
	}
	
		
}


SDKCOLOR Kalista::GetColor(int selectedCombo)
{
	/*
	{Color::Black, "Black"}, { Color::Blue, "Blue" }, { Color::Cyan, "Cyan" }, { Color::DarkBlue, "DarkBlue" },
	{ Color::DarkGreen, "DarkGreen" }, { Color::Green, "Green" }, { Color::Grey, "Grey" }, { Color::Magenta, "Magenta" },
	{ Color::Orange, "Orange" }, { Color::Purple, "Purple" }, { Color::Red, "Red" }, { Color::White, "White" }, { Color::Yellow, "Yellow" }
	*/


	//int list = Menu::Get<int>(getHitChance);

	if (selectedCombo == NULL)
	{
		return Color::White; //just in case, the fucntion fails.
	}

	switch (selectedCombo)
	{
		case 0:
			return Color::Black;
		case 1:
			return Color::Blue;
		case 2:
			return Color::Cyan;
		case 3:
			return Color::DarkBlue;
		case 4:
			return Color::DarkGreen;
		case 5:
			return Color::Green;
		case 6:
			return Color::Grey;
		case 7:
			return Color::Magenta;
		case 8:
			return Color::Orange;
		case 9:
			return Color::Purple;
		case 10:
			return Color::Red;
		case 11:
			return Color::White;
		case 12:
			return Color::Yellow;
		default:
			return Color::White;
	}

	return Color::White;
}


///This gets called X times per second, where X is your league fps.
///Put any drawings you need here
void Kalista::Draw(void * UserData)
{


	/*
	const auto target = pCore->TS->GetTarget(2000.0f);

	if (target != NULL && target != nullptr)
	{
		auto buffs
		{
			target->GetBuffs()
		};

		Vector3 pos{ target->GetPosition() };
		Vector2 screenPos{ Renderer::WorldToScreen(pos) };
		for (auto buffname : buffs)
		{




			if (pos.IsValid() && pos.IsOnScreen())
			{
				//We get the screen position and offset it a little so it doesnt draw over the above text
				//Vector2 screenPos{ Renderer::WorldToScreen(pos) };
				screenPos.y -= 20.0f;

				Draw::Text(NULL, &screenPos, buffname.Name, "Arial Narrow", &Color::White, 20, 6);


			}

		}
	}*/


	/*

	
		Vector3 pos{ Player.GetPosition() };
		
		//for (auto buffname : buffs)
		{




			if (pos.IsValid() && pos.IsOnScreen())
			{
				//We get the screen position and offset it a little so it doesnt draw over the above text
				//Vector2 screenPos{ Renderer::WorldToScreen(pos) };
				Vector2 screenPos{ Renderer::WorldToScreen(pos) };
				screenPos.y -= 20.0f;


				Draw::Text(NULL, &screenPos, std::to_string(InstDamage.size()), "Arial Narrow", &Color::White, 20, 6);

				//screenPos.y -= 20.0f;


				//Draw::Text(NULL, &screenPos, std::to_string(IncDamage.size()), "Arial Narrow", &Color::Green, 20, 6);




				//if (Player.GetHealthPercent() <= 10)
				{
					screenPos.y -= 20.0f;
					Draw::Text(NULL, &screenPos, std::to_string(Player.GetHealthPercent()), "Arial Narrow", &Color::Red, 20, 6);
				}


				
				Draw::Text(NULL, &screenPos, std::to_string(pos.x), "Arial Narrow", &Color::White, 20, 6);
				screenPos.y -= 20.0f;

				Draw::Text(NULL, &screenPos, std::to_string(pos.y), "Arial Narrow", &Color::Green, 20, 6);
				screenPos.y -= 20.0f;

				Draw::Text(NULL, &screenPos, std::to_string(pos.z), "Arial Narrow", &Color::Red, 20, 6);

				
			}

		}
	*/


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

		/*
	auto jumping = WallJump::GetWallJumpSpots();

	for (auto &[start, end] : jumping)
	{
		
		//float t1 = (float)(Menu::Get<int>("Kalista.t1"));
		//float t2 = (float)(Menu::Get<int>("Kalista.t2"));


		float t1 = 306.625f;
		float t2 = 120.977f;



	//Vector3 startPos{ start.x + t1, start.z, start.y + t2 };
	//	Vector3 endPos{ end.x+t1, end.z, end.y + t2 };


		Vector3 startPos{ start.x , start.y, start.z };
		Vector3 endPos{ end.x , end.y, end.z  };
		//Vector3 endPos{ end };
		SDKVECTOR direction{ 100.f, 100.f, 100.f };

		//if (Player.Distance(&startPos) < 10000.0f)
		{
			//Vector3 pos{ Enemy->GetPosition() };

			if (startPos.IsValid() && startPos.IsOnScreen())
			{
				//We get the screen position and offset it a little so it doesnt draw over the above text
				//Vector2 screenPos{ Renderer::WorldToScreen(startPos) };
				//screenPos.y -= 20.0f;

				//Draw::Text(NULL, &screenPos, "In AA Range", "Arial Narrow", &Color::White, 20, 6);

				Draw::Circle(&startPos, 100.0f, &Color::White, 2, &direction);
			}
			

			if (endPos.IsValid() && endPos.IsOnScreen())
			{
				//We get the screen position and offset it a little so it doesnt draw over the above text
				//Vector2 screenPos{ Renderer::WorldToScreen(startPos) };
				//screenPos.y -= 20.0f;

				//Draw::Text(NULL, &screenPos, "In AA Range", "Arial Narrow", &Color::White, 20, 6);

				Draw::Circle(&endPos, 100.0f, &Color::Red, 2, &direction);
			}

		}
	}



	*/





	SDKVECTOR PlayerLocation = Player.GetPosition();
	SDKVECTOR direction{ 100.f, 100.f, 100.f };

	if (PlayerLocation != NULL || PlayerLocation != nullptr)
	{
		
	
		//Menu::Get<int>("drawRColor");
		if (PlayerLocation.IsValid())
		{
			if (Menu::Get<bool>("Kalista.drawQ"))
			{
				auto spellQ = Spells::GetSpell(SpellSlot::Q);

				if (spellQ.IsLearned() && spellQ.IsReady() && PlayerLocation.IsOnScreen())
				{
					Draw::Circle(&PlayerLocation, spellQ.Range, &GetColor(Menu::Get<int>("Kalista.drawQColor")), 2, &direction);
				}

			}

			//

			if (Menu::Get<bool>("Kalista.drawW"))
			{
				auto spellW = Spells::GetSpell(SpellSlot::W);

				if (spellW.IsLearned() && spellW.IsReady())
				{
					Draw::Circle(&PlayerLocation, spellW.Range, &GetColor(Menu::Get<int>("Kalista.drawWColor")), 2, &direction);
				}

			}

			if (Menu::Get<bool>("Kalista.drawE"))
			{
				auto spellE = Spells::GetSpell(SpellSlot::E);

				if (spellE.IsLearned() && spellE.IsReady() && PlayerLocation.IsOnScreen())
				{
					Draw::Circle(&PlayerLocation, spellE.Range, &GetColor(Menu::Get<int>("Kalista.drawEColor")), 2, &direction);
				}

			}

			if (Menu::Get<bool>("Kalista.drawR"))
			{
				auto spellR = Spells::GetSpell(SpellSlot::R);

				if (spellR.IsLearned() && spellR.IsReady() && PlayerLocation.IsOnScreen())
				{
					Draw::Circle(&PlayerLocation, spellR.Range, &GetColor(Menu::Get<int>("Kalista.drawRColor")), 2, &direction);
				}

			}
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


		if (pSDK->EntityManager->GetLocalPlayer().GetPosition().IsValid())
		{



			auto jungle_ptr
			{
				pSDK->EntityManager->GetJungleMonsters(1500.0f, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
			};

			if (!jungle_ptr.empty())
			{



				for (auto &[netID, jungle] : jungle_ptr)
				{
					if (jungle != NULL && jungle != nullptr)
					{
						if (jungle->IsAlive() && jungle->IsVisible() && jungle->GetCampNumber() != 0) // campnumber 0 is plant include all kind of plants
						{
							if (jungle->HasBuff("kalistaexpungemarker", false))
							{
								float calc = Damage::RendDamageToHealth(jungle, false);

								if (jungle->GetHealthBarScreenPos() != NULL || jungle->GetHealthBarScreenPos() != nullptr || calc >= 0.0f)
								{



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
					}
				}
			}
		}
	}





	if (Menu::Get<bool>("Kalista.drawEDmgHPBar") || Menu::Get<bool>("Kalista.drawEDmgPctHero"))
	{

		if (pSDK->EntityManager->GetLocalPlayer().GetPosition().IsValid())
		{


			auto heroes_ptr
			{
				pSDK->EntityManager->GetEnemyHeroes(3000.0f, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
			};

			if (!heroes_ptr.empty())
			{



				for (auto &[netID, heroes] : heroes_ptr)
				{
					if (heroes != nullptr && heroes != NULL)
					{
						//if (heroes->IsAlive() && heroes->IsVisible() && heroes->HasBuff("kalistaexpungemarker", false))
						if (heroes->HasBuff("kalistaexpungemarker", false))
						{
							if (Menu::Get<bool>("Kalista.drawEDmgHPBar")) //Kalista.drawEDmgHPBar
							{
								if (Damage::RendDamageToHealth(heroes, true) >= 0.0f)
								{
									heroes->DrawDamageOnHP(Damage::RendDamageToHealth(heroes, true));
								}
							}

							if (Menu::Get<bool>("Kalista.drawEDmgPctHero"))
							{
								float calc = Damage::RendDamageToHealth(heroes, false);


								if (heroes->GetHealthBarScreenPos() != NULL || heroes->GetHealthBarScreenPos() != nullptr)
								{



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
			}
		}
	}


}








