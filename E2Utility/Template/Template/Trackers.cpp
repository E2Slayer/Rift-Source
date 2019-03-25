#include "stdafx.h"
#include "Trackers.h"
#include "AbilityTimer.h"
#include "DashJumpTracker.h"

void Trackers::Init()
{
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, Detector::Update);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick, Trackers::Tick);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, Trackers::DrawMenu);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, Trackers::Draw);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::SpellCastStart, CustomRanges::SpellCastStart);
	//TurnAround::Init();
	//SharedExperience::InitLoader();
	AbilityTimer::InitLoader();
	DashJumpTracker::Init();
}

void Trackers::Update(void * UserData)
{
	//SharedExperience::TickLoader();
	//Teleport::TickLoader();
}

void Trackers::Tick(void * UserData)
{

	if (!Menu::Get<bool>("Trackers.Config.Enable"))
	{
		return;
	}

	DashJumpTracker::TickLoader();
	//TurnAround::TickLoader();

	//SharedExperience::TickLoader();
	//SharedExperience::testing();
	//SharedExperience::TickLoader();
}

void Trackers::DrawMenu(void * UserData)
{
	Menu::Tree("Trackers", "Trackers.", true, []()
	{
		/*
		Summoners::MenuLoader();
		Consumables::MenuLoader();
		Offensives::MenuLoader();
		Defensives::MenuLoader();
		Cleansers::MenuLoader();
		*/
		//SharedExperience::MenuLoader();
		//Teleport::MenuLoader();
		AbilityTimer::MenuLoader();
		DashJumpTracker::MenuLoader();

		Menu::Tree("Trackers Configs", "Trackers.Config", false, []()
		{
			Menu::Checkbox("Enable Trackers", "Trackers.Config.Enable", true);

		});
	});
}

void Trackers::Draw(_In_ void* UserData)
{
	if (!Menu::Get<bool>("Trackers.Config.Enable"))
	{
		return;
	}

	//Trackers.AbilityTimer.Use
	DashJumpTracker::DrawLoader();
	AbilityTimer::DrawLoader();
	//SharedExperience::DrawLoader();
	//Teleport::DrawLoader();
}