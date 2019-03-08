#include "stdafx.h"
#include "Template.h"

AttackableUnit  MyTemplateClass::CurrentTarget;
AttackableUnit* MyTemplateClass::OrbTarget;




///This gets called once, at load
void MyTemplateClass::Init() {
	CurrentTarget = AttackableUnit();
	OrbTarget = NULL;

#pragma region RegisterCallbacks
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick, MyTemplateClass::Tick);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, MyTemplateClass::Update);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, MyTemplateClass::DrawMenu);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, MyTemplateClass::Draw);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Recall, MyTemplateClass::Recall);
#pragma endregion
}


struct recallingList
{
	recallingList(void* _Object, unsigned _netId, int _teamID, const char* _champName, float _recallStartedTime)
	{
		Object = _Object;
		netId = _netId;
		teamID = _teamID;
		champName = _champName;
		recallStartedTime = _recallStartedTime;
		
	}

	void* Object;
	unsigned netId;
	int teamID;
	const char* champName;
	float recallStartedTime;

};
float GlobalTime = 0;

std::vector<recallingList> CurrentRecallingList; // Container for champion list based menus. 



/*
 *	#define TEAM_TYPE_UNKNOWN							0
	#define TEAM_TYPE_ORDER								100
	#define TEAM_TYPE_CHAOS								200
	#define TEAM_TYPE_NEUTRAL							300

 */

///This gets called 30 times per second, put your logics in here instead of
///on OnUpdate so it doesnt drop too many fps
void MyTemplateClass::Recall(void* Unit, const char* Name, const char* Type, void* UserData)
{


	//Recall

	return;
	if(Name == nullptr || Type == nullptr)
	{
		return;
	}

	UNREFERENCED_PARAMETER(UserData);

	CurrentRecallingList.erase(std::remove_if(CurrentRecallingList.begin(),
		CurrentRecallingList.end(),
		[&](const recallingList stopPoint)-> bool
	{
		return stopPoint.recallStartedTime + 8.5f < Game::Time();
	}),
		CurrentRecallingList.end());

	//
	const char* test2 = nullptr;
	SdkGetAIName(Unit, &test2);

	int test0 = 0;
	SdkGetObjectTeamID(Unit, &test0);
	unsigned int test01 = 0;
	SdkGetObjectNetworkID(Unit, &test01);
//	GameObject::GetNetworkID();
	float Time;
	SdkGetGameTime(&Time); // get the current game time

	

	if(strlen(Name) == 0 && strlen(Type) == 0)
	{
		SdkUiConsoleWrite("%s Done/Aborted Recall /%s %d %d", test2, Name, test0, test01);
		SdkUiConsoleWrite("Time =  %f", GlobalTime);
	}
	else
	{
		CurrentRecallingList.push_back(recallingList(Unit, test01, test0, test2, Time));
		SdkUiConsoleWrite("%s Started Recall /%s %d %d", test2, Name, test0, test01);
		SdkUiConsoleWrite("Started Time =  %f", GlobalTime);
	}


	// difference 7.972595

	//SdkDrawLine()
	
}


///This gets called 30 times per second, put your logics in here instead of
///on OnUpdate so it doesnt drop too many fps
void MyTemplateClass::Tick(void * UserData) 
{
	auto target_ptr{ pCore->TS->GetTarget(1500.0f, DamageType::Magical) };
	if (target_ptr) {
		CurrentTarget = *(target_ptr);
	}	
}

///This gets called X times per second, where X is your league fps.
///Use this only to update stuff you need to validade on every frame.
void MyTemplateClass::Update(void * UserData) {
	GlobalTime = Game::Time();
	OrbTarget = pCore->Orbwalker->GetTarget();
}

///This gets called X times per second, where X is your league fps.
///Put any drawings you need here
void MyTemplateClass::Draw(void * UserData)
{
	

	auto testObjects{ pSDK->EntityManager->GetParticles() };



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
void MyTemplateClass::DrawMenu(void * UserData) 
{
	//SdkUiText("MataView 1.1 [Beta] - E2Slayer");
	//bool bSettingsExpanded = false;
	Menu::Tree("Recall Tracker", "Recall.Main", false, []()
	{
		//Menu::SliderInt("Font Size", "Ability.FontSize", 28, 21, 50); //&_g_CheckTeamAbilities.iFontSize, 21.0f, 50.0f, "%.0f", NULL);
		//SDKVECTOR defaultColor = { 255, 255, 255 };
		//Menu::ColorPicker("Color Picker", "Ability.Color", defaultColor);
		SdkUiText("Hello Recall");
		//MakeMenu::ColorPicker()
		//SdkUiColorPicker("Color Pick", &_g_CheckTeamAbilities.m_CurrentColor, NULL);
		//SdkUiText("Note: The settings work after pressed the Save Settings");
		//SdkUiColoredText(&_g_ColorGreen, "Note: The settings will apply \nAfter pressed the Save Settings");


	});

	
}
