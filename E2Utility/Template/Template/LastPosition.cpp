#include "stdafx.h"
#include "LastPosition.h"
#include "DropLists.h"
#include "DrawHelper.h"
#include "resource.h"
#include <iomanip>
#include <string.h>
#include <math.h>

std::vector<LastPositionStruct> lastPositions;

Vector3 zeroVector = Vector3(0.0f, 0.0f, 0.0f);



Vector3 EnemyFountainLocation = Vector3(0.0f, 0.0f, 0.0f);

void LastPosition::Init()
{
	lastPositions.clear();

	EnemyFountainLocation = Vector3(0.0f, 0.0f, 0.0f);

	auto turrets = pSDK->EntityManager->GetEnemyTurrets();

	if (turrets.empty())
	{
		return;
	}

	for (auto &[netid, turret] : turrets)
	{
		if (netid && turret)
		{
			if (turret->GetName() == NULL)
			{
				continue;
			}

			if (strstr(turret->GetName(), "Turret_OrderTurretShrine_A") && !turret->IsAlly())
			{
				if (turret->GetPosition().IsValid())
				{
					EnemyFountainLocation = Vector3(398.191f, 182.859f, 417.234f);
				}
			}
			else if (strstr(turret->GetName(), "Turret_ChaosTurretShrine") && !turret->IsAlly())
			{
				if (turret->GetPosition().IsValid())
				{
					EnemyFountainLocation = Vector3(14303.477f, 171.978f, 14382.285f);
				}
			}


		}

	}




	auto enemyList{ pSDK->EntityManager->GetEnemyHeroes() };

	for (auto[netID, value] : enemyList)
	{
		lastPositions.emplace_back(LastPositionStruct(value, EnemyFountainLocation));
		//	SdkUiConsoleWrite("did");
	}


	pSDK->EventHandler->RegisterCallback(CallbackEnum::Recall, LastPosition::RecallTrack);

}

