#include "stdafx.h"
#include "JungleTimer.h"
#include <string.h>
#include "DropLists.h"
#include <iomanip>
#include "DrawHelper.h"

std::vector<JungleCamp> jungleCamp;

std::vector< JungleCampExploit> jungleExploit;

int dragonStacks = 0;
DWORD JunglerChecker = 0;


void JungleTimer::Init()
{
	jungleCamp.clear();
	jungleExploit.clear();
	dragonStacks = 0;
	JunglerChecker = 0;

	/*
	jungleCamp.emplace_back(90.0f, 300.0f, 
		Vector3(7764.0f, 53.96f, 4022.00f), 
		std::vector<JungleMinion> 
	{
		"SRU_Red4.1.1"
	}); // blue sided red
	*/

	jungleCamp.emplace_back(90.0f, 150.0f, 
		Vector3(6962.71f, 50.31f, 5354.35f),
		std::vector<JungleMinion> 
	{ 
		{"SRU_Razorbeak3.1.1"}, {"SRU_RazorbeakMini3.1.2"}, { "SRU_RazorbeakMini3.1.3" }, { "SRU_RazorbeakMini3.1.4" }, { "SRU_RazorbeakMini3.1.5" }, { "SRU_RazorbeakMini3.1.6" }
	}); // blue sided wraith


	jungleCamp.emplace_back(102.0f, 150.0f,
		Vector3(8482.47f, 50.64f, 2705.94f),
		std::vector<JungleMinion>
	{
		{"SRU_Krug5.1.1"}, { "SRU_KrugMini5.1.2" }
	}); // blue sided golem


	jungleCamp.emplace_back(90.0f, 150.0f,
		Vector3(3780.67f, 52.46f, 6443.98f),
		std::vector<JungleMinion>
	{
		{"SRU_Murkwolf2.1.1"}, { "SRU_MurkwolfMini2.1.2" }, { "SRU_MurkwolfMini2.1.3" }
	}); // blue sided wolf

	/*
	jungleCamp.emplace_back(90.0f, 300.0f,
		Vector3(3822.00f, 52.03f, 7900.00f),
		std::vector<JungleMinion>
	{
		"SRU_Blue1.1.1"
	}); // blue sided blue SRU_Blue1.1.1 3822.00f, 52.03f, 7900.00f
	*/

	jungleCamp.emplace_back(102.0f, 150.0f,
		Vector3(2110.62f, 51.77f, 8450.98f),
		std::vector<JungleMinion>
	{
		"SRU_Gromp13.1.1"
	}); // blue sided gromp





	/*
	jungleCamp.emplace_back(90.0f, 300.0f,
		Vector3(7101.86f, 56.28f, 10900.54f),
		std::vector<JungleMinion>
	{
		"SRU_Red10.1.1"
	}); // purple sided red
	*/

	jungleCamp.emplace_back(90.0f, 150.0f,
		Vector3(7854.38f, 52.26f, 9610.47f),
		std::vector<JungleMinion>
	{
		{"SRU_Razorbeak9.1.1"}, { "SRU_RazorbeakMini9.1.2" }, { "SRU_RazorbeakMini9.1.3" }, { "SRU_RazorbeakMini9.1.4" }, { "SRU_RazorbeakMini9.1.5" }, { "SRU_RazorbeakMini9.1.6" }
	}); // purple sided wraith


	jungleCamp.emplace_back(102.0f, 150.0f,
		Vector3(6317.09f, 56.47f, 12146.45f),
		std::vector<JungleMinion>
	{
		{"SRU_Krug11.1.1"}, { "SRU_KrugMini11.1.2" }
	}); // purple sided golem


	jungleCamp.emplace_back(90.0f, 150.0f,
		Vector3(11008.15f, 62.09f, 8387.40f),
		std::vector<JungleMinion>
	{
		{"SRU_Murkwolf8.1.1"}, { "SRU_MurkwolfMini8.1.2" }, { "SRU_MurkwolfMini8.1.3" }
	}); // purple sided wolf


	/*
	jungleCamp.emplace_back(90.0f, 300.0f,
		Vector3(11031.72f, 51.72f, 6990.84f),
		std::vector<JungleMinion>
	{
		"SRU_Blue7.1.1"
	}); // purple sided blue SRU_Blue1.1.1 3822.00f, 52.03f, 7900.00f
	*/

	jungleCamp.emplace_back(102.0f, 150.0f,
		Vector3(12703.62f, 51.69f, 6443.98f),
		std::vector<JungleMinion>
	{
		"SRU_Gromp14.1.1"
	}); // purple sided gromp


	/*
	jungleCamp.emplace_back(102.0f, 1200.0f,
		Vector3(4930.12f, -71.24f, 10371.44f),
		std::vector<JungleMinion>
	{
		"SRU_RiftHerald17.1.1"
	}); // hearld

	jungleCamp.emplace_back(1200.0f, 360.0f,
		Vector3(5007.12f, -71.24f, 10471.44f),
		std::vector<JungleMinion>
	{
		"SRU_Baron12.1.1"
	}); // hearld
	*/
	//SRU_Baron12.1.1 5007.12f, -71.24f, 10471.44f



	//11031.72f, 51.72f, 6990.84f


	jungleExploit.emplace_back(90.0f, 300.0f,
		Vector3(3821.48f, 51.12f, 8101.05f), "CampRespawn"); //blue side Blue Golem

	jungleExploit.emplace_back(90.0f, 300.0f,
		Vector3(7762.24f, 53.96f, 4011.18f), "CampRespawn"); //blue side Red 

	jungleExploit.emplace_back(90.0f, 300.0f,
		Vector3(11131.72f, 51.72f, 6990.84f), "CampRespawn"); //purple side Blue Golem

	jungleExploit.emplace_back(90.0f, 300.0f,
		Vector3(7066.86f, 56.18f, 10975.54f), "CampRespawn"); //purple side Red



	jungleExploit.emplace_back(120.0f, 150.0f,
		Vector3(10500.0f, -62.81f, 5170.0f), "CampRespawn"); //dragon side crab




	jungleExploit.emplace_back(120.0f, 150.0f,
		Vector3(4400.0f, -66.53f, 9600.00f), "CampRespawn"); //baron side crab

	jungleExploit.emplace_back(300.0f, 300.0f,
		Vector3(9866.14f, -71.24f, 4414.01f), "CampRespawn"); //Dragon


	jungleExploit.emplace_back(598.0f, 360.0f,
		Vector3(5007.12f, -71.24f, 10471.44f), "CampRespawn"); //Herald and Baron

	//5007.12f, -71.24f, 10471.44f





	//CampRespawn 3821.48f, 51.12f, 8101.05f Blue Sided Blue

	//SRU_Gromp13.1.1 2110.62f, 51.77f, 8450.98f

	//SRU_Murkwolf2.1.1 3780.67f, 52.46f, 6443.98f
	//SRU_Krug5.1.1 8482.47f, 50.64f, 2705.94f
	//SRU_Razorbeak3.1.1 6962.71f, 50.31f, 5354.35f

	//SRU_Red4.1.1 7764.0f, 53.96f, 4022.00f


	pSDK->EventHandler->RegisterCallback(CallbackEnum::CreateObject, JungleTimer::OnCreate);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::DeleteObject, JungleTimer::OnDelete);


}

