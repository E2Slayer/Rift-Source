#pragma once

#include "GameObjects.hpp"

enum class TargetSelectorModes {
	Auto,
	LessCast,
	LessAttack,
	Priority,
	LowestHealth,
	LowestMaxHealth,
	Closest,
	ClosestToMouse,
	HighestAD,
	HighestAP
};

class ITargetSelector {
public:
	virtual void Override(std::shared_ptr<ITargetSelector> Instance) = 0;

	virtual void  ForceTarget(AIHeroClient* Enemy) = 0;
	virtual int   GetPriority(AIHeroClient* Unit) = 0;

	AIHeroClient* ForcedTarget;	
	virtual bool IsValidTarget(AttackableUnit * Target, float Range = 0.0f) = 0;	

	virtual AIHeroClient* GetTarget(std::vector<AIHeroClient*>& PossibleTargets, DamageType dmgType = DamageType::Mixed) = 0;
	virtual AIHeroClient* GetTarget(float maxRange = HUGE_VALF, DamageType dmgType = DamageType::Mixed) = 0;	
};