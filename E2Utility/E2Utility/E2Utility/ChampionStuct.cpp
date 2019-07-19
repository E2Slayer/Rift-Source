#include "stdafx.h"
#include "ChampionStuct.h"


AIHeroClient* Hero;
bool IsAlly;
bool Visible;
float DistanceToPlayer;
float LastEnter;
float LineStart;

ChampionStuct::ChampionStuct()
{


}

ChampionStuct::ChampionStuct(AIHeroClient* HeroInput)
{
	if (HeroInput == NULL || HeroInput == nullptr)
	{
		return;
	}

	if (!HeroInput->IsValid())
	{
		return;
	}

	Hero = HeroInput;
}


ChampionStuct::~ChampionStuct()
{
}


/*
void ChampionStuct::EnterInRange()
{
	

	bool enableChecker = false;


	if (GetTickCount() - LastEnter > 5000) // need to add cooldown
	{
		this->LineStart = GetTickCount();
	}
	LastEnter = GetTickCount();

}*/