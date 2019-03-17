#include "stdafx.h"

#include "../../Include/SDK Extensions.h"
#include "Activator.h"
#include "CustomRanges.h"



PLUGIN_SETUP("E2Utility", OnLoad);

void __cdecl OnLoad(void* UserData)
{
	LOAD_ENVIRONMENT();

	if (Player.PTR() && pSDK && pCore) 
	{
		//MyTemplateClass::Init();
		Activator::Init();
		
		CustomRanges::Init();
		//Game::PrintChat(R"(<font color="#832232">test.</font>)");
	}
}