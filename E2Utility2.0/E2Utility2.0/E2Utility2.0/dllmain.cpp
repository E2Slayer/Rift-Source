#include "stdafx.h"

#include "../../Include/SDK Extensions.h"
#include "CooldownTracker.h"
//#include "Loader.h"

PLUGIN_SETUP("E2Utility2.0", OnLoad);

void __cdecl OnLoad(void* UserData)
{
	LOAD_ENVIRONMENT();

	if (Player.PTR() && pSDK && pCore) 
	{
		//MyTemplateClass::Init();

		//Loader::OnInit();

		CooldownTracker::Initialized();
		//Game::PrintChat(R"(<font color="#832232">Template Loaded.</font>)");
	}
}