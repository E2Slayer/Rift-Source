#include "stdafx.h"

#include "../../Include/SDK Extensions.h"


#include "Activator.h"
//#include "CustomRanges.h"
#include "Detector.h"
#include "Drawings.h"
#include "Trackers.h"
#include "Others.h"
#include "GlobalSettings.h"


PLUGIN_SETUP("E2Utility", OnLoad);

void __cdecl OnLoad(void* UserData)
{
	LOAD_ENVIRONMENT();

	if (Player.PTR() && pSDK && pCore) 
	{
		
		Activator::Init();
		Detector::Init();
		Drawings::Init();
		Trackers::Init();
		Others::Init();
		
		GlobalSettings::Init();
		//CustomRanges::Init();

		//Game::PrintChat(R"(<font color="#832232">test.</font>)");
	}
}