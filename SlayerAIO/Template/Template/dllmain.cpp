#include "stdafx.h"

#include "../../Include/SDK Extensions.h"
#include "Kalista.h"


PLUGIN_SETUP("SlayerAIO", OnLoad);

void __cdecl OnLoad(void* UserData) 
{
	LOAD_ENVIRONMENT();

	if (Player.PTR() && pSDK && pCore) 
	{
		Kalista::Init();
		//Game::PrintChat(R"(<font color="#832232">Template Loaded.</font>)");
		
	}
}