void JungleTimer::MenuLoader()
{
	Menu::Tree("Jungle Timer", "Trackers.JungleTimer", false, []()
	{
		Menu::Checkbox("Use Jungle Timer", "Trackers.JungleTimer.Use", true);



		//Menu::Checkbox("Use on Ally Inhibitors", "Trackers.InhibitorTimer.Ally", true);
		//Menu::Checkbox("Use on Enemy Inhibitors", "Trackers.InhibitorTimer.Enemy", true);

		
		Menu::Checkbox("Draw Jungle Timer on the World", "Trackers.JungleTimer.World", true);
		Menu::SliderInt("Jungle Timer World Font Height", "Trackers.JungleTimer.World.FontSize", 28, 20, 40);
		Menu::SliderInt("Jungle Timer World Font Width", "Trackers.JungleTimer.World.FontSize2", 10, 4, 15);
		Menu::DropList("^-> Jungle Timer World Color", "Trackers.JungleTimer.World.Color", ColorMenuList, 11);
		Menu::DropList("^-> Jungle Timer World OutLine Color", "Trackers.JungleTimer.World.OutLineColor", ColorMenuList, 0);

		Menu::Checkbox("Draw Jungle Timer on the Minimap", "Trackers.JungleTimer.Minimap", true);
		Menu::SliderInt("Jungle Timer Minimap Font Height", "Trackers.JungleTimer.Minimap.FontSize", 13, 10, 30);
		Menu::SliderInt("Jungle Timer Minimap Font Width", "Trackers.JungleTimer.Minimap.FontSize2", 4, 1, 10);
		Menu::DropList("^-> Jungle Timer Minimap Color", "Trackers.JungleTimer.Minimap.Color", ColorMenuList, 11);
		Menu::DropList("^-> Jungle Timer Minimap OutLine Color", "Trackers.JungleTimer.Minimap.OutLineColor", ColorMenuList, 0);
		Menu::SliderInt("Jungle Timer Minimap Timer Position X-axis", "Trackers.JungleTimer.Minimap.DrawingX", 0, -50, 50);
		Menu::SliderInt("Jungle Timer Minimap Timer Position Y-axis", "Trackers.JungleTimer.Minimap.DrawingY", -0, -50, 50);
		


	});
}

