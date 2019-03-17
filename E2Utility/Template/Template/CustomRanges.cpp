#include "stdafx.h"
#include "CustomRanges.h"




void CustomRanges::Init()
{

#pragma region RegisterCallbacks
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick, Kalista::Tick);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, Kalista::Update);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, Kalista::DrawMenu);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, CustomRanges::Draw);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::SpellCastStart, CustomRanges::SpellCastStart);

#pragma endregion
}


void CustomRanges::Draw(void * UserData)
{

	//std::vector<ItemStruct> currentItems = ItemRetriever::GetAllPlayerItems();
	ItemStruct* currentItems = ItemRetriever::GetAllPlayerItems();
	Vector3 pos{ Player.GetPosition() };
	Vector2 screenPos{ Renderer::WorldToScreen(pos) };
	//for (auto const& value : currentItems)


	if (pos.IsValid() && pos.IsOnScreen())
	{


		screenPos.y -= 20.0f;

		auto hi = Menu::Get<Hotkey>("Activator.Config.ComboKey");


	
		//hi.Active;

		//Menu::Hotkey
		Draw::Text(NULL, &screenPos, std::to_string(hi.Active), "Arial", &Color::White, 24, 6);
		/*
		if (hi)
		{
			Draw::Text(NULL, &screenPos, "Active", "Arial", &Color::White, 24, 6);
		}
		else
		{
			Draw::Text(NULL, &screenPos, "nah", "Arial", &Color::White, 24, 6);
		}*/

	}



	/*
	for(int i=0; i<7; i++)
	{
		if (pos.IsValid() && pos.IsOnScreen())
		{


		//	std::ostringstream oss;

			//std::string slot = (unsigned char) hi[i].GetItemSlot();

		//	oss << "ID" << (hi[i]).GetItemID() << " Slot" << (unsigned char)(hi[i]).GetItemSlot() << " Display" << (hi[i]).GetDisplayName();
			//std::string var = oss.str();
			screenPos.y -= 20.0f;

			bool hi = Menu::Get<bool>("Activator.Config.ComboKey");

			if (hi)
			{
				Draw::Text(NULL, &screenPos, "Active", "Arial", &Color::White, 24, 6);
			}
			else
			{
				Draw::Text(NULL, &screenPos, "nah", "Arial", &Color::White, 24, 6);
			}

			
			Draw::Text(NULL, &screenPos, std::to_string(currentItems[i].GetItemID()), "Arial", &Color::White, 24, 6);
			screenPos.y -= 20.0f;
			Draw::Text(NULL, &screenPos, std::to_string((unsigned char)currentItems[i].GetItemSlot()), "Arial", &Color::White, 24, 6);
			
			//screenPos.y -= 20.0f;
			//Draw::Text(NULL, &screenPos, currentItems[i].GetDisplayName(), "Arial", &Color::White, 24, 6);
		}
	}
	*/


	Draw::Circle(&Player.GetPosition(), 750.0f, &Color::Red);
	Draw::Circle(&Player.GetPosition(), 650.0f, &Color::Green);

	const auto target = pCore->TS->GetTarget(2000.0f);
	if (target != NULL && target != nullptr)
	{
		auto buffs
		{
			target->GetBuffs()
		};
		Vector3 pos{ target->GetPosition() };
		Vector2 screenPos{ Renderer::WorldToScreen(pos) };
		for (auto buffname : buffs)
		{
			if (pos.IsValid() && pos.IsOnScreen())
			{
				//We get the screen position and offset it a little so it doesnt draw over the above text
				//Vector2 screenPos{ Renderer::WorldToScreen(pos) };
				screenPos.y -= 20.0f;
				Draw::Text(NULL, &screenPos, buffname.Name, "Arial", &Color::White, 20, 6);
			}
		}
	}



}

void CustomRanges::SpellCastStart(void* AI, PSDK_SPELL_CAST SpellCast, void* UserData)
{



	if (AI == NULL || AI == nullptr || SpellCast == NULL || SpellCast == nullptr)
	{
		return;
	}

	SdkUiConsoleWrite("Name %s ", SpellCast->Spell.Name);
	SdkUiConsoleWrite("1Name %s ", SpellCast->Spell.ScriptName);
	SdkUiConsoleWrite("2Name %s ", SpellCast->Spell.AnimationName);
	SdkUiConsoleWrite("3Name %s ", SpellCast->Spell.MissileEffectName);

	SdkUiConsoleWrite("4Name %s ", SpellCast->Spell.MissileSpeed);
}