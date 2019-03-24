#pragma once

enum class DamageType {
	Physical,
	Magical,
	Mixed,
	True
};

enum class SkillStage {
	Default,
	Wayback,
	Detonation,
	DamagePerTime,
	DamagePerSecond,
	DamagePerHalfSecond,
	DamagePerQuarterSecond,	
	EmpoweredDamagePerSecond,
	EmpoweredDamagePerHalfSecond,
	EmpoweredDamagePerQuarterSecond,
	SecondForm,	
	ThirdForm,
	SecondCast,
	ThirdCast,
	AgainstMinions,
	AgainstMonsters,
	AgainstTurrets,
	Buff,
	AOE,
	Collision,
	Empowered,
	SecondFormEmpowered,
	Effect,
	ReducedDamage
};

struct StaticAttackDamage {
	float RawTotal;
	float RawPhysical;
	float RawMagical;
	float RawTrue;
	float CalculatedPhysical;
	float CalculatedMagical;
	float CalculatedTrue;	

	bool MagicalDmg;
	bool PhysicalDmg;
	bool TrueDmg;
};

class IDamageLib {
public:
	virtual float CalculateMagicalDamage (AIBaseClient * Source, AIBaseClient * Target, float Amount) = 0;
	virtual float CalculatePhysicalDamage(AIBaseClient * Source, AIBaseClient * Target, float Amount) = 0;

	virtual float GetSpellDamage(AIHeroClient* Source, AIBaseClient* Target, unsigned char Slot, SkillStage Stage) = 0;
	virtual StaticAttackDamage GetStaticAutoAttackDamage(AIHeroClient* Source, bool MinionTarget) = 0;

	virtual float GetAutoAttackDamage(AIBaseClient* Source, AIBaseClient* Target, StaticAttackDamage* StaticDmg = NULL, bool CheckPassives = false) = 0;
	virtual float GetAutoAttackDamage(AIBaseClient* Source, AIBaseClient* Target, bool CheckPassives = false) = 0;
};