void JungleTimer::TickLoader()
{

	if (!Menu::Get<bool>("Trackers.JungleTimer.Use"))
	{

		return;
	}
	


	if (JunglerChecker + 800 > GetTickCount())
	{
		return;
	}

	JunglerChecker = GetTickCount();

	int dragonstackTemp = 0;

	auto getEnemy{ pSDK->EntityManager->GetEnemyHeroes() };

	if (!getEnemy.empty())
	{
		for (auto[netID, enemy] : getEnemy)
		{

		}
	}

	//Player.GetBuffCount();

	/*
	if (_lastCheck + CheckInterval > Environment.TickCount)
                {
                    return;
                }

                _lastCheck = Environment.TickCount;

                var dragonStacks = 0;
                foreach (var enemy in GameObjects.EnemyHeroes)
                {
                    var buff =
                        enemy.Buffs.FirstOrDefault(
                            b => b.Name.Equals("s5test_dragonslayerbuff", StringComparison.OrdinalIgnoreCase));
                    if (buff != null)
                    {
                        dragonStacks = buff.Count;
                    }
                }

                if (dragonStacks > _dragonStacks || dragonStacks == 5)
                {
                    var dCamp = _camps.FirstOrDefault(c => c.Mobs.Any(m => m.Name.Contains("Dragon")));
                    if (dCamp != null && !dCamp.Dead)
                    {
                        dCamp.Dead = true;
                        dCamp.NextRespawnTime = (int) Game.Time + dCamp.RespawnTime;
                    }
                }

                _dragonStacks = dragonStacks;

                var bCamp = _camps.FirstOrDefault(c => c.Mobs.Any(m => m.Name.Contains("Baron")));
                if (bCamp != null && !bCamp.Dead)
                {
                    var heroes = GameObjects.EnemyHeroes.Where(e => e.IsVisible);
                    foreach (var hero in heroes)
                    {
                        var buff =
                            hero.Buffs.FirstOrDefault(
                                b => b.Name.Equals("exaltedwithbaronnashor", StringComparison.OrdinalIgnoreCase));
                        if (buff != null)
                        {
                            bCamp.Dead = true;
                            bCamp.NextRespawnTime = (int) buff.StartTime + bCamp.RespawnTime;
                        }
                    }
                }
	*/





	/*
	for (auto &camp : jungleCamp)
	{

		

		if (camp.SpawnTime  > Game::Time())
		{
			continue;
		}
		

		
		auto jungle{ pSDK->EntityManager->GetJungleMonsters(1000.0f, &camp.Position) };


		for (auto &minions : camp.Minions)
		{
			minions.DeadTick = true;
		}
	//	SdkUiConsoleWrite("size : %f enf %d", camp.RespawnTimer ,jungle.size());

		int i = 0;
		for (auto &currentJungle : jungle)
		{
			if (( strstr(currentJungle.second->GetName(), "SRU_") == 0 || strstr(currentJungle.second->GetName(), "Sru_") == 0)) // && currentJungle.second->IsVisible()
			{
			//	SdkUiConsoleWrite("11111111111111");
				


				for (auto &minions : camp.Minions)
				{
					
					if (_stricmp(minions.Name, currentJungle.second->GetName()) == 0)
					{
						
						if ( currentJungle.second->GetHealth().Current > 0.0f )
						{
							//SdkUiConsoleWrite("Areyou 3");
							minions.DeadTick = false;
							i++;
						}
					}
				}

			}

		}
		//SdkUiConsoleWrite("size : %f enf %d i %d", camp.RespawnTimer, jungle.size(), i);




		if (i == 0 )
		{
			//SdkUiConsoleWrite("Areyou %d res %f", i , camp.SpawnTime);
			for (auto &minions : camp.Minions)
			{
				minions.Dead = minions.DeadTick;
			}

			

				camp.SpawnTime = Game::Time() + camp.RespawnTimer;
			
				//camp.SpawnTime = Game::Time() + camp.RespawnTimer;
			
		}

	}*/

}

