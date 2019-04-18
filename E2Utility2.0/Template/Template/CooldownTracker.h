#pragma once
#include "Trackers.h"
#include "SpriteImageLoader.h"
#include "TextHelpers.h"
#include "ChampInfo.h"

constexpr int CDRequiredExp[] = {
	0, 280, 660, 1140, 1720, 2400, 3180, 4060, 5040, 6120, 7300, 8580, 9960, 11440, 13020, 14700, 16480, 18360, INT_MAX
};


struct ManualSpell
{
	const char* Champ;
	const char* SpellName;
	SpellSlot Slot;
	float Cooldowns[5]{};
	float Additional;
	float Cooldown;
	float CooldownExpires;
	bool IsAlly;

	ManualSpell(const char* _Champ, const char* _SpellName, SpellSlot _Slot, float (&_Coooldowns)[5], float _Additional,
	            bool _IsAlly)
		: Champ(_Champ), SpellName(_SpellName), Slot(_Slot), Additional(_Additional), IsAlly(_IsAlly)
	{
		for (int i = 4; i != 0; --i)
		{
			Cooldowns[i] = _Coooldowns[i];
		}

		Cooldown = 0.0f;
		CooldownExpires = 0.0f;
	}
};

struct CooldownInisdeStruct
{
	SDK_SPELL Spell;
	float Cooldown;
	bool IsLearned;
	int SpellIMG;
	//bool IsSummonerSpell;
	float Percent;


	CooldownInisdeStruct(SDK_SPELL _Spell, float _Cooldown, bool _IsLearned, int _SpellIMG)
		: Spell(_Spell), Cooldown(_Cooldown), IsLearned(_IsLearned), SpellIMG(_SpellIMG)
	{
		//_Spell.TotalCooldown
		Percent = _Cooldown > 0 && std::abs(_Spell.TotalCooldown) > FLT_EPSILON  ? 1.0f - _Cooldown / _Spell.TotalCooldown : 1.0f;
	}


	void UpdateInfoManual(float newTime, float totalCD)
	{
		this->Cooldown = newTime;
		if (!this->IsLearned)
		{
			this->IsLearned = Spell.Learned;
		}

		Percent = newTime > 0 && std::abs(totalCD) > FLT_EPSILON ? 1.0f - newTime / totalCD : 1.0f;
	}
};


inline struct CooldownTrackerSettingsStruct
{
	bool EnableCooldownTracker = false;
	bool TrackMyself = false;
	bool TrackAlly = false;
	bool TrackEnemy = false;
	bool EnableHUD = false;


	bool EnableExpbar = false;
	int ExpbarWidth = 0;
	int ExpbarPosX = 0;
	int ExpbarPosY = 0;


	int CDBarWidth = 0;
	int CDBarLength = 0;
	int CDBarGab = 0;
	int CDBarPosX = 0;
	int CDBarPosY = 0;

	bool EnableCDTimer = false;

	int CDTimerFormat = 0;
	int CDTimerFontSize = 0;
	int CDTimerFontSize2 = 0;
	int CDTimerPosX = 0;
	int CDTimerPosY = 0;


	int SSPosX = 0;
	int SSPosY = 0;

	bool EnableSSTimer = false;
	int SSTimerFormat = 0;
	int SSTimerFontSize = 0;
	int SSTimerFontSize2 = 0;
	int SSTimerPosX = 0;
	int SSTimerPosY = 0;


	//bool MinimapTrack = false;

	//bool VisionRangeKey = false;
	//bool VisionRangeToggle = false;
} CooldownTrackerSettings;



struct CooldownSpellInfo
{
	SDK_SPELL Spell;
	unsigned short int Cooldown;
	bool IsLearned;
	unsigned short int SpellIMG;
	//float Percent;


	CooldownSpellInfo(SDK_SPELL _Spell, int _Cooldown, bool _IsLearned, int _SpellIMG)
		: Spell(_Spell), Cooldown(_Cooldown), IsLearned(_IsLearned), SpellIMG(_SpellIMG)
	{
		//_Spell.TotalCooldown
		//Percent = _Cooldown > 0 && std::abs(_Spell.TotalCooldown) > FLT_EPSILON ? 1.0f - _Cooldown / _Spell.TotalCooldown : 1.0f;
	}
};

struct CooldownBasicChampInfo
{
	AIHeroClient* Hero;
	unsigned int NetID;
	std::vector<CooldownSpellInfo> CooldownSpells;
	Vector2 HeroHealthPos;


