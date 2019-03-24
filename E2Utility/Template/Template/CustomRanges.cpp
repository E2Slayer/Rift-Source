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


	if (pos.IsValid() )
	{


		screenPos.y -= 20.0f;

		//auto hi = Menu::Get<Hotkey>("Activator.Config.ComboKey");


	
		//hi.Active;

		//Menu::Hotkey
		Draw::Text(NULL, &screenPos, std::to_string(Game::Time()), "Arial", &Color::White, 24, 6);

		screenPos.y -= 20.0f;

		Draw::Text(NULL, &screenPos, std::to_string(Player.GetPosition().x), "Arial", &Color::White, 24, 6);

		screenPos.y -= 20.0f;
		Draw::Text(NULL, &screenPos, std::to_string(Player.GetPosition().y), "Arial", &Color::Green, 24, 6);

		screenPos.y -= 20.0f;
		Draw::Text(NULL, &screenPos, std::to_string(Player.GetPosition().z), "Arial", &Color::Green, 24, 6);


		Vector3 blueFountain = Vector3(585.0f, 183.284f, 512.0f);


		auto shops = pSDK->EntityManager->GetAllyTurrets();
		



		for (auto &[netid, shop] : shops)
		{
			if (strstr(shop->GetName(), "Turret_OrderTurretShrine_A") && shop->IsAlly() || strstr(shop->GetName(), "Turret_ChaosTurretShrine") && shop->IsAlly())
			{
				Draw::Circle(&shop->GetPosition(), 1500.0f, &Color::Blue);

				if (Player.Distance(&shop->GetPosition()) <= 1500.0f)
				{
					screenPos.y -= 20.0f;
					Draw::Text(NULL, &screenPos, "Is in Fountain", "Arial", &Color::Red, 24, 6);

				}
			}

			Draw::Text(&shop->GetPosition(), NULL, shop->GetName(), "Arial", &Color::Red, 24, 6);
		}

		//Draw::Circle(&blueFountain, 900.0f, &Color::Red);



		//screenPos.y -= 20.0f;
		//auto hi = Menu::Get<Hotkey_t>("Others.AntiAFK.Toggle").Active;
	//	Draw::Text(NULL, &screenPos, std::to_string(Menu::Get<KeyToggle_t>("Others.AntiAFK.Toggle").Toggle), "Arial", &Color::Red, 24, 6);



		//Menu::Get<Hotkey>("Others.AntiAFK.Toggle").Active
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




	SDKVECTOR direction{ 100.f, 100.f, 100.f };

	auto orignialPath = Player.GetWaypoints();
	for (auto const& value : orignialPath)
	{

		Vector3 temp{ value };
		Draw::Circle(&temp, 100.0f, &Color::Green, 2, &direction);
		
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



	Draw::Circle(&Player.GetPosition(), 1050.0f, &Color::Red);
	//Draw::Circle(&Player.GetPosition(), 650.0f, &Color::Green);
	/*
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
	*/


}

void CustomRanges::SpellCastStart(void* AI, PSDK_SPELL_CAST SpellCast, void* UserData)
{



	if (AI == NULL || AI == nullptr || SpellCast == NULL || SpellCast == nullptr)
	{
		return;
	}


	/*
	SdkUiConsoleWrite("Name %s ", SpellCast->Spell.Name);
	SdkUiConsoleWrite("1Name %s ", SpellCast->Spell.ScriptName);
	SdkUiConsoleWrite("2Name %s ", SpellCast->Spell.AlternateName);
	*/
	//SdkUiConsoleWrite("2Name %s ", Player.GetResource().Type);
	//SdkUiConsoleWrite("3Name %s ", SpellCast->Spell.MissileEffectName);

	//SdkUiConsoleWrite("4Name %s ", SpellCast->Spell.MissileSpeed);
}