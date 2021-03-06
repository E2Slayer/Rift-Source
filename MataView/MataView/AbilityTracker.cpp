﻿#include "stdafx.h"
#include "AbilityTracker.h"



std::map<unsigned int, GameObject*> AbilityTracker::GameObjects;


//AttackableUnit* MyTemplateClass::OrbTarget;
//
// Used for drawing a circle with the default direction vector.
//
SDKVECTOR _g_DirectionVector = { 0, 0, 1.f };

//
// Colors in the BGRA format.
//
SDKCOLOR _g_ColorWhite = { 255, 255, 255, 255 };
SDKCOLOR _g_ColorYellow = { 0, 255, 255, 255 };
SDKCOLOR _g_ColorGreen = { 0, 255, 0, 255 };
SDKCOLOR _g_ColorRed = { 0, 0, 255, 255 };
SDKCOLOR _g_ColorPurple = { 128, 0, 128, 255 };


///This gets called once, at load
void AbilityTracker::Init() 
{
	//CurrentTarget = AttackableUnit();

	//OrbTarget = NULL;

	GameObjects;

#pragma region RegisterCallbacks
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick, AbilityTracker::Tick);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, AbilityTracker::Update);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, AbilityTracker::DrawMenu);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, AbilityTracker::Draw);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::CreateObject, AbilityTracker::OnCreate);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::DeleteObject, AbilityTracker::OnDelete);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::BuffUpdate, AbilityTracker::OnBuffCreateDelete);

	AblitiesAdder();
	
#pragma endregion
	//AbilityTracker::AblitiesAdder();

	
}

struct abilityTrackList
{
	abilityTrackList(const char* _champName, const char* _displayName, const char* _objectName, float _time, const char* _menuName, bool _settingEnabler, bool _menuCreator)
	{
		champName = _champName;
		displayName = _displayName;
		objectName = _objectName;
		time = _time;
		menuName = _menuName;
		settingEnabler = _settingEnabler;
		menuCreator = _menuCreator;
	}

	const char* champName;
	const char* objectName;
	const char* displayName;
	float time;
	bool settingEnabler;
	bool menuCreator;
	const char* menuName;
};

struct drawCollector
{
	drawCollector(void* _Object, unsigned _netId, SDKVECTOR _Position, float _TimeLeft, float _CastedTime)
	{
		Object = _Object;
		Position = _Position;
		TimeLeft = _TimeLeft;
		CastedTime = _CastedTime;
		netId = _netId;
	}

	void* Object;
	unsigned netId;
	//int TeamID;

	SDKVECTOR Position;

	float TimeLeft;
	float CastedTime;
};

struct menuCreator
{
	menuCreator(const char* _champName, bool _enabler)
	{
		champName = _champName;
		enabler = _enabler;
		
	}
	const char* champName;
	bool enabler;

};


struct
{
	//
	// Whether we should draw the attack range of a turret.
	//
	bool bMySkills;

	//
	// Whether we should draw the attack radius of only enemy turrets.
	//
	bool bAllySkills;
	bool bEnemySkills;

	bool bChampionList;
	bool bMicsList;
	float iFontSize;

	SDKVECTOR m_CurrentColor;

	float m_A, m_B, m_G, m_R;

	//	for (auto &element : AbilityDictionary) // access by reference to avoid copying
		//{
		//	SdkUiCheckbox(element.displayName, &(element.settingEnabler), NULL);
	//
		//}
} _g_CheckTeamAbilities;


std::vector<abilityTrackList> AbilityDictionary; // List for ability to draw
std::vector<drawCollector> ObjectStore; // Container for drawing to draw now
std::vector<menuCreator> bAbilitySettings; // Container for champion list based menus. 




///This gets called 30 times per second, put your logics in here instead of
///on OnUpdate so it doesnt drop too many fps
void AbilityTracker::Tick(void * UserData) 
{

	//auto gameObjects{ pSDK->EntityManager->GetParticles() };
	//GameObjects = gameObjects;
}

