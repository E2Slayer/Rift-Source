#include "stdafx.h"
#include "Teleport.h"
#include "DropLists.h"
#include "DrawHelper.h"

std::map<unsigned int, RecallInfo> EnemyTeleportList;

std::map<unsigned int, TeleportStuct> TeleportStore;

//TeleportStuct TeleportStore[];


void Teleport::InitLoader()
{

	EnemyTeleportList.clear();

	TeleportStore.clear();



	

	auto enemy1{ pSDK->EntityManager->GetEnemyHeroes() };

	if (!enemy1.empty())
	{
		for (auto &[netID, hero] : enemy1)
		{
			if (hero != nullptr && hero != NULL)
			{
				RecallInfo recallInfo;

				recallInfo.Hero = hero;
				recallInfo.Active = false;
				recallInfo.Begin = 0.0f;
				recallInfo.Duration = 0.0f;
				recallInfo.index = 0;
				recallInfo.LastChange = 0.0f;
				recallInfo.CurrentRecallType = RecallType::Unknown;
				EnemyTeleportList.emplace(netID, recallInfo);
			}
		}
	}

	RecallInfo recallInfo1;

	recallInfo1.Hero = &Player;
	recallInfo1.Active = false;
	recallInfo1.Begin = 0.0f;
	recallInfo1.Duration = 0.0f;
	recallInfo1.index = 0;
	recallInfo1.LastChange = 0.0f;
	recallInfo1.CurrentRecallType = RecallType::Unknown;
	EnemyTeleportList.emplace(Player.GetNetworkID(),recallInfo1); //for player



	pSDK->EventHandler->RegisterCallback(CallbackEnum::Recall, Teleport::Recall);

	
	//SdkUiConsoleWrite("teteport loaded");
}

void Teleport::TickLoader()
{
	//tickCount = GetTickCount();
	//SdkUiConsoleWrite("tickcount %ld", GetTickCount());

}

