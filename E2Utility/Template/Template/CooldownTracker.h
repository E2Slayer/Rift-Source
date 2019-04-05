#pragma once
#include "Trackers.h"
#include "resource.h"
#include "ChampionNames.h"

/*
internal class ManualSpell
	{
		public ManualSpell(string champ, string spell, SpellSlot slot, float[] cooldowns, float additional = 0)
		{
			Champ = champ;
			Spell = spell;
			Slot = slot;
			Cooldowns = cooldowns;
			Additional = additional;
		}

		public string Champ { get; private set; }
		public string Spell { get; private set; }
		public SpellSlot Slot { get; private set; }
		public float[] Cooldowns { get; set; }
		public float Additional { get; set; }
		public float Cooldown { get; set; }
		public float CooldownExpires { get; set; }
	}
*/

struct ManualSpell
{
	const char* Champ;
	const char* SpellName;
	SpellSlot Slot;
	float Cooldowns[5];
	float Additional;
	float Cooldown;
	float CooldownExpires;
	bool IsAlly;

	ManualSpell(const char* _Champ, const char* _SpellName, SpellSlot _Slot, float(&_Coooldowns)[5], float _Additional, bool _IsAlly)
		: Champ(_Champ), SpellName(_SpellName), Slot(_Slot), Additional(_Additional), IsAlly(_IsAlly)
	{

		for (int i = 0; i < 5; ++i) 
		{
			Cooldowns[i] = _Coooldowns[i];
		}

		Cooldown = 0.0f;
		CooldownExpires = 0.0f;
	}
};


struct CooldownMenu
{
	bool CDMySelf;
	bool CDAlly;
	bool CDEnemy;


	bool SpellEnable;
	int SpellFormat = 0;
	int SpellFont1 = 0;
	int SpellFont2 = 0;
	int SpellPosX = 0;
	int SpellPosY = 0;


	//std::pair <int, int> SpellFont;
//	std::pair <int, int> SpellPos;
	bool SummmonerSpellEnable;
	int SummmonerSpellFormat = 0;
//	std::pair <int, int> SummmonerSpellFont;
//	std::pair <int, int> SummmonerSpellPos;

	int SummmonerSpellFont1 = 0;
	int SummmonerSpellFont2 = 0;
	int SummmonerSpellPosX = 0;
	int SummmonerSpellPosY = 0;

};


struct CooldownPositionList
{
	Vector2 MainStructPos;

	Vector2 HUDPos;
	Vector2 SpellTimerPos;
	Vector2 SS1Pos;
	Vector2 SS1TimerPos;
	Vector2 SS2Pos;
	Vector2 SS2TimerPos;

	CooldownPositionList(Vector2 _MainStructPos)
		: MainStructPos(_MainStructPos)
	{
		MainStructPos = Vector3(0.0f, 0.0f, 0.0f);
		HUDPos = Vector3(0.0f, 0.0f, 0.0f);
		SpellTimerPos = Vector3(0.0f, 0.0f, 0.0f);
		SS1Pos = Vector3(0.0f, 0.0f, 0.0f);
		SS2Pos = Vector3(0.0f, 0.0f, 0.0f);
		SS1TimerPos = Vector3(0.0f, 0.0f, 0.0f);
		SS2TimerPos = Vector3(0.0f, 0.0f, 0.0f);
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
		Percent = _Cooldown > 0 && std::abs(_Spell.TotalCooldown) > FLT_EPSILON
			? 1.0f - _Cooldown / _Spell.TotalCooldown
			: 1.0f;

	}


	void UpdateInfoManual(float newTime, float totalCD)
	{

		this->Cooldown = Spell.CooldownExpires - Game::Time();
		if (!this->IsLearned)
		{
			this->IsLearned = Spell.Learned;
		}

		Percent = newTime > 0 && std::abs(totalCD) > FLT_EPSILON
			? 1.0f - newTime / totalCD
			: 1.0f;
	}


};

struct CooldownChamp
{
	AIHeroClient* Hero;
	unsigned int NetID;
	std::vector< CooldownInisdeStruct> CooldownSpells;

//	CooldownPositionList PositionList = CooldownPositionList(Vector3(0.0f, 0.0f, 0.0f));

	Vector2 HeroHealthPos;
	/*
	SDK_SPELL SpellQ;
	float SpellQCD;
	bool IsQLearned;
	SDK_SPELL SpellW;
	float SpellWCD;
	bool IsWLearned;
	SDK_SPELL SpellE;
	float SpellECD;
	bool IsELearned;
	SDK_SPELL SpellR;
	float SpellRCD;
	bool IsRLearned;
	SDK_SPELL SummonerSpells1;
	float SummonerSpells1CD;
	int SummonerSpells1IMG;
	SDK_SPELL SummonerSpells2;
	float SummonerSpells2CD;
	int SummonerSpells2IMG;
	*/
	CooldownPositionList CDPosList = CooldownPositionList(Vector3(0.0f, 0.0f, 0.0f));

	CooldownChamp(AIHeroClient* _Hero, unsigned int _NetID)
		: Hero(_Hero) , NetID(_NetID) 
	{

		CooldownSpells.clear();
		HeroHealthPos = _Hero->GetHealthBarScreenPos();


		auto spells = _Hero->GetSpells();
		for (auto &spellInside : spells)
		{

			if (int(spellInside.Slot) < 0 || int(spellInside.Slot) > 6)
			{
				continue;
			}
			float time = spellInside.CooldownExpires - Game::Time();

			CooldownSpells.emplace_back(CooldownInisdeStruct(spellInside, time, spellInside.Learned, ChampionNames::GetSummonerSpells(spellInside.ScriptName, true)));

		}



	}

	void UpdateCDChamp()
	{

		CooldownSpells.clear();
		HeroHealthPos = Hero->GetHealthBarScreenPos();


		auto spells = Hero->GetSpells();
		for (auto &spellInside : spells)
		{

			if (int(spellInside.Slot) < 0 || int(spellInside.Slot) > 6)
			{
				continue;
			}
			float time = spellInside.CooldownExpires - Game::Time();

			CooldownSpells.emplace_back(CooldownInisdeStruct(spellInside, time, spellInside.Learned, ChampionNames::GetSummonerSpells(spellInside.ScriptName, true)));

		}
	}


	
	
};


class CooldownTracker
{
public:
	static void Init();
	static void MenuLoader();
	static void TickLoader();
	static void DrawLoader();
	//static void InsideDraw(AIHeroClient* hero, bool isAlly);
	//static void InsideDrawer(AIHeroClient* hero, bool isAlly);
	static void __cdecl SpellCastStart(void* AI, PSDK_SPELL_CAST SpellCast, void* UserData);
	//static unsigned int GetSummonerSpells(const char* name, bool isReady);
};