///This gets called X times per second, where X is your league fps.
///Use this only to update stuff you need to validade on every frame.
void AbilityTracker::Update(void * UserData) 
{
	
}

///This gets called X times per second, where X is your league fps.
///Put any drawings you need here
void AbilityTracker::Draw(void * UserData) 
{
	

	/*
	if (pSDK->EntityManager->GetLocalPlayer().AsAIHeroClient())
	{
		SDKVECTOR Position = pSDK->EntityManager->GetLocalPlayer().AsAIHeroClient()->GetPosition();
		//SdkGetObjectPosition(Object, &Position);
		//	auto s2 = ObjectStore.size();
		std::string str(std::to_string(ObjectStore.size()));
		SdkDrawText(&Position, NULL, str.c_str(), "Arial Rounded MT", &_g_ColorRed, 28, 0, 3, true);
	}
	*/

	if (!ObjectStore.empty())
	{
		auto testObjects{ pSDK->EntityManager->GetParticles() };
		for (auto&[_, obj] : testObjects)
		{
			//SdkUiConsoleWrite("[added] %s  \n", obj->GetName());
			for (std::vector<drawCollector>::const_iterator it = ObjectStore.begin(); it != ObjectStore.end(); ++it)
			{
				if (&(*it) == nullptr) //if the object is invalid, don't do anything
				{
				}
				else
				{

					float Time;
					SdkGetGameTime(&Time); // get the current game time
					
					std::stringstream ss;
					ss.precision(1); //for decimal
					ss.setf(std::ios_base::fixed, std::ios_base::floatfield);

					unsigned int netID = obj->GetNetworkID();
					
					//SdkUiConsoleWrite("[added22] %s  \n", obj->GetName());
					if ((*it).netId == netID)
					{
						//SdkUiConsoleWrite("[NetID3] %d \n", netID);

						SDKVECTOR NewPosition = obj->GetPosition();
						//obj->GetPosition();
						//SdkGetObjectPosition(obj, &NewPosition);


						if ((*it).CastedTime + (*it).TimeLeft > Time)
						{
							ss << "" << ((*it).CastedTime + (*it).TimeLeft) - Time << "\n";

							auto Color = Menu::Get<SDKVECTOR>("Ability.Color");
							auto fontSize = Menu::Get<int>("Ability.FontSize");
							
							SDKCOLOR color = //https://gitlab.com/riftlol/rift-sdk/blob/master/src/examples/mdebug/drawing.h#L144
							{
								
								(uint8_t)(round(Color.z * (UINT8_MAX * 1.f))) , //B
								(uint8_t)(round(Color.y * (UINT8_MAX * 1.f))) , //G
								(uint8_t)(round(Color.x * (UINT8_MAX * 1.f))) , //R
								UINT8_MAX										//A
							};

	

					
							//PSDKVECTOR


							//PSDKCOLOR test2 = { Color.x , Color.y ,Color.z};
							//SdkDrawText(&NewPosition, NULL, ss.str().c_str(), "Arial Rounded MT", &_g_ColorGreen, fontSize, 0, 3, true);
							Draw::Text(&NewPosition, NULL, ss.str().c_str(), "Arial Rounded MT", &color, fontSize, 0, 3, true);
							//Draw::RectOutline(&NewPositionStart, &NewPosition, 23, &Color::Purple);
						}
					}


				}
			}
		}
	}

	
}