void JungleTimer::DrawLoader()
{


	if (!Menu::Get<bool>("Trackers.JungleTimer.Use"))
	{

		return;
	}




	/*
	auto buffs
	{
		Player.GetBuffs()
	};
	Vector3 pos{ Player.GetPosition() };
	Vector2 screenPos{ Renderer::WorldToScreen(pos) };
	for (auto buffname : buffs)
	{
		if (pos.IsValid() && pos.IsOnScreen())
		{
			//We get the screen position and offset it a little so it doesnt draw over the above text
			//Vector2 screenPos{ Renderer::WorldToScreen(pos) };

		//	buffname.Count;

			std::string temp = buffname.Name;

			temp += " : ";

			temp += std::to_string(buffname.Stacks);
			temp += " : ";

			temp += std::to_string(buffname.Count);
			//temp += " : ";

			//temp += std::to_string(buffname.Spell->CurrentAmmo);

			screenPos.y -= 30.0f;
			Draw::Text(NULL, &screenPos, temp, "Arial Narrow", &Color::White, 30, 10);

		}
	}
	*/
	//Player.GetBuff(dragonbuff_tooltipmanager, false).Spell.


	/*
	
	dragonbufffire
dragonbuffearth
dragonbuffair
dragonbuffwater
*/

	//dragonbuff_tooltipmanager
	/*
	int complete = Player.GetBuffStacks("dragonbuffwater", false) + Player.GetBuffStacks("dragonbuffair", false) + Player.GetBuffStacks("dragonbuffearth", false) + Player.GetBuffStacks("dragonbufffire", false);
	screenPos.x -= 120.0f;
	Draw::Text(NULL, &screenPos, std::to_string(complete), "Arial Narrow", &Color::Green, 30, 6);
	*/

	/*
	 foreach (var camp in _camps.Where(c => c.Dead))
                {
                    if (camp.NextRespawnTime - Game.Time <= 0)
                    {
                        camp.Dead = false;
                        continue;
                    }

                    if (mapEnabled && camp.Position.IsOnScreen())
                    {
                        _mapText.DrawTextCentered(
                            (camp.NextRespawnTime - (int) Game.Time).FormatTime(mapTotalSeconds),
                            Drawing.WorldToScreen(camp.Position), Color.White);
                    }
                    if (minimapEnabled)
                    {
                        _minimapText.DrawTextCentered(
                            (camp.NextRespawnTime - (int) Game.Time).FormatTime(minimapTotalSeconds),
                            camp.MinimapPosition, Color.White);
                    }
                }
	*/

	

	for (auto &camp : jungleCamp)
	{
		bool Alive = false;


		for (auto &minions : camp.Minions)
		{
			if (minions.Dead == false)
			{
				//SdkUiConsoleWrite("SP2 %s", minions.Name);
				Alive = true;
				break;
			}
	
		}

		if (Alive == true)
		{
			continue;
		}

		auto timeLeft = camp.SpawnTime - Game::Time();


		if (timeLeft > 0.0f)
		{
			//Trackers.InhibitorTimer.Ally
		



				std::stringstream ss1;
				ss1.precision(1); //for decimal
				ss1.setf(std::ios_base::fixed, std::ios_base::floatfield);


				int sec = timeLeft;

				int mins = sec / 60;
				sec = sec % 60;

				ss1 << std::setfill('0') << std::setw(2) << mins << ":" << std::setfill('0') << std::setw(2) << sec;

				//auto timeleft = value.NextRespawnTime - Game::Time();
				if (camp.Position.IsOnScreen() && camp.Position.IsValid() && Menu::Get<bool>("Trackers.JungleTimer.World"))
				{
					Vector2 screenPos{ Renderer::WorldToScreen(camp.Position) };

					DrawHelper::DrawOutlineText(NULL, &screenPos, ss1.str().c_str(), "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Trackers.JungleTimer.World.Color")), Menu::Get<int>("Trackers.JungleTimer.World.FontSize"), Menu::Get<int>("Trackers.JungleTimer.World.FontSize2"), 0,
						&DropLists::GetColor(Menu::Get<int>("Trackers.JungleTimer.World.OutLineColor")), false);
				}


				if (Menu::Get<bool>("Trackers.InhibitorTimer.Minimap"))
				{
					Vector2 screenPos2{ Renderer::WorldToMinimap(camp.Position) };
					screenPos2.x += (float)Menu::Get<int>("Trackers.JungleTimer.Minimap.DrawingX") - 8.0f;
					screenPos2.y += (float)Menu::Get<int>("Trackers.JungleTimer.Minimap.DrawingY") - 7.0f;

					DrawHelper::DrawOutlineText(NULL, &screenPos2, ss1.str().c_str(), "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Trackers.JungleTimer.Minimap.Color")), Menu::Get<int>("Trackers.JungleTimer.Minimap.FontSize"), Menu::Get<int>("Trackers.JungleTimer.Minimap.FontSize2"), 0,
						&DropLists::GetColor(Menu::Get<int>("Trackers.JungleTimer.Minimap.OutLineColor")), false);


				}
			


		}



		/*
		if (camp.Position.IsOnScreen() && camp.Position.IsValid() && timeLeft > 0.0f)
		{
			Draw::Text(&camp.Position, NULL, std::to_string(timeLeft), "Arial", &Color::Green, 30, 10, 0, false);
		}
		*/

	}




	for (auto &camp : jungleExploit)
	{

		auto timeLeft = camp.SpawnTime - Game::Time();

		if (timeLeft > 0.0f)
		{
			std::stringstream ss1;
			ss1.precision(1); //for decimal
			ss1.setf(std::ios_base::fixed, std::ios_base::floatfield);


			int sec = timeLeft;

			int mins = sec / 60;
			sec = sec % 60;

			ss1 << std::setfill('0') << std::setw(2) << mins << ":" << std::setfill('0') << std::setw(2) << sec;

			//auto timeleft = value.NextRespawnTime - Game::Time();
			if (camp.Position.IsOnScreen() && camp.Position.IsValid() && Menu::Get<bool>("Trackers.JungleTimer.World"))
			{
				Vector2 screenPos{ Renderer::WorldToScreen(camp.Position) };

				DrawHelper::DrawOutlineText(NULL, &screenPos, ss1.str().c_str(), "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Trackers.JungleTimer.World.Color")), Menu::Get<int>("Trackers.JungleTimer.World.FontSize"), Menu::Get<int>("Trackers.JungleTimer.World.FontSize2"), 0,
					&DropLists::GetColor(Menu::Get<int>("Trackers.JungleTimer.World.OutLineColor")), false);
			}


			if (Menu::Get<bool>("Trackers.InhibitorTimer.Minimap"))
			{
				Vector2 screenPos2{ Renderer::WorldToMinimap(camp.Position) };
				screenPos2.x += (float)Menu::Get<int>("Trackers.JungleTimer.Minimap.DrawingX") - 8.0f;
				screenPos2.y += (float)Menu::Get<int>("Trackers.JungleTimer.Minimap.DrawingY") - 7.0f;

				DrawHelper::DrawOutlineText(NULL, &screenPos2, ss1.str().c_str(), "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Trackers.JungleTimer.Minimap.Color")), Menu::Get<int>("Trackers.JungleTimer.Minimap.FontSize"), Menu::Get<int>("Trackers.JungleTimer.Minimap.FontSize2"), 0,
					&DropLists::GetColor(Menu::Get<int>("Trackers.JungleTimer.Minimap.OutLineColor")), false);


			}
		}


		/*
		if (camp.Position.IsOnScreen() && camp.Position.IsValid() && timeLeft > 0.0f)
		{
			Draw::Text(&camp.Position, NULL, std::to_string(timeLeft), "Arial", &Color::Grey, 30, 10, 0, false);
		}
		*/


	}

	/*
	auto jungle{ pSDK->EntityManager->GetJungleMonsters(5500.0f, &Player.GetPosition()) };


	Draw::Text(&Player.GetPosition(), NULL, std::to_string(Game::Time()), "Arial", &Color::Green, 30, 10, 0, false);

	for (auto[netID, value] : jungle)
	{

		//if ( value->GetPosition().IsOnScreen() )
		{
			auto temp = value->GetPosition();
			temp.y += 250.0f;
			Draw::Text(&temp, NULL, value->GetName(), "Arial", &Color::White, 30, 10, 0, false);


			temp.y += 50.0f;

			Draw::Text(&temp, NULL, std::to_string(value->GetSpawnTime()), "Arial", &Color::Red, 30, 10, 0, false);

			if (value->IsAlive())
			{
				temp.y += 50.0f;

				Draw::Text(&temp, NULL, std::to_string(value->GetPosition().x), "Arial", &Color::Red, 30, 10, 0, false);
				temp.y += 50.0f;

				Draw::Text(&temp, NULL, std::to_string(value->GetPosition().y), "Arial", &Color::Green, 30, 10, 0, false);
				temp.y += 50.0f;

				Draw::Text(&temp, NULL, std::to_string(value->GetPosition().z), "Arial", &Color::Orange, 30, 10, 0, false);
			}

			if (value->IsAttacking())
			{
				temp.y += 50.0f;

				Draw::Text(&temp, NULL, "Attacking !", "Arial", &Color::Green, 30, 10, 0, false);
			}
		}

		//value->GetName();

	}
	*/

	
	/*
	auto part{ pSDK->EntityManager->GetParticles(3000.0f, &Player.GetPosition()) };


	for (auto[netID, value] : part)
	{

		if (value->GetPosition().IsOnScreen())
		{
			Draw::Text(&value->GetPosition(), NULL, value->GetName(), "Arial", &Color::Orange, 30, 10, 0, false);


		}

		//value->GetName();

	}*/
}


