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

	/*
	if (enemy1.empty())
	{
		return;
	}*/

	
	if (!ally1.empty())
	{

		//Drawings.GankAlerter.Ally.Jungler
		for (auto &[netID, hero] : ally1)
		{
			if (hero != nullptr && hero != NULL)
			{
				//if (hero->GetNetworkID() != Player.GetNetworkID())
				{
					SdkUiConsoleWrite("ad added %s", hero->GetCharName());
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

					temp.CurrentExp = 0;
					temp.CurrentExpTime = 0;

					temp.NearByHeroes = 0;
					temp.UnsureNearByHeroes = 0;
		
					//ChampionEXPObjects.emplace_back(temp);
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

					temp.CurrentExp = 0;
					temp.CurrentExpTime = 0;
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


void SharedExperience::testing()
{

	auto minion_ptr{ pSDK->EntityManager->GetEnemyMinions() };


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

					if (heroPos.IsValid() && heroValue.Hero->Distance(miniPos) <= 1450.0f)
					{
						float death = value->GetDeathTime();

						if (death == NULL || death == 0)
						{
							continue;
						}

						if (death + 1.0f > Game::Time())
						{
							float currentEXP = heroValue.Hero->GetExperience();
							if (heroValue.LastExp != currentEXP)
							{
								float difference = heroValue.Hero->GetExperience() - heroValue.LastExp;
								difference = std::llround(difference);
			
								//float difference = heroValue.Hero->GetExperience() - heroValue.LastExp;
								//	std::llround(difference)
									//difference = std::llround(difference);
									//float difference2 = round(difference * 100) / 100;


								if (value->IsSuperMinion()) //92 60 40 30 24
								{
									SdkUiConsoleWrite("4dead Super Duper added %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
								}

								if (value->IsSiegeMinion() && strcmp(value->GetCharName(), "SRU_OrderMinionSiege") == 0)  //92 60 40 30 24
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
								else if (!value->IsSuperMinion() && strcmp(value->GetCharName(), "SRU_OrderMinionSiege") == 0) //59 38 26 19 15
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
								else if (value->IsLaneMinion() && !value->IsMelee()) //29 19 13 10 8
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
								else if (value->IsSuperMinion()) //92 60 40 30 24
								{
									SdkUiConsoleWrite("4dead Super added %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
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
		

		if (heroValue.CurrentExp != heroValue.Hero->GetExperience())
		{
			heroValue.CurrentExp = heroValue.Hero->GetExperience();
			heroValue.CurrentExpTime = Game::Time();
		}



//		auto heroPos = heroValue.Hero->GetServerPosition();

		//if ( Menu::Get<bool>("Detector.SharedXP.All"))
		{
			if (heroPos.IsValid())
			{
				auto minion_ptr{ pSDK->EntityManager->GetAllyMinions(1450.0f, &heroPos) }; // 1450 is more accurate but wanna make it sure

			//	auto minion_ptr{ pSDK->EntityManager->GetEnemyMinions(1400.0f, &heroPos) }; // 1450 is more accurate but wanna make it sure

				if (minion_ptr.empty())
				{
					//SdkUiConsoleWrite("emprt");
					continue;
				}


				std::map<AIMinionClient*, float> deathTimeContainer;
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


							deathTimeContainer.emplace(heroValue.Hero, death);
							
						}
					}
				}



				typedef std::function<bool(std::pair<AIMinionClient*, float>, std::pair<AIMinionClient*, float>)> Comparator;

				// Defining a lambda function to compare two pairs. It will compare two pairs using second field
				Comparator compFunctor =
					[](std::pair<AIMinionClient*, float> elem1, std::pair<AIMinionClient*, float> elem2)
				{
					return elem1.second < elem2.second;
				};

				// Declaring a set that will store the pairs using above comparision logic
				std::set<std::pair<AIMinionClient*, float>, Comparator> setOfWords(
					deathTimeContainer.begin(), deathTimeContainer.end(), compFunctor);

				std::map<AIMinionClient*, float> DiedSameTimeContainer;


				float timerTemp = 0.1f;
				for (auto it = deathTimeContainer.begin(); it != deathTimeContainer.end(); ++it)
				{
					if (it->second == timerTemp)
					{
						DiedSameTimeContainer.emplace(it->first, it->second);
					}
					timerTemp = it->second;
				}








				for (auto &[netID, value] : minion_ptr) 
				{
					if (netID && value)
					{
						if (!value->IsAlive())
						{
							
							float death = value->GetDeathTime();

							

							if (death == NULL || death == 0 )
							{
								continue;
							}

							//if (death + 2.0f > Game::Time())
							if ( death  <= Game::Time() && death > heroValue.LastExpTime)
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


									unsigned int result{ 0 };
									auto position{ value->GetPosition() };
									for (auto &[_, Enemy] : pSDK->EntityManager->GetEnemyHeroes(1400.0f, &position)) {
										if (pCore->TS->IsValidTarget(Enemy)) {
											++result;
										}
									}

									if (value->IsSuperMinion()) //92 60 40 30 24
									{
										int tempStock = 0;

										if (99.0f >= difference && difference > 80.0f) //92
										{
											tempStock = 0;
										}
										else if (80.0f >= difference && difference > 50.0f) //60
										{
											tempStock = 1;
										}
										else if (50.0f >= difference && difference > 35.0f) //40
										{
											tempStock = 2;
										}
										else if (35.0f >= difference && difference > 27.0f) //30
										{
											tempStock = 3;
										}
										else if (27.0f >= difference && difference > 20.0f) //24
										{
											tempStock = 4;
										}


										/*
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
										*/


										/*
										if ((tempStock - heroValue.NearByHeroes >= 2) && !heroValue.PossibleInvalidNumber && heroValue.UnsureNearByHeroes == 6)
										{
											
											

											if (tempStock - result <= 2)
											{
												heroValue.PossibleInvalidNumber = true;
												heroValue.UnsureNearByHeroes = tempStock;
												SdkUiConsoleWrite("1dead super1 added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
											}

										}
										else if (heroValue.PossibleInvalidNumber)
										{
											if (heroValue.UnsureNearByHeroes == tempStock)
											{
												heroValue.NearByHeroes = tempStock;
											}

											heroValue.UnsureNearByHeroes = 6;
											heroValue.PossibleInvalidNumber = false;
											SdkUiConsoleWrite("1dead super2 added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
										}
										else
										{
											heroValue.NearByHeroes = tempStock;
											SdkUiConsoleWrite("1dead super3 added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
										}

										*/
										heroValue.NearByHeroes = tempStock;
										heroValue.SiegeExp = difference;
										heroValue.LastExpTime = Game::Time();
										SdkUiConsoleWrite("1dead super added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
									}
									else if (value->IsSiegeMinion() && strcmp(value->GetCharName(), "SRU_OrderMinionSiege") == 0)  //92 60 40 30 24 SRU_OrderMinionSiege
									{

										int tempStock = 0;


										if (99.0f >= difference && difference > 80.0f) //92
										{
											tempStock = 0;
										}
										else if (80.0f >= difference && difference > 50.0f) //60
										{
											tempStock = 1;
										}
										else if (50.0f >= difference && difference > 35.0f) //40
										{
											tempStock = 2;
										}
										else if (35.0f >= difference && difference > 27.0f) //30
										{
											tempStock = 3;
										}
										else if (27.0f >= difference && difference > 20.0f) //24
										{
											tempStock = 4;
										}

										/*
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
										*/


										/*
										if ((tempStock - heroValue.NearByHeroes >= 2) && !heroValue.PossibleInvalidNumber && heroValue.UnsureNearByHeroes == 6)
										{
											if (tempStock - result <= 2)
											{
												heroValue.PossibleInvalidNumber = true;
												heroValue.UnsureNearByHeroes = tempStock;
												SdkUiConsoleWrite("1dead siege1 added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
											}
										}
										else if (heroValue.PossibleInvalidNumber)
										{
											if (heroValue.UnsureNearByHeroes == tempStock)
											{
												heroValue.NearByHeroes = tempStock;
											}

											heroValue.UnsureNearByHeroes = 6;
											heroValue.PossibleInvalidNumber = false;
											SdkUiConsoleWrite("1dead siege2 added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
										}
										else
										{
											heroValue.NearByHeroes = tempStock;
											SdkUiConsoleWrite("1dead siege3 added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
										}

										*/

										heroValue.NearByHeroes = tempStock;
										heroValue.SiegeExp = difference;
										heroValue.LastExpTime = Game::Time();
										SdkUiConsoleWrite("1dead siege added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
									}
									else if (!value->IsSuperMinion() && strcmp(value->GetCharName(), "SRU_OrderMinionMelee") == 0) //59 38 26 19 15 SRU_OrderMinionMelee
									{
										int tempStock = 0;


										if (65.0f >= difference && difference > 55.0f) //59
										{
											tempStock = 0;
										}
										else if (42.0f >= difference && difference > 32.0f) //38
										{
											tempStock = 1;
										}
										else if (32.0f >= difference && difference > 23.0f) //26
										{
											tempStock = 2;
										}
										else if (23.0f >= difference && difference > 17.0f) //19
										{
											tempStock = 3;
										}
										else if (17.0f >= difference && difference > 11) //15
										{
											tempStock = 4;
										}

										/*
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
										}*/

										/*
										if ((tempStock - heroValue.NearByHeroes >= 2) && !heroValue.PossibleInvalidNumber)
										{
											if (tempStock - result <= 2)
											{
												heroValue.PossibleInvalidNumber = true;
												heroValue.UnsureNearByHeroes = tempStock;
												SdkUiConsoleWrite("2dead melee1 added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
											}
										}
										else if (heroValue.PossibleInvalidNumber)
										{
											if (heroValue.UnsureNearByHeroes == tempStock)
											{
												heroValue.NearByHeroes = tempStock;
											}

											heroValue.UnsureNearByHeroes = 6;
											heroValue.PossibleInvalidNumber = false;

											SdkUiConsoleWrite("2dead melee2 added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
										}
										else
										{
											heroValue.NearByHeroes = tempStock;
											SdkUiConsoleWrite("2dead melee3 added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
										}*/

										heroValue.NearByHeroes = tempStock;

										heroValue.MeleeExp = difference;
										heroValue.LastExpTime = Game::Time();
										//SdkUiConsoleWrite("2dead melee added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
										SdkUiConsoleWrite("1dead melee added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
									}
									else if (strcmp(value->GetCharName(), "SRU_OrderMinionRanged") == 0) //29 19 13 10 8 SRU_OrderMinionRanged
									{
										int tempStock = 0;

										if (34.0f >= difference && difference > 25.0f) //29
										{
											tempStock = 0;
										}
										else if (25.0f >= difference && difference > 16.0f) //19
										{
											tempStock = 1;
										}
										else if (16.0f >= difference && difference > 11.5) //13
										{
											tempStock = 2;
										}
										else if (11.5 >= difference && difference > 9.0f) //10
										{
											tempStock = 3;
										}
										else if (9.0f >= difference && difference > 5.0f) //8
										{
											tempStock = 4;
										}



										/*
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
										}*/

										/*
										if ((tempStock - heroValue.NearByHeroes >= 2) && !heroValue.PossibleInvalidNumber)
										{
											if (tempStock - result <= 2)
											{
												heroValue.PossibleInvalidNumber = true;
												heroValue.UnsureNearByHeroes = tempStock;
												SdkUiConsoleWrite("3dead ranged1 added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
											}
										}
										else if (heroValue.PossibleInvalidNumber)
										{
											if (heroValue.UnsureNearByHeroes == tempStock)
											{
												heroValue.NearByHeroes = tempStock;
											}

											heroValue.UnsureNearByHeroes = 6;
											heroValue.PossibleInvalidNumber = false;
											SdkUiConsoleWrite("3dead ranged2 added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
										}
										else
										{
											heroValue.NearByHeroes = tempStock;
											SdkUiConsoleWrite("3dead ranged3 added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
										}*/

										heroValue.NearByHeroes = tempStock;
										heroValue.RangedExp = difference;
										heroValue.LastExpTime = Game::Time();
										SdkUiConsoleWrite("1dead range added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
									}

									//SdkUiConsoleWrite("Name: %s", value->GetCharName());
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

