#pragma once
#include "../../Include/SDK Extensions.h"

class Trackers
{
public:
	static void Init();

	static void __cdecl Update(_In_ void* UserData);
	static void	__cdecl	Tick(_In_ void* UserData);
	static void __cdecl DrawMenu(_In_ void* UserData);
	static void __cdecl Draw(_In_ void* UserData);
};

