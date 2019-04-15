#include "stdafx.h"
#include "Parent.h"
#include "Loader.h"


Parent::Parent()
{//
	SdkUiConsoleWrite("Parent Created");
}

Parent::~Parent()
{
	SdkUiConsoleWrite("Parent Destroyed");
}



/*
std::function<void()> Parent::ParentMenu()
{

}
*/


static void _cdecl Update(_In_ void* UserData)
{
	std::vector<std::shared_ptr<CallBacks>> vector = Loader::GetVector();
	for (std::shared_ptr<CallBacks> & value : vector)
	{
		if(value)
			value->OnUpdate(UserData);
	}
}

static void _cdecl Tick(_In_ void* UserData)
{
	std::vector<std::shared_ptr<CallBacks>> vector = Loader::GetVector();
	for (std::shared_ptr<CallBacks> & value : vector)
	{
		if (value)
			value->OnTick(UserData);
	}
}

static void _cdecl Draw(_In_ void* UserData)
{
	std::vector<std::shared_ptr<CallBacks>> vector = Loader::GetVector();
	for (std::shared_ptr<CallBacks> & value : vector)
	{
		if (value)
			value->OnDraw(UserData);
	}
}


static void _cdecl DrawMenu(_In_ void* UserData)
{
	std::vector<std::shared_ptr<CallBacks>> vector = Loader::GetVector();
	for (std::shared_ptr<CallBacks> & value : vector)
	{
		if (value)
		{
			value->OnDrawMenu(UserData);
		}
	}


}



static void _cdecl RecallCallback(void* hero, const char* name, const char* type, void* userData)
{
	std::vector<std::shared_ptr<CallBacks>> vector = Loader::GetVector();
	for (std::shared_ptr<CallBacks> & value : vector)
	{
		if (value)
			value->OnRecall(hero, name, type, userData);
	}
}


static void _cdecl GainVisionCallback(AIHeroClient* hero)
{
	std::vector<std::shared_ptr<CallBacks>> vector = Loader::GetVector();
	for (std::shared_ptr<CallBacks> & value : vector)
	{
		if (value) 
			value->OnHeroEnterView(hero);
	}
}

static void _cdecl LoseVisionCallback(AIHeroClient* hero)
{
	std::vector<std::shared_ptr<CallBacks>> vector = Loader::GetVector();
	for (std::shared_ptr<CallBacks> & value : vector)
	{
		if (value)
			value->OnHeroExitView(hero);
	}
}

static void _cdecl CreateObjectCallback(void* object, unsigned int netID, void* userData)
{
	std::vector<std::shared_ptr<CallBacks>> vector = Loader::GetVector();
	for (std::shared_ptr<CallBacks> & value : vector)
	{
		if (value)
		{
			auto sender = pSDK->EntityManager->GetObjectFromPTR(object);
			if (object == nullptr || sender == nullptr || netID == 0)
			{
				return;
			}

			const auto objectName = sender->GetName();
			auto objectPos = sender->GetPosition();
			if (objectName == nullptr || !objectPos.IsValid())
			{
				return;
			}

			if (strstr(objectName, "SRU_") || strstr(objectName, "Minion_") || strstr(objectName, "Turret"))
			{
				return;
			}

			value->OnObjectCreated(object, netID, userData);
		}
	}
}

static void _cdecl DeleteObjectCallback(void* Object, unsigned int NetworkID, void* userData)
{
	std::vector<std::shared_ptr<CallBacks>> vector = Loader::GetVector();
	for (std::shared_ptr<CallBacks> & value : vector)
	{
		if (value)
		{
			auto sender = pSDK->EntityManager->GetObjectFromPTR(Object);
			if (Object == nullptr || sender == nullptr || NetworkID == 0)
			{
				return;
			}

			const auto objectName = sender->GetName();
			if (objectName == nullptr || !sender->GetPosition().IsValid())
			{
				return;
			}

			if (strstr(objectName, "SRU_") || strstr(objectName, "Minion_") || strstr(objectName, "Turret"))
			{
				return;
			}

			value->OnObjectRemoved(Object, NetworkID, userData);
		}
	}
}

