#pragma once
#include "../../Include/SDK Extensions.h"
#include "GankAlerter.h"
#include "CustomRange.h"
#include "TurretsRange.h"

class Drawings
{
public:
	static void Init();

	static void	__cdecl	Tick(_In_ void* UserData);
	static void __cdecl DrawMenu(_In_ void* UserData);
	static void __cdecl Draw(_In_ void* UserData);
};

