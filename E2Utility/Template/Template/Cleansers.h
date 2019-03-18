#pragma once
#include "Activator.h"

extern DWORD LastTimeTickCountClean;

class Cleansers
{
public:
	static void Init();

	//static void	__cdecl	Tick(_In_ void* UserData);
	//static void	__cdecl	Update(_In_ void* UserData);
	//static void	__cdecl	Draw(_In_ void* UserData);
	//static void __cdecl DrawMenu(_In_ void* UserData);
	static void MenuLoader();
	static void TickLoader(ItemStruct currentItem);
	static void CleanseCheck();
	static void QSSCheck(int targetID, SpellSlot itemSlot);
	static void MikaelCheck(int targetID, SpellSlot itemSlot, float spellRange);

};

