#pragma once
#include "Trackers.h"
#include "TeleportDecode.h"
#include "ChampionNames.h"
#include <string.h>

/*
internal class LastPositionStruct
{
	public LastPositionStruct(Obj_AI_Hero hero)
	{
		Hero = hero;
		LastPosition = Vector3.Zero;
	}

	public Obj_AI_Hero Hero{ get; private set; }
	public bool IsTeleporting{ get; set; }
	public float LastSeen{ get; set; }
	public Vector3 LastPosition{ get; set; }
	public bool Teleported{ get; set; }
}
*/

struct LastPositionStruct
{
	//TeleportTypes _teleportStatus;
	AIHeroClient* Unit;
	bool isTeleporting;
	float LastSeen;
	Vector3 LastPosition;
	bool Teleported;
	int ChampIMG;
	float TeleportStartTime;
	

	//bool Aborted;
	float AbortedTimeDifference;

	float Radius;
	Vector3 DrawPosition;
	bool DrawCheck;

	LastPositionStruct(AIHeroClient* _Unit, Vector3 _LastPosition)
		: Unit(_Unit), LastPosition(_LastPosition)
	{


	
		isTeleporting = false;
		LastSeen = 0.0f;
		Teleported = false;
		//Aborted = false;
		AbortedTimeDifference = 0.0f;
		ChampIMG = ChampionNames::GetLastPostionChampionIMG(_Unit->GetCharName(), 308);
		TeleportStartTime = 0.0f;
		Radius = 0.0f;
		DrawPosition = Vector3(0.0f, 0.0f, 0.0f);
		DrawCheck = false;
	}
};

class LastPosition
{
public:
	static void Init();

	static void MenuLoader();
	static void TickLoader();
	static void DrawLoader();


	static void __cdecl RecallTrack(void* Unit, const char* Name, const char* Type, void* UserData);
	//static void DrawCircleMinimap(Vector3 center, float radius, PSDKCOLOR color, float thickness, int quality);
	//static void DrawCircleMap(Vector3 center, float radius, PSDKCOLOR color, float thickness, int quality);
};

