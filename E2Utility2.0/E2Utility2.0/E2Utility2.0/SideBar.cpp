#include "stdafx.h"
#include "SideBar.h"
#include "TextHelpers.h"
#include "resource.h"


std::map<unsigned int, float> _teleports;
std::map<unsigned int, TeleportStuct> TeleportDecode::TeleportStore;

SideBar::SideBar()
{
	SdkUiConsoleWrite("Sidebar created");
}


SideBar::~SideBar()
= default;

void SideBar::Initialized()
{
	
	SdkUiConsoleWrite("Sidebar Init");
	SettingsUpdate();

	//_enemyObject.emplace_back((EnemyObject(&Player)));
	auto enemyList{ pSDK->EntityManager->GetEnemyHeroes() };
	if (!enemyList.empty())
	{
		for (auto [netID, value] : enemyList)
		{
			_enemyObject.emplace_back(EnemyObject(value));
		}
	}
}

DWORD SidebarTick;

void SideBar::OnTick(void* userData)
{
	if (Game::IsOverlayOpen())
	{

		SettingsUpdate();
	}




	if (SidebarTick + 500 > GetTickCount())
	{
		return;
	}
	SidebarTick = GetTickCount();


	for (auto& enemy : _enemyObject)
	{
		if (!enemy.Unit->IsValid())
		{
			continue;
		}
		enemy.EnemyUpdate();
	}








	/*
	for (auto& enemy : _enemyObject)
	{


		enemy.EnemyUpdate();
	}*/
}

