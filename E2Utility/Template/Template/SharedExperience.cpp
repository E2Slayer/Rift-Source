#include "stdafx.h"
#include "SharedExperience.h"
#include "DropLists.h"

const float meleeEXP = 58.879882f;
const float rangedEXP = 29.439941f;
const float tankEXP = 92.00f;

std::vector<MinionObject> MinionsObjects;


std::vector<ChampionEXPObject> ChampionEXPObjects;





void SharedExperience::InitLoader()
{
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick, Drawings::Tick);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, Drawings::DrawMenu);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::DeleteObject, SharedExperience::DeleteObject);
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


	auto ally1{ pSDK->EntityManager->GetAllyHeroes() };

	auto enemy1{ pSDK->EntityManager->GetEnemyHeroes() };

	if (enemy1.empty())
	{
		return;
	}

	
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
					temp.isAlly = false;
					temp.PossibleInvalidNumber = false;
					temp.LastExp = 0;
					temp.LastExpTime = 0;

					temp.SiegeExp = 0;
					temp.RangedExp = 0;

					temp.MeleeExp = 0;


					temp.NearByHeroes = 0;
					temp.UnsureNearByHeroes = 0;
		
					ChampionEXPObjects.emplace_back(temp);
				}
			}
		}
	}
	//SummonerSmite - Smite
	
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
					temp.PossibleInvalidNumber = false;
					temp.LastExp = 0;
					temp.LastExpTime = 0;

					temp.SiegeExp = 0;
					temp.RangedExp = 0;

					temp.MeleeExp = 0;


					temp.NearByHeroes = 0;
					temp.UnsureNearByHeroes = 0;
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


		Menu::SliderInt("Detect Enemies Only Within Range From Player", "Detector.SharedXP.Range", 3000, 500, 5000);
		Menu::Checkbox("Detect All Enemies ", "Detector.SharedXP.All", false);
		SdkUiText("^-> If you check it, Shared XP Detector will always track all enemies no matter how far they are.");

		//Menu::SliderInt("Detecting CoolTime (Seconds)", "Drawings.GankAlerter.CoolTime", 5, 1, 30);

		Menu::SliderInt("Draw Only Decteted if there are minimum # enemies around", "Detector.SharedXP.DetectNumber", 0, 0, 4);


		Menu::Tree("Shared XP Drawing Settings", "Detector.SharedXP.Drawings", false, []()
		{
			Menu::SliderInt("Font Size", "Detector.SharedXP.FontSize", 24, 20, 30);

			Menu::SliderInt("Drawing Position X-axis", "Detector.SharedXP.DrawingX", 22, 1, 80);
			Menu::SliderInt("Drawing Position Y-axis", "Detector.SharedXP.DrawingY", 53, 1, 80);

			Menu::SliderInt("Don't Draw If the detector's data was # Seconds ago", "Detector.SharedXP.TimerMax", 30, 5, 60);




			Menu::SliderInt("Draw Only Decteted if there are minimum # enemies around", "Detector.SharedXP.DetectNumber", 0, 0, 4);
			Menu::DropList("^-> Color", "Detector.SharedXP.Enemy.Color", ColorMenuList, 11); //white


			Menu::Checkbox("Change the Color If mimimum # enemies around", "Detector.SharedXP.ChangeColor", true);
			Menu::SliderInt("^-> Number", "Detector.SharedXP.WarningDetectNumber", 2, 1, 4);
			Menu::DropList("^-> Color ", "Detector.SharedXP.Enemy.ColorWarning", ColorMenuList, 10); //red


			Menu::Checkbox("Use the Timer shows when the detector data collected ", "Detector.SharedXP.Timer", true);
			Menu::SliderInt("^- Minimum # Seconds to Show the Timer", "Detector.SharedXP.TimerMin", 3, 1, 20);
			

		});

	



		Menu::Tree("Shared XP Enemy Use On", "Detector.SharedXP.Enemy", false, []()
		{

			if (!ChampionEXPObjects.empty())
			{
				for (auto const ent : ChampionEXPObjects)
				{
					if ((&ent)->Hero->GetNetworkID() != Player.GetNetworkID() && (&ent)->isAlly == false)
					{
						std::string menuID = "Detector.SharedXP.Enemy.UseFor";
						menuID += (&ent)->Hero->GetCharName();
						Menu::Checkbox((&ent)->Hero->GetCharName(), menuID, true);
					}
				}
			}
			
		});


	});
}

