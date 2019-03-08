#include "stdafx.h"
#include "JungleTimer.h"
#include <iomanip>


std::map<unsigned int, GameObject*> JungleTimer::GameObjects;


void JungleTimer::Init()
{
	//CurrentTarget = AttackableUnit();

	//OrbTarget = NULL;

	GameObjects;

#pragma region RegisterCallbacks
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick, JungleTimer::Tick);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, JungleTimer::Update);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, JungleTimer::DrawMenu);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, JungleTimer::Draw);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::CreateObject, JungleTimer::OnCreate);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::DeleteObject, JungleTimer::OnDelete);
//	pSDK->EventHandler->RegisterCallback(CallbackEnum::BuffUpdate, JungleTimer::OnBuffCreateDelete);

	
#pragma endregion
	//JungleTimer::AblitiesAdder();
	JungleMonstersAdder();


}




std::vector<MonsterList> JungleList;
//std::vector<Monster> AliveList;
//std::vector<Monster> DeadList;

std::vector<testClass> testingClass;


void JungleMonstersAdder()
{
	JungleList.push_back(MonsterList(100.0f, 300.0f, Vector3(3800.99f, 52.18f, 7883.53f), { Monster("SRU_Blue1.1.1", true, false) }, true, false));
	JungleList.push_back(MonsterList(100.0f, 300.0f, Vector3(7762.24f, 53.96f, 4011.18f), { Monster("SRU_Red4.1.1", true, false) }, true, false));

	JungleList.push_back(MonsterList(100.0f, 150.0f, Vector3(3849.95f, 52.46f, 6504.36f), { Monster("SRU_Murkwolf2.1.1", true, false) , Monster("SRU_MurkwolfMini2.1.2", false, false), Monster("SRU_MurkwolfMini2.1.3", false, false) }, true, false));
	
	JungleList.push_back(MonsterList(100.0f, 150.0f, Vector3(3849.95f, 52.46f, 6504.36f), { Monster("SRU_Murkwolf2.1.1", true, false) , Monster("SRU_MurkwolfMini2.1.2", false, false), Monster("SRU_MurkwolfMini2.1.3", false, false) }, true, false));

	JungleList.push_back(MonsterList(100.0f, 150.0f, Vector3(3849.95f, 52.46f, 6504.36f), { Monster("SRU_Murkwolf2.1.1", true, false) , Monster("SRU_MurkwolfMini2.1.2", false, false), Monster("SRU_MurkwolfMini2.1.3", false, false) }, true, false));

	JungleList.push_back(MonsterList(100.0f, 150.0f, Vector3(2164.34f, 51.78f, 8383.02f), { Monster("SRU_Gromp13.1.1", true, false) }, true, false));


	
	//JungleList.push_back(MonsterList(100, 300, Vector3(3800.99f, 7883.53f, 52.18f), { Monster("SRU_Blue1.1.1", true) }, true));

}



///This gets called 30 times per second, put your logics in here instead of
///on OnUpdate so it doesnt drop too many fps
void JungleTimer::Tick(void * UserData)
{

	//auto gameObjects{ pSDK->EntityManager->GetParticles() };
	//GameObjects = gameObjects;
}

///This gets called X times per second, where X is your league fps.
///Use this only to update stuff you need to validade on every frame.
void JungleTimer::Update(void * UserData)
{

}