void Teleport::MenuLoader()
{
	Menu::Tree("Teleport Tracker", "Detector.Teleport", false, []()
	{
		Menu::Checkbox("Use Teleport Tracker", "Detector.Teleport.Use", true);



		Menu::Checkbox("Track Recall", "Detector.Teleport.Recall", true);
		Menu::Checkbox("Track Empowered Recall", "Detector.Teleport.EmpoweredRecall", true);
		SdkUiText("^-> With Rift Herald's Eyes or Baron's Buff");
		Menu::Checkbox("Track Teleport", "Detector.Teleport.Teleport", true);
		Menu::Checkbox("Track Shen's R", "Detector.Teleport.ShenR", true);
		Menu::Checkbox("Track Twisted Fate's R", "Detector.Teleport.TwisitedFateR", true);


		Menu::Tree("Chat Notification Settings", "Detector.Teleport.ChatSettings", false, []()
		{
			Menu::Checkbox("Notify Start on the Chat", "Detector.Teleport.ChatStart", true);
			Menu::Checkbox("Notify Aborted on the Chat", "Detector.Teleport.ChatAborted", true);
			Menu::Checkbox("Notify Finshed on the Chat", "Detector.Teleport.ChatFinished", true);

			Menu::Checkbox("Use [E2Utility] Prefix on the Beginning", "Detector.Teleport.Chat.Prefix", true);
			
			Menu::Tree("Champion Name Color Picker", "Detector.Teleport.ChatSettings1", false, []()
			{
				Menu::ColorPicker("Champion Name Color Picker", "Detector.Teleport.Chat.ChampName", SDKVECTOR(210, 35, 35));
			});
			

			Menu::Tree("Health Percentage Color Picker", "Detector.Teleport.ChatSettings2", false, []()
			{
				Menu::ColorPicker("Health Percentage Color Picker", "Detector.Teleport.Chat.Health", SDKVECTOR(35, 210, 35));
			});

			Menu::Tree("Started Color Picker", "Detector.Teleport.ChatSettings3", false, []()
			{
				Menu::ColorPicker("Started Color Picker", "Detector.Teleport.Chat.Started", SDKVECTOR(255, 255, 255));
			});

			Menu::Tree("Aborted Color Picker", "Detector.Teleport.ChatSettings4", false, []()
			{
				Menu::ColorPicker("Aborted Color Picker", "Detector.Teleport.Chat.Aborted", SDKVECTOR(210, 35, 35));
			});

			Menu::Tree("Finished Color Picker", "Detector.Teleport.ChatSettings5", false, []()
			{
				Menu::ColorPicker("Finished Color Picker", "Detector.Teleport.Chat.Finished", SDKVECTOR(33, 235, 25));
			});


			Menu::Tree("Teleport Type Color Picker", "Detector.Teleport.ChatSettings6", false, []()
			{
				Menu::ColorPicker("Teleport Type Color Picker", "Detector.Teleport.Chat.Teleport", SDKVECTOR(25, 150, 235));
			});

			//Menu::Checkbox("Use [E2Utility] Prefix on the Beginning", "Detector.Teleport.Chat.Prefix", true);

		});



		Menu::Tree("Teleport Tracker Drawing Settings", "Detector.Teleport.Drawings", false, []()
		{
		//	Menu::SliderInt("Font Size", "Detector.Teleport.FontSize", 24, 20, 30);

			Menu::Checkbox("Want To Use Custom Drawing Position", "Detector.Teleport.Custom", false);
			Menu::Checkbox("Track Myself for Testing Purpose", "Detector.Teleport.TrackMe", false);
			Menu::SliderInt("Drawing Position X-axis", "Detector.Teleport.DrawingX", 500, 1, 5000);
			Menu::SliderInt("Drawing Position Y-axis", "Detector.Teleport.DrawingY", 500, 1, 5000);

			//&DropLists::GetColor(Menu::Get<int>("Detector.Teleport.TextColor"))
	
			Menu::DropList("Text Color", "Detector.Teleport.TextColor", ColorMenuList, 11); //white
			Menu::DropList("^-> Outline Color", "Detector.Teleport.OutlineColor", ColorMenuList, 0); //white



			Menu::Checkbox("Change the Color", "Detector.Teleport.ChangeColor", true);
			Menu::SliderInt("^-> If enemy's HP is below # %", "Detector.Teleport.WarningHealth", 30, 1, 100);
			Menu::DropList("^-> Color ", "Detector.Teleport.ColorWarning", ColorMenuList, 10); //red
			Menu::DropList("^-> Outline Color ", "Detector.Teleport.OutlineColorWarning", ColorMenuList, 0); //white



		});



	});
}

