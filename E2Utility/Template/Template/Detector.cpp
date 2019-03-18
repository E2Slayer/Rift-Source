#include "stdafx.h"
#include "Detector.h"

void Detector::Init()
{
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick, Detector::Tick);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, Detector::DrawMenu);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, CustomRanges::Draw);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::SpellCastStart, CustomRanges::SpellCastStart);
	TurnAround::Init();
}

void Detector::Tick(void * UserData)
{
	TurnAround::TickLoader();
}

void Detector::DrawMenu(void * UserData)
{
	Menu::Tree("Detector", "Detector.", true, []()
	{
		/*
		Summoners::MenuLoader();
		Consumables::MenuLoader();
		Offensives::MenuLoader();
		Defensives::MenuLoader();
		Cleansers::MenuLoader();
		*/
		TurnAround::MenuLoader();

		Menu::Tree("Detector Configs", "Detector.Config", false, []()
		{
			Menu::Checkbox("Enable Detector", "Detector.Config.Enable", true);

		});
	});
}