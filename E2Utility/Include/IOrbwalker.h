#pragma once

#include "GameObjects.hpp"

enum class OrbwalkingMode {
	None		= (0 << 0),
	Combo		= (1 << 0),
	Mixed		= (1 << 1),
	LaneClear	= (1 << 2),
	JungleClear = (1 << 3),
	LastHit		= (1 << 4),
	Freeze		= (1 << 5),
	Flee		= (1 << 6),	
	Custom		= (1 << 7)
};

class IOrbwalker {
public:
	virtual void Override(std::shared_ptr<IOrbwalker> Instance) = 0;
		
	virtual unsigned int GetLastAutoAttack() = 0;
	virtual unsigned int GetLastMovement() = 0;	

	virtual bool IsModeActive(OrbwalkingMode Mode) = 0;
	virtual void OrbwalkTo(Vector3& Position, OrbwalkingMode SimulatedBehaviour = OrbwalkingMode::Combo) = 0;
	virtual void OrbwalkTo(Vector2& Position, OrbwalkingMode SimulatedBehaviour = OrbwalkingMode::Combo) = 0;
		
	virtual bool CanMove() = 0;
	virtual bool CanAttack() = 0;
	virtual bool CanIssueOrder() = 0;
	virtual bool IsOnWindUp() = 0;
	virtual bool IsNextAttackReady() = 0;

	virtual void ResetAttackTimer() = 0;
	virtual void ForceTarget(AIHeroClient* Enemy) = 0;
	virtual void DisableMovement(bool bCantMove) = 0;
	virtual void DisableAttack  (bool bCantAttack) = 0;		

	virtual bool  CheckValidTarget(AttackableUnit* Target, float Range = 0.0f) = 0;
	virtual bool  CheckValidAutoRange(AttackableUnit* Target) = 0;
	virtual float GetTrueAutoAttackRange(AIHeroClient* Source, AttackableUnit* Target = NULL) = 0;
	virtual AttackableUnit* GetTarget() = 0;	
};