void Teleport::DrawLoader()
{

	if (!Menu::Get<bool>("Detector.Teleport.Use") || EnemyTeleportList.empty())
	{
		return;
	}


//	Draw::RectOutline(&Player.GetPosition(), &Renderer::CursorPos().To3D(), 400.0f, &Color::Green);

	Vector2 screenPos{ Renderer::WorldToScreen(Player.GetPosition()) };

	auto resol = Renderer::GetResolution();



	Vector2 barPos = Vector2((resol.Width/2.0f) - 156.0f, (resol.Height/2.0f)+ (resol.Height / 4.0f) - 50.0f);

	if (Menu::Get<bool>("Detector.Teleport.Custom"))
	{
		barPos = Vector2((float)Menu::Get<int>("Detector.Teleport.DrawingX"), (float)Menu::Get<int>("Detector.Teleport.DrawingY"));
	}


	if (Game::IsOverlayOpen())
	{
		SdkDrawSpriteFromResource(MAKEINTRESOURCEA(IDB_RECALLBAR), &barPos, false); //312x8




			//target.second.CurrentRecallType;
			//Game::GetRecallName(target.second.CurrentRecallType);


			SDKCOLOR color = DropLists::GetColor(Menu::Get<int>("Detector.Teleport.TextColor"));


			Draw::LineScreen(&Vector2(barPos.x, barPos.y), &Vector2(barPos.x, barPos.y - 20.0f), 5.0f, &color);


			DrawHelper::DrawOutlineText(NULL, &Vector2(barPos.x - 17.0f, barPos.y - 85.0f), "ChampName", "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Detector.Teleport.TextColor")), 24, 8, 0,
				&DropLists::GetColor(Menu::Get<int>("Detector.Teleport.OutlineColor")), false);


			DrawHelper::DrawOutlineText(NULL, &Vector2(barPos.x - 7.0f, barPos.y - 70.0f), "100%", "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Detector.Teleport.TextColor")), 24, 8, 0,
				&DropLists::GetColor(Menu::Get<int>("Detector.Teleport.OutlineColor")), false);


			DrawHelper::DrawOutlineText(NULL, &Vector2(barPos.x - 7.0f, barPos.y - 50.0f), "(0.0)", "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Detector.Teleport.TextColor")), 24, 8, 0,
				&DropLists::GetColor(Menu::Get<int>("Detector.Teleport.OutlineColor")), false);


	}
	else
	{



		for (auto const& target : EnemyTeleportList)
		{
			if (target.second.Active)
			{
				SdkDrawSpriteFromResource(MAKEINTRESOURCEA(IDB_RECALLBAR), &barPos, false); //312x8



				float difference = Game::Time() - target.second.Begin;

				float percentage = (difference / (target.second.Duration))*312.0f;


				if (target.second.Duration >= difference)
				{

					//target.second.CurrentRecallType;
					//Game::GetRecallName(target.second.CurrentRecallType);


					SDKCOLOR color = DropLists::GetColor(Menu::Get<int>("Detector.Teleport.TextColor"));

					if (Menu::Get<bool>("Detector.Teleport.ChangeColor"))
					{

						float menuHealth = (float)Menu::Get<int>("Detector.Teleport.WarningHealth");

						if (target.second.Hero->GetHealthPercent() <= menuHealth)
						{
							color = DropLists::GetColor(Menu::Get<int>("Detector.Teleport.ColorWarning"));
						}

						//target.second.Hero->GetHealthPercent()
					}

					Draw::LineScreen(&Vector2(barPos.x + percentage, barPos.y), &Vector2(barPos.x + percentage, barPos.y - 20.0f), 5.0f, &color);







					DrawHelper::DrawOutlineText(NULL, &Vector2(barPos.x - 7.0f + percentage, barPos.y - 85.0f), target.second.Hero->GetCharName(), "Calibri Bold", &color, 24, 8, 0,
						&DropLists::GetColor(Menu::Get<int>("Detector.Teleport.OutlineColor")), false);

					std::stringstream ss1;
					ss1.precision(0); //for decimal
					ss1.setf(std::ios_base::fixed, std::ios_base::floatfield);
					ss1 << target.second.Hero->GetHealthPercent() << "% ";
					DrawHelper::DrawOutlineText(NULL, &Vector2(barPos.x - 7.0f + percentage, barPos.y - 70.0f), ss1.str().c_str(), "Calibri Bold", &color, 24, 8, 0,
						&DropLists::GetColor(Menu::Get<int>("Detector.Teleport.OutlineColor")), false);


				
					std::stringstream ss;
					ss.precision(1); //for decimal
					ss.setf(std::ios_base::fixed, std::ios_base::floatfield);

					ss << "(" << (target.second.Duration - difference) << ")";

					DrawHelper::DrawOutlineText(NULL, &Vector2(barPos.x - 7.0f + percentage, barPos.y - 50.0f), ss.str().c_str(), "Calibri Bold", &color, 24, 8, 0,
						&DropLists::GetColor(Menu::Get<int>("Detector.Teleport.OutlineColor")), false);
				}
			}
		}
	}

	//Draw::LineScreen(&testPos, &Renderer::CursorPos(), 5.0f, &Color::Green);



	//IDB_PNG1

	
	//Draw::RectOutline()
	//tickCount = GetTickCount();
	//SdkUiConsoleWrite("tickcount %ld", GetTickCount());

}


