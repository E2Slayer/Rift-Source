#pragma once

#include "Kalista.h"

class Damage
{
public:
	static float GetRendDamage(AIBaseClient* target, int StackCount);

	static float RendDamageToHealth(AIBaseClient* target, bool rawDamage);

	static int NumberOfRendBuff(AIBaseClient* target);

	static bool IsRendKillable(AIBaseClient* target);


	static float GetPierceDamage(AIBaseClient* target); //Q
};

