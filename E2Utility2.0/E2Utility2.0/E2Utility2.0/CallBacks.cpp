#include "stdafx.h"
#include "CallBacks.h"
#include "Loader.h"




static void _cdecl Update(_In_ void* UserData)
{
	auto plugin = Loader::GetCallBacks();
	if (plugin) 
		plugin->OnUpdate(UserData);
}

static void _cdecl Tick(_In_ void* UserData)
{
	auto plugin = Loader::GetCallBacks();
	if (plugin) 
		plugin->OnTick(UserData);
}

static void _cdecl Draw(_In_ void* UserData)
{
	auto plugin = Loader::GetCallBacks();
	if (plugin) 
		plugin->OnDraw(UserData);
}

static void _cdecl DrawMenu(_In_ void* UserData)
{
	
	auto plugin = Loader::GetCallBacks();
	if (plugin) 
		plugin->OnDrawMenu(UserData);
	
	
}

static void _cdecl RecallCallback(void* hero, const char* name, const char* type, void* userData)
{
	auto plugin = Loader::GetCallBacks();
	if (plugin) plugin->OnRecall(hero, name, type, userData);
}


static void _cdecl GainVisionCallback(AIHeroClient* hero)
{
	auto plugin = Loader::GetCallBacks();
	if (plugin) plugin->OnHeroEnterView(hero);
}

static void _cdecl LoseVisionCallback(AIHeroClient* hero)
{
	auto plugin = Loader::GetCallBacks();
	if (plugin) plugin->OnHeroExitView(hero);
}

static void _cdecl CreateObjectCallback(void* object, unsigned int netID, void* userData)
{
	auto plugin = Loader::GetCallBacks();
	if (plugin)
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

		plugin->OnObjectCreated(object, netID, userData);
	}
}

static void _cdecl DeleteObjectCallback(void* Object, unsigned int NetworkID, void* userData)
{
	auto plugin = Loader::GetCallBacks();
	if (plugin)
	{

		auto sender = pSDK->EntityManager->GetObjectFromPTR(Object);
		if (Object == nullptr || sender == nullptr || NetworkID == 0 )
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

		plugin->OnObjectRemoved(Object, NetworkID, userData);
	}
}

static void _cdecl EndSpellCallback(void* ai, PSDK_SPELL_CAST cast, void* userData)
{
	auto plugin = Loader::GetCallBacks();
	if (plugin) plugin->OnCastSpell(ai, cast, userData);
}

static void _cdecl StartCastCallback(void * AI, PSDK_SPELL_CAST SpellCast, void * UserData)
{
	auto plugin = Loader::GetCallBacks();
	if (plugin)
	{

		auto sender = pSDK->EntityManager->GetObjectFromPTR(AI);
		if (AI == nullptr || sender == nullptr || sender == NULL || !sender->IsHero() || !SpellCast->Spell.ScriptName)
		{
			return;
		}

		plugin->OnProcessSpell(AI, SpellCast, UserData);
	};
}

static void _cdecl PreCastCallback(bool* process, PSDK_SPELL spell, Vector3* position, AttackableUnit** target)
{
	auto plugin = Loader::GetCallBacks();
	if (plugin) plugin->OnPreCast(process, spell, position, target);
}

static void _cdecl AttackCallback(void* ai, void* target, bool start, bool stop, void* userData)
{
	auto plugin = Loader::GetCallBacks();
	if (plugin) plugin->OnAttack(ai, target, start, stop, userData);
}


void CallBacks::OnInit()
{

	pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, Update);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick, Tick);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, Draw);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, DrawMenu);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Recall, RecallCallback);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::CreateObject, CreateObjectCallback);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::DeleteObject, DeleteObjectCallback);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::GainVision, GainVisionCallback);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::LoseVision, LoseVisionCallback);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::PreCast, PreCastCallback);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::SpellCastStart, StartCastCallback);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::SpellCastEnd, EndSpellCallback);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Attack, AttackCallback);
	Initialized();
}
