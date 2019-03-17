#pragma once

#include "Activator.h"
#include "ItemRetriever.h"

class CustomRanges
{
public:
	static void Init();

	//static void	__cdecl	Tick(_In_ void* UserData);
	//static void	__cdecl	Update(_In_ void* UserData);
	static void	__cdecl	Draw(_In_ void* UserData);
	static void __cdecl SpellCastStart(void* AI, PSDK_SPELL_CAST SpellCast, void* UserData);
};

