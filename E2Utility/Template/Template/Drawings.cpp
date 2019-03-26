#include "stdafx.h"
#include "Drawings.h"


void Drawings::Init()
{
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick, Drawings::Tick);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, Drawings::DrawMenu);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, Drawings::Draw);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::SpellCastStart, CustomRanges::SpellCastStart);
	GankAlerter::Init();
	CustomRange::Init();
}

void Drawings::Tick(void * UserData)
{
	if (Menu::Get<bool>("Drawings.Config.Enable"))
	{
		GankAlerter::TickLoader();
	}
}

void Drawings::DrawMenu(void * UserData)
{
	Menu::Tree("Drawings", "Drawings.", false, []()
	{
		/*
		Summoners::MenuLoader();
		Consumables::MenuLoader();
		Offensives::MenuLoader();
		Defensives::MenuLoader();
		Cleansers::MenuLoader();
		*/
		GankAlerter::MenuLoader();
		CustomRange::MenuLoader();

		Menu::Tree("Drawings Configs", "Drawings.Config", false, []()
		{
			Menu::Checkbox("Enable Drawings", "Drawings.Config.Enable", true);

		});
	});
}

void Drawings::Draw(void * UserData)
{

	if (Menu::Get<bool>("Drawings.Config.Enable"))
	{
		CustomRange::DrawLoader();
	}
}
