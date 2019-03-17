#pragma once
#include "Activator.h"

class Summoners
{
private:
	/*
	static bool bSummonersExpanded;
	static bool bSmiteExpanded;
	static bool bIgniteExpanded;
	static bool bIgniteExpanded2;
	static bool bExhaustExpanded;
	static bool bExhaustExpanded2;
	static bool bHealExpanded;
	static bool bHealAllyExpanded;
	static bool bBarrierExpanded;
	*/
public:
	static void Init();

	static void	__cdecl	Tick(_In_ void* UserData);
	//static void	__cdecl	Update(_In_ void* UserData);
	static void	__cdecl	Draw(_In_ void* UserData);
	static void __cdecl DrawMenu(_In_ void* UserData);
	static void MenuLoader();
	static void TickLoader();
	static void __cdecl IgniteCheck();
	static void __cdecl ExhaustCheck();
	static void __cdecl HealCheck();
	static void __cdecl BarrierCheck();

};