void LastPosition::MenuLoader()
{
	Menu::Tree("Last Position", "Trackers.LastPosition", false, []()
	{
		Menu::Checkbox("Use Last Position", "Trackers.LastPosition.Use", true);

		Menu::Tree("World Settings", "Trackers.LastPosition.World", false, []()
		{
			Menu::Checkbox("Draw Icon on the World", "Trackers.LastPosition.World.Icon", true);
			Menu::Checkbox("Draw Recall Icon on the World", "Trackers.LastPosition.World.Recall", true);
			Menu::Checkbox("Draw Missing Timer on the World", "Trackers.LastPosition.World.Timer", true);
			Menu::SliderInt("World Missing Timer Font Height", "Trackers.LastPosition.World.FontSize", 20, 10, 40);
			Menu::SliderInt("World Missing Timer Font Width", "Trackers.LastPosition.World.FontSize2", 4, 1, 10);
			Menu::DropList("^-> World Missing Timer Color", "Trackers.LastPosition.World.Color", ColorMenuList, 11);
			Menu::DropList("^-> World Missing Timer OutLine Color", "Trackers.LastPosition.World.OutLineColor", ColorMenuList, 0);
			Menu::SliderInt("World Missing Timer Position X-axis", "Trackers.LastPosition.World.DrawingX", 0, -50, 50);
			Menu::SliderInt("World Missing Timer Position Y-axis", "Trackers.LastPosition.World.DrawingY", -0, -50, 50);

			Menu::Checkbox("Draw Expanding Circle on the World", "Trackers.LastPosition.World.Expand.Use", true);
			Menu::SliderInt("World Expanding Circle Maximum Range", "Trackers.LastPosition.World.Expand.MaxRange", 5000, 1000, 8000);
			//Menu::SliderInt("World Expanding Circle Minimum Time (Seconds)", "Trackers.LastPosition.World.Expand.Time", 3, 1, 10);
			Menu::SliderInt("World Expanding Circle Thickness", "Trackers.LastPosition.World.Expand.Thickness", 2, 1, 10);
			Menu::SliderInt("World Expanding Circle Quality", "Trackers.LastPosition.World.Expand.Quality", 30, 1, 100);
			Menu::DropList("^-> World Expanding Circle Color", "Trackers.LastPosition.World.Expand.Color", ColorMenuList, 6);
		});


		Menu::Tree("Minimap Settings", "Trackers.LastPosition.Minimap", false, []()
		{
			Menu::Checkbox("Draw Icon on the Minimap", "Trackers.LastPosition.Minimap.Icon", true);
			Menu::Checkbox("Draw Recall Icon on the Minimap", "Trackers.LastPosition.Minimap.Recall", true);
			Menu::Checkbox("Draw Missing Timer on the Minimap", "Trackers.LastPosition.Minimap.Timer", true);
			Menu::SliderInt("Minimap Missing Timer Font Height", "Trackers.LastPosition.Minimap.FontSize", 20, 10, 40);
			Menu::SliderInt("Minimap Missing Timer Font Width", "Trackers.LastPosition.Minimap.FontSize2", 4, 1, 10);
			Menu::DropList("^-> Minimap Missing Timer Color", "Trackers.LastPosition.Minimap.Color", ColorMenuList, 11);
			Menu::DropList("^-> Minimap Missing Timer OutLine Color", "Trackers.LastPosition.Minimap.OutLineColor", ColorMenuList, 0);
			Menu::SliderInt("Minimap Missing Timer Position X-axis", "Trackers.LastPosition.Minimap.DrawingX", 0, -50, 50);
			Menu::SliderInt("Minimap Missing Timer Position Y-axis", "Trackers.LastPosition.Minimap.DrawingY", -0, -50, 50);

			Menu::Checkbox("Draw Expanding Circle on the World", "Trackers.LastPosition.Minimap.Expand.Use", true);
			Menu::SliderInt("World Expanding Circle Maximum Range", "Trackers.LastPosition.Minimap.Expand.MaxRange", 5000, 1000, 8000);
			//Menu::SliderInt("World Expanding Circle Minimum Time (Seconds)", "Trackers.LastPosition.Minimap.Expand.Time", 3, 1, 10);
			Menu::SliderInt("World Expanding Circle Thickness", "Trackers.LastPosition.Minimap.Expand.Thickness", 2, 1, 10);
			Menu::SliderInt("World Expanding Circle Quality", "Trackers.LastPosition.Minimap.Expand.Quality", 30, 1, 100);
			Menu::DropList("^-> World Expanding Circle Color", "Trackers.LastPosition.Minimap.Expand.Color", ColorMenuList, 6);
		});





	});
}

void LastPosition::TickLoader()
{
	if (!Menu::Get<bool>("Trackers.LastPosition.Use"))
	{

		return;
	}



	float currentTime = Game::Time();


	for (auto &lp : lastPositions)
	{

		auto lpPos = lp.Unit->GetPosition();
		if (lp.Unit->IsAlive()  && lp.LastPosition.Distance(lpPos) > 500.0f)
		{
			lp.Teleported = false;
			lp.LastSeen = currentTime;

		}
		lp.LastPosition = lpPos;

		if (lp.Unit->IsVisible())
		{
			lp.Teleported = false;
			lp.AbortedTimeDifference = 0.0f;
			lp.TeleportStartTime = 0.0f;
			lp.DrawCheck = false;
			if (lp.Unit->IsAlive())
			{
				lp.LastSeen = currentTime;

			}
		}

		if (!lp.Unit->IsAlive())
		{
			lp.LastSeen = lp.Unit->GetDeathDuration() + currentTime + 1.0f;
		}

		if (!lp.Unit->IsVisible() && lp.Unit->IsAlive())
		{
			//Vector3 pos = Vector3(0.0f, 0.0f, 0.0f);
			if (lp.Teleported)
			{
				lp.DrawPosition = EnemyFountainLocation;
				
			}
			else if (!lp.Teleported)
			{
				lp.DrawPosition = lp.LastPosition;
			}

			lp.DrawCheck = true;

			//Vector2 mpPos = { Renderer::WorldToMinimap(pos) };

			//Vector2 mPos = { Renderer::WorldToScreen(pos) };

		
			

			

		}
		else
		{
			lp.DrawCheck = false;

		}

	}

}

