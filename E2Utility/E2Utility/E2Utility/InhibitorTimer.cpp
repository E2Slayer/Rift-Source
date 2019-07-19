#include "stdafx.h"
#include "InhibitorTimer.h"
#include "DrawHelper.h"
#include <iomanip>
#include "DropLists.h"
//std::map<unsigned int, AttackableUnit*> InhibitorList; //Actual Map for ability list that is detemined by champion list in the match

std::vector< InhibitorObject> InhibitorList;
DWORD inhibitLastChecker = 0;

void InhibitorTimer::Init()
{
	InhibitorList.clear();
	inhibitLastChecker = 0;


	auto inhibits{ pSDK->EntityManager->GetInhibitors() };

	if (inhibits.empty())
	{
		return;
	}

	//InhibitorList = inhibits;

	for (auto &[netID, value] : inhibits)
	{
		InhibitorList.emplace_back(InhibitorObject(value));
	}


	
}

void InhibitorTimer::MenuLoader()
{
	Menu::Tree("Inhibitor Timer", "Trackers.InhibitorTimer", false, []()
	{
		Menu::Checkbox("Use Inhibitor Timer", "Trackers.InhibitorTimer.Use", true);

		Menu::Checkbox("Use on Ally Inhibitors", "Trackers.InhibitorTimer.Ally", true);
		Menu::Checkbox("Use on Enemy Inhibitors", "Trackers.InhibitorTimer.Enemy", true);

		Menu::Checkbox("Draw Timers on the World", "Trackers.InhibitorTimer.World", true);
		Menu::SliderInt("World Font Height", "Trackers.InhibitorTimer.World.FontSize", 28, 20, 40);
		Menu::SliderInt("World Font Width", "Trackers.InhibitorTimer.World.FontSize2", 10, 4, 15);
		Menu::DropList("^-> World Color", "Trackers.InhibitorTimer.World.Color", ColorMenuList, 11);
		Menu::DropList("^-> World OutLine Color", "Trackers.InhibitorTimer.World.OutLineColor", ColorMenuList, 0);

		Menu::Checkbox("Draw Timers on the Minimap", "Trackers.InhibitorTimer.Minimap", true);
		Menu::SliderInt("Minimap Font Height", "Trackers.InhibitorTimer.Minimap.FontSize", 13, 10, 30);
		Menu::SliderInt("Minimap Font Width", "Trackers.InhibitorTimer.Minimap.FontSize2", 4, 1, 10);
		Menu::DropList("^-> Minimap Color", "Trackers.InhibitorTimer.Minimap.Color", ColorMenuList, 11);
		Menu::DropList("^-> Minimap OutLine Color", "Trackers.InhibitorTimer.Minimap.OutLineColor", ColorMenuList, 0);
		Menu::SliderInt("Minimap Timer Position X-axis", "Trackers.InhibitorTimer.Minimap.DrawingX", 0, -50, 50);
		Menu::SliderInt("Minimap Timer Position Y-axis", "Trackers.InhibitorTimer.Minimap.DrawingY", -0, -50, 50);



	});
}

void InhibitorTimer::TickLoader()
{

	if (!Menu::Get<bool>("Trackers.InhibitorTimer.Use"))
	{

		return;
	}
	if (inhibitLastChecker + 800 > GetTickCount())
	{
		return;
	}


	inhibitLastChecker = GetTickCount();

	if (InhibitorList.empty())
	{
		return;
	}



	for (auto &value : InhibitorList)
	{
		if (value.Object->GetHealth().Current > 0.0f)
		{
			value.LastHealth = value.Object->GetHealth().Current;
			value.Destroyed = false;
		}
		else if (!value.Destroyed && value.LastHealth > 0.0f && value.Object->GetHealth().Current <= 0.0f)
		{
			value.Destroyed = true;
			value.NextRespawnTime = Game::Time() + value.RespawnTime;
		}
	}

	
}

