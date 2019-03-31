#pragma once
#include "Detector.h"
#include "resource.h"
#include "TeleportDecode.h"

//const enum TeleportTypes { Start, Abort, Finished, Unknown };

struct RecallInfo
{
	AIHeroClient* Hero;
	float Duration;
	float Begin;
	bool Active;
	int index;
	float LastChange;
	RecallType CurrentRecallType;

	RecallInfo(AIHeroClient* _Hero)
		: Hero(_Hero)
	{
		Active = false;
		Begin = 0.0f;
		Duration = 0.0f;
		index = 0;
		LastChange = 0.0f;
		CurrentRecallType = RecallType::Unknown;
	}

};



class Teleport
{
public:

	static void InitLoader();
	static void MenuLoader();
	static void TickLoader();
	static void DrawLoader();


	//static TeleportStuct TeleportDecoderFunction(void* Unit, const char* Name, const char* Type);
	static void __cdecl RecallTrack(void* Unit, const char* Name, const char* Type, void* UserData);
	static const char* GetTeleportRealName(const char* Name);
	static void InPrintChat(const char* champName, const char* TeleportName, const char* recallStatus, float healthPCT, TeleportTypes tpType);
};

