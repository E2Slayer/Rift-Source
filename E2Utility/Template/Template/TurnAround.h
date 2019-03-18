#pragma once
#include "Detector.h"

class TurnAround
{
public:
	static void Init();

	static void MenuLoader();
	static void TickLoader();
	static void __cdecl SpellCastStart(void* AI, PSDK_SPELL_CAST SpellCast, void* UserData);

};

