#include "stdafx.h"
#include "CloneDetector.h"
#include "DropLists.h"
#include "DrawHelper.h"


//wu - 1.5f
//shaco - 18.0f
//le - 8.0f

const std::vector<const char*> CloneChampionList{ "Shaco", "Leblanc", "MonkeyKing", "Neeko" };
std::vector<CloneDraw> CloneDrawingList;
std::vector<const char*> InGameCloneChampionList;

void CloneDetector::Init()
{
	CloneDrawingList.clear();
	InGameCloneChampionList.clear();


	auto enemy1{ pSDK->EntityManager->GetEnemyHeroes() };

	if (!enemy1.empty())
	{
		for (auto &[netID, hero] : enemy1)
		{
			if (hero != nullptr && hero != NULL)
			{
				for (auto const &value : CloneChampionList)
				{
					if (strcmp(hero->GetCharName(), value) == 0)
					{
						InGameCloneChampionList.emplace_back(hero->GetName());
					}
				}
			}
		}
	}
	
	//InGameCloneChampionList.emplace_back(Player.GetName());

	pSDK->EventHandler->RegisterCallback(CallbackEnum::CreateObject, CloneDetector::OnCreate);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::DeleteObject, CloneDetector::OnDelete);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::BuffCreateAndDelete, AbilityTimer::OnBuffCreateAndDelete);

}

void CloneDetector::MenuLoader()
{
	Menu::Tree("Clone Detector", "Detector.CloneDetector", false, []()
	{
		Menu::Checkbox("Use Clone Detector", "Detector.CloneDetector.Enable", true);
		Menu::DropList("Clone Detector Style", "Detector.CloneDetector.Style", std::vector<std::string>{ "Circle", "Text", "Both" }, 0);
		Menu::SliderInt("Detected Circle Range", "Detector.CloneDetector.CircleRange", 100, 1, 500);
		Menu::DropList("Detected Circle Color", "Detector.CloneDetector.CircleColor", ColorMenuList, 10); //red
		Menu::SliderInt("Detected Font Size", "Detector.CloneDetector.FontSize", 30, 20, 40);
		Menu::DropList("Text Color", "Detector.CloneDetector.TextColor", ColorMenuList, 10); //red
		Menu::DropList("^-> Outline Color", "Detector.CloneDetector.OutlineColor", ColorMenuList, 0); //black
		Menu::SliderInt("Clone Detector Text Position X-axis", "Detector.CloneDetector.DrawingX", -20, -200, 200);
		Menu::SliderInt("Clone Detector Position Y-axis", "Detector.CloneDetector.DrawingY", -90, -200, 200);


	});

}

void CloneDetector::TickLoader()
{
}

void CloneDetector::DrawLoader()
{
	if (!Menu::Get<bool>("Detector.CloneDetector.Enable"))
	{
		return;
	}

	
	if (Game::IsOverlayOpen())
	{
		int selection = Menu::Get<int>("Detector.CloneDetector.Style");


		if (selection == 0 || selection == 2)
		{
			Draw::Circle(&Player.GetPosition(), (float)Menu::Get<int>("Detector.CloneDetector.CircleRange"), &DropLists::GetColor(Menu::Get<int>("Detector.CloneDetector.CircleColor")), 0, &SDKVECTOR(200.0f, 200.0f, 200.0f));
		}

		if (selection == 1 || selection == 2)
		{

			Vector2 screenPos{ Renderer::WorldToScreen(Player.GetPosition()) };
			screenPos.x += (float)Menu::Get<int>("Detector.CloneDetector.DrawingX");
			screenPos.y += (float)Menu::Get<int>("Detector.CloneDetector.DrawingY");
			DrawHelper::DrawOutlineText(NULL, &screenPos, "CLONE", "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Detector.CloneDetector.TextColor")), Menu::Get<int>("Detector.CloneDetector.FontSize"), 10, 0,
				&DropLists::GetColor(Menu::Get<int>("Detector.CloneDetector.OutlineColor")), false);

		}
	}
	else
	{

		if (!CloneDrawingList.empty())
		{
			for (auto & value : CloneDrawingList)
			{
				if (value.Object->GetPosition().IsValid() && value.Object->GetPosition().IsOnScreen() && value.Object->IsVisible())
				{
					int selection = Menu::Get<int>("Detector.CloneDetector.Style");


					if (selection == 0 || selection == 2)
					{
						Draw::Circle(&value.Object->GetPosition(), (float)Menu::Get<int>("Detector.CloneDetector.CircleRange"), &DropLists::GetColor(Menu::Get<int>("Detector.CloneDetector.CircleColor")), 0, &SDKVECTOR(100.0f, 100.0f, 100.0f));
					}

					if (selection == 1 || selection == 2)
					{

						Vector2 screenPos{ Renderer::WorldToScreen(value.Object->GetPosition()) };
						screenPos.x += (float)Menu::Get<int>("Detector.CloneDetector.DrawingX");
						screenPos.y += (float)Menu::Get<int>("Detector.CloneDetector.DrawingY");
						DrawHelper::DrawOutlineText(NULL, &screenPos, "CLONE", "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Detector.CloneDetector.TextColor")), Menu::Get<int>("Detector.CloneDetector.FontSize"), 10, 0,
							&DropLists::GetColor(Menu::Get<int>("Detector.CloneDetector.OutlineColor")), false);

					}



				}
			}
		}
	}

}