TeleportStuct Teleport::TeleportDecoder(void* Unit, const char* Name, const char* Type)
{
	int errorGab = 90;
	TeleportStuct result;

	result.Status = TeleportTypes::Unknown;
	result.Type = RecallType::Unknown;

	auto sender = pSDK->EntityManager->GetObjectFromPTR(Unit);
	if (Unit == nullptr || sender == nullptr || sender == NULL || !sender->IsHero() )
	{
		return result;
	}
	result.UnitNetworkId = sender->GetNetworkID();

	
	std::map<unsigned int, TeleportStuct>::iterator it = TeleportStore.find(result.UnitNetworkId);
	if (it == TeleportStore.end())
	{
		TeleportStore.emplace(result.UnitNetworkId, result);

		
	}



	if ((unsigned)strlen(Name) != 0)
	{
		RecallType recallType = Game::GetRecallType(Type);
		auto duration = Game::GetRecallDuration(recallType);
		auto time = GetTickCount();
		
		std::map<unsigned int, TeleportStuct>::iterator it2 = TeleportStore.find(result.UnitNetworkId);
		if (it2 != TeleportStore.end())
		{
			it2->second.Duration = duration;

			it2->second.Type = recallType;
			
			//SdkUiConsoleWrite("hi inisde2 %d %s", it2->second.Type, Type);
			it2->second.Start = time;
		}

		result.Status = TeleportTypes::Start;
		result.Duration = duration;
		result.Start = time;


	}
	else
	{
		

		std::map<unsigned int, TeleportStuct>::iterator it3 = TeleportStore.find(result.UnitNetworkId);
		if (it3 != TeleportStore.end())
		{

			DWORD dw = (DWORD) it3->second.Duration * 1000.0f;
			auto shorter = GetTickCount() - it3->second.Start < dw - errorGab;
		//	SdkUiConsoleWrite("Abort %ld %ld", GetTickCount() - it3->second.Start, dw - 90);


			result.Status = shorter ? TeleportTypes::Abort : TeleportTypes::Finished;

			result.Duration = 0;
			result.Start = 0;

		}

	}

	return result;
}


DWORD startTime = 0.0;

