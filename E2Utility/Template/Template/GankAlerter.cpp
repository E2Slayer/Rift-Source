#include "stdafx.h"
#include "GankAlerter.h"
#include "DropLists.h"





std::vector<ChampionObject> ChampionObjects;

//std::vector<ChampionObject>ChampionObjects;


void GankAlerter::Init()
{
	ChampionObjects.clear();


	auto ally{ pSDK->EntityManager->GetAllyHeroes() };
	auto enemy{ pSDK->EntityManager->GetEnemyHeroes() };

	if (ally.empty() && enemy.empty())
	{
		return;
	}

	if (!ally.empty())
	{

		//Drawings.GankAlerter.Ally.Jungler
		for (auto &[netID, hero] : ally)
		{
			if (hero != nullptr && hero != NULL)
			{
				if (hero->GetNetworkID() != Player.GetNetworkID())
				{

					ChampionObject temp;
					temp.Hero = hero;
					temp.Distance = 0;
					temp.LastTrigger = 0;
					temp.isAlly = true;


					
					const char* ss = hero->GetSpell((unsigned char)SpellSlot::Summoner1).ScriptName;

					const char* ss2 = hero->GetSpell((unsigned char)SpellSlot::Summoner1).ScriptName;

					if (ss == nullptr || ss2 == nullptr)
					{
						temp.isJungler = false;
					}
					else
					{
						if (strcmp(ss, "SummonerSmite") == 0)
						{
							temp.isJungler = true;
						}
						else if (strcmp(ss2, "SummonerSmite") == 0)
						{
							temp.isJungler = true;
						}
						else
						{
							temp.isJungler = false;
						}
					}

					

				/*
					if (hero->GetSpells().empty())
					{
						temp.isJungler = false;
					}
					else
					{
						int smite = int(hero->GetSpellSlotFromName("SummonerSmite"));
						//

						if (smite == int(SpellSlot::Unknown))
						{
							//SdkUiConsoleWrite("ally none2 smite %s", hero->GetCharName());
							temp.isJungler = false;
						}
						else if (smite == int(SpellSlot::Summoner1) || smite == int(SpellSlot::Summoner2))
						{
							//SdkUiConsoleWrite("ally smite added %s", hero->GetCharName());
							temp.isJungler = true;
						}
						else
						{
							//SdkUiConsoleWrite("en none smite %s", hero->GetCharName());
							temp.isJungler = false;
						}
					}
					*/
					ChampionObjects.emplace_back(temp);
				}
			}
		}
	}
	//SummonerSmite - Smite

	if (!enemy.empty())
	{
		for (auto &[netID, hero] : enemy)
		{
			if (hero != nullptr && hero != NULL)
			{
				if (netID != Player.GetNetworkID())
				{
					//SdkUiConsoleWrite("enemy added %s", hero->GetCharName());
					ChampionObject temp;
					temp.Hero = hero;
					temp.Distance = 0;
					temp.LastTrigger = 0;
					temp.isAlly = false;

					const char* ss = hero->GetSpell((unsigned char)SpellSlot::Summoner1).ScriptName;

					const char* ss2 = hero->GetSpell((unsigned char)SpellSlot::Summoner1).ScriptName;

					if (ss == nullptr || ss2 == nullptr)
					{
						temp.isJungler = false;
					}
					else
					{
						if (strcmp(ss, "SummonerSmite") == 0)
						{
							temp.isJungler = true;
						}
						else if (strcmp(ss2, "SummonerSmite") == 0)
						{
							temp.isJungler = true;
						}
						else
						{
							temp.isJungler = false;
						}
					}


					/*
					if (hero->GetSpells().empty())
					{
						temp.isJungler = false;
					}
					else
					{
						int smite = int(hero->GetSpellSlotFromName("SummonerSmite"));
						//

						if (smite == int(SpellSlot::Unknown))
						{
							//SdkUiConsoleWrite("ally none2 smite %s", hero->GetCharName());
							temp.isJungler = false;
						}
						else if (smite == int(SpellSlot::Summoner1) || smite == int(SpellSlot::Summoner2))
						{
							//SdkUiConsoleWrite("ally smite added %s", hero->GetCharName());
							temp.isJungler = true;
						}
						else
						{
							//SdkUiConsoleWrite("en none smite %s", hero->GetCharName());
							temp.isJungler = false;
						}
					}
					*/
				
					
					ChampionObjects.emplace_back(temp);
				}
			}
		}
	}

	pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, GankAlerter::Draw);

}