bool SharedExperience::AreSame(float a, float b, float tolerance)
{
	return fabs(a - b) <= tolerance;
}


void SharedExperience::TickLoader()
{	
	if (!Menu::Get<bool>("Detector.SharedXP.Use") || ChampionEXPObjects.empty())
	{
		return;
	}






	for (auto &heroValue : ChampionEXPObjects)
	{

		
		std::string menuID = "Detector.SharedXP.Enemy.UseFor";
		menuID += heroValue.Hero->GetCharName();
		if (!Menu::Get<bool>(menuID))
		{
			continue;
		}

		auto heroPos = heroValue.Hero->GetServerPosition();

		float detectingRange = (float) Menu::Get<int>("Detector.SharedXP.Range");

		if (heroPos.IsValid() && heroValue.Hero->IsVisible() && Player.Distance(heroPos) > detectingRange && !Menu::Get<bool>("Detector.SharedXP.All"))
		{
			//SdkUiConsoleWrite("emprt target %s %f", heroValue.Hero->GetCharName(), Player.Distance(heroPos));
			//SdkUiConsoleWrite("deac %f", detectingRange);
			continue;
		}
		


		auto heroPos = heroValue.Hero->GetServerPosition();

		//if ( Menu::Get<bool>("Detector.SharedXP.All"))
		{
			if (heroPos.IsValid())
			{
				auto minion_ptr{ pSDK->EntityManager->GetAllyMinions(1400.0f, &heroPos) }; // 1450 is more accurate but wanna make it sure

			//	auto minion_ptr{ pSDK->EntityManager->GetEnemyMinions(1400.0f, &heroPos) }; // 1450 is more accurate but wanna make it sure

				if (minion_ptr.empty())
				{
					//SdkUiConsoleWrite("emprt");
					continue;
				}

				for (auto &[netID, value] : minion_ptr)
				{
					if (netID && value)
					{
						if (!value->IsAlive())
						{
							
							float death = value->GetDeathTime();

							if (death == NULL || death == 0)
							{
								continue;
							}

							if (death + 0.5f > Game::Time())
							{
								
								float currentEXP = heroValue.Hero->GetExperience();
								if (heroValue.LastExp != currentEXP)
								{
									float difference = heroValue.Hero->GetExperience() - heroValue.LastExp;
									difference = std::llround(difference);

									//SdkUiConsoleWrite("inside34 %f %f %f", heroValue.Hero->GetExperience(), heroValue.LastExp, difference);

									if ( (difference <= 0.0f || difference > 500.0f) && heroValue.LastExp != 0)
									{
										continue;
									}
									
									//float difference = heroValue.Hero->GetExperience() - heroValue.LastExp;
									//	std::llround(difference)
										//difference = std::llround(difference);
										//float difference2 = round(difference * 100) / 100;


									//IsValidNumber

									if (value->IsLaneMinion() && value->IsSiegeMinion())  //92 60 40 30 24
									{

										int tempStock = 0;

										if (AreSame(90.0f, difference, 10.0f))
										{
											tempStock = 0;
										}
										else if (AreSame(60.0f, difference, 5.0f))
										{
											tempStock = 1;
										}
										else if (AreSame(40.0f, difference, 5.0f))
										{
											tempStock = 2;
										}
										else if (AreSame(30.0f, difference, 3.0f))
										{
											tempStock = 3;
										}
										else if (AreSame(24.0f, difference, 2.0f))
										{
											tempStock = 4;
										}

										if ((tempStock - heroValue.NearByHeroes >= 2) && !heroValue.PossibleInvalidNumber && heroValue.UnsureNearByHeroes == 6)
										{
											heroValue.PossibleInvalidNumber = true;
											heroValue.UnsureNearByHeroes = tempStock;
										}
										else if (heroValue.PossibleInvalidNumber)
										{
											if (heroValue.UnsureNearByHeroes == tempStock)
											{
												heroValue.NearByHeroes = tempStock;
											}

											heroValue.UnsureNearByHeroes = 6;
											heroValue.PossibleInvalidNumber = false;
										}
										else
										{
											heroValue.NearByHeroes = tempStock;
										}



										heroValue.SiegeExp = difference;
										heroValue.LastExpTime = Game::Time();
										SdkUiConsoleWrite("1dead siege added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
									}
									else if (value->IsLaneMinion() && value->IsMelee()) //59 38 26 19 15
									{
										int tempStock = 0;

										if (AreSame(59.0f, difference, 5.0f))
										{
											tempStock = 0;
										}
										else if (AreSame(38.0f, difference, 5.0f))
										{
											tempStock = 1;
										}
										else if (AreSame(26.0f, difference, 5.0f))
										{
											tempStock = 2;
										}
										else if (AreSame(19.0f, difference, 2.0f))
										{
											tempStock = 3;
										}
										else if (AreSame(15.0f, difference, 1.5f))
										{
											tempStock = 4;
										}

										if ((tempStock - heroValue.NearByHeroes >= 2) && !heroValue.PossibleInvalidNumber)
										{
											heroValue.PossibleInvalidNumber = true;
											heroValue.UnsureNearByHeroes = tempStock;
										}
										else if (heroValue.PossibleInvalidNumber)
										{
											if (heroValue.UnsureNearByHeroes == tempStock)
											{
												heroValue.NearByHeroes = tempStock;
											}

											heroValue.UnsureNearByHeroes = 6;
											heroValue.PossibleInvalidNumber = false;
										}
										else
										{
											heroValue.NearByHeroes = tempStock;
										}

										heroValue.MeleeExp = difference;
										heroValue.LastExpTime = Game::Time();
										SdkUiConsoleWrite("2dead melee added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
									}
									else if (value->IsLaneMinion() && !value->IsMelee()) //29 19 13 10 8
									{
										int tempStock = 0;
										if (AreSame(29.0f, difference, 5.0f))
										{
											tempStock = 0;
										}
										else if (AreSame(19.0f, difference, 3.0f))
										{
											tempStock = 1;
										}
										else if (AreSame(13.0f, difference, 1.5f))
										{
											tempStock = 2;
										}
										else if (AreSame(10.0f, difference, 1.0f))
										{
											tempStock = 3;
										}
										else if (AreSame(8.0f, difference, 0.9f))
										{
											tempStock = 4;
										}

										if ((tempStock - heroValue.NearByHeroes >= 2) && !heroValue.PossibleInvalidNumber)
										{
											heroValue.PossibleInvalidNumber = true;
											heroValue.UnsureNearByHeroes = tempStock;
										}
										else if (heroValue.PossibleInvalidNumber)
										{
											if (heroValue.UnsureNearByHeroes == tempStock)
											{
												heroValue.NearByHeroes = tempStock;
											}

											heroValue.UnsureNearByHeroes = 6;
											heroValue.PossibleInvalidNumber = false;
										}
										else
										{
											heroValue.NearByHeroes = tempStock;
										}

										heroValue.RangedExp = difference;
										heroValue.LastExpTime = Game::Time();
										SdkUiConsoleWrite("3dead ranged added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
									}


								}
								heroValue.LastExp = currentEXP;
								

							}


						}
					}
				}
			}
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

		std::string menuID = "Detector.SharedXP.Enemy.UseFor";
		menuID += drawHero.Hero->GetCharName();
		if (!Menu::Get<bool>(menuID))
		{
			continue;
		}

		auto heroPos = drawHero.Hero->GetServerPosition();

		float detectingRange = (float)Menu::Get<int>("Detector.SharedXP.Range");

		if (heroPos.IsValid() && Player.Distance(heroPos) > detectingRange && !Menu::Get<bool>("Detector.SharedXP.All"))
		{
			continue;
		}



		//SdkUiConsoleWrite("here1");
		Vector3 pos{ drawHero.Hero->GetPosition() };
		Vector2 screenPos{ Renderer::WorldToScreen(pos) };
		//for (auto const& value : currentItems)


		Vector2 posHP{ drawHero.Hero->GetHealthBarScreenPos() };
		posHP.x += (float)Menu::Get<int>("Detector.SharedXP.DrawingX");
		posHP.y -= (float)Menu::Get<int>("Detector.SharedXP.DrawingY");

		//Detector.SharedXP.DrawingX
		if (posHP.IsValid() && drawHero.Hero->GetPosition().IsOnScreen() && drawHero.Hero->IsAlive() && drawHero.Hero->IsVisible())
		{

			//SdkUiConsoleWrite("here12");
			screenPos.y -= 160.0f;
			screenPos.x += 80.0f;

		
			//auto hi = Menu::Get<Hotkey>("Activator.Config.ComboKey");


			//Detector.SharedXP.FontSize
			int minimumEnemy = Menu::Get<int>("Detector.SharedXP.DetectNumber");
			
			int fontSize = Menu::Get<int>("Detector.SharedXP.FontSize");

			if (minimumEnemy >= 0 && minimumEnemy < 5 && fontSize)
			{
			
				int detectNumber = Menu::Get<int>("Detector.SharedXP.WarningDetectNumber");

				if (detectNumber >= 0 && detectNumber < 5)
				{
					//Draw::Text(NULL, &screenPos, "3", "Arial", &Color::Red, 28, 6);

					screenPos.y -= 40.0f;
					//Draw::Text(NULL, &screenPos, std::to_string(minimumEnemy), "Arial", &Color::Green, 28, 6); //1
					screenPos.y -= 40.0f;

					float timePassed = Game::Time() - drawHero.LastExpTime;
					Draw::Text(NULL, &screenPos, std::to_string(timePassed), "Arial", &Color::Green, 28, 6); //0

					if (minimumEnemy <= drawHero.NearByHeroes)
					{
						screenPos.x += 60.0f;

						std::stringstream ss;
						ss.precision(1); //for decimal
						ss.setf(std::ios_base::fixed, std::ios_base::floatfield);

						ss << "" << drawHero.NearByHeroes;

						float maxTimer = (float)Menu::Get<int>("Detector.SharedXP.TimerMax");
						if (timePassed <= maxTimer)
						{



							if (Menu::Get<bool>("Detector.SharedXP.Timer"))//Detector.SharedXP.Timer
							{
								float minTimer = (float)Menu::Get<int>("Detector.SharedXP.TimerMin");
								//Detector.SharedXP.TimerMin

								if (timePassed >= minTimer)
								{
									ss << " (" << timePassed << "s)";
								}

							}


							if (detectNumber <= drawHero.NearByHeroes && Menu::Get<bool>("Detector.SharedXP.ChangeColor"))
							{

								Draw::Text(NULL, &posHP, ss.str().c_str(), "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Detector.SharedXP.Enemy.ColorWarning")), fontSize, 6);
							}
							else if (detectNumber > drawHero.NearByHeroes)
							{

								Draw::Text(NULL, &posHP, ss.str().c_str(), "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Detector.SharedXP.Enemy.Color")), fontSize, 6);
							}
							else
							{
								Draw::Text(NULL, &posHP, ss.str().c_str(), "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Detector.SharedXP.Enemy.Color")), fontSize, 6);
							}
						}
					}

				}
				
			}


		}
	}
}

void SharedExperience::DrawLoader()
{

}

