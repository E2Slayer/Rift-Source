#pragma once
#include "Detector.h"



struct MinionObject
{
	AIMinionClient* Minion;
	unsigned int netID;

};

struct ChampionEXPObject
{
	AIHeroClient* Hero;
	bool Enabled;
	float Distance;
	float LastTrigger;
	float LastExp;
	float LastExpTime;

	float CurrentExp;
	float CurrentExpTime;

	unsigned int netID;
	int NearByHeroes;
	int UnsureNearByHeroes;

	bool PossibleInvalidNumber;
	float SiegeExp;

	float RangedExp;

	float MeleeExp;

	//float CurrentExp;
	bool isAlly;
	bool isJungler;
};


class SharedExperience
{
public:

	static void InitLoader();
	

	static void MenuLoader();
	static void TickLoader();
	static void DrawLoader();
	static bool AreSame(float a, float b, float tolerance);
	static void	__cdecl	Draw(_In_ void* UserData);
	

	static void testing();
};

