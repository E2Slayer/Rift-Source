#include "stdafx.h"
#include "SharedExperience.h"
#include "DropLists.h"

std::vector<MinionObject> MinionsObjects;


std::vector<ChampionEXPObject> ChampionEXPObjects;





void SharedExperience::InitLoader()
{


	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick, Drawings::Tick);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, Drawings::DrawMenu);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::DeleteObject, SharedExperience::DeleteObject);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, SharedExperience::Draw);


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

	/*
	
		unsigned int netID;
	AIMinionClient* Minion;
	float DeathTimer;
	bool isDeadSameTime;
	*/




	ChampionEXPObjects.clear();


	//auto ally1{ pSDK->EntityManager->GetAllyHeroes() };

	auto enemy1{ pSDK->EntityManager->GetEnemyHeroes() };


	
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
					temp.Distance = 0.0f;
					temp.LastTrigger = 0.0f;
					temp.isAlly = false;
					temp.PossibleInvalidNumber = false;
					temp.LastExp = 0.0f;
					temp.LastExpTime = 0.0f;

					temp.SiegeExp = 0.0f;
					temp.RangedExp = 0.0f;

					temp.MeleeExp = 0.0f;

					temp.CurrentExp = 0.0f;
					temp.CurrentExpTime = 0.0f;
					temp.NearByHeroes = 0.;
					temp.UnsureNearByHeroes = 0;

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



