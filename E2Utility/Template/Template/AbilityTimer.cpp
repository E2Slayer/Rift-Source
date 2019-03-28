#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include "AbilityTimer.h"
#include "DrawHelper.h"
#include "DropLists.h"

std::map<const char*, AbilityItem> AbilitiesMenuMap; //for create all menu even though not all champions are in the match

std::vector<const char*> ChampionMenuList; //Menu tree creator which is not duplicated champion list

std::map<const char*, AbilityItem> AbilitiesMap; //Actual Map for ability list that is detemined by champion list in the match

std::vector< AbilityDraw> AbilityDrawing; // Drawing list



void AbilityTimer::InitLoader()
{
	AbilitiesMenuMap.clear();

	AbilitiesMap.clear();
	AbilityDrawing.clear();

	ChampionMenuList.clear();

	//AbilityItem(const char* _Name, const char* _ChampName, float _Time, const char* _MenuID)




	AbilitiesMenuMap.emplace("ItemZhonya_Base_stasis.troy", AbilityItem("Zhonya's Hourglass", "Misc", 2.5f, "Zhonya", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::OnTimeDelete));
	AbilitiesMenuMap.emplace("LifeAura.troy", AbilityItem("Guardian Angel", "Misc", 4.0f, "GuardianAngel", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::OnTimeDelete));

	AbilitiesMenuMap.emplace("SummonerTeleport", AbilityItem("Teleport (Caster)", "Misc", 4.0f, "Teleport1", AbilityType::OnBuffCreate, AbilityLocationType::Static, DeleteCondition::EarlyDelete));
	AbilitiesMenuMap.emplace("global_ss_teleport_target_blue.troy", AbilityItem("Teleport (Ally Arrival Position)", "Misc", 4.0f, "Teleport2", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::EarlyDelete));
	AbilitiesMenuMap.emplace("global_ss_teleport_target_red.troy", AbilityItem("Teleport (Enemy Arrival Position)", "Misc", 4.0f, "Teleport3", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::EarlyDelete));


	AbilitiesMenuMap.emplace("aatroxrrevive", AbilityItem("Aatrox R (Revive)", "Aatrox", 3.25f, "AatroxR", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::OnTimeDelete));

	AbilitiesMenuMap.emplace("Akali_Base_W_Cas", AbilityItem("Akali W", "Akali", 5.0f, "AkaliW", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::OnTimeDelete));
	AbilitiesMenuMap.emplace("FerociousHowl", AbilityItem("Alistar R", "Alistar", 7.0f, "AlistarR", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::OnTimeDelete));

	AbilitiesMenuMap.emplace("rebirth", AbilityItem("Anivia Egg", "Anivia", 6.0f, "AniviaPassive", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::EarlyDelete));

	AbilitiesMenuMap.emplace("braumeshieldbuff", AbilityItem("Braum E", "Braum", 3.0f, "BraumE", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::OnTimeDelete));

	AbilitiesMenuMap.emplace("Ekko_Base_W_Indicator", AbilityItem("Ekko W ", "Ekko", 3.0f, "EkkoW", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::OnTimeDelete));


	AbilitiesMenuMap.emplace("Elise_Base_Spider_E_indicator_green", AbilityItem("Elise Spider E (Ally)", "Elise", 2.5f, "EliseE", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::EarlyDelete));
	AbilitiesMenuMap.emplace("Elise_Base_Spider_E_indicator_red", AbilityItem("Elise Spider E (Enemy)", "Elise", 2.5f, "EliseE2", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::EarlyDelete));


	AbilitiesMenuMap.emplace("Crowstorm", AbilityItem("FiddleSticks R (Casting)", "FiddleSticks", 5.0f, "FiddleSticksRCast", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::OnTimeDelete));

	AbilitiesMenuMap.emplace("FioraW", AbilityItem("Fiora W", "Fiora", 0.75f, "FioraW", AbilityType::OnBuffCreate, AbilityLocationType::Static, DeleteCondition::OnTimeDelete));

	AbilitiesMenuMap.emplace("fiorarmark", AbilityItem("Fiora R (On The Target)", "Fiora", 5.0f, "FioraR", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::OnTimeDelete));

	AbilitiesMenuMap.emplace("fizzeicon", AbilityItem("Fizz E", "Fizz", 1.5f, "FizzE", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::EarlyDelete));

	AbilitiesMenuMap.emplace("Fizz_Base_R_Ring_Green", AbilityItem("Fizz R (Ground) (Ally)", "Fizz", 2.0f, "FizzR1", AbilityType::OnCreate, AbilityLocationType::Dynamic, DeleteCondition::EarlyDelete));
	AbilitiesMenuMap.emplace("Fizz_Base_R_Ring_Red", AbilityItem("Fizz R (Ground) (Enemy)", "Fizz", 2.0f, "FizzR2", AbilityType::OnCreate, AbilityLocationType::Dynamic, DeleteCondition::EarlyDelete));

	AbilitiesMenuMap.emplace("fizzrbomb", AbilityItem("Fizz R (Target)", "Fizz", 2.0f, "FizzR3", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::OnTimeDelete));



	AbilitiesMenuMap.emplace("Gangplank_Base_R_AoE_Green", AbilityItem("Gangplank R (Ally)", "Gangplank", 8.0f, "GangplankR", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::OnTimeDelete));

	AbilitiesMenuMap.emplace("Gangplank_Base_R_AoE_Red", AbilityItem("Gangplank R (Enemy)", "Gangplank", 8.0f, "GangplankR2", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::OnTimeDelete));

	//AbilitiesMenuMap.emplace("heimerdingersuperturret", AbilityItem("Heimerdinger R (Upgraded Q)", "Heimerdinger", 8.0f, "HeimerdingerRQ", AbilityType::OnBuffCreate, AbilityLocationType::Static, DeleteCondition::EarlyDelete));

	AbilitiesMenuMap.emplace("Janna_Base_R_cas_Green", AbilityItem("Janna R (Ally)", "Janna", 3.0f, "JannaR", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::EarlyDelete));
	AbilitiesMenuMap.emplace("Janna_Base_R_cas_Red", AbilityItem("Janna R (Enemy)", "Janna", 3.0f, "JannaR2", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::EarlyDelete));

	AbilitiesMenuMap.emplace("JaxCounterStrike", AbilityItem("Jax E", "Jax", 2.0f, "JaxE", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::EarlyDelete));

	AbilitiesMenuMap.emplace("karthusfallenonecastsound", AbilityItem("Karthus R (Caster)", "Karthus", 3.0f, "KarthusR", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::EarlyDelete));
	AbilitiesMenuMap.emplace("karthusfallenonetarget", AbilityItem("Karthus R (Target)", "Karthus", 3.0f, "KarthusR2", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::EarlyDelete));


	AbilitiesMenuMap.emplace("KayleR", AbilityItem("Kayle R", "Kayle", 3.0f, "KayleR", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::OnTimeDelete));

	AbilitiesMenuMap.emplace("KennenShurikenStorm", AbilityItem("Kennen R", "Kennen", 3.0f, "KennenR", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::EarlyDelete));


	AbilitiesMenuMap.emplace("Kindred_Base_R_AOE", AbilityItem("Kindred R", "Kindred", 4.0f, "KindredR", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::OnTimeDelete));


	AbilitiesMenuMap.emplace("LeBlanc_Base_W_return_indicator", AbilityItem("Leblanc W", "Leblanc", 4.0f, "LeblancW", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::EarlyDelete));

	AbilitiesMenuMap.emplace("LeBlanc_Base_RW_return_indicator", AbilityItem("Leblanc RW", "Leblanc", 4.0f, "LeblancRW", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::EarlyDelete));


	AbilitiesMenuMap.emplace("malzaharrsound", AbilityItem("Malzahar R (Caster)", "Malzahar", 2.5f, "MalzaharR", AbilityType::OnBuffCreate, AbilityLocationType::Static, DeleteCondition::EarlyDelete));
	AbilitiesMenuMap.emplace("MalzaharR", AbilityItem("Malzahar R (Target)", "Malzahar", 2.5f, "MalzaharR2", AbilityType::OnBuffCreate, AbilityLocationType::Static, DeleteCondition::EarlyDelete));


	AbilitiesMenuMap.emplace("MorganaE", AbilityItem("Morgana E", "Morgana", 5.0f, "MorganaE", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::EarlyDelete));
	AbilitiesMenuMap.emplace("MorganaR", AbilityItem("Morgana R", "Morgana", 3.0f, "MorganaR", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::OnTimeDelete));

	AbilitiesMenuMap.emplace("NeekoR", AbilityItem("Neeko R", "Neeko", 1.25f, "NeekoR", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::OnTimeDelete));
	AbilitiesMenuMap.emplace("neekor2", AbilityItem("Neeko R2", "Neeko", 1.25f, "NeekoR2", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::OnTimeDelete));

	AbilitiesMenuMap.emplace("NunuW", AbilityItem("Nunu W", "Nunu", 10.0f, "NunuW", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::EarlyDelete));
//	AbilitiesMenuMap.emplace("Nunu_Base_R_indicator_blue", AbilityItem("Nunu R (Ally)", "Nunu", 3.0f, "NunuR", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::OnTimeDelete));
//	AbilitiesMenuMap.emplace("Nunu_Base_R_indicator_red", AbilityItem("Nunu R (Enemy)", "Nunu", 3.0f, "NunuR2", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::OnTimeDelete));


	AbilitiesMenuMap.emplace("Sadism", AbilityItem("DrMundo R", "DrMundo", 12.0f, "DrMundoR", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::OnTimeDelete));
	
	AbilitiesMenuMap.emplace("SivirE", AbilityItem("Sivir E", "Sivir", 1.5f, "SivirE", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::EarlyDelete));


	AbilitiesMenuMap.emplace("Taric_Base_R_Indicator", AbilityItem("Taric R (Self)", "Taric", 2.5f, "TaricR1", AbilityType::OnCreate, AbilityLocationType::Dynamic, DeleteCondition::OnTimeDelete));

	//AbilitiesMenuMap.emplace("taricrindicator", AbilityItem("Taric R (The Same Team)", "Taric", 2.5f, "TaricR2", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::OnTimeDelete));
	AbilitiesMenuMap.emplace("TaricR", AbilityItem("Taric R (Invulnerable)", "Taric", 2.5f, "TaricR3", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::OnTimeDelete));


	AbilitiesMenuMap.emplace("Thresh_Base_Lantern", AbilityItem("Thresh W", "Thresh", 6.0f, "ThreshW", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::EarlyDelete));


	AbilitiesMenuMap.emplace("OlafRagnarok", AbilityItem("Olaf R", "Olaf", 6.0f, "OlafR", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::EarlyDelete));


	AbilitiesMenuMap.emplace("PantheonRJump", AbilityItem("Pantheon R (Caster)", "Pantheon", 2.0f, "PantheonR", AbilityType::OnBuffCreate, AbilityLocationType::Static, DeleteCondition::EarlyDelete));
	AbilitiesMenuMap.emplace("Pantheon_Base_R_indicator_green", AbilityItem("Pantheon R (Ally)", "Pantheon", 4.5f, "PantheonR1", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::EarlyDelete));
	AbilitiesMenuMap.emplace("Pantheon_Base_R_indicator_red", AbilityItem("Pantheon R (Enemy)", "Pantheon", 2.5f, "PantheonR2", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::EarlyDelete));


	AbilitiesMenuMap.emplace("shenrchannelbuffbar", AbilityItem("Shen R (Caster)", "Shen", 3.0f, "ShenR", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::EarlyDelete));

	AbilitiesMenuMap.emplace("shenrtargettracker", AbilityItem("Shen R (Target)", "Shen", 3.0f, "ShenR2", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::EarlyDelete));

	AbilitiesMenuMap.emplace("Ryze_Base_R_Start_Ally", AbilityItem("Ryze R (Ally Started Position)", "Ryze", 2.0f, "RyzeR1", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::OnTimeDelete));
	AbilitiesMenuMap.emplace("Ryze_Base_R_End_Ally", AbilityItem("Ryze R (Ally Arrival Position)", "Ryze", 3.0f, "RyzeR2", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::OnTimeDelete));
	AbilitiesMenuMap.emplace("Ryze_Base_R_Start_Enemy", AbilityItem("Ryze R (Enemy Started Position)", "Ryze", 2.0f, "RyzeR3", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::OnTimeDelete));
	AbilitiesMenuMap.emplace("Ryze_Base_R_End_Enemy", AbilityItem("Ryze R (Enemy Arrival Position)", "Ryze", 3.0f, "RyzeR4", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::OnTimeDelete));

	AbilitiesMenuMap.emplace("UndyingRage", AbilityItem("Tryndamere R", "Tryndamere", 5.0f, "TryndamereR", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::OnTimeDelete));


	AbilitiesMenuMap.emplace("Yasuo_Base_W_windwall1", AbilityItem("Yasuo W (Level1)", "Yasuo", 4.0f, "YasuoW1", AbilityType::OnCreate, AbilityLocationType::Dynamic, DeleteCondition::OnTimeDelete));
	AbilitiesMenuMap.emplace("Yasuo_Base_W_windwall2", AbilityItem("Yasuo W (Level2)", "Yasuo", 4.0f, "YasuoW2", AbilityType::OnCreate, AbilityLocationType::Dynamic, DeleteCondition::OnTimeDelete));
	AbilitiesMenuMap.emplace("Yasuo_Base_W_windwall3", AbilityItem("Yasuo W (Level3)", "Yasuo", 4.0f, "YasuoW3", AbilityType::OnCreate, AbilityLocationType::Dynamic, DeleteCondition::OnTimeDelete));
	AbilitiesMenuMap.emplace("Yasuo_Base_W_windwall4", AbilityItem("Yasuo W (Level4)", "Yasuo", 4.0f, "YasuoW4", AbilityType::OnCreate, AbilityLocationType::Dynamic, DeleteCondition::OnTimeDelete));
	AbilitiesMenuMap.emplace("Yasuo_Base_W_windwall5", AbilityItem("Yasuo W (Level5)", "Yasuo", 4.0f, "YasuoW5", AbilityType::OnCreate, AbilityLocationType::Dynamic, DeleteCondition::OnTimeDelete));



	AbilitiesMenuMap.emplace("Viktor_Catalyst_green", AbilityItem("Viktor W (Ally)", "Viktor", 4.0f, "ViktorW", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::OnTimeDelete));
	AbilitiesMenuMap.emplace("Viktor_Catalyst_red", AbilityItem("Viktor W (Enemy)", "Viktor", 4.0f, "ViktorW2", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::OnTimeDelete));
	
	AbilitiesMenuMap.emplace("VladimirSanguinePool", AbilityItem("Vladimir W", "Vladimir", 2.0f, "VladimirW", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::OnTimeDelete));


	/*
			AbilityDictionary.push_back(abilityTrackList("Ryze", "Ryze R (Ally Caster)", "Ryze_Base_R_Start_Ally", 2.0f, "MataView_RyzeR", true, false));
	AbilityDictionary.push_back(abilityTrackList("Ryze", "Ryze R (Ally Direction)", "Ryze_Base_R_End_Ally", 3.0f, "MataView_RyzeR2", true, false));
	AbilityDictionary.push_back(abilityTrackList("Ryze", "Ryze R (Enemy Caster)", "Ryze_Base_R_Start_Enemy", 2.0f, "MataView_RyzeR3", true, false));
	AbilityDictionary.push_back(abilityTrackList("Ryze", "Ryze R (Enemy Direction)", "Ryze_Base_R_End_Enemy", 3.0f, "MataView_RyzeR4", true, false));

	
	*/



	AbilitiesMenuMap.emplace("Veigar_Base_E_cage_green", AbilityItem("Veigar E (Ally)", "Veigar", 3.0f, "VeigarEAlly", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::OnTimeDelete));
	AbilitiesMenuMap.emplace("Veigar_Base_W_cas_green", AbilityItem("Veigar W (Ally)", "Veigar", 1.2f, "VeigarWAlly", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::OnTimeDelete));
	AbilitiesMenuMap.emplace("Veigar_Base_E_cage_red", AbilityItem("Veigar E (Enemy)", "Veigar", 3.0f, "VeigarEEnemy", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::OnTimeDelete));
	AbilitiesMenuMap.emplace("Veigar_Base_W_cas_red", AbilityItem("Veigar W (Enemy)", "Veigar", 1.2f, "VeigarWEnemy", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::OnTimeDelete));



	AbilitiesMenuMap.emplace("Zed_Base_W_cloneswap_buf", AbilityItem("Zed W (Shadow)", "Zed", 5.5f, "ZedW", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::EarlyDelete));
	AbilitiesMenuMap.emplace("Zed_Base_R_cloneswap_buf", AbilityItem("Zed R (Shadow)", "Zed", 7.5f, "ZedR", AbilityType::OnCreate, AbilityLocationType::Static, DeleteCondition::EarlyDelete));
	AbilitiesMenuMap.emplace("zedrtargetmark", AbilityItem("Zed R (Target Mark)", "Zed", 3.0f, "ZedR2", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::OnTimeDelete));



	//AbilityDictionary.push_back(abilityTrackList("Zed", "Zed W", "Zed_Base_W_cloneswap_buf", 5.0f, "MataView_ZedW", true, false));
	//AbilityDictionary.push_back(abilityTrackList("Zed", "Zed R", "Zed_Base_R_cloneswap_buf", 6.0f, "MataView_ZedR", true, false));


	AbilitiesMenuMap.emplace("ChronoShift", AbilityItem("Zilean R", "Zilean", 4.0f, "ZileanR", AbilityType::OnBuffCreate, AbilityLocationType::Dynamic, DeleteCondition::EarlyDelete));





	//GuardianAngel_tar.troy

	pSDK->EventHandler->RegisterCallback(CallbackEnum::CreateObject, AbilityTimer::OnCreate);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::DeleteObject, AbilityTimer::OnDelete);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::BuffCreateAndDelete, AbilityTimer::OnBuffCreateAndDelete);


	//BuffCreateAndDelete
		//AbilityItem





	auto ally{ pSDK->EntityManager->GetAllyHeroes() };
	auto enemy{ pSDK->EntityManager->GetEnemyHeroes() };

	if (ally.empty() && enemy.empty())
	{
		return;
	}


	if (!ally.empty())
	{
		for (auto &[netID, hero] : ally)
		{
			if (hero != nullptr && hero != NULL)
			{
				for (auto const& value : AbilitiesMenuMap)
				{
					if (strcmp(hero->GetCharName(), value.second.ChampName) == 0)
					{

						AbilitiesMap.emplace(value.first, value.second);

					}
				}
			}
		}
	}

	if (!enemy.empty())
	{
		for (auto &[netID, hero] : enemy)
		{
			if (hero != nullptr && hero != NULL)
			{
				for (auto const& value : AbilitiesMenuMap)
				{
					if (strcmp(hero->GetCharName(), value.second.ChampName) == 0)
					{

						AbilitiesMap.emplace(value.first, value.second);

					}
				}
			
			}
		}
	}

	for (auto const& value : AbilitiesMenuMap)
	{
		if (_stricmp(value.second.ChampName, "Misc") == 0)
		{
			//SdkUiConsoleWrite("added %s <- First (%d Type )  Menu %d", value.first, int(value.second.Type), Menu::Get<bool>("Trackers.AbilityTimer.AbilityList." + std::string(value.second.MenuID)));

			AbilitiesMap.emplace(value.first, value.second);

		}
	}





	/*
	std::sort(AbilitiesMenuMap.begin(), AbilitiesMenuMap.end(), [](const AbilityItem& lhs, const AbilityItem& rhs) {
		return std::strcmp(lhs.ChampName, rhs.ChampName) < 0;
	});
	*/


	for (auto &element : AbilitiesMenuMap) // access by reference to avoid copying
	{

		std::string t1(element.second.ChampName);

		auto it = std::find_if(
			ChampionMenuList.begin(), ChampionMenuList.end(),
			[&element](auto const& x) { return std::strcmp(x, element.second.ChampName) == 0; });
		
		if (it == ChampionMenuList.end())
		{
			ChampionMenuList.push_back(element.second.ChampName);
		}


	}






	//SdkUiConsoleWrite("Player Char name %s", Player.GetCharName());



}


