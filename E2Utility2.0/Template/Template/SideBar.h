#pragma once
#include "Trackers.h"
#include "TeleportDecode.h"
#include "SpriteImageLoader.h"
#include "ChampInfo.h"


const enum SideBarTypes
{
	HpBar,
	MpBar,
	ExpBar,
	DeathMissingTimer, 
	Level,
	Ultimate,
	SummonerSpell1,
	SummonerSpell2,

};

inline struct SideBarSettingsStruct
{

	int screenPosSelection = 0;
	int arrayStyle = 0;
	int SidebarGab = 0;

	int MainFramePosX = 0;
	int MainFramePosY = 0;

	


} SideBarSettings;


struct EnemyObject
{

	TeleportTypes _teleportStatus;
	AIHeroClient* Unit;

	float LastVisible;


	Vector3 LastPosition;
	float LastTeleportStatusTime;


	int ChampIMG;
	int CurrentLevel;

	float DeathEndTime;
	float DeathEndTimeLeftTime;
	float LastVisibleLeftTime;


	
	PSDK_SPELL SpellR{};
	float SpellRCD;
	PSDK_SPELL SummonerSpells1{};
	float SummonerSpells1CD;
	int SummonerSpells1IMG;
	PSDK_SPELL SummonerSpells2{};
	float SummonerSpells2CD;
	int SummonerSpells2IMG;
	bool TrackRecall;
	int RecallType;

	Vector2 MainFramePosition;
	Vector2 HUDPosition;
	std::map<SideBarTypes, Vector2> DrawingStartPosition;
	std::map<SideBarTypes, Vector2> DrawingEndPosition; // I store timer in there too

	EnemyObject(AIHeroClient* _Unit)
		: Unit(_Unit), LastVisible(Game::Time()), LastTeleportStatusTime(0), CurrentLevel(1), DeathEndTime(0),
		  DeathEndTimeLeftTime(0),
		  LastVisibleLeftTime(0),
		  TrackRecall(false),
		  RecallType(0)
	{
		_teleportStatus = TeleportTypes::UnknownStatus;

		//if (strstr(_Unit->GetCharName(), "Dummy") == 0)
			//return;

		auto yyy = (Unit->GetCharName());


		ChampIMG = SpriteImageLoader::GetChampionIMG(Unit->GetCharName());
		SdkUiConsoleWrite("IMg number : %d", ChampIMG);

		/*
		 *	HpBar,
			MpBar,
			ExpBar,
			DeathMissingTimer, 
			Level,
			Ultimate,
			SummonerSpell1,
			SummonerSpell2,
		 *
		 */

		Vector2 tempPos = ScreenPositionGet();

		MainFramePosition = tempPos;

		HUDPosition = Vector2(MainFramePosition.x - 11.0f, MainFramePosition.y - 10.0f);

		DrawingStartPosition.try_emplace(SideBarTypes::HpBar, &Vector2((tempPos.x - 34.0f) , tempPos.y + 20.0f)); //Hp bar Start

		DrawingEndPosition.try_emplace(SideBarTypes::HpBar, &Vector2((tempPos.x - 34.0f) + Unit->GetManaPercent() * 0.73F, tempPos.y + 20.0f)); //Hp bar 


		//DrawingEndPosition.try_emplace(SideBarTypes::HpBar, Vector2((tempPos.x - 34.0f) + Unit->GetHealthPercent()*0.73F, tempPos.y + 20.0f)); //Hp bar 

		DrawingStartPosition.try_emplace(SideBarTypes::MpBar, &Vector2((tempPos.x - 34.0f) , tempPos.y + 26.0f)); //MpBar Start

		DrawingEndPosition.try_emplace(SideBarTypes::MpBar, &Vector2((tempPos.x - 34.0f) + Unit->GetManaPercent() * 0.73F, tempPos.y + 26.0f)); //MpBar


		DrawingStartPosition.try_emplace(SideBarTypes::ExpBar, &Vector2((tempPos.x - 34.0f) , tempPos.y + 32.0f)); //MpBar Start

		DrawingEndPosition.try_emplace(SideBarTypes::ExpBar, &Vector2((tempPos.x - 34.0f) + Unit->GetManaPercent() * 0.73F, tempPos.y + 32.0f)); //MpBar





		const float currentTime = Game::Time();
		auto spells = _Unit->GetSpells();
		for (auto& spellInside : spells)
		{
			if (int(spellInside.Slot) == 0 || int(spellInside.Slot) == 1 || int(spellInside.Slot) == 2)
				continue;

			const float time = spellInside.CooldownExpires - currentTime;
			if (int(spellInside.Slot) == 3)
			{
				SpellR = &spellInside;
				SpellRCD = time;
			}
			else if (int(spellInside.Slot) == 4)
			{
				SummonerSpells1 = &spellInside;
				SummonerSpells1CD = time;

				SummonerSpells1IMG = SpriteImageLoader::GetSummonerSpellIMG(spellInside.ScriptName);
			}
			else if (int(spellInside.Slot) == 5)
			{
				SummonerSpells2 = &spellInside;
				SummonerSpells2CD = time;
				SummonerSpells2IMG = SpriteImageLoader::GetSummonerSpellIMG(spellInside.ScriptName);
			}
		}
	}

