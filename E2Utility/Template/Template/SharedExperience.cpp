#include "stdafx.h"
#include "SharedExperience.h"


const float meleeEXP = 58.879882f;
const float rangedEXP = 29.439941f;
const float tankEXP = 92.00f;

std::vector<MinionObject> MinionsObjects;


std::vector<ChampionEXPObject> ChampionEXPObjects;





void SharedExperience::InitLoader()
{
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick, Drawings::Tick);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, Drawings::DrawMenu);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::DeleteObject, SharedExperience::DeleteObject);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, SharedExperience::Draw);


	/*
	auto minions{ pSDK->EntityManager->GetEnemyMinions() };


	if (!minions.empty())
	{

		//Drawings.GankAlerter.Ally.Jungler
		for (auto &[netID, mini] : minions)
		{
			if (mini != nullptr && mini != NULL)
			{

				if (!mini->IsAlive())
				{

					SdkUiConsoleWrite("dead added %f", mini->GetDeathDuration());
				}

				MinionObject temp;
				temp.Minion = mini;
				temp.netID = netID;
				MinionsObjects.emplace_back(temp);
			}
		}
	}*/


	ChampionEXPObjects.clear();



	auto enemy1{ pSDK->EntityManager->GetEnemyHeroes() };

	if (enemy1.empty())
	{
		return;
	}

	/*
	if (!ally1.empty())
	{

		//Drawings.GankAlerter.Ally.Jungler
		for (auto &[netID, hero] : ally1)
		{
			if (hero != nullptr && hero != NULL)
			{
				//if (hero->GetNetworkID() != Player.GetNetworkID())
				{

					ChampionEXPObject temp;
					temp.Hero = hero;
					temp.Distance = 0;
					temp.LastTrigger = 0;
					temp.isAlly = true;
					temp.LastExp = 0;


					temp.SiegeExp = 0;
					temp.LastSiegeExp = 0;

					temp.RangedExp = 0;
					temp.LastRangedExp = 0;

					temp.MeleeExp = 0;
					temp.LastMeleeExp = 0;
					temp.NearByHeroes = 0;
		
					ChampionEXPObjects.emplace_back(temp);
				}
			}
		}
	}
	//SummonerSmite - Smite
	*/
	if (!enemy1.empty())
	{
		for (auto &[netID, hero] : enemy1)
		{
			if (hero != nullptr && hero != NULL)
			{
				//if (netID != Player.GetNetworkID())
				{
					//SdkUiConsoleWrite("enemy added %s", hero->GetCharName());
					ChampionEXPObject temp;
					temp.Hero = hero;
					temp.Distance = 0;
					temp.LastTrigger = 0;
					temp.isAlly = false;
					temp.LastExp = 0;

					temp.SiegeExp = 0;
					temp.LastSiegeExp = 0;

					temp.RangedExp = 0;
					temp.LastRangedExp = 0;

					temp.MeleeExp = 0;
					temp.LastMeleeExp = 0;

					temp.NearByHeroes = 0;
					/*
					unsigned char smite = hero->GetSpellSlotFromName("SummonerSmite");
					if (smite == (unsigned char)SpellSlot::Summoner1 || smite == (unsigned char)SpellSlot::Summoner2)
					{
						//SdkUiConsoleWrite("rr smite added %s", hero->GetCharName());
						temp.isJungler = true;
					}
					else
					{
						//SdkUiConsoleWrite("en none smite %s", hero->GetCharName());
						temp.isJungler = false;
					}*/
					ChampionEXPObjects.emplace_back(temp);
				}
			}
		}
	}


}

