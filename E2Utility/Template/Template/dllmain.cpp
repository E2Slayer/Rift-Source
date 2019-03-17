#include "stdafx.h"

#include "../../Include/SDK Extensions.h"
#include "Template.h"
#include "Summoners.h"
#include "Consumables.h"
#include "Offensives.h"
#include "Defensives.h"
#include "CustomRanges.h"
#include "ItemRetriever.h"

PLUGIN_SETUP("E2Utility", OnLoad);

void __cdecl OnLoad(void* UserData)
{
	LOAD_ENVIRONMENT();

	if (Player.PTR() && pSDK && pCore) 
	{
		//MyTemplateClass::Init();

		Summoners::Init();
		Consumables::Init();
	//	Offensives::Init();
	//	Defensives::Init();
		ItemRetriever::Init();
		CustomRanges::Init();
		//Game::PrintChat(R"(<font color="#832232">test.</font>)");
	}
}