#pragma once

#include "../../Include/SDK Extensions.h"

class Kalista {
private:
	//static AttackableUnit CurrentTarget;
	//static AttackableUnit* OrbTarget;
public:
	static void Init();

	static void	__cdecl	Tick(_In_ void* UserData);
	static void	__cdecl	Update(_In_ void* UserData);
	static void	__cdecl	Draw(_In_ void* UserData);
	static void __cdecl DrawMenu(_In_ void* UserData);
	static void __cdecl SpellCastStart(void* AI, PSDK_SPELL_CAST SpellCast, void* UserData);
	//static void __cdecl SpellCastEnd(void* AI, PSDK_SPELL_CAST SpellCast, void* UserData);
	static void __cdecl Attack(void* AI, void* TargetObject, bool StartAttack, bool StopAttack, void* UserData);


	static void __cdecl UnkillableMinion(AIMinionClient* Target);

	//static void __cdecl Dash(AIHeroClient* Source, PSDKVECTOR StartPos, PSDKVECTOR EndPos, unsigned int StartTick, unsigned int Duration, float Speed);

	static void Combo();
	static void Harass();
	static void LaneClear();
	static void JungleClear();
	static void Chase();
	//void Exploit(std::map<unsigned, AIBaseClient*> current);

	static void SoulBoundSaver();
	static void KillSteal();
	static void AlwaysJungleE();


	static void ExploitExecute(const OrbwalkingMode mode);
	static void Exploit(AIBaseClient* targetStore);
	
};


