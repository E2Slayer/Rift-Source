#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include "DashJumpTracker.h"
#include "DropLists.h"
#include "DrawHelper.h"

std::vector< DashJumpObject> Destinations;


std::vector<DashJumpMenu> DashJumpList
{ 
	DashJumpMenu("Ezreal", "EzrealE", "Ezreal E"), DashJumpMenu("Vayne", "VayneTumble", "Vayne Q(Invisible)"), DashJumpMenu("Kassadin", "RiftWalk", "Kassadin R"), DashJumpMenu("Shaco", "Deceive", "Shaco Q")
	, DashJumpMenu("LeeSin", "BlindMonkWOne", "LeeSin W"), DashJumpMenu("Talon", "TalonE", "Talon E"), DashJumpMenu("Leblanc", "LeblancW", "Leblanc W"), DashJumpMenu("Leblanc", "LeblancRW", "Leblanc RW")
	, DashJumpMenu("Katarina", "KatarinaEWrapper", "Katarina E"), DashJumpMenu("Zed", "ZedW", "Zed W (Swap)"), DashJumpMenu("Zed", "ZedR", "Zed R (Swap)")
	//KatarinaEWrapper
};

const float CheckInterval = 300.0f;
float _lastCheck = 0.0f;


void DashJumpTracker::Init()
{
	Destinations.clear();
	_lastCheck = 0.0f;


//	auto enemy1{ pSDK->EntityManager->GetEnemyHeroes() };


	auto enemy1{ pSDK->EntityManager->GetEnemyHeroes() };

	if (!enemy1.empty())
	{
		for (auto &[netID, hero] : enemy1)
		{
			if (hero != nullptr && hero != NULL)
			{
				//SdkUiConsoleWrite("Inside %s", hero->GetCharName());

				const char* ss = hero->GetSpell((unsigned char)SpellSlot::Summoner1).ScriptName;

				const char* ss2 = hero->GetSpell((unsigned char)SpellSlot::Summoner2).ScriptName;

				if (ss != nullptr || ss2 != nullptr)
				{
					if (_stricmp(ss, "SummonerFlash") == 0 )
					{
						Destinations.emplace_back(DashJumpObject(hero, &hero->GetSpell((unsigned char)SpellSlot::Summoner1)));
						//SdkUiConsoleWrite("flashed added 1");

					}
					else if (_stricmp(ss2, "SummonerFlash") == 0)
					{
						Destinations.emplace_back(DashJumpObject(hero, &hero->GetSpell((unsigned char)SpellSlot::Summoner2)));
						//SdkUiConsoleWrite("flashed added 2");

					}
				}

				for (auto const &value : DashJumpList)
				{
					if (_stricmp(hero->GetCharName(), value.ChampName) == 0)
					{
						auto slot = hero->GetSpellSlotFromName(value.SpellName);

	

						//slot = (unsigned char)SpellSlot::E;

						//SdkUiConsoleWrite("Inside Slot %d", slot);
						if (int(slot) >= 0 && int(slot) < 4)
						{
							//SdkUiConsoleWrite("Inside2 %s", hero->GetSpell(slot).ScriptName);
							Destinations.emplace_back(DashJumpObject(hero, &hero->GetSpell(slot)));
						}
					}

				}

				/*
				auto slot = hero->GetSpellSlotFromName("EzrealE");


				SdkUiConsoleWrite("Inside %d", hero->GetSpellSlotFromName("EzrealE"));

				if (int(slot) >= 0 && int(slot) < 4)
				{
					Destinations.emplace_back(DashJumpObject(hero, &hero->GetSpell(slot)));
				}
				*/
			
			}
		}
	}


	pSDK->EventHandler->RegisterCallback(CallbackEnum::SpellCastStart, DashJumpTracker::SpellCastStart);


}