void AbilityTimer::MenuLoader()
{
	Menu::Tree("Ability Timer", "Trackers.AbilityTimer", false, []()
	{
		Menu::Checkbox("Use Ability Timer", "Trackers.AbilityTimer.Use", true);
;

		Menu::SliderInt("Ability Font Size", "Trackers.AbilityTimer.FontSize", 34, 20, 40);
		Menu::DropList("^-> Timer Color", "Trackers.AbilityTimer.Color", ColorMenuList, 11);
		Menu::DropList("^-> Timer OutLine Color", "Trackers.AbilityTimer.OutLineColor", ColorMenuList, 0);

		Menu::SliderInt("Ability Timer Position X-axis", "Trackers.AbilityTimer.DrawingX", 0, -200, 200);
		Menu::SliderInt("Ability Timer Position Y-axis", "Trackers.AbilityTimer.DrawingY", -30, -200, 200);
	
		/*
		Menu::Checkbox("Don't Use Last Position Checker", "Drawings.GankAlerter.LastPos", false);
		SdkUiText("^-> If you turn it on, Gank Alerter is triggered every time a tracked champion is visible again        ");
		SdkUiText("^-> Might be little bit annoying but some people prefer it so I added it");
		*/


		Menu::Tree("Ability List", "Trackers.AbilityTimer.AbilityList", false, []()
		{
			for (auto& element1 : ChampionMenuList) // access by reference to avoid copying
			{
				//if (std::strcmp(element1, "Misc") != 0)
				{

					Menu::Tree(element1, ("Trackers.AbilityTimer.AbilityList." + std::string(element1)), false, [element1]()
					{
						for (auto &element2 : AbilitiesMenuMap) // access by reference to avoid copying
						{
							if (std::strcmp(element1, element2.second.ChampName) == 0)
							{
								std::string temp = "Trackers.AbilityTimer.AbilityList." + std::string(element2.second.MenuID);


								Menu::Checkbox(element2.second.Name, temp, true);
							}
						}
					});
				}
			}
		});

		/*
		Menu::Tree("Misc List", "Trackers.AbilityTimer.MiscList", false, []()
		{
			for (auto &element2 : AbilitiesMenuMap) // access by reference to avoid copying
			{
				if (std::strcmp("Misc", element2.second.ChampName) == 0)
				{
					//std::string tempStr = "Trackers.AbilityTimer.AbilityList." + std::string(element2.second.MenuID);
					//SdkUiConsoleWrite("Added %s", tempStr);

					std::string temp = "Trackers.AbilityTimer.AbilityList." + std::string(element2.second.MenuID);

					temp += element2.second.MenuID;

					Menu::Checkbox(element2.second.Name, temp, true);
				}
			}
		});
		*/

	});
}