///This gets called X times per second, where X is your league fps.
///Put any drawings you need here
void JungleTimer::Draw(void * UserData)
{


	
	if (pSDK->EntityManager->GetLocalPlayer().AsAIHeroClient())
	{
		SDKVECTOR Position = pSDK->EntityManager->GetLocalPlayer().AsAIHeroClient()->GetPosition();
		//SdkGetObjectPosition(Object, &Position);
		//	auto s2 = ObjectStore.size();
		std::string str("dead :"+std::to_string(JungleList.size()));
		SDKCOLOR _g_ColorGreen = { 0, 255, 0, 255 };
		SdkDrawText(&Position, NULL, str.c_str(), "Arial Rounded MT", &_g_ColorGreen, 28, 0, 3, true);

		Position.z -= 40.0f;
		std::string str2("Alive :"+std::to_string(JungleList.size()));
		SdkDrawText(&Position, NULL, str2.c_str(), "Arial Rounded MT", &_g_ColorGreen, 28, 0, 3, true);



	}

	auto hi{ (pSDK->EntityManager->GetJungleMonsters()) };

	

	
	for (auto &[netID, Enemy] : hi)
	{
		if (1400.0f > Player.Distance(Enemy))
		{
			Vector3 pos{ Enemy->GetPosition() };

			if (pos.IsValid() && pos.IsOnScreen()) {
				//We get the screen position and offset it a little so it doesnt draw over the above text
				Vector2 screenPos{ Renderer::WorldToScreen(pos) };
				screenPos.y -= 20.0f;


				Draw::Text(NULL, &screenPos, Enemy->GetName(), "Arial Narrow", &Color::White, 20, 6);
				screenPos.y -= 20.0f;
				Draw::Text(NULL, &screenPos, "z : "+std::to_string(Enemy->GetPosition().z), "Arial Narrow", &Color::White, 20, 6);
				screenPos.y -= 20.0f;
				Draw::Text(NULL, &screenPos, "y : " + std::to_string(Enemy->GetPosition().y), "Arial Narrow", &Color::White, 20, 6);
				screenPos.y -= 20.0f;
				Draw::Text(NULL, &screenPos, "x : " + std::to_string(Enemy->GetPosition().x), "Arial Narrow", &Color::White, 20, 6);
				screenPos.y -= 20.0f;
				Draw::Text(NULL, &screenPos, std::to_string(Enemy->GetCampNumber()), "Arial Narrow", &Color::White, 20, 6);

			}
		}
	}


	for (std::vector<MonsterList>::iterator it = JungleList.begin(); it != JungleList.end(); ++it)
	{
		if (pSDK->EntityManager->GetLocalPlayer().AsAIHeroClient())
		{
			SDKVECTOR Position = pSDK->EntityManager->GetLocalPlayer().AsAIHeroClient()->GetPosition();
			std::string str("dead :" + std::to_string(JungleList.size()));
			SDKCOLOR _g_ColorGreen = { 0, 255, 0, 255 };
			Position.z -= 100.0f;
			SdkDrawText(&Position, NULL, "DID IT", "Arial Rounded MT", &_g_ColorGreen, 28, 0, 3, true);

		}


	//	auto hi2{ (pSDK->EntityManager->GetJungleMonsters()) };


		for (std::vector<Monster>::iterator it2 = (*it).Mobs.begin(); it2 != (*it).Mobs.end(); ++it2)
		{

			//for (auto &[netID, Enemy] : hi)
			{
				//if (strstr(Enemy->GetName(), (*it2).MonsterName))
				{
					//SdkUiConsoleWrite("Created %s \n", test);

					//(*it2).isDead = false;
					bool areAllDead = std::all_of((*it).Mobs.begin(),
						(*it).Mobs.end(),
						[&](const Monster stopPoint)-> bool
					{
						return (stopPoint.isDead == true);
					});

					if (areAllDead)
					{

						Vector3 pos { (*it).Position };
						if (pos.IsValid() && pos.IsOnScreen()) 
						{
							//We get the screen position and offset it a little so it doesnt draw over the above text
							Vector2 screenPos{ Renderer::WorldToScreen(pos) };
							screenPos.y += 20.0f;
							std::stringstream ss;
							ss.precision(0); //for decimal
							ss.setf(std::ios_base::fixed, std::ios_base::floatfield);

							float time = (*it).SpawnTime - Game::Time();
							int time_mins = (int)floor(time);
							int minutes = time_mins / 60;
							int secs = time_mins % 60;




							ss << "" << std::setw(2) << std::setfill('0') << minutes <<":" << std::setw(2) << std::setfill('0') << secs << "\n";

							Draw::Text(NULL, &screenPos, ss.str().c_str(), "Arial Narrow", &Color::White, 20, 6);

							Vector2 minimap{Renderer::WorldToMinimap(pos)};
							minimap.x -= 10.0f;
							minimap.y -= 10.0f;
							Draw::Text(NULL, &minimap, ss.str().c_str(), "Arial Narrow", &Color::White, 14, 6);
						}
						//SdkUiConsoleWrite("1yeah they all dead %s \n", test);

					}
					else
					{
						//SdkUiConsoleWrite("1Nope they not all dead %s \n", test);

					}
				}
			}
		}




	}

	
}







///Your menu settings go here
void JungleTimer::DrawMenu(void * UserData)
{
	UNREFERENCED_PARAMETER(UserData);





}