	CooldownBasicChampInfo(AIHeroClient* _Hero, unsigned int _NetID)
		: Hero(_Hero), NetID(_NetID)
	{
		HeroHealthPos = _Hero->GetHealthBarScreenPos();

		auto spells = _Hero->GetSpells();
	
		if (spells.empty())
		{
			return;
		}


		const float currentTime = Game::Time();
		for (auto& spellInside : spells)
		{
			if (int(spellInside.Slot) < 0 || int(spellInside.Slot) > 6)
			{
				continue;
			}
			CooldownSpells.emplace_back(CooldownSpellInfo(spellInside, 0,
				spellInside.Learned,
				SpriteImageLoader::GetSmallSummonerSpellIMG(
					spellInside.ScriptName, false)));
		}
	}
};




struct CooldownBasicChampList
{
	std::vector<CooldownBasicChampInfo> TrackerInfo;


};








struct CooldownChamp
{
	AIHeroClient* Hero;
	unsigned int NetID;
	std::vector<CooldownInisdeStruct> CooldownSpells;

	std::map<int, Vector2> SpellsStartPosition;
	std::map<int, Vector2> SpellsEndPosition;
	std::map<int, Vector2> SpellsTimerPosition;

	bool IsVisible;
	float ExpPercent;
	Vector2 HeroHealthPos;

	PSDKVECTOR HeroHealthPosPointer;

	//CooldownPositionList CDPosList = CooldownPositionList(Vector3(0.0f, 0.0f, 0.0f));

	CooldownChamp(AIHeroClient* _Hero, unsigned int _NetID)
		: Hero(_Hero), NetID(_NetID) , IsVisible(false)
	{
		CooldownSpells.clear();
		HeroHealthPos = _Hero->GetHealthBarScreenPos();
		HeroHealthPosPointer = &Hero->GetPosition();
	
		auto spells = _Hero->GetSpells();
		if (spells.empty())
		{
			return;
		}


		Vector2 SpellsPosition = HeroHealthPos;
		SpellsPosition.x += -43.0f + CooldownTrackerSettings.CDBarPosX;
		SpellsPosition.y += -3.0f + CooldownTrackerSettings.CDBarPosY;

		Vector2 SSPosition = HeroHealthPos;
		SSPosition.x += 63.0f + CooldownTrackerSettings.SSPosX;
		SSPosition.y += -24.0f + CooldownTrackerSettings.SSPosY;

		Vector2* test = &HeroHealthPos;

		const float currentTime = Game::Time();
		for (auto& spellInside : spells)
		{
			if (int(spellInside.Slot) < 0 || int(spellInside.Slot) > 6)
			{
				continue;
			}


			CooldownSpells.emplace_back(CooldownInisdeStruct(spellInside, spellInside.CooldownExpires - currentTime,
			                                                 spellInside.Learned,
			                                                 SpriteImageLoader::GetSmallSummonerSpellIMG(
				                                                 spellInside.ScriptName, false)));
			const int slot = static_cast<int>(spellInside.Slot);
			if (slot == 4 || slot == 5)
			{
				if (slot == 5)
				{
					SSPosition.y += -15.0f;
				}

				//SpellsTimerPosition
				SpellsStartPosition.try_emplace(spellInside.Slot, &SSPosition);

				Vector2 timerPos = Vector2(SSPosition.x + 18.0F + CooldownTrackerSettings.SSTimerPosX,
				                           SSPosition.y + CooldownTrackerSettings.SSTimerPosY);

				SpellsTimerPosition.try_emplace(spellInside.Slot, &timerPos);
			}
			else
			{
				if (CooldownSpells[spellInside.Slot].IsLearned)
				{
					Vector2 LengthAdjustedPos = Vector2(
						SpellsPosition.x + CooldownSpells[spellInside.Slot].Percent * CooldownTrackerSettings.
						CDBarLength,
						SpellsPosition.y);
					SpellsStartPosition.try_emplace(spellInside.Slot, &SpellsPosition);
					SpellsEndPosition.try_emplace(spellInside.Slot, &LengthAdjustedPos);

					LengthAdjustedPos = Vector2(SpellsPosition.x + 2.0F + CooldownTrackerSettings.CDTimerPosX,
					                            SpellsPosition.y + 9.0F + CooldownTrackerSettings.CDTimerPosY);

					SpellsTimerPosition.try_emplace(spellInside.Slot, &LengthAdjustedPos);
				}
				SpellsPosition.x += CooldownTrackerSettings.CDBarGab;
			}
		}

		if (Hero->GetLevel() < 18)
		{

			ExpPercent = ChampInfo::ExperiencePercentage(Hero->GetLevel(), Hero->GetExperience())* 1.05F;
		}

	}

