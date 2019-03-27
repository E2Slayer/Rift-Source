#include "stdafx.h"
#include "Trackers.h"
#include "AbilityTimer.h"
#include "DashJumpTracker.h"
//#include "CooldownTracker.h"
#include "InhibitorTimer.h"

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
	//CooldownTracker::Init();

	InhibitorTimer::Init();
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
	AbilityTimer::TickLoader();
	DashJumpTracker::TickLoader();
	InhibitorTimer::TickLoader();
	//TurnAround::TickLoader();

	//SharedExperience::TickLoader();
	//SharedExperience::testing();
	//SharedExperience::TickLoader();
}

void Trackers::DrawMenu(void * UserData)
{
	Menu::Tree("Trackers", "Trackers.", false, []()
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

		InhibitorTimer::MenuLoader();
		//CooldownTracker::MenuLoader();


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
	InhibitorTimer::DrawLoader();
	//CooldownTracker::DrawLoader();


}