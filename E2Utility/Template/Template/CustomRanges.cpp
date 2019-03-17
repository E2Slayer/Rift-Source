#include "stdafx.h"
#include "CustomRanges.h"




void CustomRanges::Init()
{

#pragma region RegisterCallbacks
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick, Kalista::Tick);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, Kalista::Update);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, Kalista::DrawMenu);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, CustomRanges::Draw);


#pragma endregion
}


void CustomRanges::Draw(void * UserData)
{

	//std::vector<ItemStruct> currentItems = ItemRetriever::GetAllPlayerItems();
	ItemStruct* currentItems = ItemRetriever::GetAllPlayerItems();
	Vector3 pos{ Player.GetPosition() };
	Vector2 screenPos{ Renderer::WorldToScreen(pos) };
	//for (auto const& value : currentItems)
	for(int i=0; i<7; i++)
	{
		if (pos.IsValid() && pos.IsOnScreen())
		{


		//	std::ostringstream oss;

			//std::string slot = (unsigned char) hi[i].GetItemSlot();

		//	oss << "ID" << (hi[i]).GetItemID() << " Slot" << (unsigned char)(hi[i]).GetItemSlot() << " Display" << (hi[i]).GetDisplayName();
			//std::string var = oss.str();
			screenPos.y -= 20.0f;
			Draw::Text(NULL, &screenPos, std::to_string(currentItems[i].GetItemID()), "Arial", &Color::White, 24, 6);
			screenPos.y -= 20.0f;
			Draw::Text(NULL, &screenPos, std::to_string((unsigned char)currentItems[i].GetItemSlot()), "Arial", &Color::White, 24, 6);
			screenPos.y -= 20.0f;
			Draw::Text(NULL, &screenPos, currentItems[i].GetDisplayName(), "Arial", &Color::White, 24, 6);
		}
	}

}