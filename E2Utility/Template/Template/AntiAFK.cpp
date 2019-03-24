#include "stdafx.h"
#include "AntiAFK.h"
#include <cstdlib>


Spell::Active spellQ = Spell::Active(SpellSlot::Q);

const int WarningTime = 100;
float lastAction = 0;

void AntiAFK::Init()
{

}

void AntiAFK::MenuLoader()
{
	Menu::Tree("AntiAFK", "Others.AntiAFK", false, []()
	{
		Menu::Checkbox("Use Anti-AFK", "Others.AntiAFK.Use", false);
		Menu::HotkeyToggle("Anti-AFK Toggle", "Others.AntiAFK.Toggle", 73, false);


		Menu::Checkbox("Use Q", "Others.AntiAFK.UseQ", false);
	});
}

void AntiAFK::TickLoader()
{
	if (Menu::Get<bool>("Others.AntiAFK.Use") && Menu::Get<KeyToggle_t>("Others.AntiAFK.Toggle").Toggle) //Menu::Get<Hotkey>("Others.AntiAFK.Toggle").Active
	{
		//SdkUiConsoleWrite("3123124fffff");

		Execute();
	}
}


void AntiAFK::Execute()
{
	int rValue = rand()%100;

	if (Game::Time() - lastAction >= WarningTime + rValue)
	{
		SdkUiConsoleWrite("[E2Utility] Anti-AFK Activated. Time Passed From the Last Cast %f (Seconds)", Game::Time() - lastAction);

		if (spellQ.IsReady() && Menu::Get<bool>("Others.AntiAFK.UseQ"))
		{
			spellQ.Cast();
		}

		float posRand = rand() % 200 - 100;


		auto backupPos = Player.GetServerPosition();

		SDKVECTOR direction{ posRand, posRand, posRand };

		backupPos += direction;

		SdkMoveLocalPlayer(&backupPos, false);
		lastAction = Game::Time();

	}
}