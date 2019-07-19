#pragma once
#include "Trackers.h"



struct DashJumpMenu
{
	const char* ChampName;
	const char* SpellName;
	const char* MenuID;



	DashJumpMenu(const char* _ChampName, const char* _SpellName, const char* _MenuID)
		: ChampName(_ChampName) , SpellName(_SpellName), MenuID(_MenuID)
	{

	}
};


struct DashJumpObject
{
	AIHeroClient* Hero;
	float Range;
	const char* SpellName;
	bool Casted;
	Vector3 EndPos;
	float ExtraTicks;
	Vector3 StartPos;
	float TimeCasted;
	unsigned char Slot;

	DashJumpObject(AIHeroClient* _Hero, PSDK_SPELL _Spell)
		: Hero(_Hero)
	{

		this->Casted = false;

		this->StartPos = Vector3();
		this->EndPos = Vector3();
		this->ExtraTicks = 0.0f;
		this->TimeCasted = 0.0f;
		if (_Spell != NULL )
		{
			const char* ss = _Spell->ScriptName;
			if (ss != nullptr)
			{
				this->SpellName = ss;
				this->Range = _Spell->CastRange;
				this->Slot = _Spell->Slot;
			}
		}	
	}
};


class DashJumpTracker
{
public:
	static void Init();
	static void MenuLoader();
	static void TickLoader();
	static void DrawLoader();

	static void __cdecl SpellCastStart(void* AI, PSDK_SPELL_CAST SpellCast, void* UserData);

	static Vector3 CalculateEndPos(Vector3 start, Vector3 end, float maxRange);
};

