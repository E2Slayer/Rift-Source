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

int RequiredExp[] = { 0, 280, 660, 1140, 1720, 2400, 3180, 4060, 5040, 6120, 7300, 8580, 9960, 11440, 13020, 14700, 16480, 18360, 9999999999999 };

void SideBar::Init()
{

	_enemyObject.clear();

	auto enemyList{ pSDK->EntityManager->GetEnemyHeroes() };

	for (auto[netID, value] : enemyList)
	{
		_enemyObject.emplace_back(EnemyObject(value));
	//	SdkUiConsoleWrite("did");
	}


	pSDK->EventHandler->RegisterCallback(CallbackEnum::Recall, SideBar::RecallTrack);


}

void SideBar::MenuLoader()
{
	Menu::Tree("Side Bar", "Trackers.SideBar", false, []()
	{
		Menu::Checkbox("Use Side Bar", "Trackers.SideBar.Use", true);

		Menu::DropList("Side Bar Style", "Trackers.SideBar.Style", std::vector<std::string>{ "Vertical", "Horizontal" }, 0);
		Menu::DropList("Side Bar Location", "Trackers.SideBar.Location", std::vector<std::string>{ "On the Left", "On the Right", "On the Top" }, 0);

		Menu::SliderInt("Main Frame Position X-axis", "Trackers.SideBar.Main.DrawingX", 0, -2000, 2000);
		Menu::SliderInt("Main Frame Position Y-axis", "Trackers.SideBar.Main.DrawingY", 0, -2000, 2000);
		Menu::SliderInt("Gap Between Each Frames", "Trackers.SideBar.Main.Gap", 50, 100, 500);


		Menu::Tree("Summoner Spells Settings", "Trackers.SideBar.SS", false, []()
		{
			Menu::Checkbox("Draw Summoner Spells on Side Bar", "Trackers.SideBar.SS", true);


			Menu::Checkbox("Draw Summoner Spells Timer", "Trackers.SideBar.SS.Timer", true);
			Menu::DropList("Summoner Spells Timer Format", "Trackers.SideBar.SS.Format", std::vector<std::string>{ "MM:SS", "SS"}, 0);
			Menu::SliderInt("Summoner Spells Timer Position X-axis", "Trackers.SideBar.SS.DrawingX", 0, -200, 200);
			Menu::SliderInt("Summoner Spells Timer Position Y-axis", "Trackers.SideBar.SS.SCTDrawingY", 0, -200, 200);
			Menu::SliderInt("Summoner Spells Timer Font Height", "Trackers.SideBar.SS.FontSize", 20, 10, 30);
			Menu::SliderInt("Summoner Spellsn Timer Font Width", "Trackers.SideBar.SS.FontSize2", 3, 1, 10);
			Menu::DropList("^-> Summoner Spells Timer Color", "Trackers.SideBar.SS.Color", ColorMenuList, 11);
			Menu::DropList("^-> Summoner Spells Timer OutLine Color", "Trackers.SideBar.SS.OutLineColor", ColorMenuList, 0);
		});



		Menu::Tree("Ultimate Settings", "Trackers.SideBar.Ultimate", false, []()
		{
			Menu::Checkbox("Draw Ultimate on Side Bar", "Trackers.SideBar.Ultimate", true);


			Menu::Checkbox("Draw Summoner Spells Timer", "Trackers.SideBar.Ultimate.Timer", true);
			Menu::DropList("Summoner Spells Timer Format", "Trackers.SideBar.Ultimate.Format", std::vector<std::string>{ "MM:SS", "SS"}, 0);
			Menu::SliderInt("Summoner Spells Timer Position X-axis", "Trackers.SideBar.Ultimate.DrawingX", 0, -200, 200);
			Menu::SliderInt("Summoner Spells Timer Position Y-axis", "Trackers.SideBar.Ultimate.SCTDrawingY", 0, -200, 200);
			Menu::SliderInt("Summoner Spells Timer Font Height", "Trackers.SideBar.Ultimate.FontSize", 20, 10, 30);
			Menu::SliderInt("Summoner Spellsn Timer Font Width", "Trackers.SideBar.Ultimate.FontSize2", 3, 1, 10);
			Menu::DropList("^-> Summoner Spells Timer Color", "Trackers.SideBar.Ultimate.Color", ColorMenuList, 11);
			Menu::DropList("^-> Summoner Spells Timer OutLine Color", "Trackers.SideBar.Ultimate.OutLineColor", ColorMenuList, 0);
		});

		Menu::Tree("Missing Timer Settings", "Trackers.SideBar.MissTimer", false, []()
		{

			Menu::Checkbox("Draw Missing Timer", "Trackers.SideBar.MissTimer.Timer", true);
			Menu::DropList("Missing Timer Format", "Trackers.SideBar.MissTimer.Format", std::vector<std::string>{ "MM:SS", "SS"}, 1);
			Menu::SliderInt("Missing Timer Position X-axis", "Trackers.SideBar.MissTimer.DrawingX", 0, -200, 200);
			Menu::SliderInt("Missing Timer Position Y-axis", "Trackers.SideBar.MissTimer.SCTDrawingY", 0, -200, 200);
			Menu::SliderInt("Missing Timer Font Height", "Trackers.SideBar.MissTimer.FontSize", 20, 10, 30);
			Menu::SliderInt("Missing Timer Font Width", "Trackers.SideBar.MissTimer.FontSize2", 3, 1, 10);
			Menu::DropList("^-> Missing Timer Color", "Trackers.SideBar.MissTimer.Color", ColorMenuList, 11);
			Menu::DropList("^-> Missing Timer OutLine Color", "Trackers.SideBar.MissTimer.OutLineColor", ColorMenuList, 0);
		});


		Menu::Tree("Death Timer Settings", "Trackers.SideBar.DeathTimer", false, []()
		{

			Menu::Checkbox("Draw Death Timer", "Trackers.SideBar.DeathTimer.Timer", true);
			Menu::DropList("Death Timer Format", "Trackers.SideBar.DeathTimer.Format", std::vector<std::string>{ "MM:SS", "SS"}, 1);
			Menu::SliderInt("Death Timer Position X-axis", "Trackers.SideBar.DeathTimer.DrawingX", 0, -200, 200);
			Menu::SliderInt("Death Timer Position Y-axis", "Trackers.SideBar.DeathTimer.SCTDrawingY", 0, -200, 200);
			Menu::SliderInt("Death Timer Font Height", "Trackers.SideBar.DeathTimer.FontSize", 20, 10, 30);
			Menu::SliderInt("Death Timer Font Width", "Trackers.SideBar.DeathTimer.FontSize2", 3, 1, 10);
			Menu::DropList("^-> Death Timer Color", "Trackers.SideBar.DeathTimer.Color", ColorMenuList, 11);
			Menu::DropList("^-> Death Timer OutLine Color", "Trackers.SideBar.DeathTimer.OutLineColor", ColorMenuList, 0);
		});

		

		Menu::Tree("Level Display Settings", "Trackers.SideBar.Level", false, []()
		{

			Menu::Checkbox("Draw Level", "Trackers.SideBar.Level.Use", true);
			Menu::SliderInt("Level Position X-axis", "Trackers.SideBar.Level.DrawingX", 0, -200, 200);
			Menu::SliderInt("Level Position Y-axis", "Trackers.SideBar.Level.SCTDrawingY", 0, -200, 200);
			Menu::SliderInt("Level Font Height", "Trackers.SideBar.Level.FontSize", 20, 10, 30);
			Menu::SliderInt("Level Font Width", "Trackers.SideBar.Level.FontSize2", 3, 1, 10);
			Menu::DropList("^-> Level Color", "Trackers.SideBar.Level.Color", ColorMenuList, 11);
			Menu::DropList("^-> Level OutLine Color", "Trackers.SideBar.Level.OutLineColor", ColorMenuList, 0);
		});



		Menu::Tree("Bars Settings", "Trackers.SideBar.Bars", false, []()
		{

			Menu::Checkbox("Draw Bars", "Trackers.SideBar.Bars.Use", true);
			Menu::SliderInt("Bars Width", "Trackers.SideBar.Bars.Width", 3, 1, 10);
			SdkUiText("^-> Default 5");
			Menu::SliderInt("Bars Length", "Trackers.SideBar.Bars.Length", 23, 15, 40);
			SdkUiText("^-> Default 23");
			Menu::SliderInt("Gap Between Bars", "Trackers.SideBar.Bars.Gap", 26, 10, 50);
			SdkUiText("^-> Default 26, This needs to be bigger than bar legnth");


			Menu::Checkbox("Draw Health Bar", "Trackers.SideBar.Bars.Health.Use", true);
			Menu::SliderInt("Health Bar Position X-axis", "Trackers.SideBar.Bars.Health.DrawingX", 0, -200, 200);
			Menu::SliderInt("Health Bar Position Y-axis", "Trackers.SideBar.Bars.Health.DrawingY", 0, -200, 200);
			Menu::DropList("^-> Health Bar Color", "Trackers.SideBar.Bars.Health.Color", ColorMenuList, 4);
			Menu::DropList("^-> Health Bar OutLine Color", "Trackers.SideBar.Bars.Health.OutLineColor", ColorMenuList, 0);


			Menu::Checkbox("Draw Mana Bar", "Trackers.SideBar.Bars.Health.Use", true);
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


		Menu::SliderInt("SideBar Position X-axis", "Trackers.SideBar.DrawingX", 0, -50, 50);
		Menu::SliderInt("SideBar Position Y-axis", "Trackers.SideBar.DrawingY", -0, -50, 50);



	});
}

void SideBar::TickLoader()
{
}

void SideBar::DrawLoader()
{



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



		auto resol = Renderer::GetResolution();


		float AdjustX = float(Menu::Get<int>("Trackers.SideBar.DrawingX"));
		float AdjustY = float(Menu::Get<int>("Trackers.SideBar.DrawingY"));

		int screenPosSelection = Menu::Get<int>("Trackers.SideBar.Location");
		
		Vector2 MainFramePos;

		Vector2 tempPos;
		if (screenPosSelection == 0) // on the left
		{
			tempPos = Vector2(0.0f + 50.0f, (resol.Height / 2.0f) - (resol.Height / 4.0f));

		}
		else if (screenPosSelection == 1) // on the right
		{
			tempPos = Vector2(resol.Width, (resol.Height / 2.0f) - (resol.Height / 4.0f));
		}
		else if (screenPosSelection == 2) // on the top
		{
			tempPos = Vector2((resol.Width / 2.0f), -(resol.Height));
		}
		MainFramePos = tempPos;

		//Vector2 barPos = Vector2((resol.Width / 2.0f) , (resol.Height / 2.0f));
		tempPos.x += -11.0f;
		tempPos.y += -10.0f;
		SdkDrawSpriteFromResource(MAKEINTRESOURCEA(enemy.ChampIMG), &tempPos, true); // Champion Face



		Vector2 barPosStatic = Vector2((resol.Width / 2.0f), (resol.Height / 2.0f));

		/*
		barPosStatic = Vector2((resol.Width / 2.0f), (resol.Height / 2.0f));

		barPosStatic.x += -31.0f;
		barPosStatic.y += 15.0f;
		*/

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



			if (int(spellInside.Slot) == 4)
			{
				barPos = Vector2((resol.Width / 2.0f), (resol.Height / 2.0f));
				barPos.x += 30.0f; //30
				barPos.y += -22.0f; //-22
				int spellImg = ChampionNames::GetSummonerSpellIMG(spellInside.ScriptName, 297);

				SdkDrawSpriteFromResource(MAKEINTRESOURCEA(spellImg), &barPos, true); // -34, -31 is good
				if (time > 0.0f)
				{
				
					SdkDrawSpriteFromResource(MAKEINTRESOURCEA(SB_SSInvisible), &barPos, true);
					ss1 << std::setfill('0') << std::setw(2) << mins << ":" << std::setfill('0') << std::setw(2) << sec;
					barPos.x += -10.0f; //30
					barPos.y += -11.0f; //-22
					DrawHelper::DrawOutlineText(NULL, &barPos, ss1.str().c_str(), "Calibri Bold", &Color::White, 22, 4, 0,
						&Color::Black, false);
				}
				//tempSlot = "Q";
			}
			else if (int(spellInside.Slot) == 5)
			{
				barPos = Vector2((resol.Width / 2.0f), (resol.Height / 2.0f));
				barPos.x += 30.0f; //30
				barPos.y += 4.0f; //-22
				int spellImg = ChampionNames::GetSummonerSpellIMG(spellInside.ScriptName, 297);

				SdkDrawSpriteFromResource(MAKEINTRESOURCEA(spellImg), &barPos, true); // -34, -31 is good
				if (time > 0.0f)
				{
					SdkDrawSpriteFromResource(MAKEINTRESOURCEA(SB_SSInvisible), &barPos, true);
					ss1 << std::setfill('0') << std::setw(2) << mins << ":" << std::setfill('0') << std::setw(2) << sec;
					barPos.x += -10.0f; //30
					barPos.y += -11.0f; //-22

					DrawHelper::DrawOutlineText(NULL, &barPos, ss1.str().c_str(), "Calibri Bold", &Color::White, 22, 4, 0,
						&Color::Black, false);

				}


				//tempSlot = "W";
			}


			/*
			
				if (Menu::Get<bool>("Trackers.CooldownTracker.SSIcon"))
				{
					SdkDrawSpriteFromResource(MAKEINTRESOURCEA(GetSummonerSpells(spellInside.ScriptName, Game::Time() > spellInside.CooldownExpires)), &SSPosition, false);

				}


				if (time > 0.0f && Menu::Get<bool>("Trackers.CooldownTracker.SSTimer"))
				{
					int format = Menu::Get<int>("Trackers.CooldownTracker.SSTFormat");

					if (format == 0) // mm:ss
					{
						ss1 << std::setfill('0') << std::setw(2) << mins << ":" << std::setfill('0') << std::setw(2) << sec;

					}
					else if (format == 1) // ss
					{
						ss1 << std::setfill('0') << std::setw(2) << onlySec;

					}

					DrawHelper::DrawOutlineText(NULL, &Vector2(SSPosition.x + (float)Menu::Get<int>("Trackers.CooldownTracker.SSTDrawingX") + 18.0f, SSPosition.y + (float)Menu::Get<int>("Trackers.CooldownTracker.SSTDrawingY")), ss1.str().c_str(), "Calibri Bold", &Color::White, 15, 4, 0,
						&Color::Black, false);
				}
			
			*/



			/*
			auto percent = time > 0 && std::abs(spellCD) > FLT_EPSILON
				? 1.0f - time / spellCD
				: 1.0f;
				*/

		}


		barPosStatic = Vector2((resol.Width / 2.0f), (resol.Height / 2.0f));
		barPosStatic.x += -34.0f;
		barPosStatic.y += 20.0f;

		float hp = enemy.Unit->GetHealthPercent();

		float mp = enemy.Unit->GetManaPercent();

		DrawHelper::DrawOutlineLineScreen(&barPosStatic, &Vector2(barPosStatic.x + hp*0.73f, barPosStatic.y), 5.0f, &Color::DarkGreen, &Color::Black); //-40 43
		
		//DrawHelper::DrawOutlineLineScreen(&Vector2(barPosStatic.x + hp * 0.73f, barPosStatic.y), &Vector2(barPosStatic.x + hp * 1.0f, barPosStatic.y), 5.0f, &Color::Grey, &Color::Black); //-40 43

		barPosStatic.y += 6.0f;

		DrawHelper::DrawOutlineLineScreen(&barPosStatic, &Vector2(barPosStatic.x + mp * 0.73f, barPosStatic.y), 5.0f, &Color::Blue, &Color::Black); //-40 43
		//DrawHelper::DrawOutlineLineScreen(&Vector2(barPosStatic.x + mp * 0.73f, barPosStatic.y), &Vector2(barPosStatic.x + 73.0f, barPosStatic.y), 5.0f, &Color::Grey, &Color::Black); //-40 43


		float exp = enemy.Unit->GetExperience();

		float neededExp = RequiredExp[enemy.Unit->GetLevel()];

		float percent = (100.0f / (neededExp - RequiredExp[enemy.Unit->GetLevel() - 1]) * (exp - RequiredExp[enemy.Unit->GetLevel() - 1]));
		if (enemy.Unit->GetLevel() == 18)
		{
			percent = 100.0f;
		}
		barPosStatic.y += 6.0f;

		DrawHelper::DrawOutlineLineScreen(&barPosStatic, &Vector2(barPosStatic.x + percent * 0.73f, barPosStatic.y), 4.0f, &Color::Purple, &Color::Black); //-40 43
		//DrawHelper::DrawOutlineLineScreen(&Vector2(barPosStatic.x + percent * 0.73f, barPosStatic.y), &Vector2(barPosStatic.x + (100.0f * 0.73f), barPosStatic.y), 5.0f, &Color::Grey, &Color::Black); //-40 43
		//Draw::LineScreen(&Vector2(barPosStatic.x + (percent * 0.73f) + 1.0f, barPosStatic.y), &Vector2(barPosStatic.x + (100.0f * 0.73f), barPosStatic.y), 5.0f, &Color::Grey);

		//SB_Invisible
		barPos = Vector2((resol.Width / 2.0f), (resol.Height / 2.0f));
		barPos.x += -2.0f; //-2
		barPos.y += -8.0f; //-8
		DrawHelper::DrawOutlineText(NULL, &barPos, std::to_string(enemy.Unit->GetLevel()).c_str(), "Calibri Bold", &Color::White, 26, 6, 0,
			&Color::Black, false);


		//ChampionNames::GetChampionIMG(enemy.Unit->GetCharName(), 145);
		//ChampionNames::GetChampionIMG(enemy.Unit->GetCharName(), 145))


		if (!enemy.Unit->IsAlive())
		{
			barPos = Vector2((resol.Width / 2.0f), (resol.Height / 2.0f));
			barPos.x += -15.0f; //-15
			barPos.y += -25.0f; //-25


			std::stringstream ss1;
			ss1.precision(0); //for decimal
			ss1.setf(std::ios_base::fixed, std::ios_base::floatfield);

			ss1 << enemy.DeathEndTime - Game::Time();

			//SdkDrawSpriteFromResource(MAKEINTRESOURCEA(SB_Invisible), &barPos, true); // -11 , -10 is good pos
			DrawHelper::DrawOutlineText(NULL, &barPos, ss1.str().c_str(), "Calibri Bold", &Color::Red, 30, 6, 0,
				&Color::Black, false);
		}


		if (!enemy.Unit->IsVisible() || !enemy.Unit->IsAlive())
		{
			barPos = Vector2((resol.Width / 2.0f), (resol.Height / 2.0f));
			barPos.x += -12.0f; //-12
			barPos.y += -10.0f; //-10
			SdkDrawSpriteFromResource(MAKEINTRESOURCEA(SB_Invisible), &barPos, true); // -11 , -10 is good pos

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

		if (!enemy.Unit->IsVisible() && enemy.Unit->IsAlive() && Game::Time() - enemy.LastVisible > 3.0f)
		{
			barPos = Vector2((resol.Width / 2.0f), (resol.Height / 2.0f));
			barPos.x += -15.0f; //-15
			barPos.y += -25.0f; //-25


			std::stringstream ss1;
			ss1.precision(0); //for decimal
			ss1.setf(std::ios_base::fixed, std::ios_base::floatfield);

			ss1 << Game::Time() - enemy.LastVisible;

			//SdkDrawSpriteFromResource(MAKEINTRESOURCEA(SB_Invisible), &barPos, true); // -11 , -10 is good pos
			DrawHelper::DrawOutlineText(NULL, &barPos, ss1.str().c_str(), "Calibri Bold", &Color::White, 30, 6, 0,
				&Color::Black, false);
		}



		/*

		*/

		//SdkDrawSpriteFromResource(MAKEINTRESOURCEA(enemy.ChampIMG), &barPos, true);







		barPosStatic = Vector2((resol.Width / 2.0f), (resol.Height / 2.0f));

		SdkDrawSpriteFromResource(MAKEINTRESOURCEA(SB_HUD), &barPosStatic, true);


		barPosStatic = Vector2((resol.Width / 2.0f), (resol.Height / 2.0f));

		//SdkDrawSpriteFromResource(MAKEINTRESOURCEA(SB_HUD), &barPosStatic, true);


		if (enemy.RSpell.Learned && enemy.RSpell.Level > 0)
		{
			//barPos = Vector2((resol.Width / 2.0f), (resol.Height / 2.0f));
			barPosStatic.x += -34.0f;
			barPosStatic.y += -31.0f;
			if (enemy.RSpell.CooldownExpires - Game::Time() < 0)
			{
				SdkDrawSpriteFromResource(MAKEINTRESOURCEA(SB_Ultimate), &barPosStatic, true); // -34, -31 is good
			}
			else if (enemy.RSpell.CooldownExpires - Game::Time() > 0)
			{

			}

		}

		if (enemy._teleportStatus == TeleportTypes::Start || (enemy._teleportStatus == TeleportTypes::Finished || enemy._teleportStatus == TeleportTypes::Abort)
			&& Game::Time() <= enemy.LastTeleportStatusTime + 3.0f)
		{
			barPosStatic = Vector2((resol.Width / 2.0f), (resol.Height / 2.0f));

			//barPosStatic.x += AdjustX;
			//barPosStatic.y += AdjustY;
			if (enemy._teleportStatus == TeleportTypes::Start)
			{
				SdkDrawSpriteFromResource(MAKEINTRESOURCEA(SB_RecallStart), &barPosStatic, true);
			}
			else if (enemy._teleportStatus == TeleportTypes::Abort)
			{
				SdkDrawSpriteFromResource(MAKEINTRESOURCEA(SB_RecallAbort), &barPosStatic, true);
			}
			else if (enemy._teleportStatus == TeleportTypes::Finished)
			{
				SdkDrawSpriteFromResource(MAKEINTRESOURCEA(SB_RecallFinish), &barPosStatic, true);
			}
		}

		/*
		if (enemy.TeleportStatus == Packet.S2C.Teleport.Status.Start ||
			(enemy.TeleportStatus == Packet.S2C.Teleport.Status.Finish ||
				enemy.TeleportStatus == Packet.S2C.Teleport.Status.Abort) &&
			Game.Time <= enemy.LastTeleportStatusTime + 5f)
		{
			_sprite.Begin(SpriteFlags.AlphaBlend);
			_sprite.DrawCentered(
				enemy.TeleportStatus == Packet.S2C.Teleport.Status.Start
				? _teleportStartTexture
				: (enemy.TeleportStatus == Packet.S2C.Teleport.Status.Finish
					? _teleportFinishTexture
					: _teleportAbortTexture),
				new Vector2(offsetRight + (float)Math.Ceiling(3 * _scale), offsetTop + offset));
			_sprite.End();
		}
		*/

	}

	//SdkDrawSpriteFromResource(MAKEINTRESOURCEA(GetSummonerSpells(spellInside.ScriptName, Game::Time() > spellInside.CooldownExpires)), &SSPosition, false);
}

void __cdecl SideBar::RecallTrack(void * Unit, const char * Name, const char * Type, void * UserData)
{

	UNREFERENCED_PARAMETER(UserData);
	if (!Menu::Get<bool>("Trackers.SideBar.Use"))
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


	if (!netID)
	{
		return;
	}

	auto testStruct = TeleportDecode::TeleportDecoderFunction(Unit, Name, Type);

	SdkUiConsoleWrite("here");

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

