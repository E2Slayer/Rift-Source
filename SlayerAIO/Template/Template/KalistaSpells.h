#pragma once

#include "Kalista.h"

class Spells
{

public:
	//static Spell::Skillshot spellQ;
	//static Spell::Active spellW;
	//static Spell::Active spellE;
	//static Spell::Active spellR;

	static Spell::SpellBase GetSpell(SpellSlot slot);
	static void SpellQUse(const OrbwalkingMode mode);
	static void SpellWUse(const OrbwalkingMode mode);
	static void SpellEUse(const OrbwalkingMode mode);
	static void SpellEUseSub(const OrbwalkingMode mode);
	static void SpellRUse(const OrbwalkingMode mode);

	static bool SpellHumanizer(int TargetSpell);

	static bool SpellEManaSaver();
	static HitChance GetHitChanceFromDropList(const OrbwalkingMode mode);
	static const char* GetOrbWalkerNameToText(const char* champName, const OrbwalkingMode mode);
};

