#pragma once
#include "Trackers.h"



const enum JungleCampState { Alive, Dead, Unknown};


struct JungleMinion
{
	const char* Name;
	bool Dead;

	bool DeadTick;

	JungleMinion(const char* _Name) //, bool _Dead, AIMinionClient* _Unit
		: Name(_Name)//, Dead(_Dead), Unit(_Unit)
	{
		Dead = true;
		DeadTick = false;
	}
};


struct JungleCamp
{
	float SpawnTime;
	float RespawnTimer;
	Vector3 Position;
	std::vector< JungleMinion> Minions;
	JungleCampState State;
	
	float ClearTick;
	float TimeLeft;
	bool MapCheck;
	bool MinimapCheck;

	JungleCamp(float _SpawnTime, float _RespawnTimer, Vector3 _Position, std::vector< JungleMinion> _Minions) //, JungleCampState _State, float _ClearTick
		: SpawnTime(_SpawnTime), RespawnTimer(_RespawnTimer), Position(_Position), Minions(_Minions) // State(_State), ClearTick(_ClearTick
	{
		State = JungleCampState::Unknown;
		ClearTick = 0.0f;
		TimeLeft = 0.0f;
		MapCheck = false;
		MinimapCheck = false;
	}
};




struct JungleCampExploit
{
	float SpawnTime;
	float RespawnTimer;
	Vector3 Position;
	const char* Name;
	bool Created;
	float TimeLeft;
	bool MapCheck;
	bool MinimapCheck;

	JungleCampExploit(float _SpawnTime, float _RespawnTimer, Vector3 _Position, const char* _Name) //, JungleCampState _State, float _ClearTick
		: SpawnTime(_SpawnTime), RespawnTimer(_RespawnTimer), Position(_Position), Name(_Name) // State(_State), ClearTick(_ClearTick
	{
		Created = false;
		TimeLeft = 0.0f;
		MapCheck = false;
		MinimapCheck = false;
	}
};




class JungleTimer
{
public:
	static void Init();

	static void MenuLoader();
	static void TickLoader();
	static void DrawLoader();

	static bool __cdecl OnCreate(void* Object, unsigned int NetworkID, void* UserData);
	static bool __cdecl OnDelete(void* Object, unsigned int NetworkID, void* UserData);
};