void SharedExperience::testing()
{
	if (!Menu::Get<bool>("Detector.SharedXP.Use") || ChampionEXPObjects.empty())
	{
		return;
	}






	for (auto &heroValue : ChampionEXPObjects)
	{


		/*
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
		*/

		if (heroValue.CurrentExp != heroValue.Hero->GetExperience())
		{
			heroValue.CurrentExp = heroValue.Hero->GetExperience();
			heroValue.CurrentExpTime = Game::Time();
		}


		auto heroPos = heroValue.Hero->GetServerPosition();

		//if ( Menu::Get<bool>("Detector.SharedXP.All"))
		{
			if (heroPos.IsValid() && heroValue.Hero->IsAlive())
			{
				//auto minion_ptr{ pSDK->EntityManager->GetAllyMinions(1450.0f, &heroPos) }; // 1450 is more accurate but wanna make it sure

				auto minion_ptr{ pSDK->EntityManager->GetEnemyMinions(1450.0f, &heroPos) }; // 1450 is more accurate but wanna make it sure

				if (minion_ptr.empty())
				{
					//SdkUiConsoleWrite("emprt");
					continue;
				}

				/*
				std::vector< DeadMinionStruct> deathTimeContainer;

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

							DeadMinionStruct temp;

							temp.netID = netID;
							temp.Minion = value;
							temp.DeathTimer = death;
							temp.isDeadSameTime = false;
							deathTimeContainer.emplace_back(temp);

						}
					}
				}




				//std::vector< DeadMinionStruct> DiedSameTimeContainer;
				//std::map<AIMinionClient*, float> DiedSameTimeContainer;
				std::sort(deathTimeContainer.begin(), deathTimeContainer.end(), SortByX());


				std::vector< AIMinionClient*> DiedSameTimeContainer;
				DiedSameTimeContainer.clear();

				float timerTemp = 0.1f;

				int insideChecker = 0;
				AIMinionClient* previous = NULL;
				bool didPreviousDone = false;
				for (auto &[netID, minion, deathtimer, isdeadsametime] : deathTimeContainer)
				{
					if (netID && minion && deathtimer)
					{

						if (timerTemp == 0.1f) // first loop
						{
							timerTemp = deathtimer;
							DiedSameTimeContainer.emplace_back(minion);
						}
						else if (deathtimer == timerTemp && timerTemp != 0.1f)
						{

							if (deathtimer <= Game::Time() && deathtimer >= heroValue.LastExpTime)
							{
								//SdkUiConsoleWrite("Found Some %d", DiedSameTimeContainer.size());
								isdeadsametime = true;

								DiedSameTimeContainer.emplace_back(minion);

							}

							timerTemp = deathtimer;
						}
						else if (deathtimer != timerTemp )
						{
							DiedSameTimeContainer.clear();
							timerTemp = deathtimer;
						}




					}
				}


				if (DiedSameTimeContainer.size() >= 2)
				{
					//SdkUiConsoleWrite("Found Some %d", DiedSameTimeContainer.size());
				}



				Vector3 pos2{ Player.GetPosition() };
				Vector2 screenPos2{ Renderer::WorldToScreen(pos2) };
				screenPos2.y += 160.0f;
				screenPos2.x += 80.0f;
				Draw::Text(NULL, &screenPos2, std::to_string(DiedSameTimeContainer.size()), "Arial", &Color::Green, 28, 6); //0

				*/


				/*
				for (auto it = deathTimeContainer.begin(); it != deathTimeContainer.end(); ++it)
				{
					if (it->second == timerTemp)
					{
						DiedSameTimeContainer.emplace(it->first, it->second);
					}
					timerTemp = it->second;
				}
				*/





				bool previousWasSameTimeDied = false;



				//for (auto &[netID, minion, deathtimer, isdeadsametime] : deathTimeContainer)
				for (auto &[netID, value] : minion_ptr)
				{
					if (netID && value)
					{
						if (!value->IsAlive())
						{

							float death = value->GetDeathTime();

							//SdkUiConsoleWrite("4444444444");

							if (death == NULL || death == 0)
							{
								continue;
							}

							//if (death + 2.0f > Game::Time())
							if (death <= Game::Time() && death > heroValue.LastExpTime)
							{

								float currentEXP = heroValue.Hero->GetExperience();
								if (heroValue.LastExp != currentEXP)
								{
									float difference = currentEXP - heroValue.LastExp;
									difference = std::llround(difference);

									//SdkUiConsoleWrite("inside34 %f %f %f", heroValue.Hero->GetExperience(), heroValue.LastExp, difference);


									if (difference >= 100.0f && heroValue.LastExp != 0)
									{
										continue;
									}

									float targetEXP[] = { 0.0f,0.0f, 0.0f, 0.0f, 0.0f };

									//SdkUiConsoleWrite("66666");


									/*
									if (DiedSameTimeContainer.size() >= 2 && deathTimeContainer.size() > 1)
									{

										float SuperSiegeEXP[] = { 92.0f, 60.0f, 40.0f, 30.0f, 24.0f };
										float MeleeEXP[] = { 59.0f, 38.0f, 26.0f, 19.0f, 15.0f };
										float RangedEXP[] = { 29.0f, 19.0f, 13.0f, 10.0f, 8.0f };

										//float result = 0.0f;
										int nearBy = 0;



										for (int i = 0; i < 5; i++)
										{
											float result = 0.0f;
											for (auto &targetValue : DiedSameTimeContainer)
											{
												if (targetValue->IsSuperMinion()) //92 60 40 30 24
												{
													result = result + SuperSiegeEXP[i];

												}
												else if (targetValue->IsSiegeMinion() && strcmp(targetValue->GetCharName(), "SRU_ChaosMinionSiege") == 0) //SRU_OrderMinionSiege
												{
													result = result + SuperSiegeEXP[i];
												}
												else if (!targetValue->IsSuperMinion() && strcmp(targetValue->GetCharName(), "SRU_ChaosMinionMelee") == 0) //SRU_OrderMinionMelee
												{
													result = result + MeleeEXP[i];
												}
												else if (strcmp(targetValue->GetCharName(), "SRU_ChaosMinionRanged") == 0) //SRU_OrderMinionRanged
												{
													result = result + RangedEXP[i];
												}
											}

											if (difference >= result - 5.0f && difference <= result + 5.0f)
											{
												//nearBy = i;

												heroValue.NearByHeroes = i;
												SdkUiConsoleWrite("Same Time died difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), i);
												//heroValue.MeleeExp = difference;
												heroValue.LastExpTime = Game::Time();
												break;
											}

										}




										//92 60 40 30 24 super / siege
										//29 19 13 10 8 SRU_OrderMinionRanged
										//59 38 26 19 15 SRU_OrderMinionMelee

										continue;

									}
									*/

									/*
									unsigned int result{ 0 };
									auto position{ value->GetPosition() };
									for (auto &[_, Enemy] : pSDK->EntityManager->GetEnemyHeroes(1400.0f, &position)) {
										if (pCore->TS->IsValidTarget(Enemy)) {
											++result;
										}
									}
									*/



									/*
									if (DiedSameTimeContainer.size() >= 2)
									{
										continue;
									}*/

									/*
									if (!DiedSameTimeContainer.empty())
									{
										for (auto &[DiedSamevalue, DiedSamedeathtimer] : DiedSameTimeContainer)
										{

										}
									}*/

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

										heroValue.NearByHeroes = tempStock;
										heroValue.SiegeExp = difference;
										heroValue.LastExpTime = Game::Time();
										heroValue.LastExp = currentEXP;
										SdkUiConsoleWrite("1dead super added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
									}
									else if (value->IsSiegeMinion() && strcmp(value->GetCharName(), "SRU_ChaosMinionSiege") == 0)  //92 60 40 30 24 SRU_OrderMinionSiege
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


										heroValue.NearByHeroes = tempStock;
										heroValue.SiegeExp = difference;
										heroValue.LastExpTime = Game::Time();
										heroValue.LastExp = currentEXP;
										SdkUiConsoleWrite("1dead siege added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
									}
									else if (!value->IsSuperMinion() && strcmp(value->GetCharName(), "SRU_ChaosMinionMelee") == 0) //59 38 26 19 15 SRU_OrderMinionMelee
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


										heroValue.NearByHeroes = tempStock;

										heroValue.MeleeExp = difference;
										heroValue.LastExpTime = Game::Time();
										//SdkUiConsoleWrite("2dead melee added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
										heroValue.LastExp = currentEXP;
										SdkUiConsoleWrite("1dead melee added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
									}
									else if (strcmp(value->GetCharName(), "SRU_ChaosMinionRanged") == 0) //29 19 13 10 8 SRU_OrderMinionRanged
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


										heroValue.NearByHeroes = tempStock;
										heroValue.RangedExp = difference;
										heroValue.LastExpTime = Game::Time();
										SdkUiConsoleWrite("1dead range added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
										heroValue.LastExp = currentEXP;
									}

									//SdkUiConsoleWrite("Name: %s", value->GetCharName());
								}
								//heroValue.LastExpTime = Game::Time();



							}


						}
					}
				}
			}
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
		if (!Menu::Get<bool>(menuID) || !heroValue.Hero->IsAlive())
		{
			continue;
		}



		auto heroPos = heroValue.Hero->GetServerPosition();

		float detectingRange = (float) Menu::Get<int>("Detector.SharedXP.Range");

		if (heroPos.IsValid()  && Player.Distance(heroPos) > detectingRange && !Menu::Get<bool>("Detector.SharedXP.All"))
		{
			//&& heroValue.Hero->IsVisible()  deleted
			//SdkUiConsoleWrite("emprt target %s %f", heroValue.Hero->GetCharName(), Player.Distance(heroPos));
	
			continue;
		}
		else
		{

			if (heroValue.CurrentExp != heroValue.Hero->GetExperience())
			{
				heroValue.CurrentExp = heroValue.Hero->GetExperience();
				heroValue.CurrentExpTime = Game::Time();
			}

			if (heroPos.IsValid())
			{
				auto minion_ptr{ pSDK->EntityManager->GetAllyMinions(1450.0f, &heroPos) }; // 1450 is more accurate but wanna make it sure

				//auto minion_ptr{ pSDK->EntityManager->GetEnemyMinions(1450.0f, &heroPos) }; // 1450 is more accurate but wanna make it sure

				if (minion_ptr.empty())
				{
					continue;
				}

			
				//for (auto &[netID, minion, deathtimer, isdeadsametime] : deathTimeContainer)
				for (auto &[netID, value] : minion_ptr)
				{
					if (netID && value )
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
								//SdkUiConsoleWrite("0Checker  NearBy %s number %d", heroValue.Hero->GetCharName(), heroValue.NearByHeroes);

								float currentEXP = heroValue.Hero->GetExperience();
								if (heroValue.LastExp != currentEXP)
								{
									

									


									float difference = currentEXP - heroValue.LastExp;
									difference = std::llround(difference);

									//SdkUiConsoleWrite("1Checker difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);


									//SdkUiConsoleWrite("inside34 %f %f %f", heroValue.Hero->GetExperience(), heroValue.LastExp, difference);


									if (difference >= 200.0f && heroValue.LastExp != 0)
									{
										heroValue.LastExp = currentEXP;
										heroValue.LastExpTime = Game::Time();
										continue;
									}
									
									if (value->IsSuperMinion()) //92 60 40 30 24
									{
										int tempStock = 0;

										if (difference >= 100.0f)
										{
											tempStock = heroValue.NearByHeroes;
										}


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

										heroValue.NearByHeroes = tempStock;
										heroValue.SiegeExp = difference;
										heroValue.LastExpTime = Game::Time();
										heroValue.LastExp = currentEXP;
										//SdkUiConsoleWrite("1dead super added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
									}
									else if (strcmp(value->GetCharName(), "SRU_OrderMinionSiege") == 0 || strcmp(value->GetCharName(), "SRU_ChaosMinionSiege") == 0)  //92 60 40 30 24 SRU_OrderMinionSiege
									{//SRU_ChaosMinionSiege
										//value->IsSiegeMinion()
										int tempStock = 0;

										if (difference >= 100.0f)
										{
											tempStock = heroValue.NearByHeroes;
										}

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


										heroValue.NearByHeroes = tempStock;
										heroValue.SiegeExp = difference;
										heroValue.LastExpTime = Game::Time();
										heroValue.LastExp = currentEXP;
										//SdkUiConsoleWrite("1dead siege added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
									}
									else if (strcmp(value->GetCharName(), "SRU_OrderMinionMelee") == 0 || strcmp(value->GetCharName(), "SRU_ChaosMinionMelee") == 0) //59 38 26 19 15 SRU_OrderMinionMelee
									{//SRU_ChaosMinionMelee

										
										int tempStock = 0;
										if (difference >= 66.0f)
										{
											tempStock = heroValue.NearByHeroes;
										}

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


										heroValue.NearByHeroes = tempStock;

										heroValue.MeleeExp = difference;
										heroValue.LastExpTime = Game::Time();
										//SdkUiConsoleWrite("2dead melee added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
										heroValue.LastExp = currentEXP;
										//SdkUiConsoleWrite("1dead melee added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
									}
									else if (strcmp(value->GetCharName(), "SRU_OrderMinionRanged") == 0 || strcmp(value->GetCharName(), "SRU_ChaosMinionRanged") == 0) //29 19 13 10 8 SRU_OrderMinionRanged
									{//SRU_ChaosMinionRanged
										int tempStock = 0;
										if (difference >= 35.0f)
										{
											tempStock = heroValue.NearByHeroes;
										}


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


										heroValue.NearByHeroes = tempStock;
										heroValue.RangedExp = difference;
										heroValue.LastExpTime = Game::Time();
										//SdkUiConsoleWrite("1dead range added difference: %f NearBy %s number %d", difference, heroValue.Hero->GetCharName(), heroValue.NearByHeroes);
										heroValue.LastExp = currentEXP;
									}
									heroValue.LastExp = currentEXP;
									heroValue.LastExpTime = Game::Time();
									//SdkUiConsoleWrite("Name: %s", value->GetCharName());
								}
								//heroValue.LastExpTime = Game::Time();
								
						

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
	
}

void SharedExperience::DrawLoader()
{
	if (Game::IsOverlayOpen())
	{
		if (Player.GetPosition().IsValid() && Player.IsVisibleOnScreen())
		{

			Vector2 posHP{ Player.GetHealthBarScreenPos() };
			posHP.x += (float)Menu::Get<int>("Detector.SharedXP.DrawingX");
			posHP.y -= (float)Menu::Get<int>("Detector.SharedXP.DrawingY");

			int fontSize = Menu::Get<int>("Detector.SharedXP.FontSize");

			Draw::Text(NULL, &posHP, "0 (0.0s)", "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Detector.SharedXP.Enemy.Color")), fontSize, 6);
		}
	}
	else
	{

		if (ChampionEXPObjects.empty())
		{
			return;
		}

		if (!Menu::Get<bool>("Detector.SharedXP.Use"))
		{
			return;
		}





		/*
		Vector3 pos2{ Player.GetPosition() };
		Vector2 screenPos2{ Renderer::WorldToScreen(pos2) };

		Draw::Text(NULL, &screenPos2, std::to_string(timePassed), "Arial", &Color::Green, 28, 6); //0
		*/


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
				//&& heroValue.Hero->IsVisible()  deleted
				//SdkUiConsoleWrite("emprt target %s %f", heroValue.Hero->GetCharName(), Player.Distance(heroPos));
				//SdkUiConsoleWrite("deac2 %f", detectingRange);
				continue;
			}
			else
			{


				//SdkUiConsoleWrite("sec %f", detectingRange);
				//SdkUiConsoleWrite("here1");
				//Vector3 pos{ drawHero.Hero->GetPosition() };
				//Vector2 screenPos{ Renderer::WorldToScreen(pos) };
				//for (auto const& value : currentItems)


				Vector2 posHP{ drawHero.Hero->GetHealthBarScreenPos() };
				posHP.x += (float)Menu::Get<int>("Detector.SharedXP.DrawingX");
				posHP.y -= (float)Menu::Get<int>("Detector.SharedXP.DrawingY");

				//Detector.SharedXP.DrawingX
				if (posHP.IsValid() && drawHero.Hero->GetPosition().IsOnScreen() && drawHero.Hero->IsAlive() && drawHero.Hero->IsVisible())
				{
					

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
							//screenPos.y -= 160.0f;
							//screenPos.x += 80.0f;


							//screenPos.y -= 40.0f;
							//Draw::Text(NULL, &screenPos, std::to_string(minimumEnemy), "Arial", &Color::Green, 28, 6); //1
							//screenPos.y -= 40.0f;

							float timePassed = Game::Time() - drawHero.LastExpTime;
							//Draw::Text(NULL, &screenPos, std::to_string(timePassed), "Arial", &Color::Green, 28, 6); //0


						
							if (minimumEnemy <= drawHero.NearByHeroes)
							{
								//screenPos.x += 60.0f;

								std::stringstream ss;
								ss.precision(1); //for decimal
								ss.setf(std::ios_base::fixed, std::ios_base::floatfield);

								//int nearBy = drawHero.NearByHeroes;

								//int visibleNear = drawHero.Hero->CountEnemiesInRange(1000.0f);

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
	}
}

