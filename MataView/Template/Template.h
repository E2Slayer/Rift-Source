#pragma once

#include "../../Include/SDK Extensions.h"

class MyTemplateClass {
private:
	static AttackableUnit CurrentTarget;
	static AttackableUnit* OrbTarget;
public:
	static void Init();
	static void	__cdecl	Recall(void* Unit, const char* Name, const char* Type, void* UserData);
	static void	__cdecl	Tick(_In_ void* UserData);
	static void	__cdecl	Update(_In_ void* UserData);
	static void	__cdecl	Draw(_In_ void* UserData);
	static void __cdecl DrawMenu(_In_ void* UserData);
};