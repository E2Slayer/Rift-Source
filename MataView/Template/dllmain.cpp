#include "stdafx.h"

#include "../../Include/SDK Extensions.h"
#include "Template.h"
#include "AbilityTracker.h"
#include "JungleTimer.h"


PLUGIN_SETUP("MataView", OnLoad);

void __cdecl OnLoad(void* UserData)
{
	LOAD_ENVIRONMENT();

	if (Player.PTR() && pSDK && pCore) 
	{
		//MyTemplateClass::Init();
		//JungleTimer::Init();
		AbilityTracker::Init();
		Game::PrintChat(R"(MataView 1.1 Beta Loaded.)");
	}
}