void SharedExperience::MenuLoader()
{
	Menu::Tree("Shared XP Detector", "Detector.SharedXP", false, []()
	{
		Menu::Checkbox("Use Shared XP Detector", "Detector.SharedXP.Use", true);


		Menu::SliderInt("Detecting Range", "Drawings.GankAlerter.Range", 1450, 500, 1450);
		//SdkUiText("^-> You may check ranges by using the Custom Ranges feature to know how far it is");

		Menu::SliderInt("Detecting CoolTime (Seconds)", "Drawings.GankAlerter.CoolTime", 5, 1, 30);

		Menu::SliderInt("Draw Only Decteted # equal or above", "Drawings.GankAlerter.LineDuration", 1, 0, 5);

		Menu::SliderInt("Alerter Line Width", "Drawings.GankAlerter.LineWidth", 3, 1, 10);


		Menu::Checkbox("Draw Only ", "Drawings.GankAlerter.Name", true);

		/*
		Menu::Checkbox("Don't Use Last Position Checker", "Drawings.GankAlerter.LastPos", false);
		SdkUiText("^-> If you turn it on, Gank Alerter is triggered every time a tracked champion is visible again        ");
		SdkUiText("^-> Might be little bit annoying but some people prefer it so I added it");
		*/

	


		Menu::Tree("Shared XP Enemy Settings", "Detector.SharedXP.Enemy", false, []()
		{

			Menu::Checkbox("Track Only Enemy Jungler (Has Smite)", "Drawings.GankAlerter.Enemy.Jungler", false);

			Menu::Checkbox("Track Enemy With a Line", "Drawings.GankAlerter.Enemy.LineUse", true);
			Menu::DropList("^-> Color ", "Drawings.GankAlerter.Enemy.LineUseColor", ColorMenuList, 10);

			Menu::Checkbox("Use Ping to Alert Enemy (Local)", "Drawings.GankAlerter.Enemy.Ping", true);
			Menu::DropList("^-> Type of the Ping ", "Drawings.GankAlerter.Enemy.PingType", PingTypes, 2);

			//Menu::Checkbox("Track Enemy on The Minimap", "Drawings.GankAlerter.Enemy.Minimap", true);

			Menu::Tree("Enemy Settings", "Drawings.GankAlerter.EnemyList", false, []()
			{
				if (!ChampionEXPObjects.empty())
				{
					for (auto const ent : ChampionEXPObjects)
					{
						if ((&ent)->Hero->GetNetworkID() != Player.GetNetworkID() && (&ent)->isAlly == false)
						{
							std::string menuID = "Drawings.GankAlerter.Enemy.UseFor";
							menuID += (&ent)->Hero->GetCharName();
							Menu::Checkbox((&ent)->Hero->GetCharName(), menuID, true);
						}
					}
				}
			});
		});


	});
}

bool SharedExperience::AreSame(float a, float b, float tolerance)
{
	return fabs(a - b) <= tolerance;
}


