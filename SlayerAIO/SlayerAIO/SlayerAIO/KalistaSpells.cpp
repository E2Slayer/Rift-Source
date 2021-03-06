#include "stdafx.h"
#include "KalistaSpells.h"
#include "KalistaDamage.h"
//#include "KalistaWallJump.h"

//using namespace Kalista;


Spell::Skillshot spellQ = Spell::Skillshot(SpellSlot::Q, 1150.0f, SkillshotType::Line, 0.25f, 2100.0f, 40.0f, DamageType::Physical, true, CollisionFlags::YasuoWall | CollisionFlags::Minions | CollisionFlags::BraumWall);
Spell::Targeted spellW = Spell::Targeted(SpellSlot::W, 5000.0f, DamageType::Physical);
Spell::Active spellE = Spell::Active(SpellSlot::E, 1000.0f, DamageType::Physical);
Spell::Active spellR = Spell::Active(SpellSlot::R, 1200.0f, DamageType::Physical);

const float rawPierceMana[] = { 50.0f , 55.0f , 60.0f , 65.0f , 70.0f }; //Spell Q Mana
const float rawSentinelMana = 25.0f; //Spell w Mana
const float rawRendMana = 30.0f; //Spell e Mana
const float rawFatesCallMana = 100.0f; //Spell R Mana


static float lastTimeCasted[] = { 0.0f, 0.0f, 0.0f, 0.0f };


//const Vector3 drakePosition = Vector3(9836.401367f, -71.240601f, 4404.578125f);

//const Vector3 baronPosition = Vector3(4974.679199f, -71.240601f, 10430.420898f);



std::map<int, Vector3> sentinelSpots = { {0, Vector3(9836.401367f, -71.240601f, 4404.578125f)}, {1, Vector3(4974.679199f, -71.240601f, 10430.420898f)} }; //0 is drake, 1 is baron


Spell::SpellBase Spells::GetSpell(SpellSlot slot)
{
	switch (slot)
	{
		case SpellSlot::Q:
			return spellQ;
		case SpellSlot::W:
			return spellW;
		case SpellSlot::E:
			return spellE;
		case SpellSlot::R:
			return spellR;

	}

}