bool JungleTimer::OnCreate(void* Object, unsigned int NetworkID, void* UserData)
{
	UNREFERENCED_PARAMETER(UserData);
	UNREFERENCED_PARAMETER(NetworkID);

	if (!Menu::Get<bool>("Trackers.JungleTimer.Use"))
	{

		return false;
	}


	auto sender = pSDK->EntityManager->GetObjectFromPTR(Object);
	if (Object == nullptr || sender == nullptr || sender == NULL || NetworkID == 0)
	{
		return false;
	}

	auto objectName = sender->GetName();


	auto objectPos = sender->GetPosition();

	if (objectName == NULL || objectName == nullptr || !objectPos.IsValid())
	{
		return false;
	}




	//const char *p = strstr(objectName, "SRU_");
	const char *m = strstr(objectName, "Minion");
	const char *t = strstr(objectName, "Turret");
	
	if (m || t)
	{
		return false;
	}
	


	
	//SdkUiConsoleWrite(" caaaaaaa respawned %s", objectName);
	for (auto &camp : jungleCamp)
	{
		bool AllAlive = false;
		bool AllDead = false;

		for (auto &minions : camp.Minions)
		{
			if (_stricmp(minions.Name, objectName) == 0)
			{
				minions.Dead = false;
				//SdkUiConsoleWrite("respawned %s", objectName);
				
			}

		}


	

	}
	
	auto herald = Vector3(5007.12f, -71.24f, 10471.44f);

	for (auto &camp : jungleExploit)
	{
		if (Game::Time() < 1200.0f && _stricmp(camp.Name, objectName) == 0 && sender->GetPosition().Distance(camp.Position) < 100.0f)
		{
			if (sender->GetPosition().Distance(herald) < 100.0f)
			{
				camp.SpawnTime = 1200.0f;
				continue;
			}
			
		}
		
		if (_stricmp(camp.Name, objectName) == 0 && sender->GetPosition().Distance(camp.Position) < 100.0f)
		{

			camp.SpawnTime = sender->AsAIMinionClient()->GetSpawnTime() + camp.RespawnTimer;
				
			//SdkUiConsoleWrite("exploit %s", objectName);
			camp.Created = true;

				
		}

	
	}


	//jungleExploit

	//SdkUiConsoleWrite("AbilityTimer Create %s", objectName);
	//SdkUiConsoleWrite("AbilityTimer Create %s", findSkin);




	return true;
}

