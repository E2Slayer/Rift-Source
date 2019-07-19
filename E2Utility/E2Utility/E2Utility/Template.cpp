#include "stdafx.h"
#include "Template.h"

AttackableUnit  MyTemplateClass::CurrentTarget;
AttackableUnit* MyTemplateClass::OrbTarget;

///This gets called once, at load
void MyTemplateClass::Init() 
{
	CurrentTarget = AttackableUnit();
	OrbTarget = NULL;

#pragma region RegisterCallbacks
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick,	MyTemplateClass::Tick);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Update,	MyTemplateClass::Update);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, MyTemplateClass::DrawMenu);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Update,	MyTemplateClass::Draw);

	pSDK->EventHandler->RegisterCallback(CallbackEnum::SpellCastStart, MyTemplateClass::OnCast);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::SpellCastEnd, MyTemplateClass::OnCastEnd);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Attack, MyTemplateClass::OnAttack);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::PostAttack, MyTemplateClass::PostAttack);
#pragma endregion
	
}

auto SpellQ = Spell::Active(SpellSlot::Q, 300.0f, DamageType::Physical);
//auto SpellQ = Spell::Active(SpellSlot::Q, 300.0f, DamageType::Physical);
auto SpellW = Spell::Active(SpellSlot::W);
auto SpellE = Spell::Active(SpellSlot::E, 325.0f);
auto SpellR1 = Spell::Active(SpellSlot::R);
auto SpellR2 = Spell::Skillshot(SpellSlot::R, 900.0f, SkillshotType::Cone, 0.25f, 1600.0f, 45.0f, DamageType::Physical);


int QCounter = 0;

/*
struct
{
	//
	// The local player object never changes. We can store it once after we
	// retrieve it from the SDK.
	//
	void* Object;

	//
	// The player's team.
	//
	const char* Name;

	unsigned int NetID;

} _g_LocalPlayer;
*/


char _g_Spells[] = { 'Q', 'W', 'E', 'R', 'D', 'F' };

///This gets called 30 times per second, put your logics in here instead of
///on OnUpdate so it doesnt drop too many fps
void MyTemplateClass::Tick(void * UserData) 
{
	auto target_ptr{ pCore->TS->GetTarget(1500.0f, DamageType::Magical) };
	if (target_ptr) 
	{
		/*
		void* localPlayer = pSDK->EntityManager->GetLocalPlayer();



		CurrentTarget = *(target_ptr);

		auto predict{ pSDK->Prediction->GetFastPrediction(target_ptr, SdkGetAISpell(localPlayer, (unsigned char)SpellSlot::R, Spell::Skillshot), NULL, NULL, NULL) };
		pSDK->Prediction->GetFastPrediction()*/
		//pSDK->Control->CastSpell((unsigned char)SpellSlot::Q, true);

		//SDK->Control->CastSpell('Q', CurrentTarget, true);
		//pSDK->Prediction->GetFastPrediction()

		CurrentTarget = *(target_ptr);

	}	
}

///This gets called X times per second, where X is your league fps.
///Use this only to update stuff you need to validade on every frame.
void MyTemplateClass::Update(void * UserData) 
{
	OrbTarget = pCore->Orbwalker->GetTarget();


	
	

}