void Spells::SpellQUse(const OrbwalkingMode mode)
{
		if (!SpellHumanizer(0)) // Delay to stop spamming
		{
			return;
		}

		if (mode == OrbwalkingMode::Combo && Menu::Get<bool>("Kalista.ComboQ") || mode == OrbwalkingMode::Mixed && Menu::Get<bool>("Kalista.HarassQ"))
		{
			//if (Menu::Get<bool>("Kalista.miscBlockJumpingQ") && Player.IsDashing() || !Menu::Get<bool>("Kalista.miscBlockJumpingQ"))


				if (spellQ.IsLearned() && spellQ.IsReady() && SpellEManaSaver(SpellSlot::Q))
				{
					if (!pSDK->EntityManager->GetLocalPlayer().GetPosition().IsValid())
					{
						return;
					}
					
						const auto target = pCore->TS->GetTarget(spellQ.Range);

						if (target != nullptr && target != NULL)
						{
							auto prediction{ spellQ.GetPrediction(target) };

							if (prediction != nullptr) // && prediction->CollisionResult != nullptr)
							{
								if (prediction->CollisionResult != nullptr)
								{
									if (target->IsAlive())
									{
										if (prediction->Hitchance >= GetHitChanceFromDropList(mode) && prediction->CastPosition.IsValid())
										{
											spellQ.Cast(&prediction->CastPosition);
											lastTimeCasted[0] = float(Game::Time() * 1000);
										}
									}
								}


							}
						}
					

				}
			
		}
		else if (mode == OrbwalkingMode::JungleClear && Menu::Get<bool>("Kalista.JungleClearQ"))
		{
		

			if (spellQ.IsLearned() && spellQ.IsReady() && SpellEManaSaver(SpellSlot::Q))
			{
				if (!pSDK->EntityManager->GetLocalPlayer().GetPosition().IsValid())
				{
					return;
				}
				auto jungle_ptr
				{
					pSDK->EntityManager->GetJungleMonsters(spellQ.Range, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
				};


				if (jungle_ptr.empty())
				{
					return;
				}

			
					for (auto &[netID, jungle] : jungle_ptr)
					{
						if (jungle != nullptr && jungle != NULL) // campnumber 0 is plant include all kind of plants
						{
							if (jungle->IsAlive() && jungle->GetCampNumber() != 0 && jungle->IsVisible() && jungle->IsTargetable())
							{
								if ((float)Menu::Get<int>("Kalista.JungleClearQMana") <= Player.GetManaPercent())
								{
									if (Menu::Get<bool>("Kalista.JungleClearQ") && spellQ.IsInRange(jungle) && jungle->IsVisible() && Player.IsFacing(jungle))
									{
										Vector3 junglePos{ jungle->GetServerPosition() };
										if (junglePos.IsValid()) // Q usage
										{
											spellQ.Cast(&junglePos);
											lastTimeCasted[0] = float(Game::Time() * 1000);
										}
									}
								}
							}

						}
					}
				
			}
		}
		else if (mode == OrbwalkingMode::None && Menu::Get<bool>("Kalista.KillStealQ"))
		{

			if (spellQ.IsLearned() && spellQ.IsReady() && SpellEManaSaver(SpellSlot::Q))
			{
				if (!pSDK->EntityManager->GetLocalPlayer().GetPosition().IsValid())
				{
					return;
				}
				auto heroes_ptr
				{
					pSDK->EntityManager->GetEnemyHeroes(spellE.Range, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
				};

				if (heroes_ptr.empty())
				{
					return;
				}

				for (auto &[netID, heroes] : heroes_ptr)
				{
					if (heroes != nullptr && heroes != NULL)
					{
						if (pCore->TS->IsValidTarget(heroes) && heroes->Distance(&Player) > Player.GetTrueAttackRange(heroes))
						{
							auto enemyHealth = heroes->GetHealth().Current + heroes->GetHealth().AllShield;

							enemyHealth -= Damage::GetPierceDamage(heroes);

							if (enemyHealth <= 0)
							{
								auto prediction{ spellQ.GetPrediction(heroes) };

								if (prediction != nullptr) // && prediction->CollisionResult != nullptr)
								{
									if (prediction->CollisionResult != nullptr)
									{
										if (heroes->IsAlive() && heroes->IsVisible())
										{
											if (prediction->Hitchance >= HitChance::Medium && prediction->CastPosition.IsValid())
											{
												spellQ.Cast(&prediction->CastPosition);
												lastTimeCasted[0] = float(Game::Time() * 1000);
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

		/*
		else if (mode == OrbwalkingMode::Flee && Menu::Get<bool>("Kalista.WallJump"))
		{
			pCore->Orbwalker->DisableMovement(false);
			auto jumping = WallJump::GetWallJumpSpots();

			if (jumping.empty())
			{
				return;
			}

			for (auto &[start, end] : jumping)
			{
				//pCore->Orbwalker->DisableMovement(true);
				float t1 = (float)(Menu::Get<int>("Kalista.t1"));
				float t2 = (float)(Menu::Get<int>("Kalista.t2"));
				//Vector3 startPos{ start.x + t1, start.z, start.y + t2 };
				//Vector3 endPos{ end.x + t1, end.z, end.y + t2 };

				Vector3 startPos{ start.x , start.y, start.z };
				Vector3 endPos{ end.x , end.y, end.z };
				SDKVECTOR direction{ 100.f, 100.f, 100.f };



				auto mousepos{ Renderer::MousePos() };
				if (mousepos != nullptr && mousepos != NULL)
				{
					//SdkMoveLocalPlayer(&endPos, false);
				}

				if (Player.Distance(&startPos) <= 100.0f)
				{
					pCore->Orbwalker->DisableMovement(true);
					SdkMoveLocalPlayer(&startPos, false);
					if (startPos.IsValid() && startPos.IsOnScreen() && Player.Distance(&startPos) <= 25.0f)
					{
						//SdkMoveLocalPlayer(&startPos, false);
						//Vector3 pos{ Enemy->GetPosition() };

							//We get the screen position and offset it a little so it doesnt draw over the above text
							//Vector2 screenPos{ Renderer::WorldToScreen(startPos) };
							//screenPos.y -= 20.0f;

							//Draw::Text(NULL, &screenPos, "In AA Range", "Arial Narrow", &Color::White, 20, 6);

							//Draw::Circle(&startPos, 100.0f, &Color::White, 2, &direction);

						//SdkMoveMouse(&endPos);
						spellQ.Cast(&endPos);

						//auto mousepos{ Renderer::MousePos() };
						//if (mousepos != nullptr && mousepos != NULL)
						//{
						SdkMoveLocalPlayer(&endPos, false);
						//}
						pCore->Orbwalker->ResetAttackTimer();
						
					}


				}
				else if (Player.Distance(&endPos) <= 100.0f )
				{
					pCore->Orbwalker->DisableMovement(true);
					SdkMoveLocalPlayer(&endPos, false);
					if ((Player.Distance(&endPos) <= 25.0f && endPos.IsValid() && endPos.IsOnScreen()))
					{

						//SdkMoveMouse(&startPos);
						spellQ.Cast(&startPos);

						//auto mousepos{ Renderer::MousePos() };
						//if (mousepos != nullptr && mousepos != NULL)
						//{
						SdkMoveLocalPlayer(&startPos, false);
						//}
						pCore->Orbwalker->ResetAttackTimer();
						
					}
				}
			}

		}*/
	
}

void Spells::SpellWUse(const OrbwalkingMode mode)
{
	if (!SpellHumanizer(1) || !spellW.IsLearned() || !SpellEManaSaver(SpellSlot::W)) // Delay to stop spamming
	{
		return;
	}

	if (Player.CountEnemiesInRange(3000.0f) >= 1 || sentinelSpots.empty())
	{
		return;
	}



	/*
	
		Menu::Checkbox("Auto W to Dragon", "Kalista.autoWDrake", true);
			Menu::Checkbox("Auto W to Baron", "Kalista.autoWBaron", true);	*/


	// lastTimeCasted[0] = float(Game::Time() * 1000);

	if (mode == OrbwalkingMode::None && spellW.IsReady())
	{
		int randomazier = rand() % 1 + 100;

		if (randomazier == 0)
		{
			return;
		}

		for (auto &[id, location] : sentinelSpots)
		{
			if (id == 0 && Menu::Get<bool>("Kalista.autoWDrake") || id == 1 && Menu::Get<bool>("Kalista.autoWBaron"))
			{
				if (location != NULL && location != nullptr)
				{
					if (spellW.IsInRange(&location) && location.IsValid())
					{
						Vector3 randomaziedDrakePos{ location.x + (float)randomazier, location.y, location.z + (float)randomazier };
						if (randomaziedDrakePos != NULL && randomaziedDrakePos != nullptr)
						{
							if (randomaziedDrakePos.IsValid())
							{
								spellW.Cast(&location);
								lastTimeCasted[1] = float(Game::Time() * 1000);
								pCore->Orbwalker->ResetAttackTimer();
							}
						}
					}
				}
			}
		}


	}


}

void Spells::SpellEUse(const OrbwalkingMode mode)
{
	if (!SpellHumanizer(2)) // Delay to stop spamming
	{
		return;
	}

	if (mode == OrbwalkingMode::Combo && Menu::Get<bool>("Kalista.ComboE") || mode == OrbwalkingMode::Mixed && Menu::Get<bool>("Kalista.HarassE"))
	{
		if (spellE.IsLearned() && spellE.IsReady())
		{

			if (!pSDK->EntityManager->GetLocalPlayer().GetPosition().IsValid())
			{
				return;
			}
			auto heroes_ptr
			{
				pSDK->EntityManager->GetEnemyHeroes(spellE.Range, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
			};

			if (heroes_ptr.empty())
			{
				return;
			}

			for (auto &[netID, heroes] : heroes_ptr)
			{

				if (heroes != nullptr && heroes != NULL)
				{
					if (heroes->IsAlive() && heroes->IsVisible() && heroes->HasBuff("kalistaexpungemarker", false))
					{
						if (Damage::IsRendKillable(heroes->AsAIBaseClient()) && !heroes->HasUndyingBuff())
						{
							if (spellE.IsInRange(heroes))
							{
								spellE.Cast();
								lastTimeCasted[2] = float(Game::Time() * 1000);
							}
						}

						if (mode == OrbwalkingMode::Combo && Menu::Get<bool>("Kalista.ComboEMinion") || mode == OrbwalkingMode::Mixed && Menu::Get<bool>("Kalista.HarassEMinion"))
						{
							auto minions_ptr // Get enemy minions within 1000 range to kite
							{
								pSDK->EntityManager->GetEnemyMinions(Player.GetTrueAttackRange(), &pSDK->EntityManager->GetLocalPlayer().GetPosition())
							};

							if (minions_ptr.empty())
							{
								return;
							}

							for (auto &[netID, minions] : minions_ptr) // minion loop
							{
								if (minions != nullptr && minions != NULL)
								{
									if (minions->IsLaneMinion() || minions->IsSiegeMinion() || minions->IsSuperMinion())
									{
										if (minions->IsAlive() && minions->IsVisible() && minions->HasBuff("kalistaexpungemarker", false))
										{
											if (Damage::IsRendKillable(minions->AsAIBaseClient()))
											{
												if (spellE.IsInRange(minions) && spellE.IsReady())
												{
													spellE.Cast();
													lastTimeCasted[2] = float(Game::Time() * 1000);
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
	else if (mode == OrbwalkingMode::LaneClear && Menu::Get<bool>("Kalista.LaneClearE"))
	{
		if (spellE.IsLearned() && spellE.IsReady())
		{
			if (!pSDK->EntityManager->GetLocalPlayer().GetPosition().IsValid())
			{
				return;
			}

			auto minions_ptr
			{
				pSDK->EntityManager->GetEnemyMinions(spellE.Range, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
			};

			if (minions_ptr.empty())
			{
				return;
			}

			int hitNumber = Menu::Get<int>("Kalista.LaneClearEMinimum");

			if (hitNumber <= 0 || hitNumber > 10)
			{
				return;
			}

			int killableNum = 0;

			for (auto &[netID, minions] : minions_ptr)
			{
				if (minions != NULL && minions != nullptr)
				{
					/*
					if (minions->IsAlive())
					{
						Exploit(minions);
					}
					*/


					if (minions->IsLaneMinion() || minions->IsSiegeMinion() || minions->IsSuperMinion())
					{
						if (minions->IsAlive() && minions->IsVisible() && minions->HasBuff("kalistaexpungemarker", false))
						{
							if (Damage::IsRendKillable(minions->AsAIBaseClient()))
							{
								if (spellE.IsInRange(minions))
								{
									killableNum++;
									if (killableNum >= hitNumber)
									{
										spellE.Cast();
										lastTimeCasted[2] = float(Game::Time() * 1000);
									
									}
									else if (minions->GetHealth().Current <= Player.GetAttackDamage() && !Player.CanAttack() && Menu::Get<bool>("Kalista.miscEMinion"))
									{
										spellE.Cast();
										lastTimeCasted[2] = float(Game::Time() * 1000);
									}
									else if (minions->IsSiegeMinion() && Menu::Get<bool>("Kalista.LaneClearTank"))
									{
										spellE.Cast();
										lastTimeCasted[2] = float(Game::Time() * 1000);
									}
								
								}
								
								
							}
						
						}
					}
					
				}
			}
		}
	}
	else if (mode == OrbwalkingMode::JungleClear && Menu::Get<bool>("Kalista.JungleClearE"))
	{
		if (spellE.IsLearned() && spellE.IsReady())
		{
			if (!pSDK->EntityManager->GetLocalPlayer().GetPosition().IsValid())
			{
				return;
			}

			auto jungle_ptr
			{
				pSDK->EntityManager->GetJungleMonsters(spellE.Range, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
			};

			if (jungle_ptr.empty())
			{
				return;
			}

			for (auto &[netID, jungle] : jungle_ptr)
			{
				/*
				if (jungle->IsAlive() && jungle->IsValid() && jungle != NULL && jungle != nullptr && jungle->GetCampNumber() != 0)
				{
					Exploit(jungle);
				}
				*/
				if (jungle != NULL && jungle != nullptr)
				{
					if (jungle->IsAlive() && jungle->IsVisible() && jungle->GetCampNumber() != 0) // campnumber 0 is plant include all kind of plants
					{
						if (jungle->HasBuff("kalistaexpungemarker", false))
						{
							if (Damage::IsRendKillable(jungle->AsAIBaseClient()))
							{
								if (spellE.IsInRange(jungle))
								{
									spellE.Cast();
									lastTimeCasted[2] = float(Game::Time() * 1000);
								}
							}
						}

					}
				}
			}
		}

	}
	else if (mode == OrbwalkingMode::None && Menu::Get<bool>("Kalista.KillStealE"))
	{
		if (spellE.IsLearned() && spellE.IsReady())
		{

			if (!pSDK->EntityManager->GetLocalPlayer().GetPosition().IsValid())
			{
				return;
			}
			auto heroes_ptr
			{
				pSDK->EntityManager->GetEnemyHeroes(1500.0f, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
			};

			if (heroes_ptr.empty())
			{
				return;
			}

			for (auto &[netID, heroes] : heroes_ptr)
			{

				if (heroes != nullptr && heroes != NULL)
				{
					if (heroes->IsAlive() && heroes->IsVisible() && heroes->HasBuff("kalistaexpungemarker", false))
					{
						if (Damage::IsRendKillable(heroes->AsAIBaseClient()) && !heroes->HasUndyingBuff())
						{
							if (spellE.IsInRange(heroes))
							{
								spellE.Cast();
								lastTimeCasted[2] = float(Game::Time() * 1000);
							}
						}
					}
				}
			}
		}

	}
	
	

}

void Spells::SpellEUseSub(const OrbwalkingMode mode)
{
	if (!SpellHumanizer(2)) // Delay to stop spamming
	{
		return;
	}

	if (mode == OrbwalkingMode::None)
	{
		if (!Menu::Get<bool>("Kalista.JungleEDragon") && !Menu::Get<bool>("Kalista.JungleEBaron") && !Menu::Get<bool>("Kalista.JungleERift") &&
			!Menu::Get<bool>("Kalista.JungleEBlue") && !Menu::Get<bool>("Kalista.JungleERed") && !Menu::Get<bool>("Kalista.JungleECrab"))
		{
			return;
		}


		if (spellE.IsLearned() && spellE.IsReady())
		{
			if (!pSDK->EntityManager->GetLocalPlayer().GetPosition().IsValid())
			{
				return;
			}

			auto jungle_ptr
			{
				pSDK->EntityManager->GetJungleMonsters(spellE.Range, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
			};

			if (jungle_ptr.empty())
			{
				return;
			}

			for (auto &[netID, jungle] : jungle_ptr)
			{
				if (jungle != NULL && jungle != nullptr)
				{
					if (jungle->IsAlive() && jungle->IsVisible() && jungle->GetCampNumber() != 0) // campnumber 0 is plant include all kind of plants
					{
						if (jungle->HasBuff("kalistaexpungemarker", false))
						{
							if (Damage::IsRendKillable(jungle->AsAIBaseClient()))
							{
								if (spellE.IsInRange(jungle))
								{
									if (jungle->GetName() != NULL || jungle->GetName() != nullptr)
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
											lastTimeCasted[2] = float(Game::Time() * 1000);
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
	else if (mode == OrbwalkingMode::Custom)
	{
	}
}


void Spells::SpellEUseSimple()
{
	if (!SpellHumanizer(2)) // Delay to stop spamming
	{
		return;
	}

	if (spellE.IsLearned() && spellE.IsReady())
	{
		spellE.Cast();
		lastTimeCasted[2] = float(Game::Time() * 1000);
	}
}

void Spells::SpellRUse(const OrbwalkingMode mode)
{

	if (!SpellHumanizer(3)) // Delay to stop spamming
	{
		return;
	}

	if (Menu::Get<bool>("Kalista.miscR"))
	{
		if (!pSDK->EntityManager->GetLocalPlayer().GetPosition().IsValid() || !SpellEManaSaver(SpellSlot::R))
		{
			return;
		}

		auto allies_ptr
		{
			pSDK->EntityManager->GetAllyHeroes(spellR.Range, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
		};

		if (allies_ptr.empty())
		{
			return;
		}



		for (auto &[netID, allies] : allies_ptr)
		{
			if (allies != nullptr && allies != NULL)
			{
				if (allies->HasBuff("kalistacoopstrikeally") && allies->CountEnemiesInRange(700.0f) >= 1 && allies->IsAlive())
				{

					float targetHealth = (float)(Menu::Get<int>("Kalista.miscRHP"));
					if (allies->GetHealthPercent() <= targetHealth)
					{
						if (!allies->IsChannelingImportantSpell() && !allies->IsDashing() && spellR.IsReady())
						{
							spellR.Cast();
							lastTimeCasted[3] = float(Game::Time() * 1000);
						}
					}
				}
			}
		}
	}
	


}


bool Spells::SpellHumanizer(int TargetSpell)
{
	if (float(Game::Time() * 1000) == NULL || float(Game::Time() * 100) <= 0)
	{
		return false;
	}

	if (lastTimeCasted[TargetSpell] + 100.0f >= float(Game::Time() * 1000)) // Delay to stop spamming
	{
		return false;
	}

	return true;

}

bool Spells::SpellEManaSaver(SpellSlot slot)
{
	float manaCalc = 0.0f;

	if (slot == SpellSlot::Q)
	{
		manaCalc = rawPierceMana[Player.GetSpell((char)SpellSlot::Q).Level - 1];
	}
	else if (slot == SpellSlot::W)
	{
		manaCalc = rawSentinelMana;
	}
	else if (slot == SpellSlot::R)
	{
		manaCalc = rawFatesCallMana;
	}




	if ((&Player)->GetResource().Current - manaCalc >= rawRendMana && Menu::Get<bool>("Kalista.miscSaveManaE")) // 30.0f mana of Skill E 
	{
		return true;
	}
	else if (!Menu::Get<bool>("Kalista.miscSaveManaE"))
	{
		return true;
	}

	return false;
}

HitChance Spells::GetHitChanceFromDropList(const OrbwalkingMode mode)
{
	if (mode == OrbwalkingMode::None)
	{
		return HitChance::High;
	}

	const char* orbName = GetOrbWalkerNameToText("Kalista", mode);
	if (strcmp(orbName, "Error"))
	{
		return HitChance::High;
	}

	std::stringstream ss;
	ss << orbName << "QHit";
	std::string getHitChance = ss.str();

	int list = Menu::Get<int>(getHitChance);

	if (list == NULL)
	{
		return HitChance::High; //just in case, the fucntion fails.
	}

	switch (list)
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
			return HitChance::High;
			break;
	}

}

const char* Spells::GetOrbWalkerNameToText(const char* champName, const OrbwalkingMode mode)
{
	if (champName == nullptr)
	{
		return "Error";
	}

	std::string tempText = ".None";
	if (mode == OrbwalkingMode::Combo)
	{
		tempText = ".Combo";
	}
	else if (mode == OrbwalkingMode::Mixed)
	{
		tempText = ".Harass";
	}
	else if (mode == OrbwalkingMode::LaneClear)
	{
		tempText = ".LaneClear";
	}
	else if (mode == OrbwalkingMode::JungleClear)
	{
		tempText = ".JungleClear";
	}

	return (champName + tempText).c_str();
}