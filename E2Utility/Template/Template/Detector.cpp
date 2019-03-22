#include "stdafx.h"
#include "Detector.h"



void Detector::Init()
{
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, Detector::Update);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick, Detector::Tick);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, Detector::DrawMenu);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, Detector::Draw);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::SpellCastStart, CustomRanges::SpellCastStart);
	//TurnAround::Init();
	SharedExperience::InitLoader();
}

void Detector::Update(void * UserData)
{
	//SharedExperience::TickLoader();
}

void Detector::Tick(void * UserData)
{

	if (!Menu::Get<bool>("Detector.Config.Enable"))
	{
		return;
	}
	//TurnAround::TickLoader();

	//SharedExperience::TickLoader();
	//SharedExperience::testing();
	SharedExperience::TickLoader();
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
		SharedExperience::MenuLoader();

		

		Menu::Tree("Detector Configs", "Detector.Config", false, []()
		{
			Menu::Checkbox("Enable Detector", "Detector.Config.Enable", true);

		});
	});
}

void Detector::Draw(_In_ void* UserData)
{
	if (!Menu::Get<bool>("Detector.Config.Enable"))
	{
		return;
	}

	SharedExperience::DrawLoader();

}