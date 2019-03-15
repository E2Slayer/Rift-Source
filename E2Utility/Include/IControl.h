#pragma once

#include "Vectors.hpp"

class AttackableUnit;

class IControl {
public:
	virtual bool Move() = 0;
	virtual bool Move(Vector3* Pos, bool Pet = false) = 0;

	virtual bool Attack() = 0;
	virtual bool Attack(AttackableUnit* Target, bool Pet = false) = 0;

	virtual bool CastSpell(unsigned char Slot, bool Release = false) = 0;
	virtual bool CastSpell(unsigned char Slot, AttackableUnit * Target, bool Release = false) = 0;
	virtual bool CastSpell(unsigned char Slot, Vector3* Pos, bool Release = false) = 0;
	virtual bool CastSpell(unsigned char Slot, Vector3* startPos, Vector3* endPos, bool Release = false) = 0;

	virtual bool MoveMouse(Vector3 * Pos) = 0;
};