static void AblitiesAdder()
{

	AbilityDictionary.clear(); //clear the list

	//AbilityDictionary.push_back(abilityTrackList("Veigar", "Veigar E", "Veigar_Base_E_Warning_Green", 3.5f, "MataView_VeigarE", true));
	AbilityDictionary.push_back(abilityTrackList("Veigar", "Veigar E (Ally)", "Veigar_Base_E_cage_green", 3.0f, "MataView_VeigarE", true, false));
	AbilityDictionary.push_back(abilityTrackList("Veigar", "Veigar W (Ally)", "Veigar_Base_W_cas_green", 1.2f, "MataView_VeigarW", true, false));
	AbilityDictionary.push_back(abilityTrackList("Veigar", "Veigar E (Enemy)", "Veigar_Base_E_cage_red", 3.0f, "MataView_VeigarE2", true, false));
	AbilityDictionary.push_back(abilityTrackList("Veigar", "Veigar W (Enemy)", "Veigar_Base_W_cas_red", 1.2f, "MataView_VeigarW2", true, false));




	AbilityDictionary.push_back(abilityTrackList("Misc", "Zhonya's Hourglass", "ItemZhonya_Base_stasis.troy", 2.5f, "MataView_Zhonya", true, false));

	AbilityDictionary.push_back(abilityTrackList("Misc", "Guardian Angel", "LifeAura.troy", 4.0f, "MataView_GA", true, false));
	AbilityDictionary.push_back(abilityTrackList("Misc", "Redemption (Ally)", "Item_Redemption_indicator_green.troy", 2.5f, "MataView_Redemption", true, false));
	AbilityDictionary.push_back(abilityTrackList("Misc", "Redemption (Enemy)", "Item_Redemption_indicator_red.troy", 1.5f, "MataView_Redemption2", true, false));


	AbilityDictionary.push_back(abilityTrackList("Zilean", "Zilean R", "Zilean_Base_R_tar_01", 5.0f, "MataView_ZileanR", true, false));

	AbilityDictionary.push_back(abilityTrackList("Akali", "Akali W", "Akali_Base_W_Cas", 5.0f, "MataView_AkaliW", true, false));

	AbilityDictionary.push_back(abilityTrackList("Alistar", "Alistar R", "Alistar_Base_R_cas", 7.0f, "MataView_AlistarR", true, false));

	AbilityDictionary.push_back(abilityTrackList("Braum", "Braum E", "Braum_Base_E_Shield_cas", 3.0f, "MataView_BraumE", true, false));
	//AbilityDictionary.push_back(abilityTrackList("DrMundo", "DrMundo E", "DrMundo_Base_R_cas_02", 12.0f));
	AbilityDictionary.push_back(abilityTrackList("Ekko", "Ekko W", "Ekko_Base_W_Indicator", 3.0f, "MataView_EkkoW1", true, false));
	AbilityDictionary.push_back(abilityTrackList("Ekko", "Ekko W2", "Ekko_Base_W_Cas", 1.0f, "MataView_EkkoW2", true, false));
	//AbilityDictionary.push_back(abilityTrackList("Zed", "Zed W", "Zed_Base_Shadow_TC_Red", 5.0f));
	AbilityDictionary.push_back(abilityTrackList("Zed", "Zed W", "Zed_Base_W_cloneswap_buf", 5.0f, "MataView_ZedW", true, false));
	AbilityDictionary.push_back(abilityTrackList("Zed", "Zed R", "Zed_Base_R_cloneswap_buf", 6.0f, "MataView_ZedR", true, false));

	//AbilityDictionary.push_back(abilityTrackList("Fizz", "Fizz E", "Fizz_Base_E1_Indicator_Ring", 1.0f, "MataView_FizzE", true, false));
	AbilityDictionary.push_back(abilityTrackList("Fizz", "Fizz R (Ally)", "Fizz_Base_R_Ring_Green", 2.0f, "MataView_FizzR", true, false));
	AbilityDictionary.push_back(abilityTrackList("Fizz", "Fizz R (Enemy)", "Fizz_Base_R_Ring_Red", 2.0f, "MataView_FizzR2", true, false));
	AbilityDictionary.push_back(abilityTrackList("Fiddlesticks", "Fiddlesticks R (Ally)", "Fiddlesticks_Base_Crowstorm_green_cas", 5.0f, "MataView_FiddR", true, false));
	AbilityDictionary.push_back(abilityTrackList("Fiddlesticks", "Fiddlesticks R (Enemy)", "Fiddlesticks_Base_Crowstorm_red_cas", 5.0f, "MataView_FiddR2", true, false));
	AbilityDictionary.push_back(abilityTrackList("Janna", "Janna R (Ally)", "Janna_Base_R_cas_Green", 3.0f, "MataView_JannaR", true, false));
	AbilityDictionary.push_back(abilityTrackList("Janna", "Janna R (Enemy)", "Janna_Base_R_cas_Red", 3.0f, "MataView_JannaR2", true, false));
	AbilityDictionary.push_back(abilityTrackList("Jax", "Jax E", "JaxCounterStrike", 2.0f, "MataView_JaxE", true, false));
	AbilityDictionary.push_back(abilityTrackList("Elise", "Elise E (Ally)", "Elise_Base_Spider_E_indicator_green", 2.0f, "MataView_ElieseE", true, false)); //possible replacement Elise_Base_Spider_E_cas 
	AbilityDictionary.push_back(abilityTrackList("Elise", "Elise E (Enemy)", "Elise_Base_Spider_E_indicator_red", 2.0f, "MataView_ElieseE", true, false)); //possible replacement Elise_Base_Spider_E_cas 


	AbilityDictionary.push_back(abilityTrackList("Annie", "Annie R", "annierburnaura", 45.0f, "MataView_AnnieR", true, false));
	AbilityDictionary.push_back(abilityTrackList("DrMundo", "DrMundo E", "Sadism", 12.0f, "MataView_MundoR", true, false));
	AbilityDictionary.push_back(abilityTrackList("Gangplank", "Gangplank R (Ally)", "Gangplank_Base_R_AoE_Green", 8.0f, "MataView_GangplankR", true, false));
	AbilityDictionary.push_back(abilityTrackList("Gangplank", "Gangplank R (Enemy)", "Gangplank_Base_R_AoE_Red", 8.0f, "MataView_GangplankR2", true, false));

	AbilityDictionary.push_back(abilityTrackList("Karthus", "Karthus R (Ally)", "Karthus_Base_R_Target_Green", 3.0f, "MataView_KarthusR", true, false));
	AbilityDictionary.push_back(abilityTrackList("Karthus", "Karthus R (Enemy)", "Karthus_Base_R_Target_Red", 3.0f, "MataView_KarthusR", true, false));
	AbilityDictionary.push_back(abilityTrackList("Nasus", "Nasus R", "Nasus_Base_R_Aura", 15.0f, "MataView_NasusR", true, false));
	AbilityDictionary.push_back(abilityTrackList("Morgana", "Morgana E", "Morgana_Base_E_Tar", 5.0f, "MataView_MorganaE", true, false));
	AbilityDictionary.push_back(abilityTrackList("Morgana", "Morgana R", "Morgana_base_R_Indicator_Ring", 3.0f, "MataView_MorganaR", true, false));
	AbilityDictionary.push_back(abilityTrackList("Nunu", "Nunu W", "Nunu_Base_W_Snowball_RollingBall", 10.0f, "MataView_NunuW", true, false));
	AbilityDictionary.push_back(abilityTrackList("Nunu", "Nunu R", "Nunu_Base_R_Cas", 3.0f, "MataView_NunuR", true, false));
	AbilityDictionary.push_back(abilityTrackList("Ryze", "Ryze R (Ally Caster)", "Ryze_Base_R_Start_Ally", 2.0f, "MataView_RyzeR", true, false));
	AbilityDictionary.push_back(abilityTrackList("Ryze", "Ryze R (Ally Direction)", "Ryze_Base_R_End_Ally", 3.0f, "MataView_RyzeR2", true, false));
	AbilityDictionary.push_back(abilityTrackList("Ryze", "Ryze R (Enemy Caster)", "Ryze_Base_R_Start_Enemy", 2.0f, "MataView_RyzeR3", true, false));
	AbilityDictionary.push_back(abilityTrackList("Ryze", "Ryze R (Enemy Direction)", "Ryze_Base_R_End_Enemy", 3.0f, "MataView_RyzeR4", true, false));

	AbilityDictionary.push_back(abilityTrackList("Tryndamere", "Tryndamere R", "UndyingRage", 5.0f, "MataView_TryndamereR", true, false));
	//AbilityDictionary.push_back(abilityTrackList("Talon", "Talon R", "Talon_Base_R_Cas", 2.5f, "MataView_TalonR", true, false));
	AbilityDictionary.push_back(abilityTrackList("Sion", "Sion R", "Sion_Base_R_Cas", 8.0f, "MataView_SionR", true, false));

	AbilityDictionary.push_back(abilityTrackList("Vladimir", "Vladimir W", "Vladimir_Base_W_buf", 2.0f, "MataView_VladimirW", true, false));
	AbilityDictionary.push_back(abilityTrackList("Taric", "Taric R", "Taric_Base_R_Indicator", 2.5f, "MataView_TaricR", true, false));
	AbilityDictionary.push_back(abilityTrackList("Taric", "Taric R (Invulnerable)", "Taric_Base_R_buf", 2.5f, "MataView_TaricR2", true, false));

	AbilityDictionary.push_back(abilityTrackList("Rammus", "Rammus Q", "PowerBall", 6.0f, "MataView_RammusQ", true, false));
	AbilityDictionary.push_back(abilityTrackList("Rammus", "Rammus Q", "PowerBall", 6.0f, "MataView_RammusQ", true, false));
	AbilityDictionary.push_back(abilityTrackList("Viktor", "Viktor W (Ally)", "Viktor_Catalyst_green", 4.0f, "MataView_ViktorW", true, false));
	AbilityDictionary.push_back(abilityTrackList("Viktor", "Viktor W (Enemy)", "Viktor_Catalyst_red", 4.0f, "MataView_ViktorW2", true, false));
	AbilityDictionary.push_back(abilityTrackList("Viktor", "Viktor R (Ally)", "Viktor_ChaosStorm_green", 6.5f, "MataView_ViktorR", true, false));
	AbilityDictionary.push_back(abilityTrackList("Viktor", "Viktor R (Enemy)", "Viktor_ChaosStorm_red", 6.5f, "MataView_ViktorR2", true, false));

	AbilityDictionary.push_back(abilityTrackList("Galio", "Galio R (Ally)", "Galio_Base_R_Tar_Ground_Ally", 2.5f, "MataView_GalioR", true, false));
	AbilityDictionary.push_back(abilityTrackList("Galio", "Galio R (Enemy)", "Galio_Base_R_Tar_Ground_Enemy", 2.5f, "MataView_GalioR2", true, false));
	AbilityDictionary.push_back(abilityTrackList("Yasuo", "Yasuo W", "Yasuo_Base_W_windwall", 4.0f, "MataView_YasuoW", true, false));


	AbilityDictionary.push_back(abilityTrackList("Lissandra", "Lissandra R (Ally)", "Lissandra_Base_R_ring_green", 2.5f, "MataView_LissandraR", true, false));
	AbilityDictionary.push_back(abilityTrackList("Lissandra", "Lissandra R (Enemy)", "Lissandra_Base_R_ring_red", 2.5f, "MataView_LissandraR2", true, false));

	AbilityDictionary.push_back(abilityTrackList("Kennen", "Kennen R (Ally)", "Kennen_Base_R_Cast_Green", 3.0f, "MataView_KennenR", true, false));
	AbilityDictionary.push_back(abilityTrackList("Kennen", "Kennen R (Enemy)", "Kennen_Base_R_Cast_Red", 3.0f, "MataView_KennenR2", true, false));


	AbilityDictionary.push_back(abilityTrackList("Pantheon", "Pantheon R (Caster)", "Pantheon_Base_R_cas", 2.0f, "MataView_PantheonR", true, false));
	AbilityDictionary.push_back(abilityTrackList("Pantheon", "Pantheon R (Ally)", "Pantheon_Base_R_indicator_green", 4.5f, "MataView_PantheonR2", true, false));
	AbilityDictionary.push_back(abilityTrackList("Pantheon", "Pantheon R (Enemy)", "Pantheon_Base_R_indicator_red", 2.5f, "MataView_PantheonR3", true, false));

	AbilityDictionary.push_back(abilityTrackList("LeBlanc", "LeBlanc W", "LeBlanc_Base_W_return_indicator", 4.0f, "MataView_LeBlancW", true, false));
	AbilityDictionary.push_back(abilityTrackList("LeBlanc", "LeBlanc R (Mirror W)", "LeBlanc_Base_RW_return_indicator", 4.0f, "MataView_LeBlancW2", true, false));

	//AbilityDictionary.push_back(abilityTrackList("Kindred", "Kindred R", "Kindred_Base_R_AOE", 4.0f, "MataView_KindredR", true, false));






	//Kindred_Base_R_NoDeath_Buff

	//AbilityDictionary.push_back(abilityTrackList("Vladimir", "Vladimir W", "Vladimir_Base_W_buf", 2.0f, "MataView_VladimirW", true, false));


	//Nasus_Base_R_Aura

	//Gangplank_Base_R_AoE_Green

	//time is z Tryndamere_Base_R_glow_01 
	//Janna_Base_R_cas_Green





	std::sort(AbilityDictionary.begin(), AbilityDictionary.end(), [](const abilityTrackList& lhs, const abilityTrackList& rhs) {
		return std::strcmp(lhs.champName, rhs.champName) < 0;
	});

	for (auto &element : AbilityDictionary) // access by reference to avoid copying
	{

		std::string t1(element.champName);

		auto it = std::find_if(
			bAbilitySettings.begin(), bAbilitySettings.end(),
			[&element](const menuCreator& x) { return std::strcmp(x.champName, element.champName) == 0; });
		if (it != bAbilitySettings.end())
		{
			//SdkUiConsoleWrite("[added] %s  \n", "dont do it");
		}
		else
		{
			bAbilitySettings.push_back(menuCreator(element.champName, false));
			//SdkUiConsoleWrite("[added] %s  \n", element.champName);
		}

	}

	//std::sort(bAbilitySettings.begin(), bAbilitySettings.end());
	//bAbilitySettings.erase(std::unique(bAbilitySettings.begin(), bAbilitySettings.end()), bAbilitySettings.end());
}