void DashJumpTracker::MenuLoader()
{
	Menu::Tree("DashJump Tracker", "Trackers.DashJumpTracker", false, []()
	{
		Menu::Checkbox("Use DashJump Tracker", "Trackers.DashJumpTracker.Use", true);
		
		Menu::Checkbox("Draw a Line to the Arrival Position", "Trackers.DashJumpTracker.Line", true);
		Menu::DropList("^-> DashJump Tracker Text Color", "Trackers.DashJumpTracker.LineColor", ColorMenuList, 11);
		Menu::SliderInt("^-> Tracker Line Width", "Trackers.DashJumpTracker.LineWidth", 5, 1, 10);

		Menu::Checkbox("Draw a Circle on the Arrival Position", "Trackers.DashJumpTracker.Circle", true);
		Menu::DropList("^-> DashJump Tracker Circle Color", "Trackers.DashJumpTracker.CircleColor", ColorMenuList, 11);
		Menu::SliderInt("^-> DashJump Tracker Range", "Trackers.DashJumpTracker.CircleRange", 150, 100, 300);

		Menu::Checkbox("Display Champion Name on the Tracker Line", "Trackers.DashJumpTracker.ChampName", true);
		Menu::SliderInt("^-> Tracker Font Size", "Trackers.DashJumpTracker.FontSize", 30, 20, 40);
		Menu::DropList("^-> DashJump Tracker Text Color", "Trackers.DashJumpTracker.TextColor", ColorMenuList, 11);
		Menu::DropList("^-> DashJump Tracker Text OutLine Color", "Trackers.AbilityTimer.OutLineColor", ColorMenuList, 0);


		/*
		Menu::Checkbox("Don't Use Last Position Checker", "Drawings.GankAlerter.LastPos", false);
		SdkUiText("^-> If you turn it on, Gank Alerter is triggered every time a tracked champion is visible again        ");
		SdkUiText("^-> Might be little bit annoying but some people prefer it so I added it");
		*/


		/*
			{"Ezreal", "EzrealE"} , {"Vayne", "VayneTumble"} , {"Kassadin", "RiftWalk"} , {"Katarina", "KatarinaEWrapper"}, {"Shaco", "Deceive"} 
	, {"LeeSin", "BlindMonkWOne"}, {"Talon", "TalonE"} , {"Leblanc", "LeblancW"}, {"Leblanc", "LeblancRW"}, {"Lissandra", "LissandraE"}
	, {"Zed", "ZedW"}, {"Zed", "ZedR"}
		*/

		Menu::Tree("DashJump List", "Trackers.DashJumpTracker.List", false, []()
		{
			for (auto const &value : DashJumpList)
			{
				Menu::Checkbox(("Track "+std::string(value.MenuID)).c_str(), "Trackers.DashJumpTracker.List." + std::string(value.SpellName), true);
			}
			Menu::Checkbox("Track Flash", "Trackers.DashJumpTracker.List.Flash", true);
		});



	});

}

void DashJumpTracker::TickLoader()
{
	if (!Menu::Get<bool>("Trackers.DashJumpTracker.Use"))
	{

		return;
	}

	if (_lastCheck + CheckInterval > GetTickCount())
	{
		return;
	}
	_lastCheck = GetTickCount();


	for (auto &value : Destinations)
	{
		if (value.Casted)
		{
			if (Game::Time() > value.TimeCasted + 2.0f || !value.Hero->IsAlive())
			{
				value.Casted = false;
			}

			if (value.Hero->IsVisible())
			{
				value.EndPos = value.Hero->GetPosition();
			}
		}
	}

}

