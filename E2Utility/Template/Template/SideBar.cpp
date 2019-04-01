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

const int RequiredExp[] = { 0, 280, 660, 1140, 1720, 2400, 3180, 4060, 5040, 6120, 7300, 8580, 9960, 11440, 13020, 14700, 16480, 18360, 9999999999999 };

void SideBar::Init()
{

	_enemyObject.clear();

	auto enemyList{ pSDK->EntityManager->GetEnemyHeroes() };

	for (auto[netID, value] : enemyList)
	{
		_enemyObject.emplace_back(EnemyObject(value));
	//	SdkUiConsoleWrite("did");
	}


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
			Menu::Checkbox("Draw Summoner Spells on Side Bar", "Trackers.SideBar.SS.Use", true);


			Menu::Checkbox("Draw Summoner Spells Timer", "Trackers.SideBar.SS.Timer", true);
			Menu::DropList("Summoner Spells Timer Format", "Trackers.SideBar.SS.Format", std::vector<std::string>{ "MM:SS", "SS"}, 0);
			Menu::SliderInt("Summoner Spells Timer Position X-axis", "Trackers.SideBar.SS.DrawingX", 0, -200, 200);
			Menu::SliderInt("Summoner Spells Timer Position Y-axis", "Trackers.SideBar.SS.DrawingY", 0, -200, 200);
			Menu::SliderInt("Summoner Spells Timer Font Height", "Trackers.SideBar.SS.FontSize", 20, 10, 30);
			Menu::SliderInt("Summoner Spells Timer Font Width", "Trackers.SideBar.SS.FontSize2", 4, 1, 10);
			Menu::DropList("^-> Summoner Spells Timer Color", "Trackers.SideBar.SS.Color", ColorMenuList, 11);
			Menu::DropList("^-> Summoner Spells Timer OutLine Color", "Trackers.SideBar.SS.OutLineColor", ColorMenuList, 0);
		});



		Menu::Tree("Ultimate Settings", "Trackers.SideBar.Ultimate", false, []()
		{
			Menu::Checkbox("Draw Ultimate on Side Bar", "Trackers.SideBar.Ultimate.Use", true);


			Menu::Checkbox("Draw Ultimate Timer", "Trackers.SideBar.Ultimate.Timer", true);
			Menu::DropList("Ultimate Timer Format", "Trackers.SideBar.Ultimate.Format", std::vector<std::string>{ "MM:SS", "SS"}, 0);
			Menu::SliderInt("Ultimate Timer Position X-axis", "Trackers.SideBar.Ultimate.DrawingX", 0, -200, 200);
			Menu::SliderInt("Ultimate Timer Position Y-axis", "Trackers.SideBar.Ultimate.DrawingY", 0, -200, 200);
			Menu::SliderInt("Ultimate Timer Font Height", "Trackers.SideBar.Ultimate.FontSize", 20, 10, 30);
			Menu::SliderInt("Ultimate Timer Font Width", "Trackers.SideBar.Ultimate.FontSize2", 3, 1, 10);
			Menu::DropList("^-> Summoner Ultimate Timer Color", "Trackers.SideBar.Ultimate.Color", ColorMenuList, 11);
			Menu::DropList("^-> Summoner Ultimate Timer OutLine Color", "Trackers.SideBar.Ultimate.OutLineColor", ColorMenuList, 0);
		});

		Menu::Tree("Missing Timer Settings", "Trackers.SideBar.MissTimer", false, []()
		{

			Menu::Checkbox("Draw Missing Timer", "Trackers.SideBar.MissTimer.Timer", true);
			//Menu::DropList("Missing Timer Format", "Trackers.SideBar.MissTimer.Format", std::vector<std::string>{ "MM:SS", "SS"}, 1);
			Menu::SliderInt("Missing Timer Position X-axis", "Trackers.SideBar.MissTimer.DrawingX", 0, -200, 200);
			Menu::SliderInt("Missing Timer Position Y-axis", "Trackers.SideBar.MissTimer.DrawingY", 0, -200, 200);
			Menu::SliderInt("Missing Timer Font Height", "Trackers.SideBar.MissTimer.FontSize", 24, 10, 30);
			Menu::SliderInt("Missing Timer Font Width", "Trackers.SideBar.MissTimer.FontSize2", 4, 1, 10);
			Menu::DropList("^-> Missing Timer Color", "Trackers.SideBar.MissTimer.Color", ColorMenuList, 11);
			Menu::DropList("^-> Missing Timer OutLine Color", "Trackers.SideBar.MissTimer.OutLineColor", ColorMenuList, 0);
		});


		Menu::Tree("Death Timer Settings", "Trackers.SideBar.DeathTimer", false, []()
		{

			Menu::Checkbox("Draw Death Timer", "Trackers.SideBar.DeathTimer.Timer", true);
			//Menu::DropList("Death Timer Format", "Trackers.SideBar.DeathTimer.Format", std::vector<std::string>{ "MM:SS", "SS"}, 1);
			Menu::SliderInt("Death Timer Position X-axis", "Trackers.SideBar.DeathTimer.DrawingX", 0, -200, 200);
			Menu::SliderInt("Death Timer Position Y-axis", "Trackers.SideBar.DeathTimer.DrawingY", 0, -200, 200);
			Menu::SliderInt("Death Timer Font Height", "Trackers.SideBar.DeathTimer.FontSize", 24, 10, 30);
			Menu::SliderInt("Death Timer Font Width", "Trackers.SideBar.DeathTimer.FontSize2", 4, 1, 10);
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
			Menu::SliderInt("Bars Width", "Trackers.SideBar.Bars.Width", 5, 1, 10);
			SdkUiText("^-> Default 5");
			Menu::SliderInt("Bars Length", "Trackers.SideBar.Bars.Length", 73, 15, 100);
			SdkUiText("^-> Default 73");
			Menu::SliderInt("Gap Between Bars", "Trackers.SideBar.Bars.Gap", 6, 1, 20);
			SdkUiText("^-> Default 6");


			Menu::Checkbox("Draw Health Bar", "Trackers.SideBar.Bars.Health.Use", true);
			Menu::SliderInt("Health Bar Position X-axis", "Trackers.SideBar.Bars.Health.DrawingX", 0, -200, 200);
			Menu::SliderInt("Health Bar Position Y-axis", "Trackers.SideBar.Bars.Health.DrawingY", 0, -200, 200);
			Menu::DropList("^-> Health Bar Color", "Trackers.SideBar.Bars.Health.Color", ColorMenuList, 4);
			Menu::DropList("^-> Health Bar OutLine Color", "Trackers.SideBar.Bars.Health.OutLineColor", ColorMenuList, 0);


			Menu::Checkbox("Draw Mana Bar", "Trackers.SideBar.Bars.Mana.Use", true);
			Menu::SliderInt("Mana Bar Position X-axis", "Trackers.SideBar.Bars.Mana.DrawingX", 0, -200, 200);
			Menu::SliderInt("Mana Bar Position Y-axis", "Trackers.SideBar.Bars.Mana.DrawingY", 0, -200, 200);
			Menu::DropList("^-> Mana Bar Color", "Trackers.SideBar.Bars.Mana.Color", ColorMenuList, 1);
			Menu::DropList("^-> Mana Bar OutLine Color", "Trackers.SideBar.Bars.Mana.OutLineColor", ColorMenuList, 0);



			Menu::Checkbox("Draw Experience Bar", "Trackers.SideBar.Bars.Experience.Use", true);
			Menu::SliderInt("Experience Bar Position X-axis", "Trackers.SideBar.Bars.Experience.DrawingX", 0, -200, 200);
			Menu::SliderInt("Experience Bar Position Y-axis", "Trackers.SideBar.Bars.Experience.DrawingY", 0, -200, 200);
			Menu::DropList("^-> Experience Bar Color", "Trackers.SideBar.Bars.Experience.Color", ColorMenuList, 9);
			Menu::DropList("^-> Experience Bar OutLine Color", "Trackers.SideBar.Bars.Experience.OutLineColor", ColorMenuList, 0);

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


		//Menu::Checkbox("Use on Ally Inhibitors", "Trackers.InhibitorTimer.Ally", true);
		//Menu::Checkbox("Use on Enemy Inhibitors", "Trackers.InhibitorTimer.Enemy", true);


		//Menu::SliderInt("SideBar Position X-axis", "Trackers.SideBar.DrawingX", 0, -50, 50);
		//Menu::SliderInt("SideBar Position Y-axis", "Trackers.SideBar.DrawingY", -0, -50, 50);



	});
}