void AbilityTimer::TickLoader()
{
	if (!Menu::Get<bool>("Trackers.AbilityTimer.Use"))
	{

		return;
	}


	bool menu = Menu::Get<bool>("Trackers.AbilityTimer.AbilityList.Zhonya");
	//SdkUiConsoleWrite("neeeeeee %d", menu);

	if (!AbilityDrawing.empty())
	{
		auto i = std::begin(AbilityDrawing);

		while (i != std::end(AbilityDrawing))
		{
			// Do some stuff
			if (i->End + 10.0f < Game::Time())
			{
				i = AbilityDrawing.erase(i);
			}
			else
			{
				++i;
			}
		}
	}
}

void AbilityTimer::DrawLoader()
{


	if (!Menu::Get<bool>("Trackers.AbilityTimer.Use"))
	{

		return;
	}


	if (Game::IsOverlayOpen())
	{
		Vector2 screenPos{ Renderer::WorldToScreen(Player.GetPosition()) };
		screenPos.x += (float)Menu::Get<int>("Trackers.AbilityTimer.DrawingX");
		screenPos.y += (float)Menu::Get<int>("Trackers.AbilityTimer.DrawingY");

		DrawHelper::DrawOutlineText(NULL, &screenPos, "0.0", "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Trackers.AbilityTimer.Color")), Menu::Get<int>("Trackers.AbilityTimer.FontSize"), 10, 0,
			&DropLists::GetColor(Menu::Get<int>("Trackers.AbilityTimer.OutLineColor")), false);


	}
	else
	{
		if (!AbilityDrawing.empty())
		{

			for (auto & value : AbilityDrawing)
			{
				if (value.End > Game::Time() && value.Position.IsValid() && value.Position.IsOnScreen())
				{


					Vector2 screenPos{ Renderer::WorldToScreen(value.Position) };

					if (value.Object->GetPosition().IsValid() && value.Object->GetPosition().IsOnScreen())
					{

						if (value.IsDynamic)
						{
							screenPos = Renderer::WorldToScreen(value.Object->GetPosition());
						}
						screenPos.x += (float)Menu::Get<int>("Trackers.AbilityTimer.DrawingX");
						screenPos.y += (float)Menu::Get<int>("Trackers.AbilityTimer.DrawingY");


						std::stringstream ss1;
						ss1.precision(1); //for decimal
						ss1.setf(std::ios_base::fixed, std::ios_base::floatfield);

						ss1 << value.End - Game::Time();

						//Draw::LineScreen(&screenPos, &Vector2(screenPos.x + 30.0f, screenPos.y), 30.0f, &Color::White);

						DrawHelper::DrawOutlineText(NULL, &screenPos, ss1.str().c_str(), "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Trackers.AbilityTimer.Color")), Menu::Get<int>("Trackers.AbilityTimer.FontSize"), 10, 0,
							&DropLists::GetColor(Menu::Get<int>("Trackers.AbilityTimer.OutLineColor")), false);

					}
					//Draw::Text(NULL, &screenPos, ss1.str(), "Calibri Bold", &Color::White, 30, 10);
				}

			}
		}
	}


}



