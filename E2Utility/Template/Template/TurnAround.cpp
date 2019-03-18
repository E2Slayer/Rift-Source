#include "stdafx.h"
#include "TurnAround.h"




struct TurnAroundSpellInfo
{
	const char* Name;
	const char* Owner;
	float Range;
	bool TurnAroundOpposite;
	float CastTime;

};


DWORD MovementBlocker = 0;

std::vector<TurnAroundSpellInfo> turnAroundInfos;

void TurnAround::Init()
{

	turnAroundInfos.clear();

	auto checkEnemy = pSDK->EntityManager->GetEnemyHeroes();
	
	if (checkEnemy.empty())
	{
		return;
	}

	for (auto const &value : checkEnemy)
	{
		if (strcmp(value.second->GetCharName(), "Cassiopeia") == 0 && Menu::Get<bool>("Detector.TurnAround.TurnAroundCassiopeiaR"))
		{
			TurnAroundSpellInfo Cass;
			Cass.Owner = "Cassiopeia";
			Cass.Name = "CassiopeiaR";
			Cass.Range = 1000.0f;
			Cass.TurnAroundOpposite = true;
			Cass.CastTime = 0.85f;
			turnAroundInfos.emplace_back(Cass);
		}
		else if (strcmp(value.second->GetCharName(), "Tryndamere") == 0 && Menu::Get<bool>("Detector.TurnAround.TryndamereW"))
		{
			TurnAroundSpellInfo Tryn;
			Tryn.Owner = "Tryndamere";
			Tryn.Name = "TryndamereW";
			Tryn.Range = 900.0f;
			Tryn.TurnAroundOpposite = false;
			Tryn.CastTime = 0.65f;
			turnAroundInfos.emplace_back(Tryn);
		}
	}

	if (turnAroundInfos.empty())
	{
		return;
	}

	pSDK->EventHandler->RegisterCallback(CallbackEnum::SpellCastStart, TurnAround::SpellCastStart);
	
}

void TurnAround::MenuLoader()
{


	Menu::Tree("TurnAround", "Detector.TurnAround", false, []()
	{
		Menu::Checkbox("Use TurnAround", "Detector.TurnAround.Use", true);
		Menu::Checkbox("Use TurnAround On Cassiopeia R", "Detector.TurnAroundCassiopeiaR", true);
		Menu::Checkbox("Use TurnAround On Tryndamere W", "Detector.TurnAround.TryndamereW", true);
	});


}

void TurnAround::TickLoader()
{
	if (turnAroundInfos.empty())
	{
		return;
	}

	if (!Menu::Get<bool>("Detector.TurnAround.Use"))
	{
		return;
	}

}



void TurnAround::SpellCastStart(void* AI, PSDK_SPELL_CAST SpellCast, void* UserData)
{
	if (AI == NULL || AI == nullptr || SpellCast == NULL || SpellCast == nullptr)
	{
		return;
	}

	if (!Menu::Get<bool>("Detector.TurnAround.Use"))
	{
		return;
	}

	if (!Menu::Get<bool>("Detector.TurnAround.TryndamereW") && !Menu::Get<bool>("Detector.TurnAround.TurnAroundCassiopeiaR"))
	{
		return;
	}

	if (turnAroundInfos.empty())
	{
		return;
	}

	if (SpellCast->Spell.ScriptName == NULL)
	{
		return;
	}
	
	if (MovementBlocker >= GetTickCount())
	{
		return;
	}

	for (auto const &value : turnAroundInfos)
	{
		if (strcmp(value.Name, SpellCast->Spell.ScriptName) == 0)
		{

			AIHeroClient* caster = (AIHeroClient*) pSDK->EntityManager->GetObjectFromPTR(AI);

			if (caster == NULL || caster == nullptr)
			{
				continue;
			}

			auto orignialPath = Player.GetWaypoints();


			if (orignialPath.empty())
			{
				return;
			}

			PSDKVECTOR finalDestination{ &orignialPath.back() };

			auto firstPos = Player.GetServerPosition();

			auto casterPos = (caster)->GetServerPosition();




			if (Player.GetServerPosition().Distance(casterPos) + Player.GetBoundingRadius() <= value.Range)
			{



				auto secondInput = Player.GetServerPosition().Distance(casterPos) + (value.TurnAroundOpposite ? 100 : -100);

				auto PosAdjust = caster->GetServerPosition().Extended(firstPos, secondInput);
				SdkMoveLocalPlayer(&PosAdjust, false);
				MovementBlocker = GetTickCount() + value.CastTime*1000.0f;
				pCore->Orbwalker->DisableMovement(true);
				PSDKVECTOR tt = &PosAdjust;
				
				int delay = ((value.CastTime + 0.1) * 1000);

				pSDK->EventHandler->DelayedAction([finalDestination]()
				{
					SdkMoveLocalPlayer(finalDestination, false);
					//SdkUiConsoleWrite("Player Tried to Move : %f", Game::Time());
					pCore->Orbwalker->DisableMovement(false);
					
				}, delay);
			}


			/*
			auto mousepos{ Renderer::MousePos() };
			if (mousepos != nullptr && mousepos != NULL)
			{
				SdkMoveLocalPlayer(&mousepos, false);
			}

			SpellCast->StartPosition;
			*/
		}
	}


	//SdkUiConsoleWrite("1Name %s ", SpellCast->Spell.ScriptName);
}