///This gets called X times per second, where X is your league fps.
///Put any drawings you need here
void MyTemplateClass::Draw(void * UserData)
{
	
	//CurrentTarget is a copy of an object, so we check if its still valid.
	if (pCore->TS->IsValidTarget(&CurrentTarget)) 
	{
		Vector3 pos{ CurrentTarget.GetPosition() };
		
		if (pos.IsValid() && pos.IsOnScreen()) {
			Draw::Text(&pos, NULL, "This is CurrentTarget", "Arial Narrow", &Color::Red, 20, 6);

			//auto SpellQ = Spell::Skillshot(SpellSlot::R, 900.0f, SkillshotType::Cone, 0.25f, 1600.0f, 45, DamageType::Physical);

			if (pCore->Orbwalker->IsModeActive(OrbwalkingMode::Combo))
			{

				Vector3 targetPos{ CurrentTarget.GetPosition() };
				

			}
		}		
	}
	
	if (pCore->TS->IsValidTarget(OrbTarget)) {
		Vector3 pos{ OrbTarget->GetPosition() };
		
		if (pos.IsValid() && pos.IsOnScreen()) {
		
			//We get the screen position and offset it a little so it doesnt draw over the above text
			Vector2 screenPos{ Renderer::WorldToScreen(pos) };			
			screenPos.y += 20.0f;
		
			Draw::Text(NULL, &screenPos, "This is OrbTarget", "Arial Narrow", &Color::White, 20, 6);

		//	auto spellq = Spell::SpellBase(SpellSlot::W, 300.0f, DamageType::Physical);

			//auto SpellQ = Spell::Active(SpellSlot::Q, 300.0f, DamageType::Physical);
			if (pCore->Orbwalker->IsModeActive(OrbwalkingMode::Combo))
			{

				
				//auto tester = SpellQ.Cast(&targetPos);
				//if (!pSDK->EntityManager->GetLocalPlayer().IsAttacking())
				{
					//SpellQ.Cast(OrbTarget->AsAIBaseClient());

				}
	

			}

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


void __cdecl MyTemplateClass::PostAttack(AttackableUnit* Target)
{
	

	//SdkUiConsoleWrite("PostAttack %f \n", Game::Time());
	//SpellQ.Cast();
}


void __cdecl MyTemplateClass::OnAttack(void* AI, void* TargetObject, bool StartAttack, bool StopAttack, void* UserData)
{

	/*
	auto a1 = pSDK->EntityManager->GetLocalPlayer().GetNetworkID();

	auto a2 = pSDK->EntityManager->GetObjectFromPTR(AI)->GetNetworkID();

	if (a1 != a2)
	{
		return;
	}
	
	//SdkUiConsoleWrite("OnAttack %f \n", Game::Time());
	if (pCore->Orbwalker->IsModeActive(OrbwalkingMode::Combo) && SpellQ.IsReady())
	{

		
		//pSDK->EventHandler->DelayedAction(CancelAnimation, 300);
		SdkUiConsoleWrite("Delayed Action \n");
	}*/
}

bool test = false;


void __cdecl MyTemplateClass::OnCastEnd(void* AI, PSDK_SPELL_CAST SpellCast, void* UserData)
{
	
	try
	{
		
		if (!pSDK->EntityManager->IsValidObj(AI))
		{
			return;
		}
		

		/*
		const char* name1;
		SdkGetAIName(AI, &name1);

		void* local;

		SdkGetLocalPlayer(&local);

		const char* name2;
		SdkGetAIName(local, &name2);


		if (std::strcmp(name1, name2) != 0)
		{
			return;
		}
		*/

		if (pCore->Orbwalker->IsModeActive(OrbwalkingMode::Combo))
		{




			if (SpellCast->IsAutoAttack && !test)
			{
				//SdkUiConsoleWrite("OnCastEnd Name is %s \n", "yesy");
				SpellQ.Cast(&CurrentTarget.GetServerPosition());
				//Game::ShowEmote()
				//pSDK->EventHandler->DelayedAction(CancelAnimation2, 400);
				//test = false;

			}
			else
			{
				SdkUiConsoleWrite("OnCastEnd Name is %s \n", "nope");
				//pCore->Orbwalker->ResetAttackTimer();

			}
		}
	}
	catch (const std::exception& e)
	{
		SdkUiConsoleWrite("Exception %s \n", e.what());
	}

}



void __cdecl MyTemplateClass::OnCast(void* AI, PSDK_SPELL_CAST SpellCast, void* UserData)
{

	
	if (!pSDK->EntityManager->IsValidObj(AI) || !pCore->Orbwalker->IsModeActive(OrbwalkingMode::Combo))
	{
		return;
	}
	/*
	const char* name1;
	SdkGetAIName(AI, &name1);

	void* local;

	SdkGetLocalPlayer(&local);

	const char* name2;
	SdkGetAIName(local, &name2);


	if (std::strcmp(name1, name2) != 0)
	{
		return;
	}
	*/

	SdkUiConsoleWrite("4Animation Name is %s \n", SpellCast->Spell.AnimationName);

	//IEventHandler::DelayedAction(NULL, 100);



		std::string Name(SpellCast->Spell.AnimationName);

		auto name = SpellCast->Spell.AnimationName;



		int t = 0;
		if (strcmp(name, "Spell1") == 0)
		{
			if (QCounter == 0)
			{
				t = 291;
				QCounter = 1;
			}
			else if (QCounter == 1)
			{
				t = 290;
				QCounter = 2;
			}
			else if (QCounter == 2)
			{
				t = 343;
				QCounter = 0;
			}
		}
		else if (strcmp(name, "Spell2") == 0)
		{
			t = 170;
		}
		else if (strcmp(name, "Spell3") == 0)
		{

		}
		else if (strcmp(name, "Spell4a") == 0)
		{
			t = 0;
		}
		else if (strcmp(name, "Spell4b") == 0)
		{
			t = 150;
		}


		
		int calculate = t - Game::Ping();
		SdkUiConsoleWrite("a1m I here %f \n", Game::Time());
		test = true;
		pSDK->EventHandler->DelayedAction(CancelAnimation, 330);

	
}


void MyTemplateClass::CancelAnimation()
{
	
	try
	{
		//Game::ShowEmote(EMOTE_TYPE_TAUNT);
		//pSDK->Control->Move(&Renderer::MousePos());
		//pSDK->Control->Move(&Vector3(7813.07f, 53.81f, 4051.33f));

		Vector3 pos{ CurrentTarget.GetPosition() };

		SdkMoveLocalPlayer(&pos, false);

		//Vector3(7813.07f, 4051.33f, 53.81f)
		//pCore->Orbwalker->ResetAttackTimer();
		//pSDK->EventHandler->DelayedAction(CancelAnimation2, 350);
		//test = true;
		test = false;
	}
	catch(const std::exception& e)
	{
		SdkUiConsoleWrite("Exception %s \n", e.what());
	}

}