bool AbilityTimer::OnDelete(void* Object, unsigned int NetworkID, void* UserData)
{
	UNREFERENCED_PARAMETER(UserData);
	UNREFERENCED_PARAMETER(NetworkID);

	if (!Menu::Get<bool>("Trackers.AbilityTimer.Use"))
	{

		AbilityDrawing.clear();
		return false;
	}

	if (AbilitiesMap.empty())
	{
		return false;
	}

	auto sender = pSDK->EntityManager->GetObjectFromPTR(Object);
	if (Object == nullptr || sender == nullptr || sender == NULL || NetworkID == 0 || !sender->IsParticle()) //|| !sender->IsParticle()
	{
		return false;
	}

	auto objectName = sender->GetName();


	if (objectName == NULL || objectName == nullptr)
	{
		return false;
	}

	const char *p = strstr(objectName, "SRU_");
	const char *m = strstr(objectName, "Minion_");
	if (p || m)
	{
		return false;
	}

	//SdkUiConsoleWrite("AbilityTimer Delete %s", objectName);


	auto i = std::begin(AbilityDrawing);

	while (i != std::end(AbilityDrawing))
	{
		// Do some stuff
		if (i->End < Game::Time() && i->Object->GetNetworkID() == sender->GetNetworkID())
		{
			i = AbilityDrawing.erase(i);
		}
		else if (i->Object->GetNetworkID() == sender->GetNetworkID() && i->DeleteEarly && strcmp(objectName, i->TargetName) == 0)
		{
			i = AbilityDrawing.erase(i);
		}
		else
		{
			++i;
		}
	}

}

