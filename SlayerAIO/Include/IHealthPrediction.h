#pragma once

#include "GameObjects.hpp"

#pragma warning(push, 0)
#include <vector>
#pragma warning(pop)

class IHealthPred {
public:
	virtual unsigned int CalculateMissileTravelTime(AIBaseClient* Target, Vector3* StartPos, float MissileDelay, float MissileSpeed) = 0;
	virtual float GetHealthPrediction(AIBaseClient* Target, unsigned int Time, bool SimulateDmg = false) = 0;
	virtual float GetDamagePrediction(AIBaseClient* Target, unsigned int Time, bool SimulateDmg = false) = 0;
	virtual bool  CanKillMinion(AIBaseClient* Minion, float missileDelay, float missileSpeed) = 0;
};