void Teleport::Recall(void* Unit, const char* Name, const char* Type, void* UserData)
{
	UNREFERENCED_PARAMETER(UserData);
	if (!Menu::Get<bool>("Detector.Teleport.Use") || !Menu::Get<bool>("Detector.Config.Enable"))
	{
		UNREFERENCED_PARAMETER(Unit);
		UNREFERENCED_PARAMETER(Name);
		UNREFERENCED_PARAMETER(Type);
		return;
	}

	//SdkUiConsoleWrite("Start %s %s", Name, Type);
	auto sender = pSDK->EntityManager->GetObjectFromPTR(Unit);
	if (Unit == nullptr || sender == nullptr || sender == NULL || !sender->IsHero())
	{
		return;
	}

	auto netID = sender->GetNetworkID();


	if (!Menu::Get<bool>("Detector.Teleport.TrackMe") && netID == Player.GetNetworkID())
	{
		return;
	}

	if (!netID)
	{
		return;
	}


	/*
	
			Menu::Checkbox("Track Recall", "Detector.Teleport.Recall", true);
		Menu::Checkbox("Track Empowered Recall", "Detector.Teleport.EmpoweredRecall", true);
		SdkUiText("^-> With Rift Herald's Eyes or Baron's Buff");
		Menu::Checkbox("Track Teleport", "Detector.Teleport.Teleport", true);
		Menu::Checkbox("Track Shen's R", "Detector.Teleport.ShenR", true);
		Menu::Checkbox("Track Twisted Fate's R", "Detector.Teleport.TwisitedFateR", true);
	*/

	if ( (strcmp(Type, "recall") == 0 && !Menu::Get<bool>("Detector.Teleport.Recall")) || 
		(strcmp(Type, "SuperRecall") == 0 && !Menu::Get<bool>("Detector.Teleport.EmpoweredRecall")) ||
		(strcmp(Type, "SummonerTeleport") == 0 && !Menu::Get<bool>("Detector.Teleport.Teleport")) ||
		(strcmp(Type, "shenrchannelmanager") == 0 && !Menu::Get<bool>("Detector.Teleport.ShenR")) ||
		(strcmp(Type, "Gate") == 0 && !Menu::Get<bool>("Detector.Teleport.TwisitedFateR"))  //!Menu::Get<bool>("Detector.Teleport.TwisitedFateR")
		)
	{
		return;
	}







	auto testStruct = TeleportDecoder(Unit, Name, Type);


	std::map<unsigned int, RecallInfo>::iterator it = EnemyTeleportList.find(netID);
	if (it != EnemyTeleportList.end())
	{

		//Detector.Teleport.TrackMe
		if (netID != testStruct.UnitNetworkId )
		{
			return;
		}


		if (testStruct.Status == TeleportTypes::Start)
		{


			it->second.Begin = Game::Time();
			it->second.Duration = testStruct.Duration;
			it->second.Active = true;
			it->second.CurrentRecallType = Game::GetRecallType(Type);


			//SdkUiConsoleWrite("Start %f", it->second.Hero->AsAIHeroClient()->GetHealthPercent());
			if (Menu::Get<bool>("Detector.Teleport.ChatStart"))
			{
				//it->second.Hero->GetHealthPercent()
				PrintChat(sender->AsAIHeroClient()->GetCharName(), Type, "Started", it->second.Hero->AsAIHeroClient()->GetHealthPercent(), testStruct.Status);

			}

			//SdkUiConsoleWrite("Start %ld %f", testStruct.Start, testStruct.Duration);
		}
		else if (testStruct.Status == TeleportTypes::Abort)
		{
			it->second.Active = false;

			if (Menu::Get<bool>("Detector.Teleport.ChatAborted"))
			{
				//
				PrintChat(sender->AsAIHeroClient()->GetCharName(), Game::GetRecallName(it->second.CurrentRecallType).c_str(), "Aborted", it->second.Hero->AsAIHeroClient()->GetHealthPercent(), testStruct.Status);

			}
			//SdkUiConsoleWrite("Abort %ld %f", testStruct.Start, testStruct.Duration);
		}
		else if (testStruct.Status == TeleportTypes::Finished)
		{
			it->second.Active = false;

			if (Menu::Get<bool>("Detector.Teleport.ChatFinished"))
			{

				PrintChat(sender->AsAIHeroClient()->GetCharName(), Game::GetRecallName(it->second.CurrentRecallType).c_str(), "Finished", it->second.Hero->AsAIHeroClient()->GetHealthPercent(), testStruct.Status);

			}
			//SdkUiConsoleWrite("Finished %ld %f", testStruct.Start, testStruct.Duration);
		}
		else if (testStruct.Status == TeleportTypes::Unknown)
		{
			it->second.Active = false;
		}


		//		Menu::Checkbox("Notify Start on the Chat", "Detector.Teleport.ChatStart", true);
		//Menu::Checkbox("Notify Aborted on the Chatr", "Detector.Teleport.ChatAborted", true);
		//Menu::Checkbox("Notify Finshed on the Chat", "Detector.Teleport.ChatFinshed", true);
	}
	//AIHeroClient* tempHero = (AIHeroClient*)pSDK->EntityManager->GetObjectFromPTR(Unit);




	/*
	DWORD tick = GetTickCount64();



	if ((unsigned)strlen(Name) == 0)
	{
	//	float diff = time - startTime;

		DWORD newTick = tick - startTime ;

			//SdkUiConsoleWrite("teteport Ended Difference: %f %s", diff, Name);

			SdkUiConsoleWrite("1tickcount done %ld", newTick);


			//least difference 7968
	}
	else
	{
		//SdkUiConsoleWrite("tickcount started %ld", tick);
		//SdkUiConsoleWrite("teteport started %f %s", time, Name);
		startTime = tick;
	}
	*/


	/*
	auto recallType = Game::GetRecallType(Type);

	auto recallName = Game::GetRecallName(recallType);
	SdkUiConsoleWrite("Name : %s Time %f", recallName, Game::Time());
	*/
}



