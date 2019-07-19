#pragma once
#include "Trackers.h"
#include "TeleportDecode.h"
#include "SpriteImageLoader.h"


enum SideBarTypes
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
	bool IsRLearned;
	PSDK_SPELL SummonerSpells1{};
	float SummonerSpells1CD;
	int SummonerSpells1IMG;
	PSDK_SPELL SummonerSpells2{};
	float SummonerSpells2CD;
	int SummonerSpells2IMG;
	bool TrackRecall;
	int RecallType;


	bool MissingTimerBool;

	Vector2 MainFramePosition;
	Vector2 HUDPosition;
	std::map<SideBarTypes, Vector2> DrawingStartPosition;
	std::map<SideBarTypes, Vector2> DrawingEndPosition; // I store timer in there too

	EnemyObject(AIHeroClient* _Unit)
		: Unit(_Unit), LastVisible(Game::Time()), LastTeleportStatusTime(0), CurrentLevel(1), DeathEndTime(0),
		DeathEndTimeLeftTime(0),
		LastVisibleLeftTime(0),
		IsRLearned(false),
		TrackRecall(false),
		RecallType(0),
		MissingTimerBool(false)
	{
		_teleportStatus = TeleportTypes::UnknownStatus;

		//if (strstr(_Unit->GetCharName(), "Dummy") == 0)
			//return;


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

		const Vector2 tempPos = ScreenPositionGet();

		MainFramePosition = tempPos;

		HUDPosition = Vector2(MainFramePosition.x - 11.0f, MainFramePosition.y - 10.0f);

		DrawingStartPosition.try_emplace(SideBarTypes::HpBar, Vector2((tempPos.x - 34.0f) , tempPos.y + 20.0f)); //Hp bar Start

		DrawingEndPosition.try_emplace(SideBarTypes::HpBar, Vector2((tempPos.x - 34.0f) + Unit->GetManaPercent() * 0.73F, tempPos.y + 20.0f)); //Hp bar 


		//DrawingEndPosition.try_emplace(SideBarTypes::HpBar, Vector2((tempPos.x - 34.0f) + Unit->GetHealthPercent()*0.73F, tempPos.y + 20.0f)); //Hp bar 

		DrawingStartPosition.try_emplace(SideBarTypes::MpBar, Vector2((tempPos.x - 34.0f) , tempPos.y + 26.0f)); //MpBar Start

		DrawingEndPosition.try_emplace(SideBarTypes::MpBar, Vector2((tempPos.x - 34.0f) + Unit->GetManaPercent() * 0.73F, tempPos.y + 26.0f)); //MpBar


		DrawingStartPosition.try_emplace(SideBarTypes::ExpBar, Vector2((tempPos.x - 34.0f) , tempPos.y + 32.0f)); //MpBar Start

		DrawingEndPosition.try_emplace(SideBarTypes::ExpBar, Vector2((tempPos.x - 34.0f) + Unit->GetManaPercent() * 0.73F, tempPos.y + 32.0f)); //MpBar



		DrawingStartPosition.try_emplace(SideBarTypes::DeathMissingTimer, Vector2(tempPos.x - 15.0f, tempPos.y - 25.0f)); //Death Timer

		DrawingStartPosition.try_emplace(SideBarTypes::Level, Vector2(tempPos.x - 3.0f, tempPos.y + 5.0f)); //Level

		DrawingStartPosition.try_emplace(SideBarTypes::Ultimate, Vector2(tempPos.x - 34.0f, tempPos.y - 31.0f)); //Death Timer

		DrawingEndPosition.try_emplace(SideBarTypes::Ultimate, Vector2(tempPos.x - 38.0f, tempPos.y - 39.0f)); //Death Timer


		DrawingStartPosition.try_emplace(SideBarTypes::SummonerSpell1, Vector2((tempPos.x + 30.0f), tempPos.y + 22.0f)); //MpBar Start

		DrawingEndPosition.try_emplace(SideBarTypes::SummonerSpell1, Vector2((tempPos.x + 20.0f), tempPos.y - 30.0f)); //MpBar

		DrawingStartPosition.try_emplace(SideBarTypes::SummonerSpell2, Vector2((tempPos.x + 30.0f), tempPos.y + 4.0f)); //MpBar Start

		DrawingEndPosition.try_emplace(SideBarTypes::SummonerSpell2, Vector2((tempPos.x + 20.0f), tempPos.y - 4.0f)); //MpBar



		//DrawingEndPosition.try_emplace(SideBarTypes::ExpBar, Vector2((tempPos.x - 34.0f) + Unit->GetManaPercent() * 0.73F, tempPos.y + 32.0f)); //MpBar

		//Vector2(tempPos.x - 15.0f + SideBarMenuList[5].DrawX, tempPos.y - 25.0f + SideBarMenuList[5].DrawY);




		const float currentTime = Game::Time();
		auto spells = _Unit->GetSpells();
		for (auto& spellInside : spells)
		{
			if (int(spellInside.Slot) == 0 || int(spellInside.Slot) == 1 || int(spellInside.Slot) == 2)
				continue;

			const float time = spellInside.CooldownExpires - currentTime;
			if (int(spellInside.Slot) == 3)
			{
				if (!IsRLearned)
				{
					this->IsRLearned = spellInside.Learned;
				}


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

		HUDPosition = Vector2(MainFramePosition.x - 11.0f, MainFramePosition.y - 10.0f);


		DrawingStartPosition[SideBarTypes::HpBar] = Vector2((tempPos.x - 34.0f), tempPos.y + 20.0f); //Hp bar Start

		DrawingEndPosition[SideBarTypes::HpBar] = Vector2((tempPos.x - 34.0f) + Unit->GetHealthPercent() * 0.73F, tempPos.y + 20.0f); //Hp bar 

		DrawingStartPosition[SideBarTypes::MpBar] = Vector2((tempPos.x - 34.0f), tempPos.y + 26.0f); //MpBar Start

		DrawingEndPosition[SideBarTypes::MpBar] = Vector2((tempPos.x - 34.0f) + Unit->GetManaPercent() * 0.73F, tempPos.y + 26.0f); //MpBar


		/*
		 *
		DrawingStartPosition.try_emplace(SideBarTypes::DeathMissingTimer, Vector2(tempPos.x - 15.0f, tempPos.y - 25.0f)); //Death Timer

		DrawingStartPosition.try_emplace(SideBarTypes::Level, Vector2(tempPos.x - 3.0f, tempPos.y + 5.0f)); //Level

		DrawingStartPosition.try_emplace(SideBarTypes::Ultimate, Vector2(tempPos.x - 34.0f, tempPos.y - 31.0f)); //Death Timer

		DrawingStartPosition.try_emplace(SideBarTypes::SummonerSpell1, Vector2((tempPos.x + 30.0f), tempPos.y + 22.0f)); //MpBar Start

		DrawingEndPosition.try_emplace(SideBarTypes::SummonerSpell1, Vector2((tempPos.x + 20.0f), tempPos.y - 30.0f)); //MpBar

		DrawingStartPosition.try_emplace(SideBarTypes::SummonerSpell2, Vector2((tempPos.x + 30.0f), tempPos.y + 4.0f)); //MpBar Start

		DrawingEndPosition.try_emplace(SideBarTypes::SummonerSpell2, Vector2((tempPos.x + 20.0f), tempPos.y - 4.0f)); //MpBar
		 *
		 *
		 */

		DrawingStartPosition[SideBarTypes::DeathMissingTimer] = Vector2(tempPos.x - 15.0f, tempPos.y - 25.0f); //Death

		DrawingStartPosition[SideBarTypes::Level] = Vector2(tempPos.x + 5.0f, tempPos.y - 6.0f); //Level 

		DrawingStartPosition[SideBarTypes::Ultimate] = Vector2(tempPos.x - 34.0f, tempPos.y - 31.0f); //MpBar Start

		DrawingEndPosition[SideBarTypes::Ultimate] = Vector2(tempPos.x - 40.0f, tempPos.y - 39.0f); //Death Timer


		DrawingStartPosition[SideBarTypes::SummonerSpell1] = Vector2((tempPos.x + 30.0f), tempPos.y - 22.0f); //MpBar

		DrawingEndPosition[SideBarTypes::SummonerSpell1] = Vector2((tempPos.x + 20.0f), tempPos.y - 30.0f); //MpBar

		DrawingStartPosition[SideBarTypes::SummonerSpell2] = Vector2((tempPos.x + 30.0f), tempPos.y + 4.0f); //MpBar

		DrawingEndPosition[SideBarTypes::SummonerSpell2] = Vector2((tempPos.x + 20.0f), tempPos.y - 4.0f); //MpBar


		//DrawingStartPosition[SideBarTypes::ExpBar] = Vector2((tempPos.x - 34.0f) , tempPos.y + 32.0f); //MpBar Start

		//DrawingEndPosition[SideBarTypes::ExpBar] = Vector2((tempPos.x - 34.0f) + ChampInfo::ExperiencePercentage(Unit->GetLevel(), Unit->GetExperience()) * 0.73F, tempPos.y + 32.0f); //MpBar

	}

	void EnemyUpdate()
	{



		// Unit->GetHealthPercent() * 0.73F;
		DrawingEndPosition[SideBarTypes::HpBar].x = DrawingStartPosition[SideBarTypes::HpBar].x + Unit->GetHealthPercent() * 0.73F;

		DrawingEndPosition[SideBarTypes::MpBar].x = DrawingStartPosition[SideBarTypes::MpBar].x + Unit->GetManaPercent() * 0.73F; //MpBar

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
				if (!IsRLearned)
				{
					this->IsRLearned = spellInside.Learned;
				}

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
		
		if (!Unit->IsAlive() && currentTime > DeathEndTime)
		{
			DeathEndTime = currentTime + Unit->GetDeathDuration() + 1.0f;
		}
		else if (Unit->IsAlive())
		{
			DeathEndTime = 0.0f;
		}


		auto enemyPos = Unit->GetPosition();
		if (!Unit->IsAlive()) // Death Timer
		{
			DeathEndTimeLeftTime = DeathEndTime - currentTime;
		}
		else if ( (Unit->IsAlive() && LastPosition.Distance(enemyPos) > 500.0f) || Unit->IsVisible() || !Unit->IsAlive())
		{
			LastVisible = currentTime;
		}

		LastPosition = enemyPos;


		if (!Unit->IsVisible() && Unit->IsAlive() && currentTime - LastVisible > 3.0F) // Missing Timer
		{

			LastVisibleLeftTime = currentTime - LastVisible;
			MissingTimerBool = true;
		}
		else
		{
			MissingTimerBool = false;
		}

		if (SpellR->Learned && SpellR->Level > 0 )
		{
			SpellRCD = SpellR->CooldownExpires - currentTime;;

		}

		SummonerSpells1CD = SummonerSpells1->CooldownExpires - currentTime;
		SummonerSpells2CD = SummonerSpells2->CooldownExpires - currentTime;




	}

	static Vector2 ScreenPositionGet()
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
	void SettingsUpdate();

};