	void UpdateCDChamp()
	{
		if(Hero->IsVisibleOnScreen())
		{
			IsVisible = true;
		}
		else
		{
			IsVisible = false;
		}

		auto spells = Hero->GetSpells();
		if (spells.empty())
		{
			return;
		}


		const float currentTime = Game::Time();
		for (auto& spellInside : spells)
		{
			const int slot = static_cast<int>(spellInside.Slot);

			CooldownSpells[spellInside.Slot].Spell = spellInside;
			CooldownSpells[spellInside.Slot].Cooldown = spellInside.CooldownExpires - currentTime;
			CooldownSpells[spellInside.Slot].IsLearned = spellInside.Learned;

			CooldownSpells[spellInside.Slot].SpellIMG = SpriteImageLoader::GetSmallSummonerSpellIMG(spellInside.ScriptName, CooldownSpells[spellInside.Slot].Cooldown > 0.0f);


		}

		ExpPercent = ChampInfo::ExperiencePercentage(Hero->GetLevel(), Hero->GetExperience()) * 1.05f;

		
	}

	void UpdatePosition()
	{
		HeroHealthPos = Hero->GetHealthBarScreenPos();
	


		Vector2 SpellsPosition = HeroHealthPos;
		SpellsPosition.x += -43.0f + CooldownTrackerSettings.CDBarPosX;
		SpellsPosition.y += -3.0f + CooldownTrackerSettings.CDBarPosY;

		Vector2 SSPosition = HeroHealthPos;
		SSPosition.x += 63.0f + CooldownTrackerSettings.SSPosX;
		SSPosition.y += -24.0f + CooldownTrackerSettings.SSPosY;



		for (int slot = 0; slot <6; ++slot)
		{

			if (slot == 4 || slot == 5)
			{
				if (slot == 5)
				{
					SSPosition.y += 15.0f;
				}


				SpellsStartPosition[slot] = &SSPosition;


				Vector2 timerPos = Vector2(SSPosition.x + 18.0F + CooldownTrackerSettings.SSTimerPosX,
					SSPosition.y + CooldownTrackerSettings.SSTimerPosY);

				SpellsTimerPosition[slot] = &timerPos;
			}
			else
			{
				if (CooldownSpells[slot].IsLearned)
				{
					
					
					auto LengthAdjustedPos = Vector2(SpellsPosition.x + CooldownSpells[slot].Percent * CooldownTrackerSettings.CDBarLength,SpellsPosition.y);

					SpellsStartPosition[slot] = &SpellsPosition;
					SpellsEndPosition[slot] = &LengthAdjustedPos;

					LengthAdjustedPos = Vector2(SpellsPosition.x + 2.0F + CooldownTrackerSettings.CDTimerPosX,
						SpellsPosition.y + 9.0F + CooldownTrackerSettings.CDTimerPosY);

					SpellsTimerPosition[slot] = &LengthAdjustedPos;
				}
				SpellsPosition.x += CooldownTrackerSettings.CDBarGab;
			}
		}

		if (Hero->GetLevel() < 18)
		{

			Vector2 tempPos = HeroHealthPos;
			tempPos.x += -47.0f + CooldownTrackerSettings.ExpbarPosX;
			tempPos.y += -32.0f + CooldownTrackerSettings.ExpbarPosY;

			SpellsStartPosition[6] = &tempPos;
			tempPos.x += ExpPercent;
			SpellsEndPosition[6] = &tempPos;
		}
	}
};


class CooldownTracker 
{
private:

	
public:
	CooldownTracker();
	~CooldownTracker();
	static void Initialized();
	static void TickLoader(CooldownBasicChampList& cdlist);
	static void OnTick(void* userData);
	static void OnDraw(void* userData);
	static void OnDrawMenu(void* userData);
	static void OnProcessSpell(void* ai, PSDK_SPELL_CAST cast, void* userData);
	//void Menu() override;
	static void SettingsUpdate();
	static void ManualSpellListInit();

	static void DrawLoader(const CooldownBasicChampList& cdlist);
};