std::string sTemp;
///Your menu settings go here
void AbilityTracker::DrawMenu(void * UserData) 
{
	UNREFERENCED_PARAMETER(UserData);

	bool bAbilityExpanded = true;
	Menu::Tree("Ability Tracker", "Ability.Main", &bAbilityExpanded, []()
	{
		SdkUiText("MataView 1.1 [Beta] - E2Slayer");
		bool bSettingsExpanded = false;
		Menu::Tree("Settings", "Ability.Settings", &bSettingsExpanded, []()
		{
			Menu::SliderInt("Font Size", "Ability.FontSize", 28, 21, 50); //&_g_CheckTeamAbilities.iFontSize, 21.0f, 50.0f, "%.0f", NULL);
			SDKVECTOR defaultColor = { 255, 255, 255 };
			Menu::ColorPicker("Color Picker", "Ability.Color", defaultColor);
			
			//MakeMenu::ColorPicker()
			//SdkUiColorPicker("Color Pick", &_g_CheckTeamAbilities.m_CurrentColor, NULL);
			//SdkUiText("Note: The settings work after pressed the Save Settings");
			//SdkUiColoredText(&_g_ColorGreen, "Note: The settings will apply \nAfter pressed the Save Settings");
			

		});



		Menu::Tree("Champions", "Ability.Champions", false, []()
		{
			for (auto& element1 : bAbilitySettings) // access by reference to avoid copying
			{
				if (std::strcmp(element1.champName, "Misc") != 0)
				{
					//SdkUiConsoleWrite("[added] %s  \n", element1.champName);
					sTemp = element1.champName;
					Menu::Tree((element1).champName, ("Ability.Champions." + std::string((element1).champName)), false, []()
					{
						for (auto &element2 : AbilityDictionary) // access by reference to avoid copying
						{
							if (std::strcmp(sTemp.c_str(), element2.champName) == 0)
							{
							//	SdkUiCheckbox(element2.displayName, &element2.settingEnabler, NULL);
								Menu::Checkbox(element2.displayName, ("Ability.Champions." + sTemp), true);
								//SdkUiConsoleWrite("[added] %s  \n", element.champName);
							}
						}
					});
				}

			}

		});

		Menu::Tree("Misc", "Ability.Misc", false, []()
		{
			for (auto& element1 : bAbilitySettings) // access by reference to avoid copying
			{
				if (std::strcmp(element1.champName, "Misc") == 0)
				{
					for (auto &element2 : AbilityDictionary) // access by reference to avoid copying
					{
						if (std::strcmp(element1.champName, element2.champName) == 0)
						{
							Menu::Checkbox(element2.displayName, ("Ability.Misc." + std::string(element1.champName)), true);
						}
					}


				}
			}

		});


	});



	
}

