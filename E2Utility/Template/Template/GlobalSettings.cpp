#include "stdafx.h"
#include "GlobalSettings.h"


void GlobalSettings::Init()
{
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick, GlobalSettings::Tick);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, GlobalSettings::DrawMenu);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, CustomRanges::Draw);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::SpellCastStart, CustomRanges::SpellCastStart);
	//Others::Init();
	//AutoEvents::Init();


	DrawHelper::SetFont(Menu::Get<int>("GlobalSettings.FontType"), Menu::Get<bool>("GlobalSettings.UseOutline"));

}

void GlobalSettings::Tick(void * UserData)
{
	if (Game::IsOverlayOpen())
	{
		DrawHelper::SetFont(Menu::Get<int>("GlobalSettings.FontType"), Menu::Get<bool>("GlobalSettings.UseOutline"));
	}
	else
	{
		return;
	}
}

void GlobalSettings::DrawMenu(void * UserData)
{
	Menu::Tree("Global Settings", "GlobalSettings.", false, []()
	{

		Menu::DropList("Font Type", "GlobalSettings.FontType", std::vector<std::string>{ "Calibri", "Calibri Bold", "Arial", "Tahoma", "Verdana", "Times New Roman", "Lucida Console" }, 0);
		Menu::Checkbox("Use Outline", "GlobalSettings.UseOutline", true);

		
	});
}