void SideBar::OnDraw(void* userData)
{

	for (auto& enemy : _enemyObject)
	{

		/*
		if(!enemy.Unit->IsValid())
		{
			return;
		}
		*/
		//SdkUiConsoleWrite("Sidebar drawing secne");
		SdkDrawSpriteFromResource(MAKEINTRESOURCEA(enemy.ChampIMG), &enemy.HUDPosition, true); // Champion Face


		
		if (!enemy.Unit->IsVisible() || !enemy.Unit->IsAlive()) //Make champion icon darker for Mising and Death Timer
		{

			SdkDrawSpriteFromResource(MAKEINTRESOURCEA(SB_Invisible), &enemy.HUDPosition, true);

		}
		
		//Vector2 background = Vector2(enemy.DrawingStartPosition[HpBar].x, enemy.DrawingStartPosition[HpBar].y - 2.0f);

		//Draw::LineScreen(&background, &Vector2(background.x + 75.0f, background.y), 20.0f, &Color::Black);


		Draw::LineScreen(&enemy.DrawingStartPosition[HpBar], &enemy.DrawingEndPosition[HpBar], 5.0f, &Color::DarkGreen);

		Draw::LineScreen(&enemy.DrawingStartPosition[MpBar], &enemy.DrawingEndPosition[MpBar], 5.0f, &Color::Blue);

		
		TextHelpers::DrawOutlineText(nullptr, &enemy.DrawingStartPosition[Level],
		                             std::to_string(enemy.Unit->GetLevel()), "",&Color::White, 20, 4, 0, &Color::Black);

		

	


		if (enemy.MissingTimerBool)
		{
			TextHelpers::DrawOutlineText(nullptr, &enemy.DrawingStartPosition[DeathMissingTimer],
				(TextHelpers::TimeFormat(enemy.LastVisibleLeftTime, TimerStyle(1))), "",
				&Color::White
				, 24, 6, 0, &Color::Black);
		}
	
		
		if (!enemy.Unit->IsAlive()) // Death Timer
		{
			TextHelpers::DrawOutlineText(nullptr, &enemy.DrawingStartPosition[DeathMissingTimer],
				(TextHelpers::TimeFormat(enemy.DeathEndTimeLeftTime, TimerStyle(1))), "",
				&Color::Red
				, 24, 6, 0, &Color::Black);

		}



		//Draw::LineScreen(&enemy.DrawingStartPosition[ExpBar], &enemy.DrawingEndPosition[ExpBar], 5.0f, &Color::Purple);




		SdkDrawSpriteFromResource(MAKEINTRESOURCEA(enemy.SummonerSpells1IMG), &enemy.DrawingStartPosition[SummonerSpell1], true); // Summoner Spell 1 Drawing
		if (enemy.SummonerSpells1CD > 0.0f)
		{
			SdkDrawSpriteFromResource(MAKEINTRESOURCEA(SB_SSInvisible), &enemy.DrawingStartPosition[SummonerSpell1], true); // when its CD


			TextHelpers::DrawOutlineText(nullptr, &enemy.DrawingEndPosition[SummonerSpell1],
				(TextHelpers::TimeFormat(enemy.SummonerSpells1CD, TimerStyle(1))), "",
				&Color::White
				, 18, 4, 0, &Color::Black);

			

		}

		SdkDrawSpriteFromResource(MAKEINTRESOURCEA(enemy.SummonerSpells2IMG), &enemy.DrawingStartPosition[SummonerSpell2], true); // Summoner Spell 1 Drawing
		if (enemy.SummonerSpells2CD > 0.0f)
		{
			SdkDrawSpriteFromResource(MAKEINTRESOURCEA(SB_SSInvisible), &enemy.DrawingStartPosition[SummonerSpell2], true); // when its CD
			TextHelpers::DrawOutlineText(nullptr, &enemy.DrawingEndPosition[SummonerSpell2],
				(TextHelpers::TimeFormat(enemy.SummonerSpells2CD, TimerStyle(1))), "",
				&Color::White
				, 18, 4, 0, &Color::Black);
		}



		SdkDrawSpriteFromResource(MAKEINTRESOURCEA(SB_HUD), &enemy.MainFramePosition, true); // HUD Hud MainFrame


		if (enemy.IsRLearned)
		{
			if (enemy.SpellRCD > 0.0F) // on cd Ultimate
			{
				TextHelpers::DrawOutlineText(nullptr, &enemy.DrawingEndPosition[Ultimate],
					(TextHelpers::TimeFormat(enemy.SpellRCD, TimerStyle(2))), "",
					&Color::White
					, 16, 3, 0, &Color::Black);

			}
			else if (enemy.SpellRCD < 0.0f) //ready
			{
				SdkDrawSpriteFromResource(MAKEINTRESOURCEA(SB_Ultimate), &enemy.DrawingStartPosition[Ultimate], true);
			}
		}

	}
}