void GankAlerter::MenuLoader()
{
	
	Menu::Tree("GankAlerter", "Drawings.GankAlerter", false, []()
	{
		Menu::Checkbox("Use GankAlerter", "Drawings.GankAlerter.Use", true);

		
		Menu::SliderInt("Detecting Range", "Drawings.GankAlerter.Range", 3000, 500, 5000);
		//SdkUiText("^-> You may check ranges by using the Custom Ranges feature to know how far it is");

		Menu::SliderInt("Detecting CoolTime (Seconds)", "Drawings.GankAlerter.CoolTime", 5, 1, 30);

		Menu::SliderInt("Alerter Line Last Duration (Seconds)", "Drawings.GankAlerter.LineDuration", 3, 1, 10);

		Menu::SliderInt("Alerter Line Width", "Drawings.GankAlerter.LineWidth", 3, 1, 10);


		Menu::Checkbox("Draw Champion Name on an Alerter Line", "Drawings.GankAlerter.Name", true);

		/*
		Menu::Checkbox("Don't Use Last Position Checker", "Drawings.GankAlerter.LastPos", false);
		SdkUiText("^-> If you turn it on, Gank Alerter is triggered every time a tracked champion is visible again        ");
		SdkUiText("^-> Might be little bit annoying but some people prefer it so I added it");
		*/

		Menu::Checkbox("Track Ally", "Drawings.GankAlerter.Ally.Use", true);

		Menu::Tree("Ally Settings", "Drawings.GankAlerter.Ally", false, []()
		{
			
			Menu::Checkbox("Track Only Ally Jungler (Has Smite)", "Drawings.GankAlerter.Ally.Jungler", false);


			Menu::Checkbox("Track Ally With a Line", "Drawings.GankAlerter.Ally.LineUse", true);
			Menu::DropList("^-> Color", "Drawings.GankAlerter.Ally.LineUseColor", ColorMenuList, 5);

			Menu::Checkbox("Use Ping to Alert Ally (Local)", "Drawings.GankAlerter.Ally.Ping", true);
			Menu::DropList("^-> Type of the Ping", "Drawings.GankAlerter.Ally.PingType", PingTypes, 4);


			//Menu::Checkbox("Track Ally on The Minimap", "Drawings.GankAlerter.Ally.Minimap", true);

			Menu::Tree("Ally Settings", "Drawings.GankAlerter.AllyList", false, []()
			{
				if (!ChampionObjects.empty())
				{
					for (auto const ent : ChampionObjects)
					{
						if ((&ent)->Hero->GetNetworkID() != Player.GetNetworkID() && (&ent)->isAlly == true)
						{
							std::string menuID = "Drawings.GankAlerter.Ally.UseFor";
							menuID += (&ent)->Hero->GetCharName();
							Menu::Checkbox((&ent)->Hero->GetCharName(), menuID, true);
						}
					}
				}
			});
		});

		Menu::Checkbox("Track Enemy", "Drawings.GankAlerter.Enemy.Use", true);

		Menu::Tree("Enemy Settings", "Drawings.GankAlerter.Enemy", false, []()
		{

			Menu::Checkbox("Track Only Enemy Jungler (Has Smite)", "Drawings.GankAlerter.Enemy.Jungler", false);

			Menu::Checkbox("Track Enemy With a Line", "Drawings.GankAlerter.Enemy.LineUse", true);
			Menu::DropList("^-> Color ", "Drawings.GankAlerter.Enemy.LineUseColor", ColorMenuList, 10);

			Menu::Checkbox("Use Ping to Alert Enemy (Local)", "Drawings.GankAlerter.Enemy.Ping", true);
			Menu::DropList("^-> Type of the Ping ", "Drawings.GankAlerter.Enemy.PingType", PingTypes, 2);

			//Menu::Checkbox("Track Enemy on The Minimap", "Drawings.GankAlerter.Enemy.Minimap", true);

			Menu::Tree("Enemy Settings", "Drawings.GankAlerter.EnemyList", false, []()
			{
				if (!ChampionObjects.empty())
				{
					for (auto const ent : ChampionObjects)
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

void GankAlerter::TickLoader()
{

	if (Menu::Get<bool>("Drawings.GankAlerter.Use")) 
	{

		if (!ChampionObjects.empty())
		{
			for (auto& value : ChampionObjects)
			{
				GankChecker(&value, false);

			}
		}
	}
}


void GankAlerter::Draw(void * UserData)
{


	if (Menu::Get<bool>("Drawings.GankAlerter.Use"))
	{
		if (!ChampionObjects.empty())
		{
			for (auto& value : ChampionObjects)
			{
				GankChecker(&value, true);

			}
		}
	}
}


void GankAlerter::GankChecker(ChampionObject* value, bool isDrawing)
{
	
	if (value == nullptr || value == NULL) // campnumber 0 is plant include all kind of plants
	{
		return;
	}
	


	std::string menuID = "Drawings.GankAlerter.";

	//Menu::Get<bool>("Drawings.GankAlerter.Ally.Jungler")

	if (value->isAlly)
	{
		if (!Menu::Get<bool>("Drawings.GankAlerter.Ally.Use"))
		{
			
			return;
		}

		if (Menu::Get<bool>("Drawings.GankAlerter.Ally.Jungler") && value->isJungler == false)
		{
			return;
		}

		menuID += "Ally";
	}
	else if (!value->isAlly)
	{
		if (!Menu::Get<bool>("Drawings.GankAlerter.Enemy.Use"))
		{
			
			return;
		}

		if (Menu::Get<bool>("Drawings.GankAlerter.Enemy.Jungler") && value->isJungler == false)
		{
			return;
		}

		menuID += "Enemy";
	}


	
	std::string menuIDOrignial = menuID;

	//std::string copy = menuID;

	//copy += "Use";

	if (value->Hero != nullptr && value->Hero != NULL) // campnumber 0 is plant include all kind of plants
	{
		
		if (value->Hero->IsAlive())
		{
			//SdkUiConsoleWrite("Hello");
			menuID += ".UseFor";
			menuID += value->Hero->GetCharName();
			
			if (Menu::Get<bool>(menuID))
			{
				
				if (!isDrawing && value->Hero->IsVisible())
				{

					float distance = (value->Hero)->Distance(&Player);
					float range = (float)Menu::Get<int>("Drawings.GankAlerter.Range");
					float cooldown = (float)Menu::Get<int>("Drawings.GankAlerter.CoolTime");

					//if ((distance <= range && !Menu::Get<bool>("Drawings.GankAlerter.LastPos")) || Menu::Get<bool>("Drawings.GankAlerter.LastPos"))
					{//if (obj.Distance > range && distance <= range && Game.Time > obj.LastTrigger + cooldown)
						if (value->Distance > range && distance <= range && Game::Time() > value->LastTrigger + cooldown) //value.Distance > range &&
						{
							value->LastTrigger = Game::Time();
							auto pos = value->Hero->GetPosition();

							menuIDOrignial += ".Ping";
							if (Menu::Get<bool>(menuIDOrignial) && pos.IsValid())
							{
								menuIDOrignial += "Type";

								Game::SendPing(pos, DropLists::GetPingType(Menu::Get<int>(menuIDOrignial)), true);
								//SdkUiConsoleWrite("castedPing");
							}

						}
					}
					value->Distance = distance;

				}
				else if (isDrawing)
				{
					menuIDOrignial += ".LineUse";

					//SdkUiConsoleWrite("castedDrawing %s", menuIDOrignial);

					if (Menu::Get<bool>(menuIDOrignial))
					{
						float duration = (float)Menu::Get<int>("Drawings.GankAlerter.LineDuration");

						if (Game::Time() < value->LastTrigger + duration)
						{

							SDKVECTOR playerPos{ Player.GetPosition() };
							SDKVECTOR targetPos{ (value->Hero)->GetPosition() };

							if (playerPos.IsValid() && targetPos.IsValid())
							{
								menuIDOrignial += "Color";


								auto lineColor = DropLists::GetColor(Menu::Get<int>(menuIDOrignial));

								//Drawings.GankAlerter.LineWidth
								float width = (float)Menu::Get<int>("Drawings.GankAlerter.LineWidth");
								Draw::Line(&playerPos, &targetPos, width, &lineColor);
								//Drawings.GankAlerter.Name
								if (Menu::Get<bool>("Drawings.GankAlerter.Name"))
								{
									Draw::Text(&playerPos.Extended(targetPos, 200.0f), NULL, value->Hero->GetCharName(), "Arial", &lineColor, 24, 8);
								}
							}
						}


					}
				}


			}

		}


	}
}

