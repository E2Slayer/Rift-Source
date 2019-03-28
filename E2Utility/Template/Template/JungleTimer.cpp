#include "stdafx.h"
#include "JungleTimer.h"


void JungleTimer::Init()
{






}

void JungleTimer::MenuLoader()
{
	Menu::Tree("Inhibitor Timer", "Trackers.InhibitorTimer", false, []()
	{
		Menu::Checkbox("Use Inhibitor Timer", "Trackers.InhibitorTimer.Use", true);

		Menu::Checkbox("Use on Ally Inhibitors", "Trackers.InhibitorTimer.Ally", true);
		Menu::Checkbox("Use on Enemy Inhibitors", "Trackers.InhibitorTimer.Enemy", true);

		/*
		Menu::Checkbox("Draw Timers on the World", "Trackers.InhibitorTimer.World", true);
		Menu::SliderInt("World Font Height", "Trackers.InhibitorTimer.World.FontSize", 28, 20, 40);
		Menu::SliderInt("World Font Width", "Trackers.InhibitorTimer.World.FontSize2", 10, 4, 15);
		Menu::DropList("^-> World Color", "Trackers.InhibitorTimer.World.Color", ColorMenuList, 11);
		Menu::DropList("^-> World OutLine Color", "Trackers.AbilityTimer.World.OutLineColor", ColorMenuList, 0);

		Menu::Checkbox("Draw Timers on the Minimap", "Trackers.InhibitorTimer.Minimap", true);
		Menu::SliderInt("Minimap Font Height", "Trackers.InhibitorTimer.Minimap.FontSize", 13, 10, 30);
		Menu::SliderInt("Minimap Font Width", "Trackers.InhibitorTimer.Minimap.FontSize2", 4, 1, 10);
		Menu::DropList("^-> Minimap Color", "Trackers.InhibitorTimer.Minimap.Color", ColorMenuList, 11);
		Menu::DropList("^-> Minimap OutLine Color", "Trackers.AbilityTimer.Minimap.OutLineColor", ColorMenuList, 0);
		Menu::SliderInt("Minimap Timer Position X-axis", "Trackers.AbilityTimer.Minimap.DrawingX", 0, -50, 50);
		Menu::SliderInt("Minimap Timer Position Y-axis", "Trackers.AbilityTimer.Minimap.DrawingY", -0, -50, 50);
		*/


	});
}

void JungleTimer::TickLoader()
{

	if (!Menu::Get<bool>("Trackers.InhibitorTimer.Use"))
	{

		return;
	}
	




}

void JungleTimer::DrawLoader()
{


	if (!Menu::Get<bool>("Trackers.InhibitorTimer.Use"))
	{

		return;
	}

	
	auto jungle{ pSDK->EntityManager->GetJungleMonsters(5500.0f, &Player.GetPosition()) };


	Draw::Text(&Player.GetPosition(), NULL, std::to_string(Game::Time()), "Arial", &Color::Green, 30, 10, 0, false);

	for (auto[netID, value] : jungle)
	{

		//if ( value->GetPosition().IsOnScreen() )
		{
			Draw::Text(&value->GetPosition(), NULL, value->GetName(), "Arial", &Color::White, 30, 10, 0, false);

			auto temp = value->GetPosition();

			temp.y += 50.0f;

			Draw::Text(&temp, NULL, std::to_string(value->GetSpawnTime()), "Arial", &Color::Red, 30, 10, 0, false);

			if (value->IsAlive())
			{
				temp.y += 50.0f;

				Draw::Text(&temp, NULL, std::to_string(value->GetHealth().Current), "Arial", &Color::Red, 30, 10, 0, false);
			}

			if (value->IsAttacking())
			{
				temp.y += 50.0f;

				Draw::Text(&temp, NULL, "Attacking !", "Arial", &Color::Green, 30, 10, 0, false);
			}
		}

		//value->GetName();

	}



	auto enemy{ pSDK->EntityManager->GetEnemyHeroes() };


	for (auto[netID, value] : enemy)
	{

		if (value->GetPosition().IsOnScreen() && value->IsValid())
		{
			Draw::Text(&value->GetPosition(), NULL, value->GetName(), "Arial", &Color::White, 30, 10, 0, false);

	
		}

		//value->GetName();

	}

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