void SideBar::OnDrawMenu(void* userData)
{
	
	Menu::Tree("Side Bar", "Trackers.SideBar", false, []()
		{
			Menu::Checkbox("Use Side Bar", "Trackers.SideBar.Use", true);

			Menu::DropList("Side Bar Style", "Trackers.SideBar.Style", std::vector<std::string>{ "Vertical", "Horizontal" }, 0);
			Menu::DropList("Side Bar Location", "Trackers.SideBar.Location", std::vector<std::string>{ "On the Left", "On the Right", "On the Top" }, 1);

			Menu::SliderInt("Main Frame Position X-axis", "Trackers.SideBar.Main.DrawingX", 0, -2000, 2000);
			Menu::SliderInt("Main Frame Position Y-axis", "Trackers.SideBar.Main.DrawingY", 0, -2000, 2000);
			Menu::SliderInt("Gap Between Each Frames", "Trackers.SideBar.Main.Gap", 100, 100, 1000);


			Menu::Tree("Summoner Spells Settings", "Trackers.SideBar.SS", false, []()
				{
					Menu::Checkbox("Draw Summoner Spells on Side Bar", "Trackers.SideBar.SSIcon.Use", true);
					Menu::SliderInt("Summoner Spells Icon Position X-axis", "Trackers.SideBar.SSIcon.DrawingX", 0, -200, 200);
					Menu::SliderInt("Summoner Spells Icon Position Y-axis", "Trackers.SideBar.SSIcon.DrawingY", 0, -200, 200);

					Menu::Checkbox("Draw Summoner Spells Timer", "Trackers.SideBar.SS.Use", true);
					//Menu::DropList("Summoner Spells Timer Format", "Trackers.SideBar.SS.Format", std::vector<std::string>{ "MM:SS", "SS"}, 0);
					Menu::SliderInt("Summoner Spells Timer Position X-axis", "Trackers.SideBar.SS.DrawingX", 0, -200, 200);
					Menu::SliderInt("Summoner Spells Timer Position Y-axis", "Trackers.SideBar.SS.DrawingY", 0, -200, 200);
					Menu::SliderInt("Summoner Spells Timer Font Height", "Trackers.SideBar.SS.FontSize", 16, 10, 30);
					Menu::SliderInt("Summoner Spells Timer Font Width", "Trackers.SideBar.SS.FontSize2", 4, 1, 10);

				});



			Menu::Tree("Ultimate Settings", "Trackers.SideBar.Ultimate", false, []()
				{
					Menu::Checkbox("Draw Ultimate on Side Bar", "Trackers.SideBar.UltimateIcon.Use", true);


					Menu::Checkbox("Draw Ultimate Timer", "Trackers.SideBar.Ultimate.Use", true);
					//Menu::DropList("Ultimate Timer Format", "Trackers.SideBar.Ultimate.Format", std::vector<std::string>{ "MM:SS", "SS"}, 0);
					Menu::SliderInt("Ultimate Timer Position X-axis", "Trackers.SideBar.Ultimate.DrawingX", 0, -200, 200);
					Menu::SliderInt("Ultimate Timer Position Y-axis", "Trackers.SideBar.Ultimate.DrawingY", 0, -200, 200);
					Menu::SliderInt("Ultimate Timer Font Height", "Trackers.SideBar.Ultimate.FontSize", 16, 10, 30);
					Menu::SliderInt("Ultimate Timer Font Width", "Trackers.SideBar.Ultimate.FontSize2", 4, 1, 10);

				});

			Menu::Tree("Missing Timer Settings", "Trackers.SideBar.MissTimer", false, []()
				{

					Menu::Checkbox("Draw Missing Timer", "Trackers.SideBar.MissTimer.Use", true);
					//Menu::DropList("Missing Timer Format", "Trackers.SideBar.MissTimer.Format", std::vector<std::string>{ "MM:SS", "SS"}, 1);
					Menu::SliderInt("Missing Timer Position X-axis", "Trackers.SideBar.MissTimer.DrawingX", 0, -200, 200);
					Menu::SliderInt("Missing Timer Position Y-axis", "Trackers.SideBar.MissTimer.DrawingY", 0, -200, 200);
					Menu::SliderInt("Missing Timer Font Height", "Trackers.SideBar.MissTimer.FontSize", 24, 10, 30);
					Menu::SliderInt("Missing Timer Font Width", "Trackers.SideBar.MissTimer.FontSize2", 6, 1, 10);

				});


			Menu::Tree("Death Timer Settings", "Trackers.SideBar.DeathTimer", false, []()
				{

					Menu::Checkbox("Draw Death Timer", "Trackers.SideBar.DeathTimer.Use", true);
					//Menu::DropList("Death Timer Format", "Trackers.SideBar.DeathTimer.Format", std::vector<std::string>{ "MM:SS", "SS"}, 1);
					Menu::SliderInt("Death Timer Position X-axis", "Trackers.SideBar.DeathTimer.DrawingX", 0, -200, 200);
					Menu::SliderInt("Death Timer Position Y-axis", "Trackers.SideBar.DeathTimer.DrawingY", 0, -200, 200);
					Menu::SliderInt("Death Timer Font Height", "Trackers.SideBar.DeathTimer.FontSize", 24, 10, 30);
					Menu::SliderInt("Death Timer Font Width", "Trackers.SideBar.DeathTimer.FontSize2", 6, 1, 10);

				});



			Menu::Tree("Level Display Settings", "Trackers.SideBar.Level", false, []()
				{

					Menu::Checkbox("Draw Level", "Trackers.SideBar.Level.Use", true);
					Menu::SliderInt("Level Position X-axis", "Trackers.SideBar.Level.DrawingX", 0, -200, 200);
					Menu::SliderInt("Level Position Y-axis", "Trackers.SideBar.Level.DrawingY", 0, -200, 200);
					Menu::SliderInt("Level Font Height", "Trackers.SideBar.Level.FontSize", 20, 10, 30);
					Menu::SliderInt("Level Font Width", "Trackers.SideBar.Level.FontSize2", 3, 1, 10);

				});


			Menu::Tree("Recall Border Settings", "Trackers.SideBar.Recall", false, []()
				{

					Menu::Checkbox("Draw Recall Border", "Trackers.SideBar.Recall.Use", true);
					Menu::DropList("Recall Border Color - On Start", "Trackers.SideBar.Recall.OnStart", std::vector<std::string>{ "Blue", "Dark Yellow", "Green", "None"}, 0);
					Menu::DropList("Recall Border Color - On Abort", "Trackers.SideBar.Recall.OnAbort", std::vector<std::string>{ "Blue", "Dark Yellow", "Green", "None"}, 1);
					Menu::DropList("Recall Border Color - On Finish", "Trackers.SideBar.Recall.OnFinish", std::vector<std::string>{ "Blue", "Dark Yellow", "Green", "None"}, 2);
				});


			Menu::Tree("Bars Settings", "Trackers.SideBar.Bars", false, []()
				{

					Menu::Checkbox("Draw Bars", "Trackers.SideBar.Bars.Use", true);
					/*
					Menu::SliderInt("Bars Width", "Trackers.SideBar.Bars.Width", 5, 1, 10);
					SdkUiText("^-> Default 5");
					Menu::SliderInt("Bars Length", "Trackers.SideBar.Bars.Length", 73, 15, 100);
					SdkUiText("^-> Default 73");
					Menu::SliderInt("Gap Between Bars", "Trackers.SideBar.Bars.Gap", 6, 1, 20);
					SdkUiText("^-> Default 6");
					*/

					Menu::Checkbox("Draw Health Bar", "Trackers.SideBar.Bars.Health.Use", true);
					Menu::SliderInt("Health Bar Position X-axis", "Trackers.SideBar.Bars.Health.DrawingX", 0, -200, 200);
					Menu::SliderInt("Health Bar Position Y-axis", "Trackers.SideBar.Bars.Health.DrawingY", 0, -200, 200);


					Menu::Checkbox("Draw Mana Bar", "Trackers.SideBar.Bars.Mana.Use", true);
					Menu::SliderInt("Mana Bar Position X-axis", "Trackers.SideBar.Bars.Mana.DrawingX", 0, -200, 200);
					Menu::SliderInt("Mana Bar Position Y-axis", "Trackers.SideBar.Bars.Mana.DrawingY", 0, -200, 200);



					Menu::Checkbox("Draw Experience Bar", "Trackers.SideBar.Bars.Experience.Use", true);
					Menu::SliderInt("Experience Bar Position X-axis", "Trackers.SideBar.Bars.Experience.DrawingX", 0, -200, 200);
					Menu::SliderInt("Experience Bar Position Y-axis", "Trackers.SideBar.Bars.Experience.DrawingY", 0, -200, 200);


				});


			/*
				Menu::Checkbox("Draw Spells Cooldown Timer", "Trackers.CooldownTracker.SCTimer", true);
				Menu::DropList("Spells Cooldown Timer Format", "Trackers.CooldownTracker.SCTFormat", std::vector<std::string>{ "MM:SS", "SS" , "SS But MM:SS on R"}, 1);
				Menu::SliderInt("Spells Cooldown Timer Position X-axis", "Trackers.CooldownTracker.SCTDrawingX", 0, -200, 200);
				Menu::SliderInt("Spells Cooldown Timer Position Y-axis", "Trackers.CooldownTracker.SCTDrawingY", 0, -200, 200);
				Menu::SliderInt("Spells Cooldown Timer Font Height", "Trackers.CooldownTracker.SCTFontSize", 13, 10, 30);
				Menu::SliderInt("Spells Cooldown Timer Font Width", "Trackers.CooldownTracker.SCTFontSize2", 4, 1, 10);
				Menu::DropList("^-> Spells Cooldown Timer Color", "Trackers.CooldownTracker.SCTColor", ColorMenuList, 11);
				Menu::DropList("^-> Spells Cooldown Timer OutLine Color", "Trackers.CooldownTracker.SCTOutLineColor", ColorMenuList, 0);
			*/


			//	Menu::SliderInt("SideBar Position X-axis", "Trackers.SideBar.DrawingX", 0, -50, 50);
			//	Menu::SliderInt("SideBar Position Y-axis", "Trackers.SideBar.DrawingY", 0, -50, 50);



		});

}

