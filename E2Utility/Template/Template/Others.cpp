#include "stdafx.h"
#include "Others.h"

void Others::Init()
{
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick, Others::Tick);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, Others::DrawMenu);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, CustomRanges::Draw);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::SpellCastStart, CustomRanges::SpellCastStart);
	//Others::Init();

}

void Others::Tick(void * UserData)
{
	if (Menu::Get<bool>("Others.Config.Enable"))
	{
		AntiAFK::TickLoader();
	}
}

void Others::DrawMenu(void * UserData)
{
	Menu::Tree("Others", "Others.", false, []()
	{
		/*
		Summoners::MenuLoader();
		Consumables::MenuLoader();
		Offensives::MenuLoader();
		Defensives::MenuLoader();
		Cleansers::MenuLoader();
		*/
		AntiAFK::MenuLoader();

		Menu::Tree("Others Configs", "Others.Config", false, []()
		{
			Menu::Checkbox("Enable Others", "Others.Config.Enable", true);

		});
	});
}
