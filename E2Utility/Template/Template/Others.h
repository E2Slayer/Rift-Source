#pragma once
#include "../../Include/SDK Extensions.h"
#include "AntiAFK.h"

class Others
{
public:
	static void Init();

	static void	__cdecl	Tick(_In_ void* UserData);
	static void __cdecl DrawMenu(_In_ void* UserData);
};

