#include "stdafx.h"
#include "Template.h"

AttackableUnit  MyTemplateClass::CurrentTarget;
AttackableUnit* MyTemplateClass::OrbTarget;

///This gets called once, at load
void MyTemplateClass::Init() {
	CurrentTarget = AttackableUnit();
	OrbTarget = NULL;

#pragma region RegisterCallbacks
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick,	MyTemplateClass::Tick);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Update,	MyTemplateClass::Update);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, MyTemplateClass::DrawMenu);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Update,	MyTemplateClass::Draw);	
#pragma endregion
}

///This gets called 30 times per second, put your logics in here instead of
///on OnUpdate so it doesnt drop too many fps
void MyTemplateClass::Tick(void * UserData) {
	auto target_ptr{ pCore->TS->GetTarget(1500.0f, DamageType::Magical) };
	if (target_ptr) {
		CurrentTarget = *(target_ptr);
	}	
}

///This gets called X times per second, where X is your league fps.
///Use this only to update stuff you need to validade on every frame.
void MyTemplateClass::Update(void * UserData) {
	OrbTarget = pCore->Orbwalker->GetTarget();
}

///This gets called X times per second, where X is your league fps.
///Put any drawings you need here
void MyTemplateClass::Draw(void * UserData){
	
	//CurrentTarget is a copy of an object, so we check if its still valid.
	if (pCore->TS->IsValidTarget(&CurrentTarget)) {
		Vector3 pos{ CurrentTarget.GetPosition() };
		
		if (pos.IsValid() && pos.IsOnScreen()) {
			Draw::Text(&pos, NULL, "This is CurrentTarget", "Arial Narrow", &Color::Red, 20, 6);
		}		
	}
	
	if (pCore->TS->IsValidTarget(OrbTarget)) {
		Vector3 pos{ OrbTarget->GetPosition() };
		
		if (pos.IsValid() && pos.IsOnScreen()) {
		
			//We get the screen position and offset it a little so it doesnt draw over the above text
			Vector2 screenPos{ Renderer::WorldToScreen(pos) };			
			screenPos.y += 20.0f;
		
			Draw::Text(NULL, &screenPos, "This is OrbTarget", "Arial Narrow", &Color::White, 20, 6);
		}		
	}

	auto Enemies{ pSDK->EntityManager->GetEnemyHeroes() };
	for (auto &[netID, Enemy] : Enemies) {
		if (pCore->Orbwalker->GetTrueAutoAttackRange(&Player, Enemy) > Player.Distance(Enemy)) {
			Vector3 pos{ Enemy->GetPosition() };

			if (pos.IsValid() && pos.IsOnScreen()) {
				//We get the screen position and offset it a little so it doesnt draw over the above text
				Vector2 screenPos{ Renderer::WorldToScreen(pos) };
				screenPos.y -= 20.0f;

				Draw::Text(NULL, &screenPos, "In AA Range", "Arial Narrow", &Color::White, 20, 6);
			}
		}
	}	
}

///Your menu settings go here
void MyTemplateClass::DrawMenu(void * UserData) {
	
}
