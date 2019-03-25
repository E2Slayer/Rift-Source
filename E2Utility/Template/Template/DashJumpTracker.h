#pragma once
#include "Trackers.h"




/*
 private class DestinationObject
		{
			public DestinationObject(Obj_AI_Hero hero, SpellDataInst spell)
			{
				Hero = hero;
				if (spell != null && spell.Slot != SpellSlot.Unknown)
				{
					SpellName = spell.SData.Name;
					Range = spell.SData.CastRange;
				}
			}

			public Obj_AI_Hero Hero { get; private set; }
			public float Range { get; private set; }
			public string SpellName { get; private set; }
			public bool Casted { get; set; }
			public Vector3 EndPos { get; set; }
			public int ExtraTicks { get; set; }
			public Vector3 StartPos { get; set; }
			public int TimeCasted { get; set; }
		}
*/


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

	DashJumpObject(AIHeroClient* _Hero, PSDK_SPELL _Spell)
		: Hero(_Hero)
	{
		if (_Spell != NULL )
		{
			const char* ss = _Spell->ScriptName;
			if (ss != nullptr)
			{
				this->SpellName = ss;
				this->Range = _Spell->CastRange;

				this->Casted = false;

				this->StartPos = Vector3();
				this->EndPos = Vector3();
				this->ExtraTicks = 0.0f;
				this->TimeCasted = 0.0f;
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

