#pragma once

#include "../../Include/SDK Extensions.h"

class AbilityTracker
{
private:
	//static AttackableUnit CurrentTarget;
	//static AttackableUnit* OrbTarget;
	static std::map<unsigned int, GameObject*> GameObjects;
public:
	
	static void Init();
	
	static void	__cdecl	Recall(void* Unit, const char* Name, const char* Type, void* UserData);
	static void	__cdecl	Tick(_In_ void* UserData);
	static void	__cdecl	Update(_In_ void* UserData);
	static void	__cdecl	Draw(_In_ void* UserData);
	static void __cdecl DrawMenu(_In_ void* UserData);

	static bool __cdecl OnCreate(_In_ void* Object, _In_ unsigned int NetworkID, _In_opt_ void* UserData);
	static bool __cdecl OnDelete(_In_ void* Object, _In_ unsigned int NetworkID, _In_opt_ void* UserData);
	static void __cdecl OnBuffCreateDelete(void* AI, bool Created, unsigned char Type, float StartTime, float EndTime, const char* Name, void* CasterObject, unsigned int CasterID, int Stacks, bool HasCount, int Count, PSDK_SPELL Spell, void* UserData);
};

static void AblitiesAdder();

