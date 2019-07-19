#pragma once

#include "../../Include/SDK Extensions.h"

class MyTemplateClass {
private:
	static AttackableUnit CurrentTarget;
	static AttackableUnit* OrbTarget;
public:
	static void Init();

	static void	__cdecl	Tick(_In_ void* UserData);
	static void	__cdecl	Update(_In_ void* UserData);
	static void	__cdecl	Draw(_In_ void* UserData);
	static void __cdecl DrawMenu(_In_ void* UserData);
	static void __cdecl OnCast(void* AI, PSDK_SPELL_CAST SpellCast, void* UserData);
	static void __cdecl OnCastEnd(void* AI, PSDK_SPELL_CAST SpellCast, void* UserData);
	static void __cdecl OnAttack(void* AI, void* TargetObject, bool StartAttack, bool StopAttack, void* UserData);
	static void __cdecl PostAttack(AttackableUnit* Target);
	static void CancelAnimation();
};


