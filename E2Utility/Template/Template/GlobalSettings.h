#pragma once
#include "../../Include/SDK Extensions.h"
#include "DrawHelper.h"
class GlobalSettings
{
public:
	static void Init();

	static void	__cdecl	Tick(_In_ void* UserData);
	static void __cdecl DrawMenu(_In_ void* UserData);
};

