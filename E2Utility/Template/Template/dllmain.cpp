#include "stdafx.h"

#include "../../Include/SDK Extensions.h"
#include "Activator.h"
#include "CustomRanges.h"
#include "Detector.h"
#include "Drawings.h"

#include "Others.h"


PLUGIN_SETUP("E2Utility", OnLoad);

void __cdecl OnLoad(void* UserData)
{
	LOAD_ENVIRONMENT();

	if (Player.PTR() && pSDK && pCore) 
	{
		//MyTemplateClass::Init();
		Activator::Init();
		Drawings::Init();
		CustomRanges::Init();
		Others::Init();
		Detector::Init();
		//Game::PrintChat(R"(<font color="#832232">test.</font>)");
	}
}