bool __cdecl CloneDetector::OnCreate(void * Object, unsigned int NetworkID, void * UserData)
{

	UNREFERENCED_PARAMETER(UserData);
	UNREFERENCED_PARAMETER(NetworkID);
	if (!Menu::Get<bool>("Detector.CloneDetector.Enable"))
	{
		return false;
	}
	auto sender = pSDK->EntityManager->GetObjectFromPTR(Object);
	if (Object == nullptr || sender == nullptr || sender == NULL || NetworkID == 0 || sender->IsParticle() ) //|| !sender->IsParticle()
	{
		return false;
	}

	auto objectName = sender->GetName();

	

	if (objectName == NULL || objectName == nullptr)
	{
		return false;
	}


	const char *p = strstr(objectName, "SRU_");
	const char *m = strstr(objectName, "Minion_");
	const char *r = strstr(objectName, "SRUAP_");
	if (p || m || r)
	{
		return false;
	}
	//SdkUiConsoleWrite("sender Type %d", sender->GetTypeFlags());

	for (auto const &value : InGameCloneChampionList)
	{

		if (strcmp(value, objectName) == 0)
		{
			//SdkUiConsoleWrite("didyou Created %s", objectName);
			CloneDrawingList.emplace_back(CloneDraw(sender->GetPosition(), sender, Game::Time() + 30.0f, objectName));
		}

	}
	


//	SdkUiConsoleWrite("AbilityTimer Created %s", objectName);

	return false;
}

bool __cdecl CloneDetector::OnDelete(void * Object, unsigned int NetworkID, void * UserData)
{


	UNREFERENCED_PARAMETER(UserData);
	UNREFERENCED_PARAMETER(NetworkID);
	if (!Menu::Get<bool>("Detector.CloneDetector.Enable"))
	{
		return false;
	}

	if (CloneDrawingList.empty())
	{
		return false;
	}

	auto sender = pSDK->EntityManager->GetObjectFromPTR(Object);
	if (Object == nullptr || sender == nullptr || sender == NULL || NetworkID == 0 || sender->IsParticle()) //|| !sender->IsParticle()
	{
		return false;
	}

	auto objectName = sender->GetName();


	if (objectName == NULL || objectName == nullptr)
	{
		return false;
	}

	const char *p = strstr(objectName, "SRU_");
	const char *m = strstr(objectName, "Minion_");
	const char *r = strstr(objectName, "SRUAP_");
	if (p || m || r)
	{
		return false;
	}

	//SdkUiConsoleWrite("AbilityTimer Deleted %s", objectName);

	
	auto i = std::begin(CloneDrawingList);

	while (i != std::end(CloneDrawingList))
	{
		// Do some stuff
		if (i->End < Game::Time())
		{
			i = CloneDrawingList.erase(i);
		}
		else if (i->Object->GetNetworkID() == sender->GetNetworkID() ) //&& strcmp(objectName, i->TargetName) == 0
		{
			i = CloneDrawingList.erase(i);
		}
		else
		{
			++i;
		}
	}

	return false;
}