void DashJumpTracker::DrawLoader()
{

	if (!Menu::Get<bool>("Trackers.DashJumpTracker.Use"))
	{
		return;
	}

	if (Destinations.empty())
	{
		return;
	}

	for (auto &value : Destinations)
	{
		if (value.Casted && value.StartPos.IsOnScreen() && value.EndPos.IsOnScreen())
		{
			Vector2 startpos{ Renderer::WorldToScreen(value.StartPos) };
			Vector2 endpos{ Renderer::WorldToScreen(value.EndPos) };


			if (Menu::Get<bool>("Trackers.DashJumpTracker.Line"))
			{
				Draw::LineScreen(&startpos, &endpos, (float)Menu::Get<int>("Trackers.DashJumpTracker.LineWidth"), &DropLists::GetColor(Menu::Get<int>("Trackers.DashJumpTracker.LineColor")));
			}

			if (Menu::Get<bool>("Trackers.DashJumpTracker.Circle"))
			{
				Draw::Circle(&value.EndPos, (float)Menu::Get<int>("Trackers.DashJumpTracker.CircleRange"), &DropLists::GetColor(Menu::Get<int>("Trackers.DashJumpTracker.CircleColor")), 0, &SDKVECTOR(100.0f, 100.0f, 100.0f));
			}

			/*
					Menu::Checkbox("Draw a Line to the Arrival Position", "Trackers.DashJumpTracker.Line", true);
		Menu::DropList("^-> DashJump Tracker Text Color", "Trackers.DashJumpTracker.LineColor", ColorMenuList, 11);
		Menu::SliderInt("^-> Tracker Line Width", "Trackers.DashJumpTracker.LineWidth", 5, 1, 10);

		Menu::Checkbox("Draw a Circle on the Arrival Position", "Trackers.DashJumpTracker.Circle", true);
		Menu::DropList("^-> DashJump Tracker Circle Color", "Trackers.DashJumpTracker.CircleColor", ColorMenuList, 11);

			*/

			if (Menu::Get<bool>("Trackers.DashJumpTracker.ChampName"))
			{
				Vector2 textPos{ Renderer::WorldToScreen(value.StartPos.Extended(value.EndPos, 200.0f)) };

				DrawHelper::DrawOutlineText(NULL, &textPos, value.Hero->GetCharName(), "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Trackers.DashJumpTracker.TextColor")), Menu::Get<int>("Trackers.DashJumpTracker.FontSize"), 10, 0,
					&DropLists::GetColor(Menu::Get<int>("Trackers.AbilityTimer.OutLineColor")), false);
			}
			//Draw::Text(&value.StartPos.Extended(value.EndPos, 200.0f), NULL, value.Hero->GetCharName(), "Arial", &Color::Yellow, 24, 10);

	
		}
	}
}

