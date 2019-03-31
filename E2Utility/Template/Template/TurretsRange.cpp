#include "stdafx.h"
#include "TurretsRange.h"



std::map<unsigned int, AITurretClient*> AllyTurretList;
std::map<unsigned int, AITurretClient*> EnemyTurretList;

std::vector< AITurretClient*> FountainList;
const float FountainRange = 1350.0f;
const float TurretRange = 875.0f;

void TurretsRange::Init()
{
	AllyTurretList.clear();
	EnemyTurretList.clear();
	FountainList.clear();

	auto allyList { pSDK->EntityManager->GetAllyTurrets() };

	if (!allyList.empty())
	{
		for (auto[netId, turret] : allyList)
		{
			if (turret->IsValid())
			{
				if ( (strstr(turret->GetName(), "Turret_OrderTurretShrine_A") && turret->IsAlly()) || (strstr(turret->GetName(), "Turret_ChaosTurretShrine") && turret->IsAlly()))
				{
					FountainList.emplace_back(turret);
				}
				else
				{
					AllyTurretList.emplace(netId, turret);
				}

			}
		}

	}


	auto enemyList{ pSDK->EntityManager->GetEnemyTurrets() };

	if (!enemyList.empty())
	{
		for (auto[netId, turret] : enemyList)
		{
			if (turret->IsValid())
			{
				if ((strstr(turret->GetName(), "Turret_OrderTurretShrine_A") && !turret->IsAlly()) || (strstr(turret->GetName(), "Turret_ChaosTurretShrine") && !turret->IsAlly()))
				{
					FountainList.emplace_back(turret);
				}
				else
				{
					EnemyTurretList.emplace(netId, turret);
				}

			}
		}

	}







	/*
	if (strstr(turret->GetName(), "Turret_OrderTurretShrine_A") && turret->IsAlly() || strstr(turret->GetName(), "Turret_ChaosTurretShrine") && turret->IsAlly())
			{


				if (turret->GetPosition().IsValid() )
				{
					fountainLocation = turret->GetPosition();
				}
			}
	
	*/
}

void TurretsRange::MenuLoader()
{
	Menu::Tree("Turrets Range", "Drawings.TurretsRange", false, []()
	{
		Menu::Checkbox("Use Turrets Range", "Drawings.TurretsRange.Use", true);



		Menu::Checkbox("Draw Ally Turrets Range", "Drawings.TurretsRange.Ally", true);
		Menu::DropList("^-> Color No One Under Ally Turrets Range", "Drawings.TurretsRange.Ally.NotInRange.Color", ColorMenuList, 6);
		Menu::Checkbox("Use Different Color If # Enemy under Ally Turrets", "Drawings.TurretsRange.Ally.ChangeColor", true);
		Menu::DropList("^-> Color Enemy Under Ally Turrets Range", "Drawings.TurretsRange.Ally.InRange.ChangeColor", ColorMenuList, 10);
		Menu::SliderInt("^-> Minimum Enemy #", "Drawings.TurretsRange.Ally.MinimumEnemy", 1, 1, 5);


		Menu::Checkbox("Draw Enemy Turrets Range", "Drawings.TurretsRange.Enemy", true);
		Menu::Checkbox("Draw Enemy Turrets Range Only Visible", "Drawings.TurretsRange.Enemy.Visible", false);
		Menu::DropList("^-> Color Not Under Enemy Turrets Range", "Drawings.TurretsRange.Enemy.NotInRange.Color", ColorMenuList, 6);
		Menu::Checkbox("Use Different Color If Me under Ally Turrets", "Drawings.TurretsRange.Enemy.ChangeColor", true);
		Menu::DropList("^-> Color Me Under Enemy Turrets Range", "Drawings.TurretsRange.Enemy.InRange.Color", ColorMenuList, 10);


		Menu::Checkbox("Draw Ally Fountain Turret Range", "Drawings.TurretsRange.AllyFountain", false);
		Menu::DropList("^-> Color Ally Fountain Turret Range", "Drawings.TurretsRange.AllyFountain.Color", ColorMenuList, 6);
		Menu::Checkbox("Draw Enemy Fountain Turret Range", "Drawings.TurretsRange.EnemyFountain", true);
		Menu::DropList("^-> Color Enemy Fountain Turret Range", "Drawings.TurretsRange.EnemyFountain.Color", ColorMenuList, 10);

	});
}

void TurretsRange::TickLoader()
{

}

