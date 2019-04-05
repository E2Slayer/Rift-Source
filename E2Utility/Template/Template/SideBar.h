#pragma once
#include "Trackers.h"
#include "TeleportDecode.h"
#include "ChampionNames.h"
#include <string.h>

struct SideBarMenu
{
	const char* MenuName;
	bool IsTimer;
	bool Enable;
	float DrawX;
	float DrawY;
	float FontSize1;
	float FontSize2;
	int FontColor;
	int OutlineFontColor;

	SideBarMenu(const char* _MenuName, bool isTimer)
		: MenuName(_MenuName), IsTimer(isTimer)
	{
		/*
					Menu::DropList("Summoner Spells Timer Format", "Trackers.SideBar.SS.Format", std::vector<std::string>{ "MM:SS", "SS"}, 0);
			Menu::SliderInt("Summoner Spells Timer Position X-axis", "Trackers.SideBar.SS.DrawingX", 0, -200, 200);
			Menu::SliderInt("Summoner Spells Timer Position Y-axis", "Trackers.SideBar.SS.DrawingY", 0, -200, 200);
			Menu::SliderInt("Summoner Spells Timer Font Height", "Trackers.SideBar.SS.FontSize", 20, 10, 30);
			Menu::SliderInt("Summoner Spells Timer Font Width", "Trackers.SideBar.SS.FontSize2", 4, 1, 10);
			Menu::DropList("^-> Summoner Spells Timer Color", "Trackers.SideBar.SS.Color", ColorMenuList, 11);
			Menu::DropList("^-> Summoner Spells Timer OutLine Color", "Trackers.SideBar.SS.OutLineColor", ColorMenuList, 0);
		*/

		//Trackers.SideBar.Bars.Health.DrawingX

		Enable = Menu::Get<bool>("Trackers.SideBar." + std::string(_MenuName) + ".Use");
		DrawX = float(Menu::Get<int>("Trackers.SideBar." +std::string(_MenuName)+ ".DrawingX" ));
		DrawY = float(Menu::Get<int>("Trackers.SideBar." + std::string(_MenuName) + ".DrawingY"));
		FontSize1 = 0;
		FontSize2 = 0;
		FontColor = 0;
		OutlineFontColor = 0;

		if (isTimer)
		{
			//Enable = Menu::Get<bool>("Trackers.SideBar." + std::string(_MenuName) + ".Timer");
			FontSize1 = float(Menu::Get<int>("Trackers.SideBar." + std::string(_MenuName) + ".FontSize"));
			FontSize2 = float(Menu::Get<int>("Trackers.SideBar." + std::string(_MenuName) + ".FontSize2"));
			FontColor = Menu::Get<int>("Trackers.SideBar." + std::string(_MenuName) + ".Color");
			OutlineFontColor = Menu::Get<int>("Trackers.SideBar." + std::string(_MenuName) + ".OutLineColor");

		}
	}

	void UpdateInsideFloats()
	{
		this->Enable = Menu::Get<bool>("Trackers.SideBar." + std::string(MenuName) + ".Use");
		this->DrawX = float(Menu::Get<int>("Trackers.SideBar." + std::string(MenuName) + ".DrawingX"));
		this->DrawY = float(Menu::Get<int>("Trackers.SideBar." + std::string(MenuName) + ".DrawingY"));


		if (this->IsTimer)
		{
			this->FontSize1 = float(Menu::Get<int>("Trackers.SideBar." + std::string(MenuName) + ".FontSize"));
			this->FontSize2 = float(Menu::Get<int>("Trackers.SideBar." + std::string(MenuName) + ".FontSize2"));
			this->FontColor = Menu::Get<int>("Trackers.SideBar." + std::string(MenuName) + ".Color");
			this->OutlineFontColor = Menu::Get<int>("Trackers.SideBar." + std::string(MenuName) + ".OutLineColor");

		}
	}

};

struct SidePositionList
{
	Vector2 MainStructPos;
	
	Vector2 HpBarPos;
	Vector2 HpBarOrignalPos;
	Vector2 MpBarPos;
	Vector2 MpBarOrignalPos;
	Vector2 ExpBarPos;
	Vector2 ExpBarOrignalPos;
	Vector2 LevelPos;
	Vector2 DeathTimerPos;
	Vector2 MissingTimerPos;
	bool MissingTimerBool;
	Vector2 UltimatePos;
	Vector2 SS1Pos;
	Vector2 SS1TimerPos;
	Vector2 SS2Pos;
	Vector2 SS2TimerPos;

