#pragma once
#include "../../Include/SDK Extensions.h"

class CallBacks
{
public:
	CallBacks() {};
	virtual ~CallBacks() = default;

	void OnInit();


	/*
	 * Got most of ideas from Dev-Tucker
	 * https://github.com/tucker-rift/aio-template/blob/master/aio-template/ChampionPlugin.h
	 * 
	 */

	virtual void OnUpdate(void* userData) {};
	virtual void OnTick(void* userData) {};
	virtual void OnDraw(void* userData) {};
	virtual void OnDrawMenu(void* userData) {};

	virtual void OnObjectCreated(void* object, unsigned int netID, void* userData) {};
	virtual void OnObjectRemoved(void* object, unsigned int netID, void* userData) {};
	virtual void OnAttack(void* ai, void* target, bool start, bool stop, void* userData) {};
	virtual void OnProcessSpell(void* ai, PSDK_SPELL_CAST cast, void* userData) {};
	virtual void OnCastSpell(void* ai, PSDK_SPELL_CAST cast, void* userData) {};
	virtual void OnPreCast(bool* process, PSDK_SPELL spell, Vector3* position, AttackableUnit** target) {};
	virtual void OnRecall(void* hero, const char* name, const char* type, void* userData) {};
	virtual void OnHeroEnterView(AIHeroClient* hero) {};
	virtual void OnHeroExitView(AIHeroClient* hero) {};

	virtual void Initialized() = 0;
};