static void _cdecl EndSpellCallback(void* ai, PSDK_SPELL_CAST cast, void* userData)
{
	std::vector<std::shared_ptr<CallBacks>> vector = Loader::GetVector();
	for (std::shared_ptr<CallBacks> & value : vector)
	{
		if (value)
			value->OnCastSpell(ai, cast, userData);
	}
}

static void _cdecl StartCastCallback(void * AI, PSDK_SPELL_CAST SpellCast, void * UserData)
{
	std::vector<std::shared_ptr<CallBacks>> vector = Loader::GetVector();
	for (std::shared_ptr<CallBacks> & value : vector)
	{
		if (value)
		{
			auto sender = pSDK->EntityManager->GetObjectFromPTR(AI);
			if (AI == nullptr || sender == nullptr || sender == NULL || !sender->IsHero() || !SpellCast->Spell.ScriptName)
			{
				return;
			}

			value->OnProcessSpell(AI, SpellCast, UserData);
		}
	};
}

static void _cdecl PreCastCallback(bool* process, PSDK_SPELL spell, Vector3* position, AttackableUnit** target)
{
	std::vector<std::shared_ptr<CallBacks>> vector = Loader::GetVector();
	for (std::shared_ptr<CallBacks> & value : vector)
	{
		if (value)
			value->OnPreCast(process, spell, position, target);
	}
}

static void _cdecl AttackCallback(void* ai, void* target, bool start, bool stop, void* userData)
{
	std::vector<std::shared_ptr<CallBacks>> vector = Loader::GetVector();
	for (std::shared_ptr<CallBacks> & value : vector)
	{
		if (value)
			value->OnAttack(ai, target, start, stop, userData);
	}


}


void Parent::RegisterCallback()
{
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, Update);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick, Tick);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, Draw);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, DrawMenu);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::SpellCastStart, StartCastCallback);

	pSDK->EventHandler->RegisterCallback(CallbackEnum::Recall, RecallCallback);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::CreateObject, CreateObjectCallback);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::DeleteObject, DeleteObjectCallback);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::GainVision, GainVisionCallback);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::LoseVision, LoseVisionCallback);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::PreCast, PreCastCallback);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::SpellCastStart, StartCastCallback);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::SpellCastEnd, EndSpellCallback);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Attack, AttackCallback);
}


/*
void Parent::OnDrawMenu(_In_ void * userData)
{

	Menu();
	//SdkUiConsoleWrite("Are you hrer");
	
	Menu::Tree(category_name.c_str(), category_name + ".Menu", false, [this]
	{


		Menu();


		Menu::Tree((category_name + " Configs").c_str(), category_name + ".Config", false, [this]()
		{
			//SdkUiCheckbox(("Enable " + category_name).c_str(), &Enabled, NULL);

			Menu::Checkbox(("Enable " + category_name).c_str(), category_name + ".Config.Enable", true);
		});
	});

	
}

*/

void Parent::Initialized()
{
	SdkUiConsoleWrite("how about here");
	
}

/*
void Parent::DrawMenu(_In_ void* UserData)
{
	//Loader::OnInit();
	//std::vector<std::shared_ptr<CallBacks>> vector = Loader::GetVector();

	//SdkUiConsoleWrite("Are you hrer2");
	Menu::Tree("test", "test.Menu", false, [ UserData]
	{
		std::vector<std::shared_ptr<CallBacks>> vector = Loader::GetVector();
		for (std::shared_ptr<CallBacks> & value : vector)
		{
			value->OnDrawMenu(UserData);
		}

	});

}*/


/*
void _cdecl Parent::Tick(_In_ void* UserData)
{
	std::vector<std::shared_ptr<CallBacks>> vector = Loader::GetVector();
	for (std::shared_ptr<CallBacks> & value : vector)
	{
		value->OnTick(UserData);
	}
}*/