void SideBar::OnRecall(void* Unit, const char* Name, const char* Type, void* UserData)
{
	
	UNREFERENCED_PARAMETER(UserData);
	/*
	if (!Menu::Get<bool>("Trackers.SideBar.Use") || !Menu::Get<bool>("Trackers.SideBar.Recall.Use"))
	{
		UNREFERENCED_PARAMETER(Unit);
		UNREFERENCED_PARAMETER(Name);
		UNREFERENCED_PARAMETER(Type);
		return;
	}
	*/
	//SdkUiConsoleWrite("Start %s %s", Name, Type);
	auto sender = pSDK->EntityManager->GetObjectFromPTR(Unit);
	if (Unit == nullptr || sender == nullptr || !sender->IsHero() || sender->IsAlly())
	{
		return;
	}

	auto netID = sender->GetNetworkID();


	if (!netID)
	{
		return;
	}

	auto testStruct = TeleportDecode::TeleportDecoderFunction(Unit, Name, Type);

	for (auto& value : _enemyObject)
	{
		if (value.Unit->GetNetworkID() == netID)
		{
			if (testStruct.Status == TeleportTypes::Abort || testStruct.Status == TeleportTypes::Finished)
			{


				pSDK->EventHandler->DelayedAction([testStruct, netID]()
					{
						_teleports[netID] = Game::Time() + testStruct.Status == TeleportTypes::Finished ? 300.0f : 200.0f;

						//SdkUiConsoleWrite("Cast Cleanse Delayed : %f", Game::Time());
					}, 250);


			}
			//SdkUiConsoleWrite("TP Added %d", testStruct.Status);
			value._teleportStatus = testStruct.Status;
			value.LastTeleportStatusTime = Game::Time();

		}
	}

}

void SideBar::SettingsUpdate()
{
//	CooldownTrackerSettings.EnableCooldownTracker = Menu::Get<bool>("Trackers.CooldownTracker.Use");

	SideBarSettings.screenPosSelection = Menu::Get<int>("Trackers.SideBar.Location");
	SideBarSettings.arrayStyle = Menu::Get<int>("Trackers.SideBar.Style");
	SideBarSettings.SidebarGab = Menu::Get<int>("Trackers.SideBar.Main.Gap");
	SideBarSettings.MainFramePosX = Menu::Get<int>("Trackers.SideBar.Main.DrawingX");
	SideBarSettings.MainFramePosY = Menu::Get<int>("Trackers.SideBar.Main.DrawingY");

	int i = 0;
	for (auto& enemy : _enemyObject)
	{
		enemy.PositionUpdate(i);
		++i;
	}

}