void LastPosition::DrawLoader()
{
	if (!Menu::Get<bool>("Trackers.LastPosition.Use"))
	{

		return;
	}


	//DrawCircleMinimap(Player.GetPosition(), 1000.0f, &Color::Green, 1.0f, 30);

	//DrawCircleMap(Player.GetPosition(), 1000.0f, &Color::Red, 3.0f, 100);



	Vector2 mpPos;
	Vector2 mPos;
	float currentTime = Game::Time();
	for (auto &lp : lastPositions)
	{
		mpPos = { Renderer::WorldToMinimap(lp.DrawPosition) };

		mPos = { Renderer::WorldToScreen(lp.DrawPosition) };

		if (lp.DrawCheck )
		{
			if (lp.LastSeen != 0.0f && currentTime - lp.LastSeen > 3.0f)
			{
				float tempTime = (currentTime)-lp.AbortedTimeDifference;
				float tempLastSeen = lp.LastSeen;
				if (lp.isTeleporting)
				{
					if (lp.TeleportStartTime - lp.AbortedTimeDifference > 0.0f)
					{
						tempTime = lp.TeleportStartTime - lp.AbortedTimeDifference;
					}
					else
					{
						tempTime = currentTime;
					}
				}
				if (tempTime < 0.0f)
				{
					tempTime = currentTime;
				}
				lp.Radius = std::abs((tempTime - tempLastSeen - 1.0f)*lp.Unit->GetMovementSpeed()*1.1f);
			}

			if (lp.DrawPosition.IsOnScreen(float(Menu::Get<int>("Trackers.LastPosition.World.Expand.MaxRange"))) && Menu::Get<bool>("Trackers.LastPosition.World.Expand.Use") && lp.Radius <= Menu::Get<int>("Trackers.LastPosition.World.Expand.MaxRange")) //map
			{
				//Draw::Circle(&pos, radius, &Color::White, 0, &Vector3(100.0f,100.0f,100.0f));
				DrawHelper::DrawCircleMap(lp.DrawPosition, lp.Radius, &DropLists::GetColor(Menu::Get<int>("Trackers.LastPosition.World.Expand.Color")), float(Menu::Get<int>("Trackers.LastPosition.World.Expand.Thickness")), Menu::Get<int>("Trackers.LastPosition.World.Expand.Quality"));

			}

			if (Menu::Get<bool>("Trackers.LastPosition.Minimap.Expand.Use") && lp.Radius <= Menu::Get<int>("Trackers.LastPosition.Minimap.Expand.MaxRange")) //minimap
			{
				DrawHelper::DrawCircleMinimap(lp.DrawPosition, lp.Radius, &DropLists::GetColor(Menu::Get<int>("Trackers.LastPosition.Minimap.Expand.Color")), float(Menu::Get<int>("Trackers.LastPosition.Minimap.Expand.Thickness")), Menu::Get<int>("Trackers.LastPosition.Minimap.Expand.Quality"));
			}


			auto time = currentTime - lp.LastSeen;
			std::stringstream ss1;
			ss1.precision(1); //for decimal
			ss1.setf(std::ios_base::fixed, std::ios_base::floatfield);

			int sec = time;
			int mins = sec / 60;
			sec = sec % 60;
			int onlySec = time;
			ss1 << std::setfill('0') << std::setw(2) << onlySec;
			if (lp.DrawPosition.IsOnScreen(50.0f) & Menu::Get<bool>("Trackers.LastPosition.World.Timer"))
			{
				Vector2 tempPos = mPos;
				tempPos.x += -4.0f + float(Menu::Get<int>("Trackers.LastPosition.World.DrawingX"));
				tempPos.y += 5.0f + float(Menu::Get<int>("Trackers.LastPosition.World.DrawingY"));
				DrawHelper::DrawOutlineText(NULL, &tempPos, ss1.str().c_str(), "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Trackers.LastPosition.World.Color")), Menu::Get<int>("Trackers.LastPosition.World.FontSize"), Menu::Get<int>("Trackers.LastPosition.World.FontSize2"), 0,
					&DropLists::GetColor(Menu::Get<int>("Trackers.LastPosition.World.OutLineColor")), false);
			}

			if (Menu::Get<bool>("Trackers.LastPosition.Minimap.Timer"))
			{
				Vector2 tempPos = mpPos;
				tempPos.x += -3.0f + float(Menu::Get<int>("Trackers.LastPosition.Minimap.DrawingX"));
				tempPos.y += 15.0f + float(Menu::Get<int>("Trackers.LastPosition.Minimap.DrawingY"));
				DrawHelper::DrawOutlineText(NULL, &tempPos, ss1.str().c_str(), "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Trackers.LastPosition.Minimap.Color")), Menu::Get<int>("Trackers.LastPosition.Minimap.FontSize"), Menu::Get<int>("Trackers.LastPosition.Minimap.FontSize2"), 0,
					&DropLists::GetColor(Menu::Get<int>("Trackers.LastPosition.Minimap.OutLineColor")), false);
			}
		
		
			if (lp.DrawPosition.IsOnScreen(50.0f) && Menu::Get<bool>("Trackers.LastPosition.World.Icon")) //map
			{

				SdkDrawSpriteFromResource(MAKEINTRESOURCEA(lp.ChampIMG), &mPos, true);

			}

			if (Menu::Get<bool>("Trackers.LastPosition.Minimap.Icon")) //minimap
			{
				SdkDrawSpriteFromResource(MAKEINTRESOURCEA(lp.ChampIMG), &mpPos, true);
			}

			if (lp.isTeleporting)
			{
				if (lp.DrawPosition.IsOnScreen(50.0f) && Menu::Get<bool>("Trackers.LastPosition.World.Recall")) //map
				{

					SdkDrawSpriteFromResource(MAKEINTRESOURCEA(LP_Teleport), &mPos, true);
				}

				if (Menu::Get<bool>("Trackers.LastPosition.Minimap.Recall")) //minimap
				{
					SdkDrawSpriteFromResource(MAKEINTRESOURCEA(LP_Teleport), &mpPos, true);
				}
			}
		}
	}

}