bool JungleTimer::OnDelete(void* Object, unsigned int NetworkID, void* UserData)
{
	UNREFERENCED_PARAMETER(UserData);
	UNREFERENCED_PARAMETER(NetworkID);

	if (!Menu::Get<bool>("Trackers.JungleTimer.Use"))
	{

		return false;
	}


	auto sender = pSDK->EntityManager->GetObjectFromPTR(Object);
	if (Object == nullptr || sender == nullptr || sender == NULL || NetworkID == 0)
	{
		return false;
	}

	auto objectName = sender->GetName();


	auto objectPos = sender->GetPosition();

	if (objectName == NULL || objectName == nullptr || !objectPos.IsValid())
	{
		return false;
	}


	const char *m = strstr(objectName, "Minion");
	const char *t = strstr(objectName, "Turret");

	if (m || t)
	{
		return false;
	}

	//SdkUiConsoleWrite("dead %s", objectName);


	for (auto &camp : jungleCamp)
	{


		for (auto &minions : camp.Minions)
		{
			auto jungle{ pSDK->EntityManager->GetJungleMonsters(1000.0f, &camp.Position) };


			if (_stricmp(minions.Name, objectName) == 0)
			{
				minions.Dead = true;
				//SdkUiConsoleWrite("dead %s", objectName);
				//float spt = Game::Time() + camp.RespawnTimer;
				camp.SpawnTime = Game::Time() + camp.RespawnTimer - 4.0f;
			}

		}

		

	}


	for (auto &camp : jungleExploit)
	{


		if (_stricmp(camp.Name, objectName) == 0 && camp.Created == true && sender->GetPosition().Distance(camp.Position) < 100.0f)
		{

			//SdkUiConsoleWrite("exploit deleted %s", objectName);
			camp.Created = false;
		}


	}
}