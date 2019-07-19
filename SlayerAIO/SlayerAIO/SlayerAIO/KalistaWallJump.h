#pragma once
#include "Kalista.h"


class WallJump
{
public:
	static void WallJumpSpotsInitializer();

	static void WallJumpMode();

	static std::map<Vector3, Vector3> GetWallJumpSpots();
};