void DashJumpTracker::SpellCastStart(void * AI, PSDK_SPELL_CAST SpellCast, void * UserData)
{
	
	UNREFERENCED_PARAMETER(UserData);
	
	if (!Menu::Get<bool>("Trackers.DashJumpTracker.Use"))
	{

		return;
	}
	

	if (Destinations.empty())
	{
		return;
	}

	auto sender = pSDK->EntityManager->GetObjectFromPTR(AI);
	if (AI == nullptr || sender == nullptr || sender == NULL || !sender->IsHero() ) //later need to add not ally
	{
		return;
	}

	//SdkUiConsoleWrite("Spell: %s", SpellCast->Spell.Name);
	//SdkUiConsoleWrite("2Spell: %s", SpellCast->Spell.ScriptName);
	//SdkUiConsoleWrite("2Spell: %s", SpellCast->Spell.DisplayName);


	for (auto const &value : DashJumpList)
	{
		if (_stricmp(SpellCast->Spell.ScriptName, value.SpellName) == 0)
		{
			auto menuBool = Menu::Get<bool>("Trackers.DashJumpTracker.List." + std::string(value.SpellName));

			if (menuBool == false)
			{
				return;
			}

		}

	}

	
	/*
		DashJumpMenu("Ezreal", "EzrealE", "Ezreal E"), DashJumpMenu("Vayne", "VayneTumble", "Vayne Q(Invisible)"), DashJumpMenu("Kassadin", "RiftWalk", "Kassadin R"), DashJumpMenu("Shaco", "Deceive", "Shaco Q")
	, DashJumpMenu("LeeSin", "BlindMonkWOne", "LeeSin W"), DashJumpMenu("Talon", "TalonE", "Talon E"), DashJumpMenu("Leblanc", "LeblancW", "Leblanc W"), DashJumpMenu("Leblanc", "LeblancRW", "Leblanc RW")
	, DashJumpMenu("Zed", "ZedW", "Zed W (Swap)"), DashJumpMenu("Zed", "ZedR", "Zed R (Swap)")

	
	*/

	for (auto &value : Destinations)
	{
		if (value.Hero->GetNetworkID() == sender->GetNetworkID())
		{
			if (_stricmp(SpellCast->Spell.ScriptName, "VayneInquisition") == 0 && Menu::Get<bool>("Trackers.DashJumpTracker.List.VayneTumble"))
			{
				if (value.ExtraTicks >= 0)
				{
					value.ExtraTicks = Game::Time() + 5.0f + 2.0f * SpellCast->Spell.Level;
					return;
				}
			}
			else if (_stricmp(SpellCast->Spell.ScriptName, "VayneTumble") == 0 && Menu::Get<bool>("Trackers.DashJumpTracker.List.VayneTumble"))
			{
				if (Game::Time() >= value.ExtraTicks)
				{
					return;
				}
				value.StartPos = SpellCast->StartPosition;
				value.EndPos = CalculateEndPos(SpellCast->StartPosition, SpellCast->EndPosition, value.Range);
			}
			else if (_stricmp(SpellCast->Spell.ScriptName, "KatarinaE") == 0 && Menu::Get<bool>("Trackers.DashJumpTracker.List.KatarinaEWrapper"))//KatarinaEWrapper
			{
				value.StartPos = SpellCast->StartPosition;
				value.EndPos = CalculateEndPos(SpellCast->StartPosition, SpellCast->EndPosition, value.Range);

			}
			/*
			else if (_stricmp(SpellCast->Spell.ScriptName, "LeblancRW") == 0)
			{

				Destinations[index- 2].Casted = false;
			//	value.StartPos = Destinations[index - 2].StartPos;
				value.StartPos = SpellCast->StartPosition;
				value.EndPos = CalculateEndPos(SpellCast->StartPosition, SpellCast->EndPosition, value.Range);

			}
			else if (_stricmp(SpellCast->Spell.ScriptName, "LeblancWReturn") == 0)
			{


				Destinations[index - 1].Casted = false;
				Destinations[index + 1].Casted = false;
				Destinations[index + 2].Casted = false;



				value.StartPos = SpellCast->StartPosition;
				value.EndPos = Destinations[index - 1].StartPos;

			}
			else if (_stricmp(SpellCast->Spell.ScriptName, "LeblancRWReturn") == 0)
			{

					Destinations[index - 3].Casted = false;
					Destinations[index - 2].Casted = false;
					Destinations[index - 1].Casted = false;
				



				value.StartPos = SpellCast->StartPosition;
				value.EndPos = Destinations[index - 1].StartPos;

			}
			*/
			else if (_stricmp(SpellCast->Spell.ScriptName, "ZedW2") == 0 && _stricmp(SpellCast->Spell.ScriptName, "ZedW") != 0 && Menu::Get<bool>("Trackers.DashJumpTracker.List.ZedW"))
			{
				value.StartPos = SpellCast->StartPosition;
				value.EndPos = CalculateEndPos(SpellCast->StartPosition, SpellCast->EndPosition, value.Range);

			}
			else if (_stricmp(SpellCast->Spell.ScriptName, "ZedR2") == 0 && _stricmp(SpellCast->Spell.ScriptName, "ZedR") != 0 && Menu::Get<bool>("Trackers.DashJumpTracker.List.ZedR"))
			{
				value.StartPos = SpellCast->StartPosition;
				value.EndPos = CalculateEndPos(SpellCast->StartPosition, SpellCast->EndPosition, value.Range);
			

			}
			else if (_stricmp(SpellCast->Spell.ScriptName, value.SpellName) == 0 && _stricmp(SpellCast->Spell.ScriptName, "ZedW") != 0 && _stricmp(SpellCast->Spell.ScriptName, "ZedR") != 0 )
			{
				value.StartPos = SpellCast->StartPosition;
				value.EndPos = CalculateEndPos(SpellCast->StartPosition, SpellCast->EndPosition, value.Range);

				//SdkUiConsoleWrite("Noramal: %s", SpellCast->Spell.ScriptName);
				
			}
			value.Casted = true;
			value.TimeCasted = Game::Time();
		}
		

	}


}

Vector3 DashJumpTracker::CalculateEndPos(Vector3 start, Vector3 end, float maxRange)
{

	auto dist = start.Distance(end);
	auto endPos = end;

	if (dist > maxRange)
	{
		endPos = start.Extended(end, maxRange);
	}

	if (endPos.IsWall())
	{
		for (int i = 0; i < 500; i = i + 100)
		{
			auto pos = start.Extended(endPos, dist + i);
			if (!pos.IsWall())
			{
				return pos;
			}
		}
	}

	return endPos;

}