bool AbilityTimer::OnCreate(void* Object, unsigned int NetworkID, void* UserData)
{
	UNREFERENCED_PARAMETER(UserData);
	UNREFERENCED_PARAMETER(NetworkID);
	if (!Menu::Get<bool>("Trackers.AbilityTimer.Use"))
	{

		AbilityDrawing.clear();
		return false;
	}


	if (AbilitiesMap.empty())
	{
		return false;
	}

	auto sender = pSDK->EntityManager->GetObjectFromPTR(Object);
	if (Object == nullptr || sender == nullptr || sender == NULL || NetworkID == 0 )
	{
		return false;
	}

	auto objectName = sender->GetName();


	auto objectPos = sender->GetPosition();

	if (objectName == NULL || objectName == nullptr || !objectPos.IsValid())
	{
		return false;
	}

	const char *p = strstr(objectName, "SRU_");
	const char *m = strstr(objectName, "Minion_");
	const char *t = strstr(objectName, "Turret");
	if (p || m || t)
	{
		return false;
	}


	std::string findSkin = objectName;

	size_t found = findSkin.find("Skin");
	if (found != std::string::npos)
	{
		std::string FirstPart = findSkin.substr(0, found);

		std::string SecondPart = findSkin.substr(found+6);

		FirstPart += "Base";

		FirstPart += SecondPart;

		findSkin = FirstPart;
		//SdkUiConsoleWrite("AbilityTimer Changed Create %s", FirstPart);

	}

	//SdkUiConsoleWrite("AbilityTimer Create %s", objectName);
	//SdkUiConsoleWrite("AbilityTimer Create %s", findSkin);

	
	
	for (auto const& value : AbilitiesMap)
	{
		
		//std::string tempStr = "Trackers.AbilityTimer.AbilityList." + std::string(value.second.MenuID);
		//SdkUiConsoleWrite("first %s", tempStr);
		bool menu = Menu::Get<bool>("Trackers.AbilityTimer.AbilityList." + std::string(value.second.MenuID));

		if (_stricmp(value.first, findSkin.c_str()) == 0 && int(value.second.Type) == int(AbilityType::OnCreate) && menu)
		{
			// && int(value.second.Type) == int(AbilityType::OnCreate) && Menu::Get<bool>("Trackers.AbilityTimer.AbilityList." + std::string(value.second.MenuID)))
			bool isDynamic = false;
			bool isEarlyDelete = false;

			if (value.second.LocationType == AbilityLocationType::Dynamic)
			{
				isDynamic = true;
			}

			if (value.second.DeleteEarly == DeleteCondition::EarlyDelete)
			{
				isEarlyDelete = true;
			}
			//SdkUiConsoleWrite("AbilityTimer Create 2222222%s", objectName);
			AbilityDrawing.emplace_back(AbilityDraw(objectPos, sender, Game::Time() + value.second.Time, isDynamic, isEarlyDelete, objectName));
		}
	}


	return true;
}