void __cdecl LastPosition::RecallTrack(void * Unit, const char * Name, const char * Type, void * UserData)
{
	UNREFERENCED_PARAMETER(UserData);
	
	if (!Menu::Get<bool>("Trackers.LastPosition.Use"))
	{
		UNREFERENCED_PARAMETER(Unit);
		UNREFERENCED_PARAMETER(Name);
		UNREFERENCED_PARAMETER(Type);
		return;
	}

	//SdkUiConsoleWrite("Start %s %s", Name, Type);
	auto sender = pSDK->EntityManager->GetObjectFromPTR(Unit);
	if (Unit == nullptr || sender == nullptr || sender == NULL || !sender->IsHero() || sender->IsAlly())
	{
		return;
	}

	auto netID = sender->GetNetworkID();


	if (!netID)
	{
		return;
	}

	auto testStruct = TeleportDecode::TeleportDecoderFunction(Unit, Name, Type);

	//SdkUiConsoleWrite("here");

	/*
	std::vector<EnemyObject> _enemyObject;

	*/

	float currentTime = Game::Time();

	for (auto &value : lastPositions)
	{
		if (value.Unit->GetNetworkID() == netID)
		{
			if (testStruct.Status == TeleportTypes::Start)
			{
				value.isTeleporting = true;
				value.TeleportStartTime = currentTime;
				//value.Aborted = false;
				//value.AbortedTime = 0.0f;
			}
			else if (testStruct.Status == TeleportTypes::Abort)
			{

				value.isTeleporting = false;

				//auto temp = value.AbortedTimeDifference;
				
				value.AbortedTimeDifference += (currentTime - value.TeleportStartTime);
				
				//value.AbortedTimeDifference = (Game::Time() - value.AbortedTimeDifference) - value.TeleportStartTime;
				//value.Aborted = true;
				//value.AbortedTime = Game::Time();
				//value.LastSeen = Game::Time();
			}
			else if (testStruct.Status == TeleportTypes::Finished)
			{
				value.Teleported = true;
				value.isTeleporting = false;
				//value.Aborted = false;
				value.LastSeen = currentTime;
				//value.AbortedTime = 0.0f;
				value.AbortedTimeDifference = 0.0f;
				value.TeleportStartTime = 0.0f;
				value.DrawPosition = EnemyFountainLocation;
			}

		}
	}


}