const char* Teleport::GetTeleportName(const char* Name)
{
	if (strcmp(Name, "recall") == 0)
		return "Recall";

	if (strcmp(Name, "SummonerTeleport") == 0)
		return "Teleport";

	if (strcmp(Name, "SuperRecall") == 0)
		return "Empowered Recall";

	if (strcmp(Name, "shenrchannelmanager") == 0)
		return "Shen R";

	if (strcmp(Name, "Gate") == 0)
		return "Twisted Fate R";

	return "Unknown";
}

void Teleport::PrintChat(const char* champName, const char* TeleportName, const char* recallStatusText, float healthPCT, TeleportTypes tpType)
{
	std::stringstream sstream;

	if (Menu::Get<bool>("Detector.Teleport.Chat.Prefix") )
	{
		sstream << R"(<font color="#25dbad">[E2Utility] </font>)" <<"";
	}

	auto Color = Menu::Get<SDKCOLOR>("Detector.Teleport.Chat.ChampName");

	sstream << R"(<font color="#)";
	sstream << std::hex << int(Color.R);
	sstream << std::hex << int(Color.G);
	sstream << std::hex << int(Color.B);
	sstream << R"(">)" << champName << R"(</font>)";

	//sstream << champName << R"(</font>)";
	//SdkUiConsoleWrite(" %s", sstream.str());

	
	Color = Menu::Get<SDKCOLOR>("Detector.Teleport.Chat.Health");
	sstream << R"(<font color="#)";
	sstream << std::hex << int(Color.R);
	sstream << std::hex << int(Color.G);
	sstream << std::hex << int(Color.B);
	sstream << R"(">)";



	std::stringstream ss1;
	ss1.precision(0); //for decimal
	ss1.setf(std::ios_base::fixed, std::ios_base::floatfield);
	int health = (int)healthPCT;


	ss1 << " ["<< healthPCT << "%]";
	sstream << ss1.str() << R"(</font>)";


	sstream << R"(<font color = "#FFFFFF"> has </font>)";


	if (tpType == TeleportTypes::Start)
	{
		Color = Menu::Get<SDKCOLOR>("Detector.Teleport.Chat.Started");
	}
	else if (tpType == TeleportTypes::Abort)
	{
		Color = Menu::Get<SDKCOLOR>("Detector.Teleport.Chat.Aborted");
	}
	else if (tpType == TeleportTypes::Finished)
	{
		Color = Menu::Get<SDKCOLOR>("Detector.Teleport.Chat.Finished");
	}

	sstream << R"(<font color="#)";
	sstream << std::hex << int(Color.R);
	sstream << std::hex << int(Color.G);
	sstream << std::hex << int(Color.B);
	sstream << R"(">)";

	sstream << recallStatusText << R"(</font>)";



	sstream << R"(<font color = "#FFFFFF"> - </font>)";

	Color = Menu::Get<SDKCOLOR>("Detector.Teleport.Chat.Teleport");
	sstream << R"(<font color="#)";
	sstream << std::hex << int(Color.R);
	sstream << std::hex << int(Color.G);
	sstream << std::hex << int(Color.B);
	sstream << R"(">)";

	sstream << GetTeleportName(TeleportName) << R"(</font>)";
	
	Game::PrintChat(sstream.str(), CHAT_FLAG_UNKNOWN1);
}