bool __cdecl AbilityTracker::OnCreate(_In_ void* Object, _In_ unsigned int NetworkID, _In_opt_ void* UserData)
{
	UNREFERENCED_PARAMETER(UserData);



	if(!pSDK->EntityManager->IsValidObj(Object) || Object == nullptr || Object == NULL)
	{
		return false;
		//SdkUiConsoleWrite("[Checker] it aint proper object  \n");
	}

	



	SDKVECTOR Position = pSDK->EntityManager->GetObjectFromPTR(Object)->GetPosition();



	const char* test = pSDK->EntityManager->GetObjectFromPTR(Object)->GetName();

	//SdkGetObjectName(Object, &test);



	std::string s1(test);


	/*
	if ((!strstr(s1.c_str(), "Minion")) && (!strstr(s1.c_str(), "SRU") && (!strstr(s1.c_str(), "urret") && (!strstr(s1.c_str(), "Pain")))))
	{
		SDKVECTOR tempPos;
		SdkGetObjectPosition(Object, &tempPos);

		unsigned int netID;
		SdkGetObjectNetworkID(Object, &netID);

	}
	*/

	for (std::vector<abilityTrackList>::const_iterator it = AbilityDictionary.begin(); it != AbilityDictionary.end(); ++it)
	{



		if (&(*it) == nullptr) //if the object is invalid, don't do anything
		{
			//SdkUiConsoleWrite("am I here\n");
		}
		else
		{
			//SdkUiConsoleWrite("[added Here2] %s  \n", pSDK->EntityManager->GetObjectFromPTR(Object)->GetName());

			if (!(*it).settingEnabler)
				break;

			auto temp = (*it).objectName;
			std::string s2(temp);
			std::transform(s1.begin(), s1.end(), s1.begin(), ::tolower);

			std::transform(s2.begin(), s2.end(), s2.begin(), ::tolower);

			//if (SDKSTATUS_SUCCESS(SdkIsObjectParticle(Object)) && (strstr(s1.c_str(), temp)))
			if ((strstr(s1.c_str(), s2.c_str())) && !(strstr(s1.c_str(), "activate") )) //because of yasuo windwall
			{

				if (!(strstr(s1.c_str(), "end")) && !(strstr(s1.c_str(), "death")))
				{
					int TeamID = pSDK->EntityManager->GetObjectFromPTR(Object)->GetTeamID();
					float Time = Game::Time();

					unsigned int netID = pSDK->EntityManager->GetObjectFromPTR(Object)->GetNetworkID();

					drawCollector testObject = drawCollector(&Object, netID, Position, (*it).time, Time);

					//	SdkUiConsoleWrite("[simple] time is z %f \n", Time);
				//	SdkUiConsoleWrite("[Owner] %s \n", pSDK->EntityManager->GetObjectFromPTR(Object)->GetObjectOwner());
					ObjectStore.push_back(testObject);
					//SdkUiConsoleWrite("[added Here] %s %d \n", pSDK->EntityManager->GetObjectFromPTR(Object)->GetName(), netID);

					//	SdkUiConsoleWrite("[NetID2] %d \n", netID);
				}

			}
		}
	}

	return true;
}