	SidePositionList(Vector2 _MainStructPos)
		: MainStructPos(_MainStructPos)
	{
		MainStructPos = Vector3(0.0f, 0.0f, 0.0f);
		 HpBarPos = Vector3(0.0f, 0.0f, 0.0f);
		 MpBarPos = Vector3(0.0f, 0.0f, 0.0f);;
		 ExpBarPos = Vector3(0.0f, 0.0f, 0.0f);
		 LevelPos = Vector3(0.0f, 0.0f, 0.0f);
		 DeathTimerPos = Vector3(0.0f, 0.0f, 0.0f);
		 MissingTimerPos = Vector3(0.0f, 0.0f, 0.0f);
		bool MissingTimerBool = false;
		 UltimatePos = Vector3(0.0f, 0.0f, 0.0f);
		 SS1Pos = Vector3(0.0f, 0.0f, 0.0f);
		 SS2Pos = Vector3(0.0f, 0.0f, 0.0f);
		 SS1TimerPos = Vector3(0.0f, 0.0f, 0.0f);
		 SS2TimerPos = Vector3(0.0f, 0.0f, 0.0f);
	}



};

struct EnemyObject
{
	TeleportTypes _teleportStatus;
	//SDK_SPELL RSpell;
	AIHeroClient* Unit;
	float DeathEndTime;
	float LastVisible;
	Vector3 LastPosition;
	float LastTeleportStatusTime;
	int ChampIMG;
	int CurrentLevel;
	float DeathEndTimeLeftTime;
	float LastVisibleLeftTime;
	SDK_SPELL SpellR;
	float SpellRCD;
	SDK_SPELL SummonerSpells1;
	float SummonerSpells1CD;
	int SummonerSpells1IMG;
	SDK_SPELL SummonerSpells2;
	float SummonerSpells2CD;
	int SummonerSpells2IMG;
	bool TrackRecall;
	int RecallType;

	float HPlength;
	float MPlength;

	float EXPlength;

	SidePositionList PositionList = SidePositionList(Vector3(0.0f, 0.0f, 0.0f));






	EnemyObject(AIHeroClient* _Unit)
		: Unit(_Unit)
	{

		_teleportStatus = TeleportTypes::UnknownStatus;
	//	RSpell = _Unit->GetSpell((unsigned char)SpellSlot::R);

		LastVisible = Game::Time();
		LastPosition = Vector3(0.0f, 0.0f, 0.0f);

		DeathEndTime = 0.0f;
		LastTeleportStatusTime = 0.0f;
		//SdkUiConsoleWrite("img nmber1 %s", _Unit->GetCharName());
		ChampIMG = ChampionNames::GetChampionIMG(_Unit->GetCharName(), 145);
		CurrentLevel = 1;
		DeathEndTimeLeftTime = 0;
	    PositionList = SidePositionList(Vector3(0.0f, 0.0f, 0.0f));



		HPlength = 73.0f;
		MPlength = 73.0f;

		EXPlength = 0.0f;

		auto spells = _Unit->GetSpells();
		for (auto &spellInside : spells)
		{
			float time = spellInside.CooldownExpires - Game::Time();
			if (int(spellInside.Slot) == 3)
			{
				SpellR = spellInside;
				SpellRCD = time;
			}
			else if (int(spellInside.Slot) == 4)
			{
				SummonerSpells1 = spellInside;
				SummonerSpells1CD = time;

				SummonerSpells1IMG = ChampionNames::GetSummonerSpellIMG(spellInside.ScriptName, 297);
			}
			else if (int(spellInside.Slot) == 5)
			{
				SummonerSpells2 = spellInside;
				SummonerSpells2CD = time;
				SummonerSpells2IMG = ChampionNames::GetSummonerSpellIMG(spellInside.ScriptName, 297);
			}
			else
			{
				continue;
			}

		}
	}
};


class SideBar
{
public:
	static void Init();

	static void MenuLoader();
	static void TickLoader();
	static void DrawLoader();


	static void __cdecl RecallTrack(void* Unit, const char* Name, const char* Type, void* UserData);
};

