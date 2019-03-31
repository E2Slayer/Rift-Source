#pragma once
#include "Detector.h"
#include "resource.h"

const enum TeleportTypes { Start, Abort, Finished, Unknown };

struct RecallInfo
{
	AIHeroClient* Hero;
	float Duration;
	float Begin;
	bool Active;
	int index;
	float LastChange;
	RecallType CurrentRecallType;

};

struct TeleportStuct
{
	float Duration;


	DWORD Start;

	TeleportTypes Status;

	RecallType Type;

	unsigned int UnitNetworkId;
};



class Teleport
{
public:
	//static void Init();

	static void InitLoader();
	static void MenuLoader();
	static void TickLoader();
	static void DrawLoader();


	static TeleportStuct TeleportDecoderFunction(void* Unit, const char* Name, const char* Type);
	static void __cdecl RecallTrack(void* Unit, const char* Name, const char* Type, void* UserData);
	static const char* GetTeleportRealName(const char* Name);
	static void InPrintChat(const char* champName, const char* TeleportName, const char* recallStatus, float healthPCT, TeleportTypes tpType);
};