void InhibitorTimer::DrawLoader()
{


	if (!Menu::Get<bool>("Trackers.InhibitorTimer.Use"))
	{

		return;
	}

	/*
	auto inhibits{ pSDK->EntityManager->GetInhibitors() };

	if (inhibits.empty())
	{
		return;
	}



	for (auto &[netID, value] : inhibits)
	{

		if (value->GetPosition().IsOnScreen() && value->GetHealth().Current == 0.0f)
		{
			//SdkUiConsoleWrite("Draw %f", value->GetHealth().Current);
			
			//auto timeleft = (value->GetDeathTime() + 300.0f) - Game::Time();

			Vector2 screenPos{ Renderer::WorldToMinimap(value->GetPosition()) };

			DrawHelper::DrawOutlineText(NULL, &screenPos, "test2", "Calibri Bold", &Color::White, 30, 10, 0,
				&Color::Black, false);
		}

	}
	*/


	if (Game::IsOverlayOpen())
	{

		auto inhi = pSDK->EntityManager->GetInhibitors(1000.0f, &Player.GetPosition());

		for (auto &[netID, value] : inhi)
		{
			if (value->GetPosition().IsOnScreen() && value->GetPosition().IsValid() && Menu::Get<bool>("Trackers.InhibitorTimer.World"))
			{
				Vector2 screenPos{ Renderer::WorldToScreen(value->GetPosition()) };

				DrawHelper::DrawOutlineText(NULL, &screenPos, "00:00", "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Trackers.InhibitorTimer.World.Color")), Menu::Get<int>("Trackers.InhibitorTimer.World.FontSize"), Menu::Get<int>("Trackers.InhibitorTimer.World.FontSize2"), 0,
					&DropLists::GetColor(Menu::Get<int>("Trackers.InhibitorTimer.World.OutLineColor")), false);
			}


			if (Menu::Get<bool>("Trackers.InhibitorTimer.Minimap"))
			{
				Vector2 screenPos2{ Renderer::WorldToMinimap(value->GetPosition()) };
				screenPos2.x += (float)Menu::Get<int>("Trackers.InhibitorTimer.Minimap.DrawingX") - 8.0f;
				screenPos2.y += (float)Menu::Get<int>("Trackers.InhibitorTimer.Minimap.DrawingY") - 7.0f;

				DrawHelper::DrawOutlineText(NULL, &screenPos2, "00:00", "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Trackers.InhibitorTimer.Minimap.Color")), Menu::Get<int>("Trackers.InhibitorTimer.Minimap.FontSize"), Menu::Get<int>("Trackers.InhibitorTimer.Minimap.FontSize2"), 0,
					&DropLists::GetColor(Menu::Get<int>("Trackers.InhibitorTimer.Minimap.OutLineColor")), false);


			}
		}


	}
	else
	{

		if (InhibitorList.empty())
		{
			return;
		}


		for (auto &value : InhibitorList)
		{

			if (value.Destroyed && value.NextRespawnTime > Game::Time() && value.Object->GetHealth().Current == 0.0f)
			{
				//Trackers.InhibitorTimer.Ally
				if (value.Object->IsAlly() && Menu::Get<bool>("Trackers.InhibitorTimer.Ally") || !value.Object->IsAlly() && Menu::Get<bool>("Trackers.InhibitorTimer.Enemy"))
				{



					std::stringstream ss1;
					ss1.precision(1); //for decimal
					ss1.setf(std::ios_base::fixed, std::ios_base::floatfield);


					int sec = value.NextRespawnTime - Game::Time();

					int mins = sec / 60;
					sec = sec % 60;

					ss1 << std::setfill('0') << std::setw(2) << mins << ":" << std::setfill('0') << std::setw(2) << sec;

					//auto timeleft = value.NextRespawnTime - Game::Time();
					if (value.Position.IsOnScreen() && value.Position.IsValid() && Menu::Get<bool>("Trackers.InhibitorTimer.World"))
					{
						Vector2 screenPos{ Renderer::WorldToScreen(value.Position) };

						DrawHelper::DrawOutlineText(NULL, &screenPos, ss1.str().c_str(), "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Trackers.InhibitorTimer.World.Color")), Menu::Get<int>("Trackers.InhibitorTimer.World.FontSize"), Menu::Get<int>("Trackers.InhibitorTimer.World.FontSize2"), 0,
							&DropLists::GetColor(Menu::Get<int>("Trackers.InhibitorTimer.World.OutLineColor")), false);
					}


					if (Menu::Get<bool>("Trackers.InhibitorTimer.Minimap"))
					{
						Vector2 screenPos2{ Renderer::WorldToMinimap(value.Object->GetPosition()) };
						screenPos2.x += (float)Menu::Get<int>("Trackers.InhibitorTimer.Minimap.DrawingX") - 8.0f;
						screenPos2.y += (float)Menu::Get<int>("Trackers.InhibitorTimer.Minimap.DrawingY") - 7.0f;

						DrawHelper::DrawOutlineText(NULL, &screenPos2, ss1.str().c_str(), "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Trackers.InhibitorTimer.Minimap.Color")), Menu::Get<int>("Trackers.InhibitorTimer.Minimap.FontSize"), Menu::Get<int>("Trackers.InhibitorTimer.Minimap.FontSize2"), 0,
							&DropLists::GetColor(Menu::Get<int>("Trackers.InhibitorTimer.Minimap.OutLineColor")), false);


					}
				}


			}
		}
	}


	/*
	
	if (InhibitorList.empty())
	{
		return;
	}


	for (auto &value : InhibitorList)
	{
		if (value.Destroyed && value.NextRespawnTime > Game::Time() )
		{
			auto timeleft = value.NextRespawnTime - Game::Time();
			if (value.Position.IsOnScreen() && value.Position.IsValid())
			{
				Vector2 screenPos{ Renderer::WorldToScreen(value.Position) };

				DrawHelper::DrawOutlineText(NULL, &screenPos, "testetes", "Calibri Bold", &Color::White, 30, 10, 0,
					&Color::Black, false);
			}

			
			
			Vector2 screenPos{ Renderer::WorldToMinimap(value.Object->GetPosition()) };

			DrawHelper::DrawOutlineText(NULL, &screenPos, std::to_string(timeleft).c_str(), "Calibri Bold", &Color::White, 30, 10, 0, &Color::Black, false);
			

		}
	}
	
	*/
	//Vector2 screenPos{ Renderer::WorldToScreen(Player.GetPosition()) };
	//screenPos.x += (float)Menu::Get<int>("Trackers.AbilityTimer.DrawingX");
	//screenPos.y += (float)Menu::Get<int>("Trackers.AbilityTimer.DrawingY");

	//DrawHelper::DrawOutlineText(NULL, &screenPos, "0.0", "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Trackers.AbilityTimer.Color")), Menu::Get<int>("Trackers.AbilityTimer.FontSize"), 10, 0,
		//&DropLists::GetColor(Menu::Get<int>("Trackers.AbilityTimer.OutLineColor")), false);

}