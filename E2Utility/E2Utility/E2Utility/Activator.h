#pragma once

#include "../../Include/SDK Extensions.h"
#include "Summoners.h"
#include "Consumables.h"
#include "Offensives.h"
#include "Defensives.h"
#include "Cleansers.h"
#include "ItemStruct.h"
#include "ItemRetriever.h"


extern DWORD LastTimeTickCount;

class Activator
{
public:
	static void Init();

	static void	__cdecl	Tick(_In_ void* UserData);
	static void	__cdecl	Update(_In_ void* UserData);
	static void __cdecl DrawMenu(_In_ void* UserData);

};

