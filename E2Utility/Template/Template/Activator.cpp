#include "stdafx.h"
#include "Activator.h"


DWORD LastTimeTickCount = 0;

void Activator::Init()
{

	pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick, Activator::Tick);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, Activator::Update);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, Activator::DrawMenu);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::ModuleLoadAndUnload)

	ItemRetriever::Init();
	Summoners::Init();
	Consumables::Init();
	Offensives::Init();
	Defensives::Init();
	Cleansers::Init();
	

	
}

void Activator::Update(void * UserData)
{
	if (!Menu::Get<bool>("Activator.Config.Enable"))
	{
		return;
	}

	if (Player.IsAlive() && !Player.IsRecalling())
	{
		Cleansers::UpdateLoader();
	}
}

void Activator::Tick(void * UserData)
{




	if (!Menu::Get<bool>("Activator.Config.Enable"))
	{
		return;
	}

	/*
	if (Player.HasBuffType((unsigned char)BUFF_TYPE_STUN))
	{
		auto difference = Player.GetBuffByType((unsigned char)BUFF_TYPE_STUN).EndTime - Player.GetBuffByType((unsigned char)BUFF_TYPE_STUN).StartTime;

		SdkUiConsoleWrite("Duration : %f", difference);
	}*/

	if (Player.IsAlive() && !Player.IsRecalling())
	{
		Cleansers::TickLoader();
		ItemStruct* currentItems = ItemRetriever::GetAllPlayerItems();
		for (int i = 0; i < 7; i++)
		{
			if (currentItems[i].GetItemID() == 0)
			{
				continue;
			}

			Summoners::TickLoader();
			Consumables::TickLoader(currentItems[i]);
			Offensives::TickLoader(currentItems[i]);
			Defensives::TickLoader(currentItems[i]);
			
		}
	}
}

void Activator::DrawMenu(void * UserData)
{
	Menu::Tree("Activator", "Activator.", false, []()
	{
		Summoners::MenuLoader();
		Consumables::MenuLoader();
		Offensives::MenuLoader();
		Defensives::MenuLoader();
		Cleansers::MenuLoader();

		Menu::Tree("Activator Configs", "Activator.Config", false, []()
		{
			Menu::Checkbox("Enable Activator", "Activator.Config.Enable", true);
			Menu::Hotkey("Activator Combo Key", "Activator.Config.ComboKey", 32); //32 is space bar

			Menu::Checkbox("Enable Humanizer", "Activator.Config.HumanizerEnable", true);
			Menu::SliderInt("Humanizer Delay (ms)", "Activator.Config.HumanizerDelay", 200, 10, 500);
			SdkUiText("^-> This is a Spamming Preventer. I recommend you to keep it around 150 - 300 ms");

		});
	});
}