void SideBar::TickLoader()
{
}

void SideBar::DrawLoader()
{
	if (!Menu::Get<int>("Trackers.SideBar.Use"))
	{
		return;
	}


	auto resol = Renderer::GetResolution();

	

	float AdjustX = float(Menu::Get<int>("Trackers.SideBar.DrawingX"));
	float AdjustY = float(Menu::Get<int>("Trackers.SideBar.DrawingY"));

	int screenPosSelection = Menu::Get<int>("Trackers.SideBar.Location");
	
	int arrayStyle = Menu::Get<int>("Trackers.SideBar.Style");
	int gabMainFrames = float(Menu::Get<int>("Trackers.SideBar.Main.Gap"));

	Vector2 MainFramePos;

	Vector2 tempPos;
	if (screenPosSelection == 0) // on the left
	{
		tempPos = Vector2(0.0f + 50.0f, (resol.Height / 2.0f) - (resol.Height / 4.0f) - (resol.Height / 10.0f));

	}
	else if (screenPosSelection == 1) // on the right
	{
		tempPos = Vector2(resol.Width - 50.0f, (resol.Height / 2.0f) - (resol.Height / 4.0f) - (resol.Height / 10.0f));
	}
	else if (screenPosSelection == 2) // on the top
	{
		tempPos = Vector2((resol.Width / 2.0f) - (resol.Width / 4.0f), 50.0f);
	}

	float AdjustMainFrameX = float(Menu::Get<int>("Trackers.SideBar.Main.DrawingX"));
	float AdjustMainFrameY = float(Menu::Get<int>("Trackers.SideBar.Main.DrawingY"));

	tempPos.x += AdjustMainFrameX;
	tempPos.y += AdjustMainFrameY;

	MainFramePos = tempPos;



	for (auto& enemy : _enemyObject)
	{

		
		if (!enemy.Unit->IsAlive() && Game::Time() > enemy.DeathEndTime)
		{
			enemy.DeathEndTime = Game::Time() + enemy.Unit->GetDeathDuration() + 1.0f;
		}
		else if (enemy.Unit->IsAlive())
		{
			enemy.DeathEndTime = 0.0f;
		}


		tempPos = MainFramePos;
		tempPos.x += -11.0f;
		tempPos.y += -10.0f;
		SdkDrawSpriteFromResource(MAKEINTRESOURCEA(enemy.ChampIMG), &tempPos, true); // Champion Face



		//Vector2 barPosStatic = Vector2((resol.Width / 2.0f), (resol.Height / 2.0f));

		/*
		barPosStatic = Vector2((resol.Width / 2.0f), (resol.Height / 2.0f));

		barPosStatic.x += -31.0f;
		barPosStatic.y += 15.0f;
		*/

		
		if (Menu::Get<bool>("Trackers.SideBar.Bars.Use")) // Health
		{
			float length = float(Menu::Get<int>("Trackers.SideBar.Bars.Length"));
			length = length / 100.0f;

			float barGab = float(Menu::Get<int>("Trackers.SideBar.Bars.Gap"));

			if (Menu::Get<bool>("Trackers.SideBar.Bars.Health.Use")) // Health
			{
				tempPos = MainFramePos;
				tempPos.x += -34.0f + float(Menu::Get<int>("Trackers.SideBar.Bars.Health.DrawingX"));
				tempPos.y += 20.0f + float(Menu::Get<int>("Trackers.SideBar.Bars.Health.DrawingY"));



				float hp = enemy.Unit->GetHealthPercent();
				DrawHelper::DrawOutlineLineScreen(&tempPos, &Vector2(tempPos.x + hp * length, tempPos.y), float(Menu::Get<int>("Trackers.SideBar.Bars.Width")), &DropLists::GetColor(Menu::Get<int>("Trackers.SideBar.Bars.Health.Color")), &DropLists::GetColor(Menu::Get<int>("Trackers.SideBar.Bars.Health.OutLineColor"))); //-40 43

				//DrawHelper::DrawOutlineLineScreen(&Vector2(barPosStatic.x + hp * 0.73f, barPosStatic.y), &Vector2(barPosStatic.x + hp * 1.0f, barPosStatic.y), 5.0f, &Color::Grey, &Color::Black); //-40 43
			}

			if (Menu::Get<bool>("Trackers.SideBar.Bars.Mana.Use")) // Mana
			{

				

				float mp = enemy.Unit->GetManaPercent();

				tempPos = MainFramePos;
				tempPos.y += barGab;

				tempPos.x += -34.0f + float(Menu::Get<int>("Trackers.SideBar.Bars.Mana.DrawingX"));
				tempPos.y += 20.0f + float(Menu::Get<int>("Trackers.SideBar.Bars.Mana.DrawingY"));


				DrawHelper::DrawOutlineLineScreen(&tempPos, &Vector2(tempPos.x + mp * length, tempPos.y), float(Menu::Get<int>("Trackers.SideBar.Bars.Width")), &DropLists::GetColor(Menu::Get<int>("Trackers.SideBar.Bars.Mana.Color")), &DropLists::GetColor(Menu::Get<int>("Trackers.SideBar.Bars.Mana.OutLineColor")));//-40 43
				//DrawHelper::DrawOutlineLineScreen(&Vector2(barPosStatic.x + mp * 0.73f, barPosStatic.y), &Vector2(barPosStatic.x + 73.0f, barPosStatic.y), 5.0f, &Color::Grey, &Color::Black); //-40 43
			}


			if (Menu::Get<bool>("Trackers.SideBar.Bars.Experience.Use")) // Mana
			{
				
				float exp = enemy.Unit->GetExperience();

				float neededExp = RequiredExp[enemy.Unit->GetLevel()];

				float percent = (100.0f / (neededExp - RequiredExp[enemy.Unit->GetLevel() - 1]) * (exp - RequiredExp[enemy.Unit->GetLevel() - 1]));
				if (enemy.Unit->GetLevel() == 18)
				{
					percent = 100.0f;
				}


				tempPos = MainFramePos;
				tempPos.y += (barGab * 2.0f);

				tempPos.x += -34.0f + float(Menu::Get<int>("Trackers.SideBar.Bars.Experience.DrawingX"));
				tempPos.y += 20.0f + float(Menu::Get<int>("Trackers.SideBar.Bars.Experience.DrawingY"));




				DrawHelper::DrawOutlineLineScreen(&tempPos, &Vector2(tempPos.x + percent * length, tempPos.y), float(Menu::Get<int>("Trackers.SideBar.Bars.Width")) - 1.0f, &DropLists::GetColor(Menu::Get<int>("Trackers.SideBar.Bars.Experience.Color")), &DropLists::GetColor(Menu::Get<int>("Trackers.SideBar.Bars.Experience.OutLineColor")));
				//DrawHelper::DrawOutlineLineScreen(&Vector2(barPosStatic.x + percent * 0.73f, barPosStatic.y), &Vector2(barPosStatic.x + (100.0f * 0.73f), barPosStatic.y), 5.0f, &Color::Grey, &Color::Black); //-40 43
				//Draw::LineScreen(&Vector2(barPosStatic.x + (percent * 0.73f) + 1.0f, barPosStatic.y), &Vector2(barPosStatic.x + (100.0f * 0.73f), barPosStatic.y), 5.0f, &Color::Grey);
			}
		}


		if (Menu::Get<bool>("Trackers.SideBar.Level.Use")) // Level
		{
			tempPos = MainFramePos; // Level
			tempPos.x += 3.0f + float(Menu::Get<int>("Trackers.SideBar.Level.DrawingX")); //-2
			tempPos.y += -5.0f + float(Menu::Get<int>("Trackers.SideBar.Level.DrawingY")); //-8
			DrawHelper::DrawOutlineText(NULL, &tempPos, std::to_string(enemy.Unit->GetLevel()).c_str(), "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Trackers.SideBar.Level.Color")), Menu::Get<int>("Trackers.SideBar.Level.FontSize"), Menu::Get<int>("Trackers.SideBar.Level.FontSize2"), 0,
				&DropLists::GetColor(Menu::Get<int>("Trackers.SideBar.Level.OutlineColor")), false);

		}

		if (Menu::Get<bool>("Trackers.SideBar.DeathTimer.Timer")) 
		{
			if (!enemy.Unit->IsAlive()) // Death Timer
			{
				tempPos = MainFramePos;
				tempPos.x += -15.0f + float(Menu::Get<int>("Trackers.SideBar.DeathTimer.DrawingX")); //-15
				tempPos.y += -25.0f + float(Menu::Get<int>("Trackers.SideBar.DeathTimer.DrawingY")); //-25


				std::stringstream ss1;
				ss1.precision(0); //for decimal
				ss1.setf(std::ios_base::fixed, std::ios_base::floatfield);

				ss1 << enemy.DeathEndTime - Game::Time();

				DrawHelper::DrawOutlineText(NULL, &tempPos, ss1.str().c_str(), "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Trackers.SideBar.DeathTimer.Color")), Menu::Get<int>("Trackers.SideBar.DeathTimer.FontSize"), Menu::Get<int>("Trackers.SideBar.DeathTimer.FontSize2"), 0,
					&DropLists::GetColor(Menu::Get<int>("Trackers.SideBar.DeathTimer.OutlineColor")), false);
			}
		}


		if (!enemy.Unit->IsVisible() || !enemy.Unit->IsAlive()) //Make champion icon darker for Mising and Death Timer
		{
			tempPos = MainFramePos;
			tempPos.x += -12.0f;
			tempPos.y += -10.0f; 
			SdkDrawSpriteFromResource(MAKEINTRESOURCEA(SB_Invisible), &tempPos, true); 

		}



		auto zeroVector = Vector3(0.0f, 0.0f, 0.0f);
		auto enemyPos = enemy.Unit->GetPosition();
		if (enemy.Unit->IsAlive() && enemy.LastPosition.Distance(zeroVector) > 50.0f && enemy.LastPosition.Distance(enemyPos) > 500.0f)
		{
			enemy.LastVisible = Game::Time();
		}

		enemy.LastPosition = enemyPos;

		if (enemy.Unit->IsVisible() || !enemy.Unit->IsAlive())
		{
			enemy.LastVisible = Game::Time();
		}


		if (Menu::Get<bool>("Trackers.SideBar.MissTimer.Timer")) 
		{
			if (!enemy.Unit->IsVisible() && enemy.Unit->IsAlive() && Game::Time() - enemy.LastVisible > 3.0f) // Missing Timer
			{
				tempPos = MainFramePos;
				tempPos.x += -15.0f + float(Menu::Get<int>("Trackers.SideBar.MissTimer.DrawingX"));
				tempPos.y += -25.0f + float(Menu::Get<int>("Trackers.SideBar.MissTimer.DrawingY"));


				std::stringstream ss1;
				ss1.precision(0); //for decimal
				ss1.setf(std::ios_base::fixed, std::ios_base::floatfield);

				ss1 << Game::Time() - enemy.LastVisible;

				//SdkDrawSpriteFromResource(MAKEINTRESOURCEA(SB_Invisible), &barPos, true); // -11 , -10 is good pos
				DrawHelper::DrawOutlineText(NULL, &tempPos, ss1.str().c_str(), "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Trackers.SideBar.MissTimer.Color")), Menu::Get<int>("Trackers.SideBar.MissTimer.FontSize"), Menu::Get<int>("Trackers.SideBar.MissTimer.FontSize2"), 0,
					&DropLists::GetColor(Menu::Get<int>("Trackers.SideBar.MissTimer.OutlineColor")), false);
			}
		}


		/*

		*/

		//SdkDrawSpriteFromResource(MAKEINTRESOURCEA(enemy.ChampIMG), &barPos, true);






		tempPos = MainFramePos;


		SdkDrawSpriteFromResource(MAKEINTRESOURCEA(SB_HUD), &tempPos, true); // HUD Hud MainFrame


		auto spells = enemy.Unit->GetSpells();
		for (auto &spellInside : spells)
		{
			float time = spellInside.CooldownExpires - Game::Time();
			//float spellCD = spellInside.TotalCooldown;

			/*
			if (int(spellInside.Slot) >= 0 && int(spellInside.Slot) < 4)
			{
				std::string tempSlot = "";
				if (int(spellInside.Slot) == 0)
				{
					tempSlot = "Q";
				}
				else if (int(spellInside.Slot) == 1)
				{
					tempSlot = "W";
				}
				else if (int(spellInside.Slot) == 2)
				{
					tempSlot = "E";
				}

				if (time < 0.0f)
				{
					DrawHelper::DrawOutlineText(NULL, &barPosStatic, tempSlot.c_str(), "Calibri Bold", &Color::White, 26, 6, 0,
						&Color::Black, false);
				}
				else
				{
					DrawHelper::DrawOutlineText(NULL, &barPosStatic, tempSlot.c_str(), "Calibri Bold", &Color::Red, 26, 6, 0,
						&Color::Black, false);
				}

				barPosStatic.x += 20.0f;
			}*/

			std::stringstream ss1;
			ss1.precision(1); //for decimal
			ss1.setf(std::ios_base::fixed, std::ios_base::floatfield);

			int sec = time;
			int mins = sec / 60;
			sec = sec % 60;
			int onlySec = time;

			tempPos = MainFramePos;

			if (int(spellInside.Slot) == 3 && spellInside.Learned && spellInside.Level > 0 && Menu::Get<bool>("Trackers.SideBar.Ultimate.Use"))
			{
				tempPos = MainFramePos;



				tempPos.x += -34.0f + float(Menu::Get<int>("Trackers.SideBar.Ultimate.DrawingX"));
				tempPos.y += -31.0f + float(Menu::Get<int>("Trackers.SideBar.Ultimate.DrawingY"));
		

				if (time > 0.0f && Menu::Get<bool>("Trackers.SideBar.Ultimate.Timer"))
				{

					tempPos.x += -8.0f;
					tempPos.y += -11.0f;
					int format = Menu::Get<int>("Trackers.SideBar.Ultimate.Format");

					if (format == 0) // mm:ss
					{
						ss1 << std::setfill('0') << std::setw(2) << mins << ":" << std::setfill('0') << std::setw(2) << sec;

					}
					else if (format == 1) // ss
					{
						ss1 << std::setfill('0') << std::setw(2) << onlySec;

					}
					DrawHelper::DrawOutlineText(NULL, &tempPos, ss1.str().c_str(), "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Trackers.SideBar.Ultimate.Color")), Menu::Get<int>("Trackers.SideBar.Ultimate.FontSize"), Menu::Get<int>("Trackers.SideBar.Ultimate.FontSize2"), 0,
						&DropLists::GetColor(Menu::Get<int>("Trackers.SideBar.Ultimate.OutlineColor")), false);
				}
				else if (time < 0.0f)
				{
					SdkDrawSpriteFromResource(MAKEINTRESOURCEA(SB_Ultimate), &tempPos, true);
				}
			}
			else if ( (int(spellInside.Slot) == 4 || int(spellInside.Slot) == 5) && Menu::Get<bool>("Trackers.SideBar.Ultimate.Use"))
			{
				tempPos = MainFramePos;
				tempPos.x += 30.0f; //30

				if (int(spellInside.Slot) == 4)
				{
					tempPos.y += -22.0f; //-22
				}
				else if (int(spellInside.Slot) == 5)
				{
					tempPos.y += 4.0f; //-22
				}

				int spellImg = ChampionNames::GetSummonerSpellIMG(spellInside.ScriptName, 297);

				SdkDrawSpriteFromResource(MAKEINTRESOURCEA(spellImg), &tempPos, true); // Summoner Spell 1 Drawing
				if (time > 0.0f)
				{
					SdkDrawSpriteFromResource(MAKEINTRESOURCEA(SB_SSInvisible), &tempPos, true); // when its CD

					if (Menu::Get<bool>("Trackers.SideBar.SS.Timer"))
					{
						int format = Menu::Get<int>("Trackers.SideBar.SS.Format");

						if (format == 0) // mm:ss
						{
							ss1 << std::setfill('0') << std::setw(2) << mins << ":" << std::setfill('0') << std::setw(2) << sec;

						}
						else if (format == 1) // ss
						{
							ss1 << std::setfill('0') << std::setw(2) << onlySec;

						}
						tempPos.x += -10.0f;
						tempPos.y += -11.0f;
						DrawHelper::DrawOutlineText(NULL, &tempPos, ss1.str().c_str(), "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Trackers.SideBar.SS.Color")), Menu::Get<int>("Trackers.SideBar.SS.FontSize"), Menu::Get<int>("Trackers.SideBar.SS.FontSize2"), 0,
							&DropLists::GetColor(Menu::Get<int>("Trackers.SideBar.SS.OutlineColor")), false);
					}
				}
			}
	

		}













	
		if (Menu::Get<bool>("Trackers.SideBar.Recall.Use"))
		{
			if (enemy._teleportStatus == TeleportTypes::Start || (enemy._teleportStatus == TeleportTypes::Finished || enemy._teleportStatus == TeleportTypes::Abort)
				&& Game::Time() <= enemy.LastTeleportStatusTime + 3.0f)
			{
				tempPos = MainFramePos; // Recall Tracker Sprite drawer.

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
					SdkDrawSpriteFromResource(MAKEINTRESOURCEA(FinalSelection), &tempPos, true);
				}


			}
		}
	


		if (arrayStyle == 0) //vertical 
		{
			MainFramePos.y += gabMainFrames;
		}
		else if (arrayStyle == 1) // horizontal
		{
			MainFramePos.x += gabMainFrames;
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




	/*
	var packet = Packet.S2C.Teleport.Decoded(sender, args);
	var enemyObject = _enemyObjects.FirstOrDefault(e = > e.Unit.NetworkId == packet.UnitNetworkId);
	if (enemyObject != null)
	{
		if (packet.Type == Packet.S2C.Teleport.Type.Teleport &&
			(packet.Status == Packet.S2C.Teleport.Status.Finish ||
				packet.Status == Packet.S2C.Teleport.Status.Abort))
		{
			var time = Game.Time;
			Utility.DelayAction.Add(
				250, delegate
				{
					var cd = packet.Status == Packet.S2C.Teleport.Status.Finish ? 300 : 200;
					_teleports[packet.UnitNetworkId] = time + cd;
				});
		}
		enemyObject.TeleportStatus = packet.Status;
	}*/
}

