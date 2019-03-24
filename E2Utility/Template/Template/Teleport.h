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


	static void __cdecl Update(_In_ void* UserData);
	static void	__cdecl	Tick(_In_ void* UserData);
	static void __cdecl DrawMenu(_In_ void* UserData);
	static void __cdecl Draw(_In_ void* UserData);

	static TeleportStuct TeleportDecoder(void* Unit, const char* Name, const char* Type);
	static void __cdecl Recall(void* Unit, const char* Name, const char* Type, void* UserData);
	static const char* GetTeleportName(const char* Name);
	static void PrintChat(const char* champName, const char* TeleportName, const char* recallStatus, float healthPCT, TeleportTypes tpType);
};