bool __cdecl AbilityTracker::OnDelete(_In_ void* Object, _In_ unsigned int NetworkID, _In_opt_ void* UserData)
{

	UNREFERENCED_PARAMETER(UserData);


	if(!pSDK->EntityManager->IsValidObj(Object) || Object == nullptr || Object == NULL)
	{
		return false;
	}

	ObjectStore.erase(std::remove_if(ObjectStore.begin(),
		ObjectStore.end(),
		[&](const drawCollector stopPoint)-> bool
	{
		return stopPoint.CastedTime + stopPoint.TimeLeft < Game::Time();
	}),
		ObjectStore.end());

	return true;
	
}

void __cdecl AbilityTracker::OnBuffCreateDelete(void* AI, bool Created, unsigned char Type, float StartTime, float EndTime, const char* Name, void* CasterObject, unsigned int CasterID, int Stacks, bool HasCount, int Count, PSDK_SPELL Spell, void* UserData)
{
	UNREFERENCED_PARAMETER(UserData);

	if (!pSDK->EntityManager->IsValidObj(AI) || AI == nullptr || AI == NULL)
	{
		return;
	}


	if(Name == nullptr || Name == NULL)
	{
		return;
	}

	//SdkUiConsoleWrite("[buffcre1] %s  \n", Name);
	std::string s1(Name);
	if (Created)
	{
		for (std::vector<abilityTrackList>::const_iterator it = AbilityDictionary.begin(); it != AbilityDictionary.end(); ++it)
		{
			
			if (!(*it).settingEnabler)
				break;

			auto temp = (*it).objectName;
			std::string s2(temp);
			std::transform(s1.begin(), s1.end(), s1.begin(), ::tolower);

			std::transform(s2.begin(), s2.end(), s2.begin(), ::tolower);

			//if (SDKSTATUS_SUCCESS(SdkIsObjectParticle(Object)) && (strstr(s1.c_str(), temp)))
			if ((strstr(s1.c_str(), s2.c_str())))
			{
				int TeamID = pSDK->EntityManager->GetObjectFromPTR(AI)->GetTeamID();
				float Time = Game::Time();

				unsigned int netID = pSDK->EntityManager->GetObjectFromPTR(AI)->GetNetworkID();

				drawCollector testObject = drawCollector(&AI, netID, pSDK->EntityManager->GetObjectFromPTR(AI)->GetPosition(), (*it).time, Time);

				//	SdkUiConsoleWrite("[simple] time is z %f \n", Time);

				ObjectStore.push_back(testObject);
				//SdkUiConsoleWrite("[added Here] %s %d \n", pSDK->EntityManager->GetObjectFromPTR(Object)->GetName(), netID);

				//SdkUiConsoleWrite("[NetID2] %d \n", netID);


			}

		}
	}
	else
	{
		/*
		bool bDead;
		SdkIsObjectDead(AI, &bDead);
		if (bDead)
			return true;*/
		const char* test = nullptr;

		SdkGetObjectName(AI, &test);



		std::string s1(test);





		ObjectStore.erase(std::remove_if(ObjectStore.begin(),
			ObjectStore.end(),
			[&](const drawCollector stopPoint)-> bool
		{
			unsigned int netID;
			SdkGetObjectNetworkID(AI, &netID);

			if (stopPoint.netId == netID)
			{
				//SdkUiConsoleWrite("[buffcre] deleted %d \n", netID);
				return true;
			}
			float Time;
			SdkGetGameTime(&Time); // get the current game time

			return stopPoint.CastedTime + stopPoint.TimeLeft < Time;
		}),
			ObjectStore.end());


		//ObjectStore.erase(std::remove_if(bAbilitySettings.begin(), bAbilitySettings.end(),
			//	[&element](const menuCreator& x) { return std::strcmp(x.champName, element.champName) == 0; });))
	}
}