void SharedExperience::TickLoader()
{
	
	/*
	for (auto &value : ChampionEXPObjects)
	{
		if (value.LastExp != value.Hero->GetExperience())
		{

			float difference = value.Hero->GetExperience() - value.LastExp;
		//	std::llround(difference)
			//difference = std::llround(difference);
			//float difference2 = round(difference * 100) / 100;
			
		
			if (difference == 59.0f) //38 26 19 15
			{
				SdkUiConsoleWrite("Difference Melee %f", difference);

			}
			else if (difference == 29.0f) //19 13 10 8
			{
				SdkUiConsoleWrite("Difference Range %f", difference);

			}
			else if (difference == 92.0f) //60 40 30 24
			{
				SdkUiConsoleWrite("Difference Siege %f", difference);

			}
			else
			{
				SdkUiConsoleWrite("Difference Other %f", difference);

			}
	
		}
		value.LastExp = value.Hero->GetExperience();
	}*/

	//auto minion_ptr{ pSDK->EntityManager->GetEnemyMinions() };


	if (ChampionEXPObjects.empty())
	{
		return;
	}
	auto minion_ptr{ pSDK->EntityManager->GetAllyMinions() };


	if (minion_ptr.empty())
	{
		return;
	}

	for (auto &[netID, value] : minion_ptr)
	{
		if (netID && value)
		{
			if (!value->IsAlive())
			{
				auto miniPos = value->GetPosition();


				for (auto &heroValue : ChampionEXPObjects)
				{
					auto heroPos = heroValue.Hero->GetPosition();

					if (heroPos.IsValid() && heroValue.Hero->Distance(miniPos) <= 1450.0f )
					{
						float death = value->GetDeathTime();

						if (death == NULL || death == 0)
						{
							continue;
						}

						if (death + 1.0f > Game::Time() )
						{

							float currentEXP = heroValue.Hero->GetExperience();
							if (heroValue.LastExp != currentEXP)
							{
								float difference = heroValue.Hero->GetExperience() - heroValue.LastExp;
								difference = std::llround(difference);
								if (difference <= 0.0f || difference > 500.0f)
								{
									continue;
								}

								//float difference = heroValue.Hero->GetExperience() - heroValue.LastExp;
								//	std::llround(difference)
									//difference = std::llround(difference);
									//float difference2 = round(difference * 100) / 100;

								if (value->IsSiegeMinion() )  //92 60 40 30 24
								{

									if (AreSame(90.0f, difference, 10.0f))
									{
										heroValue.NearByHeroes = 0;
									}
									else if (AreSame(60.0f, difference, 5.0f))
									{
										heroValue.NearByHeroes = 1;
									}
									else if (AreSame(40.0f, difference, 5.0f))
									{
										heroValue.NearByHeroes = 2;
									}
									else if (AreSame(30.0f, difference, 3.0f))
									{
										heroValue.NearByHeroes = 3;
									}
									else if (AreSame(24.0f, difference, 2.0f))
									{
										heroValue.NearByHeroes = 4;
									}

									heroValue.SiegeExp = difference;
									SdkUiConsoleWrite("1dead siege added %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
								}
								else if (value->IsLaneMinion() && value->IsMelee() ) //59 38 26 19 15
								{

									if (AreSame(59.0f, difference, 5.0f))
									{
										heroValue.NearByHeroes = 0;
									}
									else if (AreSame(38.0f, difference, 5.0f))
									{
										heroValue.NearByHeroes = 1;
									}
									else if (AreSame(26.0f, difference, 5.0f))
									{
										heroValue.NearByHeroes = 2;
									}
									else if (AreSame(19.0f, difference, 2.0f))
									{
										heroValue.NearByHeroes = 3;
									}
									else if (AreSame(15.0f, difference, 1.5f))
									{
										heroValue.NearByHeroes = 4;
									}

									heroValue.MeleeExp = difference;
									SdkUiConsoleWrite("2dead Melee added %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
								}
								else if (value->IsLaneMinion() && !value->IsMelee() ) //29 19 13 10 8
								{

									if (AreSame(29.0f, difference, 5.0f))
									{
										heroValue.NearByHeroes = 0;
									}
									else if (AreSame(19.0f, difference, 3.0f))
									{
										heroValue.NearByHeroes = 1;
									}
									else if (AreSame(13.0f, difference, 1.5f))
									{
										heroValue.NearByHeroes = 2;
									}
									else if (AreSame(10.0f, difference, 1.0f))
									{
										heroValue.NearByHeroes = 3;
									}
									else if (AreSame(8.0f, difference, 0.9f))
									{
										heroValue.NearByHeroes = 4;
									}

									heroValue.RangedExp = difference;
									SdkUiConsoleWrite("3dead Ranged added %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
								}


							}
							heroValue.LastExp = currentEXP;
							heroValue.LastExpTime = Game::Time();









							/*
							if (value->IsSiegeMinion())
							{
								

								SdkUiConsoleWrite("dead siege added %f NearBy %s", value->GetDeathTime(), heroValue.Hero->GetCharName());
							}
							else if (value->IsLaneMinion() && value->IsMelee())
							{
								SdkUiConsoleWrite("dead Melee added %f NearBy %s", value->GetDeathTime(), heroValue.Hero->GetCharName());
							}
							else if (value->IsLaneMinion() && !value->IsMelee())
							{
								SdkUiConsoleWrite("dead Ranged added %f NearBy %s", value->GetDeathTime(), heroValue.Hero->GetCharName());
							}
							*/

						}
					}
				}
			}
			//MinionObject temp;
			//temp.Minion = value;
			//temp.netID = netID;
			//MinionsObjects.emplace_back(temp);
		}
	}

}


void SharedExperience::Draw(_In_ void* UserData)
{
	if (ChampionEXPObjects.empty())
	{
		return;
	}

	for (auto const& drawHero : ChampionEXPObjects)
	{
		Vector3 pos{ drawHero.Hero->GetPosition() };
		Vector2 screenPos{ Renderer::WorldToScreen(pos) };
		//for (auto const& value : currentItems)


		if (pos.IsValid() && pos.IsOnScreen() && drawHero.Hero->IsAlive() && drawHero.Hero->IsVisible())
		{


			screenPos.y -= 160.0f;
			screenPos.x += 80.0f;
			Draw::Text(NULL, &screenPos, "Nearby: ", "Arial", &Color::Red, 28, 6);

			screenPos.x += 60.0f;

			//auto hi = Menu::Get<Hotkey>("Activator.Config.ComboKey");





			//hi.Active;

			//Menu::Hotkey
			Draw::Text(NULL, &screenPos, std::to_string(drawHero.NearByHeroes), "Arial", &Color::Red, 28, 6);
		}
	}
}

void SharedExperience::DrawLoader()
{

}

bool __cdecl SharedExperience::DeleteObject(void * Object, unsigned int NetworkID, void * UserData)
{

	return true;
	if (Object && NetworkID)
	{
	

		
		//if(pSDK->EntityManager->GetObjectFromPTR(Object))
		//pSDK->EntityManager->GetObjectFromPTR(Object);
		AIMinionClient* minions = (AIMinionClient*) pSDK->EntityManager->GetObjectFromPTR(Object);

		if ( minions)
		{


				auto miniPos = minions->GetPosition();
				if (miniPos.IsValid() && minions->GetTeamID() != Player.GetTeamID())
				{
					if (minions->IsLaneMinion() || minions->IsSiegeMinion())
					{
						if (Player.Distance(&miniPos) <= 1450.0f)
						{
							SdkUiConsoleWrite("mini ded x %f y %f z %f %s", miniPos.x, miniPos.y, miniPos.z, minions->GetCharName());
						}
					}
					/*
					for (auto& value : ChampionEXPObjects)
					{
						if (value.Hero->Distance(&miniPos) <= 1450.0f)
						{
							//SdkUiConsoleWrite("mini ded x %f y %f z %f Nearby %s", miniPos.x, miniPos.y, miniPos.z, value.Hero->GetCharName());

						}

						//value.Hero->GetServerPosition()
					}*/



				}
			
		


			
		}

	}

	return true;
}