bool __cdecl JungleTimer::OnCreate(_In_ void* Object, _In_ unsigned int NetworkID, _In_opt_ void* UserData)
{
	//
// Is this object dead?
//

	//
	// Another 'is dead' check.
	//
//	SDK_HEALTH Health;
//	SdkGetUnitHealth(Object, &Health);
//	if (Health.Current < 1.0f)
//		return true;

	//
	// Can this object be seen in the fog of war?
	//
	//bool bInFogOfWar;
//	SdkIsUnitVisible(Object, &bInFogOfWar);
	//if (!bInFogOfWar)
	//	return true;

	SDKVECTOR Position;
	SdkGetObjectPosition(Object, &Position);



	const char* test = nullptr;

	SdkGetObjectName(Object, &test);



	//SdkGetObjectAcceleration_t
	std::string s1(test);


	if (!strstr(s1.c_str(), "Minion") && !strstr(s1.c_str(), "urret") && !strstr(s1.c_str(), "Pain"))
	{
		SDKVECTOR tempPos;
		SdkGetObjectPosition(Object, &tempPos);

		unsigned int netID;
		SdkGetObjectNetworkID(Object, &netID);


		if ((s1.c_str(), "SRU"))
		{
			SdkUiConsoleWrite("[test] time is z %s \n", test);
		}

	}
	else
	{
		return true;
	}


	for (std::vector<MonsterList>::iterator it = JungleList.begin(); it != JungleList.end(); ++it)
	{



		if (&(*it) == nullptr) //if the object is invalid, don't do anything
		{
			//SdkUiConsoleWrite("am I here\n");
		}
		else
		{
			//SdkUiConsoleWrite("[added Here2] %s  \n", pSDK->EntityManager->GetObjectFromPTR(Object)->GetName());

			//if (!(*it).settingEnabler)
			//	break;

			for (std::vector<Monster>::iterator it2 = (*it).Mobs.begin(); it2 != (*it).Mobs.end(); ++it2)
			{

				if (strstr(test, (*it2).MonsterName))
				{
					SdkUiConsoleWrite("Created %s \n", test);

					(*it2).isDead = false;
					bool areAllDead = std::all_of((*it).Mobs.begin(),
						(*it).Mobs.end(),
						[&](const Monster stopPoint)-> bool
					{
						return (stopPoint.isDead == true);
					});

					if (areAllDead)
					{
						SdkUiConsoleWrite("1yeah they all dead %s \n", test);

					}
					else
					{
						SdkUiConsoleWrite("1Nope they not all dead %s \n", test);

					}
				}
			}


		

		}
	}




	return true;
}

bool __cdecl JungleTimer::OnDelete(_In_ void* Object, _In_ unsigned int NetworkID, _In_opt_ void* UserData)
{

	SDKVECTOR Position;
	SdkGetObjectPosition(Object, &Position);



	const char* test = nullptr;

	SdkGetObjectName(Object, &test);



	//SdkGetObjectAcceleration_t
	std::string s1(test);


	if (!strstr(s1.c_str(), "Minion") && !strstr(s1.c_str(), "urret") && !strstr(s1.c_str(), "Pain"))
	{
		
		SDKVECTOR tempPos;
		SdkGetObjectPosition(Object, &tempPos);

		unsigned int netID;
		SdkGetObjectNetworkID(Object, &netID);


		if ((s1.c_str(), "SRU"))
		{
			SdkUiConsoleWrite("[test] time is z %s \n", test);
		}

		
	}
	else
	{
		return true;
	}


	for (std::vector<testClass>::const_iterator it = testingClass.begin(); it != testingClass.end(); ++it)
	{
		//(*it).testBool = true;
	}

	//SdkGetAIName()

	
	for (std::vector<MonsterList>::iterator it = JungleList.begin(); it != JungleList.end(); ++it)
	{



		//(*it).IsDead = true;

		if (&(*it) == nullptr) //if the object is invalid, don't do anything
		{
			//SdkUiConsoleWrite("am I here\n");
		}
		else
		{
			//SdkUiConsoleWrite("[added Here2] %s  \n", pSDK->EntityManager->GetObjectFromPTR(Object)->GetName());

			//if (!(*it).settingEnabler)
			//	break;


			if (!JungleList.empty())
			{
				//if ((int)(*it).Mobs.size == 1)
				{
					//(*it).Mobs.at(0).MonsterName

				}
				//else if ((int)(*it).Mobs.size() > 1)
				{

					for (std::vector<Monster>::iterator it2 = (*it).Mobs.begin(); it2 != (*it).Mobs.end(); ++it2)
					{
						if (strstr(test, (*it2).MonsterName))
						{
							(*it2).isDead = true;
							float calculateRespawnTime = Game::Time() + (*it).RespawnTime;
							(*it).SpawnTime = calculateRespawnTime - 3.0f; // 3.0f for correction
							SdkUiConsoleWrite("Deleted %s \n", test);

							bool areAllDead = std::all_of((*it).Mobs.begin(),
								(*it).Mobs.end(),
								[&](const Monster stopPoint)-> bool
							{
								return (stopPoint.isDead == true);
							});

							if (areAllDead)
							{
								SdkUiConsoleWrite("2yeah they all dead %s \n", test);

							}
							else
							{
								SdkUiConsoleWrite("2Nope they not all dead %s \n", test);

							}

						}
					}





				}

				//i

			}


			

		}
	}

	





	




}

