#include "stdafx.h"
#include "SideBar.h"
#include "DropLists.h"
#include "resource.h"
#include "DrawHelper.h"
#include "ChampionNames.h"
#include <iomanip>
#include <string.h>

std::vector<EnemyObject> _enemyObject;

std::map<unsigned int, float> _teleports;
std::map<unsigned int, TeleportStuct> TeleportDecode::TeleportStore;

std::vector<SideBarMenu> SideBarMenuList;

Resolution PlayerResolution;

Vector2 MainFramePosition;

bool FirstRunChecker = false;

const float length = 0.73f;

int arrayStyle = 0;
int SidebarGab = 100;

DWORD SidebarTick;

/*
Vector2 HpBarPos;
Vector2 MpBarPos;
Vector2 ExpBarPos;
Vector2 LevelPos;
Vector2 DeathTimerPos;
Vector2 MissingTimerPos;
bool MissingTimerBool;
Vector2 UltimatePos;
Vector2 SS1Pos;
Vector2 SS2Pos;
*/



const int RequiredExp[] = { 0, 280, 660, 1140, 1720, 2400, 3180, 4060, 5040, 6120, 7300, 8580, 9960, 11440, 13020, 14700, 16480, 18360, 9999999999999 };

void SideBar::Init()
{
	PlayerResolution = Renderer::GetResolution();
	_enemyObject.clear();
	SideBarMenuList.clear();
	FirstRunChecker = false;
	arrayStyle = 0;
	SidebarGab = 100;
	SidebarTick = 0;
	//MissingTimerBool = false;

	auto enemyList{ pSDK->EntityManager->GetEnemyHeroes() };

	for (auto[netID, value] : enemyList)
	{
		_enemyObject.emplace_back(EnemyObject(value));
	//	SdkUiConsoleWrite("did");
	}

	/*

	SideBarMenuList[0] = SideBarMenu("Main", false);
	SideBarMenuList[1] = SideBarMenu("SSIcon", false);
	SideBarMenuList[2] = SideBarMenu("SS", true);
	SideBarMenuList[3] = SideBarMenu("Ultimate", true);
	SideBarMenuList[4] = SideBarMenu("MissTimer", true);
	SideBarMenuList[5] = SideBarMenu("DeathTimer", true);
	SideBarMenuList[6] = SideBarMenu("Level", true);
	SideBarMenuList[7] = SideBarMenu("Bars.Health", true);
	SideBarMenuList[8] = SideBarMenu("Bars.Mana", true);
	SideBarMenuList[9] = SideBarMenu("Bars.Experience", true);
	*/


	SideBarMenuList.emplace_back("Main", false); //Main Frame 0

	SideBarMenuList.emplace_back("SSIcon", false); //Summoner Spells Icon 1

	SideBarMenuList.emplace_back("SS", true); //Summoner Spells Timer 2


	SideBarMenuList.emplace_back("Ultimate", true); //Ultimate Timer 3


	SideBarMenuList.emplace_back("MissTimer", true); //MissTimer Timer 4

	SideBarMenuList.emplace_back("DeathTimer", true); //DeathTimer Timer 5

	SideBarMenuList.emplace_back("Level", true); //Level 6

	SideBarMenuList.emplace_back("Bars.Health", false); //Health 7

	SideBarMenuList.emplace_back("Bars.Mana", true); //Mana 8
	SideBarMenuList.emplace_back("Bars.Experience", true); //Experience 9

	SideBarMenuList.emplace_back("Bars", false); //Bar 10
	SideBarMenuList.emplace_back("Recall", false); //Recall 11


	//_enemyObject.emplace_back(EnemyObject(&Player));

	pSDK->EventHandler->RegisterCallback(CallbackEnum::Recall, SideBar::RecallTrack);


}