void AbilityTimer::OnBuffCreateAndDelete(void* AI, bool Created, unsigned char Type, float StartTime, float EndTime, const char* Name, void* CasterObject, unsigned int CasterID, int Stacks, bool HasCount, int Count, PSDK_SPELL Spell, void* UserData)
{

	//Created 0 = delete / Created = 1 created
	UNREFERENCED_PARAMETER(UserData);
	UNREFERENCED_PARAMETER(Type);
	UNREFERENCED_PARAMETER(CasterObject);
	UNREFERENCED_PARAMETER(CasterID);
	UNREFERENCED_PARAMETER(Stacks);
	UNREFERENCED_PARAMETER(HasCount);
	UNREFERENCED_PARAMETER(Count);
	UNREFERENCED_PARAMETER(Spell);
	if (!Menu::Get<bool>("Trackers.AbilityTimer.Use"))
	{

		AbilityDrawing.clear();
		return ;
	}


	auto sender = pSDK->EntityManager->GetObjectFromPTR(AI);
	if (AI == nullptr || sender == nullptr || sender == NULL || Name == NULL || EndTime - StartTime < 0 || EndTime < 0)
	{
		return;
	}

	if (AI == nullptr || AI == NULL)
	{
		return;
	}


	if (Name == nullptr || Name == NULL)
	{
		return;
	}

	//SdkUiConsoleWrite("AbilityTimer Buff %s", Name);
	//SdkUiConsoleWrite("AbilityTimer Buff Deleted? %d", Created);


	if (Created == 1)
	{
		for (auto const& value : AbilitiesMap)
		{
			if (_stricmp(value.first, Name) == 0 && value.second.Type == AbilityType::OnBuffCreate && Menu::Get<bool>("Trackers.AbilityTimer.AbilityList." + std::string(value.second.MenuID)))
			{
				bool isDynamic = false;
				bool isEarlyDelete = false;

				if (value.second.LocationType == AbilityLocationType::Dynamic)
				{
					isDynamic = true;
				}

				if (value.second.DeleteEarly == DeleteCondition::EarlyDelete)
				{
					isEarlyDelete = true;
				}

				if (EndTime > Game::Time() + value.second.Time)
				{
					if (EndTime - StartTime < 100.0f)
					{
						AbilityDrawing.emplace_back(AbilityDraw(sender->GetPosition(), sender, EndTime, isDynamic, isEarlyDelete, Name));
					}
				}
				else
				{
					AbilityDrawing.emplace_back(AbilityDraw(sender->GetPosition(), sender, Game::Time() + value.second.Time, isDynamic, isEarlyDelete, Name));
				}

				//AbilityDrawing.emplace_back(AbilityDraw(sender->GetPosition(), sender, Game::Time() + value.second.Time, isDynamic, isEarlyDelete));
			}
		}
	}
	else if (Created == 0)
	{
		auto i = std::begin(AbilityDrawing);

		while (i != std::end(AbilityDrawing))
		{
			// Do some stuff
			if (i->End < Game::Time() && i->Object->GetNetworkID() == sender->GetNetworkID())
			{
				i = AbilityDrawing.erase(i);
			}
			else if (i->Object->GetNetworkID() == sender->GetNetworkID() && i->DeleteEarly && strcmp(Name, i->TargetName) == 0)
			{
				i = AbilityDrawing.erase(i);
			}
			else
			{
				++i;
			}
		}
	}
}