void TurretsRange::DrawLoader()
{

	if (!Menu::Get<bool>("Drawings.TurretsRange.Use"))
	{
		return;
	}


	/*
	                 t.IsValid && !t.IsDead && t.Health > 1f && t.IsVisible &&
                            (t.IsAlly && drawAlly || t.IsEnemy && drawEnemy) && t.Position.IsOnScreen(TurretRange)))
	*/

	if (Menu::Get<bool>("Drawings.TurretsRange.Ally") && !AllyTurretList.empty())
	{
		for (auto[netId, turret] : AllyTurretList)
		{
			if (turret->IsValid() && turret->IsAlive() && turret->IsVisible() && turret->IsAlly() && turret->GetPosition().IsOnScreen(TurretRange))
			{
				if (Menu::Get<bool>("Drawings.TurretsRange.Ally.ChangeColor") && turret->IsVisible())
				{
					auto EnemyNumber{ pSDK->EntityManager->GetEnemyHeroes(TurretRange, &turret->GetPosition()) };

					if (EnemyNumber.size() >= Menu::Get<int>("Drawings.TurretsRange.Ally.MinimumEnemy"))
					{
						Draw::Circle(&turret->GetPosition(), TurretRange, &DropLists::GetColor(Menu::Get<int>("Drawings.TurretsRange.Ally.InRange.ChangeColor")));
					}
					else
					{
						Draw::Circle(&turret->GetPosition(), TurretRange, &DropLists::GetColor(Menu::Get<int>("Drawings.TurretsRange.Ally.NotInRange.Color")));
					}

				}
				else if (!Menu::Get<bool>("Drawings.TurretsRange.Ally.ChangeColor"))
				{
					Draw::Circle(&turret->GetPosition(), TurretRange, &DropLists::GetColor(Menu::Get<int>("Drawings.TurretsRange.Ally.NotInRange.Color")));
				}
			}
		}
	}


	if (Menu::Get<bool>("Drawings.TurretsRange.Enemy") && !EnemyTurretList.empty())
	{
		for (auto[netId, turret] : EnemyTurretList)
		{
			if (turret->IsValid() && turret->IsAlive() && !turret->IsAlly() && turret->GetPosition().IsOnScreen(TurretRange))
			{
				if ( (Menu::Get<bool>("Drawings.TurretsRange.Enemy.Visible") && turret->IsVisible()) || !Menu::Get<bool>("Drawings.TurretsRange.Enemy.Visible"))
				{

					auto turretLocation = turret->GetPosition();
					if (Player.GetPosition().Distance(turretLocation) <= TurretRange)
					{
						Draw::Circle(&turret->GetPosition(), TurretRange, &DropLists::GetColor(Menu::Get<int>("Drawings.TurretsRange.Enemy.InRange.Color")));
						//Drawings.TurretsRange.Enemy.Visible
					}
					else
					{
						Draw::Circle(&turret->GetPosition(), TurretRange, &DropLists::GetColor(Menu::Get<int>("Drawings.TurretsRange.Enemy.NotInRange.Color")));
						//Draw::Circle(&turret->GetPosition(), TurretRange, &DropLists::GetColor(Menu::Get<int>("Drawings.TurretsRange.Enemy.InRange.Color")));
					}

				}

			}
		}
	}



	/*
	
		Menu::Checkbox("Draw Ally Fountain Turret Range", "Drawings.TurretsRange.AllyFountain", false);
		Menu::DropList("^-> Color Ally Fountain Turret Range", "Drawings.TurretsRange.AllyFountain.Color", ColorMenuList, 6);
		Menu::Checkbox("Draw Enemy Fountain Turret Range", "Drawings.TurretsRange.EnemyFountain", true);
		Menu::DropList("^-> Color Enemy Fountain Turret Range", "Drawings.TurretsRange.EnemyFountain.Color", ColorMenuList, 10);
	
	*/

	if (!FountainList.empty())
	{

		for (auto &turret : FountainList)
		{
			if (turret->IsValid() && turret->IsAlive() && turret->GetPosition().IsOnScreen(FountainRange))
			{
				Vector3 adjustHeight = Vector3(turret->GetPosition().x, 95.0f, turret->GetPosition().z);
				if (turret->IsAlly() && Menu::Get<bool>("Drawings.TurretsRange.AllyFountain"))
				{
					Draw::Circle(&adjustHeight, FountainRange, &DropLists::GetColor(Menu::Get<int>("Drawings.TurretsRange.AllyFountain.Color")));
				}
				else if (!turret->IsAlly() && Menu::Get<bool>("Drawings.TurretsRange.EnemyFountain"))
				{

					Draw::Circle(&adjustHeight, FountainRange, &DropLists::GetColor(Menu::Get<int>("Drawings.TurretsRange.EnemyFountain.Color")));
				}

			}
		}

	}
}