void SideBar::MenuLoader()
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
			Menu::DropList("^-> Summoner Spells Timer Color", "Trackers.SideBar.SS.Color", ColorMenuList, 11);
			Menu::DropList("^-> Summoner Spells Timer OutLine Color", "Trackers.SideBar.SS.OutLineColor", ColorMenuList, 0);
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
			Menu::DropList("^-> Summoner Ultimate Timer Color", "Trackers.SideBar.Ultimate.Color", ColorMenuList, 11);
			Menu::DropList("^-> Summoner Ultimate Timer OutLine Color", "Trackers.SideBar.Ultimate.OutLineColor", ColorMenuList, 0);
		});

		Menu::Tree("Missing Timer Settings", "Trackers.SideBar.MissTimer", false, []()
		{

			Menu::Checkbox("Draw Missing Timer", "Trackers.SideBar.MissTimer.Use", true);
			//Menu::DropList("Missing Timer Format", "Trackers.SideBar.MissTimer.Format", std::vector<std::string>{ "MM:SS", "SS"}, 1);
			Menu::SliderInt("Missing Timer Position X-axis", "Trackers.SideBar.MissTimer.DrawingX", 0, -200, 200);
			Menu::SliderInt("Missing Timer Position Y-axis", "Trackers.SideBar.MissTimer.DrawingY", 0, -200, 200);
			Menu::SliderInt("Missing Timer Font Height", "Trackers.SideBar.MissTimer.FontSize", 24, 10, 30);
			Menu::SliderInt("Missing Timer Font Width", "Trackers.SideBar.MissTimer.FontSize2", 6, 1, 10);
			Menu::DropList("^-> Missing Timer Color", "Trackers.SideBar.MissTimer.Color", ColorMenuList, 11);
			Menu::DropList("^-> Missing Timer OutLine Color", "Trackers.SideBar.MissTimer.OutLineColor", ColorMenuList, 0);
		});


		Menu::Tree("Death Timer Settings", "Trackers.SideBar.DeathTimer", false, []()
		{

			Menu::Checkbox("Draw Death Timer", "Trackers.SideBar.DeathTimer.Use", true);
			//Menu::DropList("Death Timer Format", "Trackers.SideBar.DeathTimer.Format", std::vector<std::string>{ "MM:SS", "SS"}, 1);
			Menu::SliderInt("Death Timer Position X-axis", "Trackers.SideBar.DeathTimer.DrawingX", 0, -200, 200);
			Menu::SliderInt("Death Timer Position Y-axis", "Trackers.SideBar.DeathTimer.DrawingY", 0, -200, 200);
			Menu::SliderInt("Death Timer Font Height", "Trackers.SideBar.DeathTimer.FontSize", 24, 10, 30);
			Menu::SliderInt("Death Timer Font Width", "Trackers.SideBar.DeathTimer.FontSize2", 6, 1, 10);
			Menu::DropList("^-> Death Timer Color", "Trackers.SideBar.DeathTimer.Color", ColorMenuList, 10);
			Menu::DropList("^-> Death Timer OutLine Color", "Trackers.SideBar.DeathTimer.OutLineColor", ColorMenuList, 0);
		});

		

		Menu::Tree("Level Display Settings", "Trackers.SideBar.Level", false, []()
		{

			Menu::Checkbox("Draw Level", "Trackers.SideBar.Level.Use", true);
			Menu::SliderInt("Level Position X-axis", "Trackers.SideBar.Level.DrawingX", 0, -200, 200);
			Menu::SliderInt("Level Position Y-axis", "Trackers.SideBar.Level.DrawingY", 0, -200, 200);
			Menu::SliderInt("Level Font Height", "Trackers.SideBar.Level.FontSize", 20, 10, 30);
			Menu::SliderInt("Level Font Width", "Trackers.SideBar.Level.FontSize2", 3, 1, 10);
			Menu::DropList("^-> Level Color", "Trackers.SideBar.Level.Color", ColorMenuList, 11);
			Menu::DropList("^-> Level OutLine Color", "Trackers.SideBar.Level.OutLineColor", ColorMenuList, 0);
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
			Menu::DropList("^-> Health Bar Color", "Trackers.SideBar.Bars.Health.Color", ColorMenuList, 4);
		//	Menu::DropList("^-> Health Bar OutLine Color", "Trackers.SideBar.Bars.Health.OutLineColor", ColorMenuList, 0);


			Menu::Checkbox("Draw Mana Bar", "Trackers.SideBar.Bars.Mana.Use", true);
			Menu::SliderInt("Mana Bar Position X-axis", "Trackers.SideBar.Bars.Mana.DrawingX", 0, -200, 200);
			Menu::SliderInt("Mana Bar Position Y-axis", "Trackers.SideBar.Bars.Mana.DrawingY", 0, -200, 200);
			Menu::DropList("^-> Mana Bar Color", "Trackers.SideBar.Bars.Mana.Color", ColorMenuList, 1);
			//Menu::DropList("^-> Mana Bar OutLine Color", "Trackers.SideBar.Bars.Mana.OutLineColor", ColorMenuList, 0);



			Menu::Checkbox("Draw Experience Bar", "Trackers.SideBar.Bars.Experience.Use", true);
			Menu::SliderInt("Experience Bar Position X-axis", "Trackers.SideBar.Bars.Experience.DrawingX", 0, -200, 200);
			Menu::SliderInt("Experience Bar Position Y-axis", "Trackers.SideBar.Bars.Experience.DrawingY", 0, -200, 200);
			Menu::DropList("^-> Experience Bar Color", "Trackers.SideBar.Bars.Experience.Color", ColorMenuList, 9);
			//Menu::DropList("^-> Experience Bar OutLine Color", "Trackers.SideBar.Bars.Experience.OutLineColor", ColorMenuList, 0);

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

void SideBar::TickLoader()
{
	if (!Menu::Get<bool>("Trackers.SideBar.Use"))
	{
		return;
	}



	if (Game::IsOverlayOpen() || FirstRunChecker == false)
	{
		FirstRunChecker = true;
		int screenPosSelection = Menu::Get<int>("Trackers.SideBar.Location");
		arrayStyle = Menu::Get<int>("Trackers.SideBar.Style");
		SidebarGab = Menu::Get<int>("Trackers.SideBar.Main.Gap");
		//int gabMainFrames = float(Menu::Get<int>("Trackers.SideBar.Main.Gap"));

		Vector2 MainFramePos;

		Vector2 tempPos;
		if (screenPosSelection == 0) // on the left
		{
			tempPos = Vector2(0.0f + 50.0f, (PlayerResolution.Height / 2.0f) - (PlayerResolution.Height / 4.0f) - (PlayerResolution.Height / 10.0f));

		}
		else if (screenPosSelection == 1) // on the right
		{
			tempPos = Vector2(PlayerResolution.Width - 50.0f, (PlayerResolution.Height / 2.0f) - (PlayerResolution.Height / 4.0f) - (PlayerResolution.Height / 10.0f));
		}
		else if (screenPosSelection == 2) // on the top
		{
			tempPos = Vector2((PlayerResolution.Width / 2.0f) - (PlayerResolution.Width / 4.0f), 50.0f);
		}



		for (auto &value : SideBarMenuList)
		{
			value.UpdateInsideFloats();
		}


		//SideBarMenuList[0].UpdateInsideFloats(); // Main Frame update







		//float AdjustMainFrameX = float(Menu::Get<int>("Trackers.SideBar.Main.DrawingX"));
		//float AdjustMainFrameY = float(Menu::Get<int>("Trackers.SideBar.Main.DrawingY"));

		tempPos.x += SideBarMenuList[0].DrawX;
		tempPos.y += SideBarMenuList[0].DrawY;
		MainFramePosition = tempPos;



		if (!_enemyObject.empty())
		{
			for (auto& enemy : _enemyObject)
			{


				enemy.PositionList.HpBarPos = Vector2((tempPos.x - 34.0f + SideBarMenuList[7].DrawX) + enemy.HPlength, tempPos.y + 20.0f + SideBarMenuList[7].DrawY);
				enemy.PositionList.HpBarOrignalPos = Vector2((tempPos.x - 34.0f + SideBarMenuList[7].DrawX), tempPos.y + 20.0f + SideBarMenuList[7].DrawY);

				enemy.PositionList.MpBarPos = Vector2((tempPos.x - 34.0f + SideBarMenuList[8].DrawX) + enemy.MPlength, tempPos.y + 26.0f + SideBarMenuList[8].DrawY);
				enemy.PositionList.MpBarOrignalPos = Vector2((tempPos.x - 34.0f + SideBarMenuList[8].DrawX), tempPos.y + 26.0f + SideBarMenuList[8].DrawY);

				enemy.PositionList.ExpBarPos = Vector2((tempPos.x - 34.0f + SideBarMenuList[9].DrawX) + enemy.EXPlength, tempPos.y + 32.0f + SideBarMenuList[9].DrawY);
				enemy.PositionList.ExpBarOrignalPos = Vector2((tempPos.x - 34.0f + SideBarMenuList[9].DrawX), tempPos.y + 32.0f + SideBarMenuList[9].DrawY);

				enemy.PositionList.DeathTimerPos = Vector2(tempPos.x - 15.0f + SideBarMenuList[5].DrawX, tempPos.y - 25.0f + SideBarMenuList[5].DrawY);

				enemy.PositionList.MissingTimerPos = Vector2(tempPos.x - 15.0f + SideBarMenuList[4].DrawX, tempPos.y - 25.0f + SideBarMenuList[4].DrawY);
				enemy.PositionList.LevelPos = Vector2(tempPos.x + 3.0f + SideBarMenuList[6].DrawX, tempPos.y - 5.0f + SideBarMenuList[6].DrawY);

				enemy.PositionList.UltimatePos = Vector2(tempPos.x - 34.0f + SideBarMenuList[3].DrawX, tempPos.y - 29.0f + SideBarMenuList[3].DrawY);

				enemy.PositionList.SS1Pos = Vector2(tempPos.x + 30.0f + SideBarMenuList[1].DrawX, tempPos.y - 22.0f + SideBarMenuList[1].DrawY);
				enemy.PositionList.SS1TimerPos = Vector2(tempPos.x + 10.0f + SideBarMenuList[2].DrawX, tempPos.y - 26.0f + SideBarMenuList[2].DrawY);

				enemy.PositionList.SS2Pos = Vector2(tempPos.x + 30.0f + SideBarMenuList[1].DrawX, tempPos.y + 4.0f + SideBarMenuList[1].DrawY);

				enemy.PositionList.SS2TimerPos = Vector2(tempPos.x + 20.0f + SideBarMenuList[2].DrawX, tempPos.y - 4.0f + SideBarMenuList[2].DrawY);




				if (arrayStyle == 0) //vertical
				{
					tempPos.y += float(SidebarGab);
				}
				else if (arrayStyle == 1) // horizontal
				{
					tempPos.x += float(SidebarGab);
				}

			}
		}

	}


	if (SidebarTick + 500 > GetTickCount())
	{
		return;
	}
	SidebarTick = GetTickCount();


	for (auto& enemy : _enemyObject)
	{
		enemy.EnemyUpdate();

		float currentTime = Game::Time();

		if (!enemy.Unit->IsAlive() && currentTime > enemy.DeathEndTime)
		{
			enemy.DeathEndTime = currentTime + enemy.Unit->GetDeathDuration() + 1.0f;
		}
		else if (enemy.Unit->IsAlive())
		{
			enemy.DeathEndTime = 0.0f;
		}



		if (SideBarMenuList[10].Enable)
		{



			enemy.HPlength = enemy.Unit->GetHealthPercent()*length;

			enemy.MPlength = enemy.Unit->GetManaPercent()*length;

		
			//HpBarPos = Vector2((MainFramePosition.x - 34.0f) + hp * length, MainFramePosition.y + 20.0f);

		
		//	float mp = enemy.Unit->GetManaPercent();

			//MpBarPos = Vector2((MainFramePosition.x - 34.0f) + mp * length, MainFramePosition.y + 26.0f);

			//float exp = enemy.Unit->GetExperience();

			float percent = 0.0f;
			if (enemy.Unit->GetLevel() == 18)
			{
				percent = 100.0f;
			}
			else
			{
				percent = (100.0f / (RequiredExp[enemy.Unit->GetLevel()] - RequiredExp[enemy.Unit->GetLevel() - 1]) * (enemy.Unit->GetExperience() - RequiredExp[enemy.Unit->GetLevel() - 1]));
			}

			enemy.EXPlength = percent * length;



		}

		if (SideBarMenuList[6].Enable) // Level
		{

			enemy.CurrentLevel = enemy.Unit->GetLevel();

		}

		if (SideBarMenuList[5].Enable)
		{
			if (!enemy.Unit->IsAlive()) // Death Timer
			{
				enemy.DeathEndTimeLeftTime = enemy.DeathEndTime - currentTime;
			}
		}


		SDKVECTOR enemyPos = enemy.Unit->GetPosition();
		if (enemy.Unit->IsAlive() && enemy.LastPosition.Distance(enemyPos) > 500.0f)
		{
			enemy.LastVisible = currentTime;
		}

		enemy.LastPosition = enemyPos;

		if (enemy.Unit->IsVisible() || !enemy.Unit->IsAlive())
		{
			enemy.LastVisible = currentTime;
		}

		if (SideBarMenuList[4].Enable)
		{
			if (!enemy.Unit->IsVisible() && enemy.Unit->IsAlive() && currentTime - enemy.LastVisible > 3.0f) // Missing Timer
			{

				enemy.LastVisibleLeftTime = currentTime - enemy.LastVisible;
				enemy.PositionList.MissingTimerBool = true;
			}
			else
			{
				enemy.PositionList.MissingTimerBool = false;
			}
		}


		if (enemy.SpellR.Learned && enemy.SpellR.Level > 0 && SideBarMenuList[3].Enable)
		{
			enemy.SpellRCD = enemy.SpellR.CooldownExpires - currentTime;;

		}


		if (SideBarMenuList[2].Enable)
		{
			enemy.SummonerSpells1CD = enemy.SummonerSpells1.CooldownExpires - currentTime;
			//SdkUiConsoleWrite("1CD %f", enemy.SummonerSpells1CD);
			
			enemy.SummonerSpells2CD = enemy.SummonerSpells2.CooldownExpires - currentTime;
			//SdkUiConsoleWrite("2CD %f", enemy.SummonerSpells2CD);
		}

		if (SideBarMenuList[11].Enable)
		{
			if (enemy._teleportStatus == TeleportTypes::Start || (enemy._teleportStatus == TeleportTypes::Finished || enemy._teleportStatus == TeleportTypes::Abort)
				&& currentTime <= enemy.LastTeleportStatusTime + 3.0f)
			{
				//tempPos = MainFramePos; // Recall Tracker Sprite drawer.

				int recallSelection = 3; // 3 is none
				if (enemy._teleportStatus == TeleportTypes::Start)
				{
					recallSelection = Menu::Get<int>("Trackers.SideBar.Recall.OnStart");
				}
				else if (enemy._teleportStatus == TeleportTypes::Abort)
				{
					recallSelection = Menu::Get<int>("Trackers.SideBar.Recall.OnAbort");
				}
				else if (enemy._teleportStatus == TeleportTypes::Finished)
				{
					recallSelection = Menu::Get<int>("Trackers.SideBar.Recall.OnFinish");
				}

				int FinalSelection = SB_RecallStart;



				if (recallSelection != 3)
				{
					if (recallSelection == 0) // blue
					{
						FinalSelection = SB_RecallStart;
					}
					else if (recallSelection == 1) //dark yellow
					{
						FinalSelection = SB_RecallAbort;
					}
					else if (recallSelection == 2) //green
					{
						FinalSelection = SB_RecallFinish;
					}
					enemy.TrackRecall = true;
					enemy.RecallType = FinalSelection;

			
				}


			}
			else
			{
				enemy.TrackRecall = false;
			}
		}

	}





}

void SideBar::DrawLoader()
{
	
	if (!Menu::Get<bool>("Trackers.SideBar.Use"))
	{
		return;
	}
	


	Vector2 tempMainPos = MainFramePosition;


	for (auto& enemy : _enemyObject)
	{



		SdkDrawSpriteFromResource(MAKEINTRESOURCEA(enemy.ChampIMG), &Vector2(tempMainPos.x - 11.0f, tempMainPos.y - 10.0f), true); // Champion Face




		if (SideBarMenuList[10].Enable) // Health
		{

			Vector2 background = Vector2(enemy.PositionList.HpBarOrignalPos.x, enemy.PositionList.HpBarOrignalPos.y - 2.0f);

			Draw::LineScreen(&background, &Vector2(background.x + 75.0f, background.y), 20.0f, &Color::Black);

			if (SideBarMenuList[7].Enable) // Health
			{
				//SdkUiConsoleWrite("i %d", SideBarMenuList[7].FontColor);

				//DrawHelper::DrawOutlineLineScreen(&enemy.PositionList.HpBarOrignalPos, &enemy.PositionList.HpBarPos, 5.0f, &Color::DarkGreen, &Color::Black); //-40 43
				Draw::LineScreen(&enemy.PositionList.HpBarOrignalPos, &enemy.PositionList.HpBarPos, 5.0f, &Color::DarkGreen);
				//DrawHelper::DrawOutlineLineScreen(&Vector2(barPosStatic.x + hp * 0.73f, barPosStatic.y), &Vector2(barPosStatic.x + hp * 1.0f, barPosStatic.y), 5.0f, &Color::Grey, &Color::Black); //-40 43
			}

			if (SideBarMenuList[8].Enable) // Mana
			{

				//DrawHelper::DrawOutlineLineScreen(&enemy.PositionList.MpBarOrignalPos, &enemy.PositionList.MpBarPos, 5.0f, &Color::Blue, &Color::Black); //-40 43
				Draw::LineScreen(&enemy.PositionList.MpBarOrignalPos, &enemy.PositionList.MpBarPos, 5.0f, &DropLists::GetColor(SideBarMenuList[8].FontColor));
				//DrawHelper::DrawOutlineLineScreen(&tempPos, &Vector2(tempPos.x + mp * length, tempPos.y), float(Menu::Get<int>("Trackers.SideBar.Bars.Width")), &DropLists::GetColor(Menu::Get<int>("Trackers.SideBar.Bars.Mana.Color")), &DropLists::GetColor(Menu::Get<int>("Trackers.SideBar.Bars.Mana.OutLineColor")));//-40 43
				//DrawHelper::DrawOutlineLineScreen(&Vector2(barPosStatic.x + mp * 0.73f, barPosStatic.y), &Vector2(barPosStatic.x + 73.0f, barPosStatic.y), 5.0f, &Color::Grey, &Color::Black); //-40 43
			}


			if (SideBarMenuList[9].Enable) // Mana
			{

				//DrawHelper::DrawOutlineLineScreen(&enemy.PositionList.ExpBarOrignalPos, &enemy.PositionList.ExpBarPos, 5.0f, &Color::Purple, &Color::Black); //-40 43
				Draw::LineScreen(&enemy.PositionList.ExpBarOrignalPos, &enemy.PositionList.ExpBarPos, 5.0f, &DropLists::GetColor(SideBarMenuList[9].FontColor));
				//DrawHelper::DrawOutlineLineScreen(&tempPos, &Vector2(tempPos.x + percent * length, tempPos.y), float(Menu::Get<int>("Trackers.SideBar.Bars.Width")) - 1.0f, &DropLists::GetColor(Menu::Get<int>("Trackers.SideBar.Bars.Experience.Color")), &DropLists::GetColor(Menu::Get<int>("Trackers.SideBar.Bars.Experience.OutLineColor")));
				//DrawHelper::DrawOutlineLineScreen(&Vector2(barPosStatic.x + percent * 0.73f, barPosStatic.y), &Vector2(barPosStatic.x + (100.0f * 0.73f), barPosStatic.y), 5.0f, &Color::Grey, &Color::Black); //-40 43
				//Draw::LineScreen(&Vector2(barPosStatic.x + (percent * 0.73f) + 1.0f, barPosStatic.y), &Vector2(barPosStatic.x + (100.0f * 0.73f), barPosStatic.y), 5.0f, &Color::Grey);
			}
		}


		if (SideBarMenuList[6].Enable) // Level
		{

			DrawHelper::DrawOutlineText(NULL, &enemy.PositionList.LevelPos, std::to_string(enemy.CurrentLevel).c_str(), "Calibri Bold", &DropLists::GetColor(SideBarMenuList[6].FontColor), SideBarMenuList[6].FontSize1, SideBarMenuList[6].FontSize2, 0,
				&DropLists::GetColor(SideBarMenuList[6].OutlineFontColor), false);

			/*
			DrawHelper::DrawOutlineText(NULL, &tempPos, std::to_string(enemy.Unit->GetLevel()).c_str(), "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Trackers.SideBar.Level.Color")), Menu::Get<int>("Trackers.SideBar.Level.FontSize"), Menu::Get<int>("Trackers.SideBar.Level.FontSize2"), 0,
				&DropLists::GetColor(Menu::Get<int>("Trackers.SideBar.Level.OutlineColor")), false);
				*/

		}

		if (SideBarMenuList[5].Enable)
		{
			if (!enemy.Unit->IsAlive()) // Death Timer
			{

				std::stringstream ss1;
				ss1.precision(0); //for decimal
				ss1.setf(std::ios_base::fixed, std::ios_base::floatfield);

				ss1 << enemy.DeathEndTimeLeftTime;

				DrawHelper::DrawOutlineText(NULL, &enemy.PositionList.DeathTimerPos, ss1.str().c_str(), "Calibri Bold", &DropLists::GetColor(SideBarMenuList[5].FontColor), SideBarMenuList[5].FontSize1, SideBarMenuList[5].FontSize2, 0,
					&DropLists::GetColor(SideBarMenuList[5].OutlineFontColor), false);

				/*
				DrawHelper::DrawOutlineText(NULL, &tempPos, ss1.str().c_str(), "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Trackers.SideBar.DeathTimer.Color")), Menu::Get<int>("Trackers.SideBar.DeathTimer.FontSize"), Menu::Get<int>("Trackers.SideBar.DeathTimer.FontSize2"), 0,
					&DropLists::GetColor(Menu::Get<int>("Trackers.SideBar.DeathTimer.OutlineColor")), false);
					*/

			}
		}


		if (!enemy.Unit->IsVisible() || !enemy.Unit->IsAlive()) //Make champion icon darker for Mising and Death Timer
		{

			SdkDrawSpriteFromResource(MAKEINTRESOURCEA(SB_Invisible), &Vector2(tempMainPos.x - 12.0f, tempMainPos.y - 10.0f), true);

		}




		if (SideBarMenuList[4].Enable)
		{
			if (enemy.PositionList.MissingTimerBool) // Missing Timer
			{

				std::stringstream ss1;
				ss1.precision(0); //for decimal
				ss1.setf(std::ios_base::fixed, std::ios_base::floatfield);

				ss1 << enemy.LastVisibleLeftTime;

				DrawHelper::DrawOutlineText(NULL, &enemy.PositionList.MissingTimerPos, ss1.str().c_str(), "Calibri Bold", &DropLists::GetColor(SideBarMenuList[4].FontColor), SideBarMenuList[4].FontSize1, SideBarMenuList[4].FontSize2, 0,
					&DropLists::GetColor(SideBarMenuList[4].OutlineFontColor), false);
				/*
				DrawHelper::DrawOutlineText(NULL, &tempPos, ss1.str().c_str(), "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Trackers.SideBar.MissTimer.Color")), Menu::Get<int>("Trackers.SideBar.MissTimer.FontSize"), Menu::Get<int>("Trackers.SideBar.MissTimer.FontSize2"), 0,
					&DropLists::GetColor(Menu::Get<int>("Trackers.SideBar.MissTimer.OutlineColor")), false);
					*/

			}
		}






		SdkDrawSpriteFromResource(MAKEINTRESOURCEA(SB_HUD), &tempMainPos, true); // HUD Hud MainFrame


		if (enemy.SpellRCD > 0.0f && SideBarMenuList[3].Enable) // on cd Ultimate
		{
			std::stringstream ss1;
			ss1.precision(0); //for decimal
			ss1.setf(std::ios_base::fixed, std::ios_base::floatfield);

			ss1 << enemy.SpellRCD;

			DrawHelper::DrawOutlineText(NULL, &Vector2(enemy.PositionList.UltimatePos.x - 3.0f, enemy.PositionList.UltimatePos.y - 10.0f), ss1.str().c_str(), "Calibri Bold", &DropLists::GetColor(SideBarMenuList[3].FontColor), SideBarMenuList[3].FontSize1, SideBarMenuList[3].FontSize2, 0,
				&DropLists::GetColor(SideBarMenuList[3].OutlineFontColor), false);
		}
		else if (enemy.SpellRCD < 0.0f && enemy.SpellR.Learned && enemy.SpellR.Level > 0 && Menu::Get<bool>("Trackers.SideBar.Ultimate.Use")) //ready
		{
			SdkDrawSpriteFromResource(MAKEINTRESOURCEA(SB_Ultimate), &enemy.PositionList.UltimatePos, true);
		}



		if (SideBarMenuList[1].Enable)
		{
			SdkDrawSpriteFromResource(MAKEINTRESOURCEA(enemy.SummonerSpells1IMG), &enemy.PositionList.SS1Pos, true); // Summoner Spell 1 Drawing
			if (enemy.SummonerSpells1CD > 0.0f)
			{
				SdkDrawSpriteFromResource(MAKEINTRESOURCEA(SB_SSInvisible), &enemy.PositionList.SS1Pos, true); // when its CD
				if (SideBarMenuList[2].Enable)
				{
					std::stringstream ss1;
					ss1.precision(0); //for decimal
					ss1.setf(std::ios_base::fixed, std::ios_base::floatfield);


					int sec = enemy.SummonerSpells1CD;
					int mins = sec / 60;
					sec = sec % 60;;

					ss1 << std::setfill('0') << std::setw(2) << mins << ":" << std::setfill('0') << std::setw(2) << sec;


					DrawHelper::DrawOutlineText(NULL, &enemy.PositionList.SS1TimerPos, ss1.str().c_str(), "Calibri Bold", &DropLists::GetColor(SideBarMenuList[2].FontColor), SideBarMenuList[2].FontSize1, SideBarMenuList[2].FontSize2, 0,
						&DropLists::GetColor(SideBarMenuList[2].OutlineFontColor), false);
				}

			}

			SdkDrawSpriteFromResource(MAKEINTRESOURCEA(enemy.SummonerSpells2IMG), &enemy.PositionList.SS2Pos, true); // Summoner Spell 1 Drawing
			if (enemy.SummonerSpells2CD > 0.0f)
			{
				SdkDrawSpriteFromResource(MAKEINTRESOURCEA(SB_SSInvisible), &enemy.PositionList.SS2Pos, true); // when its CD
				if (SideBarMenuList[2].Enable)
				{
					std::stringstream ss1;
					ss1.precision(0); //for decimal
					ss1.setf(std::ios_base::fixed, std::ios_base::floatfield);


					int sec = enemy.SummonerSpells2CD;
					int mins = sec / 60;
					sec = sec % 60;;

					ss1 << std::setfill('0') << std::setw(2) << mins << ":" << std::setfill('0') << std::setw(2) << sec;


					DrawHelper::DrawOutlineText(NULL, &enemy.PositionList.SS2TimerPos, ss1.str().c_str(), "Calibri Bold", &DropLists::GetColor(SideBarMenuList[2].FontColor), SideBarMenuList[2].FontSize1, SideBarMenuList[2].FontSize2, 0,
						&DropLists::GetColor(SideBarMenuList[2].OutlineFontColor), false);
				}
			}

		
		}


		if (SideBarMenuList[11].Enable && enemy.TrackRecall)
		{

			SdkDrawSpriteFromResource(MAKEINTRESOURCEA(enemy.RecallType), &tempMainPos, true);
		}



		if (arrayStyle == 0) //vertical
		{
			tempMainPos.y += float(SidebarGab);
		}
		else if (arrayStyle == 1) // horizontal
		{
			tempMainPos.x += float(SidebarGab);
		}


	}

	//SdkDrawSpriteFromResource(MAKEINTRESOURCEA(GetSummonerSpells(spellInside.ScriptName, Game::Time() > spellInside.CooldownExpires)), &SSPosition, false);
}

void __cdecl SideBar::RecallTrack(void * Unit, const char * Name, const char * Type, void * UserData)
{

	UNREFERENCED_PARAMETER(UserData);
	if (!Menu::Get<bool>("Trackers.SideBar.Use") || !Menu::Get<bool>("Trackers.SideBar.Recall.Use"))
	{
		UNREFERENCED_PARAMETER(Unit);
		UNREFERENCED_PARAMETER(Name);
		UNREFERENCED_PARAMETER(Type);
		return;
	}

	//SdkUiConsoleWrite("Start %s %s", Name, Type);
	auto sender = pSDK->EntityManager->GetObjectFromPTR(Unit);
	if (Unit == nullptr || sender == nullptr || sender == NULL || !sender->IsHero() || sender->IsAlly())
	{
		return;
	}

	auto netID = sender->GetNetworkID();


	if (!netID)
	{
		return;
	}

	auto testStruct = TeleportDecode::TeleportDecoderFunction(Unit, Name, Type);

	//SdkUiConsoleWrite("here");

	/*
	std::vector<EnemyObject> _enemyObject;

	*/


	for (auto &value : _enemyObject)
	{
		if (value.Unit->GetNetworkID() == netID)
		{
			if (testStruct.Status == TeleportTypes::Abort || testStruct.Status == TeleportTypes::Finished)
			{


				float time = Game::Time();
				pSDK->EventHandler->DelayedAction([testStruct, time, netID]()
				{
					auto cd = testStruct.Status == TeleportTypes::Finished ? 300.0f : 200.0f;
					//LastTimeTickCountClean = GetTickCount();
					_teleports[netID] = time + cd;

					//SdkUiConsoleWrite("Cast Cleanse Delayed : %f", Game::Time());
				}, 250.0f);


			}
			//SdkUiConsoleWrite("TP Added %d", testStruct.Status);
			value._teleportStatus = testStruct.Status;
			value.LastTeleportStatusTime = Game::Time();

		}
	}


}

