#pragma once
#include "Trackers.h"


const enum AbilityType {OnCreate, OnBuffCreate};

const enum AbilityLocationType { Static, Dynamic };

const enum DeleteCondition {EarlyDelete, OnTimeDelete};

struct AbilityItem
{
	const char* Name;
	const char* ChampName;
	float Time;
	const char* MenuID;
	AbilityType Type;
	AbilityLocationType LocationType;
	DeleteCondition DeleteEarly;

	AbilityItem(const char* _Name, const char* _ChampName, float _Time, const char* _MenuID, AbilityType _Type, AbilityLocationType _LocationType, DeleteCondition _DeleteEarly)
		: Name(_Name), ChampName(_ChampName), Time(_Time), MenuID(_MenuID), Type(_Type), LocationType(_LocationType), DeleteEarly(_DeleteEarly)
	{

	}
};


struct AbilityDraw
{
	Vector3 Position;
	GameObject* Object;
	float End;
	bool IsDynamic;
	bool DeleteEarly;
	const char* TargetName;

	AbilityDraw(Vector3 _Position, GameObject* _Object, float _End, bool _IsDynamic, bool _DeleteEarly, const char* _TargetName) : 
		Position(_Position), Object(_Object), End(_End) , IsDynamic(_IsDynamic), DeleteEarly(_DeleteEarly), TargetName(_TargetName)
	{

	}
};





class AbilityTimer
{
public:
	static void InitLoader();
	static void MenuLoader();
	static void TickLoader();
	static void DrawLoader();
	static bool __cdecl OnCreate(void* Object, unsigned int NetworkID, void* UserData);
	static bool __cdecl OnDelete(void* Object, unsigned int NetworkID, void* UserData);
	static void __cdecl OnBuffCreateAndDelete(void* AI, bool Created, unsigned char Type, float StartTime, float EndTime, const char* Name, void* CasterObject, unsigned int CasterID, int Stacks, bool HasCount, int Count, PSDK_SPELL Spell, void* UserData);
};