	void PositionUpdate(int ChampionNumber)
	{
		Vector2 tempPos = ScreenPositionGet();

		if(SideBarSettings.arrayStyle == 0) //vertical
		{
			tempPos.y += float(SideBarSettings.SidebarGab * ChampionNumber);
		}
		else //horizontal
		{
			tempPos.x += float(SideBarSettings.SidebarGab* ChampionNumber);
		}
		
		MainFramePosition = tempPos;

		HUDPosition = &Vector2(MainFramePosition.x - 11.0f, MainFramePosition.y - 10.0f);


		DrawingStartPosition[SideBarTypes::HpBar] = &Vector2((tempPos.x - 34.0f), tempPos.y + 20.0f); //Hp bar Start

		DrawingEndPosition[SideBarTypes::HpBar] = &Vector2((tempPos.x - 34.0f) + Unit->GetHealthPercent() * 0.73F, tempPos.y + 20.0f); //Hp bar 

		DrawingStartPosition[SideBarTypes::MpBar] = &Vector2((tempPos.x - 34.0f), tempPos.y + 26.0f); //MpBar Start

		DrawingEndPosition[SideBarTypes::MpBar] = &Vector2((tempPos.x - 34.0f) + Unit->GetManaPercent() * 0.73F, tempPos.y + 26.0f); //MpBar




		//DrawingStartPosition[SideBarTypes::ExpBar] = Vector2((tempPos.x - 34.0f) , tempPos.y + 32.0f); //MpBar Start

		//DrawingEndPosition[SideBarTypes::ExpBar] = Vector2((tempPos.x - 34.0f) + ChampInfo::ExperiencePercentage(Unit->GetLevel(), Unit->GetExperience()) * 0.73F, tempPos.y + 32.0f); //MpBar

	}

	void EnemyUpdate()
	{




		DrawingEndPosition[SideBarTypes::HpBar] = &Vector2((MainFramePosition.x - 34.0f) + Unit->GetHealthPercent() * 0.73F, MainFramePosition.y + 20.0f); //Hp bar 


		DrawingEndPosition[SideBarTypes::MpBar] = &Vector2((MainFramePosition.x - 34.0f) + Unit->GetManaPercent() * 0.73F, MainFramePosition.y + 26.0f); //MpBar

		//DrawingEndPosition[SideBarTypes::ExpBar] = Vector2((MainFramePosition.x - 34.0f) + ChampInfo::ExperiencePercentage(Unit->GetLevel(), Unit->GetExperience()) * 0.73F, MainFramePosition.y + 32.0f); //MpBar


		const float currentTime = Game::Time();
		auto spells = Unit->GetSpells();
		for (auto& spellInside : spells)
		{
			if (int(spellInside.Slot) == 0 || int(spellInside.Slot) == 1 || int(spellInside.Slot) == 2)
				continue;

			const float time = spellInside.CooldownExpires - currentTime;
			if (int(spellInside.Slot) == 3)
			{
				SpellR = &spellInside;
				SpellRCD = time;
			}
			else if (int(spellInside.Slot) == 4)
			{

				SummonerSpells1 = &spellInside;
				SummonerSpells1CD = time;

				//SummonerSpells1IMG = SpriteImageLoader::GetSummonerSpellIMG(spellInside.ScriptName);
			}
			else if (int(spellInside.Slot) == 5)
			{
				SummonerSpells2 = &spellInside;
				SummonerSpells2CD = time;
				//SummonerSpells2IMG = SpriteImageLoader::GetSummonerSpellIMG(spellInside.ScriptName);
			}

		}
	}

	Vector2 ScreenPositionGet()
	{
		const int screenPosSelection = SideBarSettings.screenPosSelection;
		const auto PlayerResolution = Renderer::GetResolution();

		if (screenPosSelection == 0) // on the left
		{
			return Vector2(0.0f + 50.0f, (PlayerResolution.Height / 2.0f) - (PlayerResolution.Height / 4.0f) - (PlayerResolution.Height / 10.0f));

		}
		else if (screenPosSelection == 1) // on the right
		{
			return Vector2(PlayerResolution.Width - 50.0f, (PlayerResolution.Height / 2.0f) - (PlayerResolution.Height / 4.0f) - (PlayerResolution.Height / 10.0f));
		}
		else if (screenPosSelection == 2) // on the top
		{
			return Vector2((PlayerResolution.Width / 2.0f) - (PlayerResolution.Width / 4.0f), 50.0f);
		}



		return Vector2();
	}
};





class SideBar :
	public Trackers
{
private:
	std::vector<EnemyObject> _enemyObject;

public:


	SideBar();
	~SideBar();
	void Initialized() override;
	void OnTick(void* userData) override;
	void OnDraw(void* userData) override;
	void OnDrawMenu(void* userData) override;
	void OnRecall(void* Unit, const char* Name, const char* Type, void* UserData) override;
	void SettingsUpdate() const;

};

