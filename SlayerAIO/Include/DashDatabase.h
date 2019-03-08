#pragma once

#include "Vectors.hpp"
#include <map>
#include <string>

struct GapcloserData {
	bool IsBlink;
	bool IsDash;
	bool IsTargetted;

	bool  CanVaryInLength;
	float MaxRange;
	float Speed;
	float WindUp;     //Time hero stays immobile before dash/blink
	float WindDown;  //Time hero stays immobile before dash/blink


	//Only Instances Have These
	float StartTime;
	float EndTime;

	Vector3 StartPos;
	Vector3 EndPos;
};

GapcloserData MakeBlink(float range, float speed, float windUp, float windDown, bool canVaryInLength = true, bool IsTargetted = false) {
	return { true, false, IsTargetted, canVaryInLength, range, speed, windUp, windDown};
}

GapcloserData MakeDash(float range, float speed, float windUp, float windDown, bool canVaryInLength = true, bool IsTargetted = false) {
	return { true, false, IsTargetted, canVaryInLength, range, speed, windUp, windDown };
}

__declspec(selectany) std::map<std::string, GapcloserData> GapcloserDatabase {
	//Blinks
	{"SummonerFlash", MakeBlink(420.0f, HUGE_VALF, 0.0f, 0.0f)},

	//Linear/Circular Dashes


	//Targetted Dashes


};