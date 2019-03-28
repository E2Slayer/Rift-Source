#include "stdafx.h"
#include <string.h>
#include "CooldownTracker.h"
#include <windows.h>
#include "DrawHelper.h"
#include <iomanip>
#include "DropLists.h"

//SdkDrawSpriteFromResource(MAKEINTRESOURCEA(102), &barPos, false); //312x8

/*
	const char* Champ;
	const char* SpellName;
	SpellSlot Slot;
	float& Cooldowns;
	float Additional;
*/




std::vector<ManualSpell> ManualSpellLists;

std::vector<ManualSpell> ActualManualSpellLists;

std::map<unsigned int, AIHeroClient*> _spellDatas;


//std::map<unsigned int, std::vector<_SDK_SPELL>> _summonerDatas;

//const SpellSlot _spellSlots[] { SpellSlot::Q, SpellSlot::W, SpellSlot::E, SpellSlot::R };
//const SpellSlot _summonerSlots[] { SpellSlot::Summoner1, SpellSlot::Summoner2 };

void CooldownTracker::Init()
{


	_spellDatas.clear();
	ManualSpellLists.clear();
	//10 / 9.5 / 9 / 8.5 / 8
	float skillcds[] = { 10.0f, 9.5f, 9.0f, 8.5f, 8.0f };
	ManualSpellLists.emplace_back(ManualSpell("Lux", "LuxLightStrikeKugel", SpellSlot::E, skillcds, 0.0f, false));

	skillcds[0] = 11.0f;
	skillcds[1] = 11.0f;
	skillcds[2] = 11.0f;
	skillcds[3] = 11.0f;
	skillcds[4] = 11.0f;

	ManualSpellLists.emplace_back(ManualSpell("Gragas", "GragasQ", SpellSlot::Q, skillcds, 0.0f, false));


	skillcds[0] = 16.0f;
	skillcds[1] = 14.5f;
	skillcds[2] = 13.0f;
	skillcds[3] = 11.5f;
	skillcds[4] = 11.0f;
	ManualSpellLists.emplace_back(ManualSpell("Rengar", "RengarW", SpellSlot::W, skillcds, 0.0f, false));


	skillcds[0] = 16.0f;
	skillcds[1] = 15.0f;
	skillcds[2] = 14.0f;
	skillcds[3] = 13.0f;
	skillcds[4] = 12.0f;
	ManualSpellLists.emplace_back(ManualSpell("Aatrox", "AatroxQ3", SpellSlot::Q, skillcds, 0.0f, false));



	skillcds[0] = 160.0f;
	skillcds[1] = 140.0f;
	skillcds[2] = 120.0f;
	skillcds[3] = 120.0f;
	skillcds[4] = 120.0f;
	ManualSpellLists.emplace_back(ManualSpell("Aatrox", "AatroxR", SpellSlot::R, skillcds, 10.0f, false));


	/*
	skillcds[0] = 6.0f;
	skillcds[1] = 6.0f;
	skillcds[2] = 6.0f;
	skillcds[3] = 6.0f;
	skillcds[4] = 6.0f;
	ManualSpellLists.emplace_back(ManualSpell("TwistedFate", "PickACard", SpellSlot::W, skillcds, 0.0f, false));
	*/

	skillcds[0] = 6.0f;
	skillcds[1] = 6.0f;
	skillcds[2] = 6.0f;
	skillcds[3] = 6.0f;
	skillcds[4] = 6.0f;
	ManualSpellLists.emplace_back(ManualSpell("Rumble", "RumbleGrenade", SpellSlot::E, skillcds, 0.0f, false));

	/*
	skillcds[0] = 16.0f;
	skillcds[1] = 14.5f;
	skillcds[2] = 13.0f;
	skillcds[3] = 11.5f;
	skillcds[4] = 10.0f;
	ManualSpellLists.emplace_back(ManualSpell("Fizz", "FizzE", SpellSlot::E, skillcds, 0.0f, false));
	*/
	// 24 / 21 / 18 / 15 / 12
	skillcds[0] = 24.0f;
	skillcds[1] = 21.0f;
	skillcds[2] = 18.0f;
	skillcds[3] = 15.0f;
	skillcds[4] = 12.0f;
	ManualSpellLists.emplace_back(ManualSpell("Ziggs", "ZiggsW", SpellSlot::W, skillcds, 0.0f, false));

	/*
	skillcds[0] = 13.0f;
	skillcds[1] = 13.0f;
	skillcds[2] = 13.0f;
	skillcds[3] = 13.0f;
	skillcds[4] = 13.0f;
	ManualSpellLists.emplace_back(ManualSpell("Riven", "RivenTriCleave", SpellSlot::Q, skillcds, 0.0f, false));
*/

	/*
	skillcds[0] = 120.0f;
	skillcds[1] = 90.0f;
	skillcds[2] = 60.0f;
	skillcds[3] = 60.0f;
	skillcds[4] = 60.0f;
	ManualSpellLists.emplace_back(ManualSpell("Riven", "RivenFengShuiEngine", SpellSlot::R, skillcds, 15.0f, false));
	*/
	//RengarW


	/*
	if (Menu::Get<bool>("Trackers.CooldownTracker.Myself"))
	{
		_spellDatas.try_emplace(Player.GetNetworkID(), Player.AsAIHeroClient());
	}
	*/
	//SdkUiConsoleWrite(" aaaaaadded 12243");

	auto ally1{ pSDK->EntityManager->GetAllyHeroes() };
	if (!ally1.empty())
	{

		//Drawings.GankAlerter.Ally.Jungler
		for (auto &[netID, hero] : ally1)
		{
			if (hero != nullptr && hero != NULL)
			{
				_spellDatas.try_emplace(netID, hero);

				for (auto &manual : ManualSpellLists)
				{
					if (_stricmp(manual.Champ, hero->GetCharName()) == 0)
					{
						manual.IsAlly = true;
						ActualManualSpellLists.emplace_back(manual);
					}
				}
			}
		}
	}
	


	
	auto enemy1{ pSDK->EntityManager->GetEnemyHeroes() };

	if (!enemy1.empty())
	{
		for (auto &[netID, hero] : enemy1)
		{
			if (hero != nullptr && hero != NULL)
			{
				if (netID != Player.GetNetworkID())
				{
					_spellDatas.try_emplace(netID, hero);
					for (auto &manual : ManualSpellLists)
					{
						if (_stricmp(manual.Champ, hero->GetCharName()) == 0)
						{
							manual.IsAlly = false;
							ActualManualSpellLists.emplace_back(manual);
						}
					}
				}
			}
		}
	}
	

	pSDK->EventHandler->RegisterCallback(CallbackEnum::SpellCastStart, CooldownTracker::SpellCastStart);
}

void CooldownTracker::MenuLoader()
{

	Menu::Tree("Cooldown Tracker", "Trackers.CooldownTracker", false, []()
	{
		Menu::Checkbox("Use Cooldown Trackers", "Trackers.CooldownTracker.Use", true);

		Menu::Checkbox("Track MySelf", "Trackers.CooldownTracker.Myself", false);
		Menu::Checkbox("Track Ally", "Trackers.CooldownTracker.Ally", true);
		Menu::Checkbox("Track Enemy", "Trackers.CooldownTracker.Enemy", true);
		//SdkUiText("^-> If You Changed one of them, Reloading the Plugin is Recommended for Better Perfomances.");


		Menu::Tree("Spells Cooldown", "Trackers.CooldownTracker.SC", false, []()
		{
			Menu::Checkbox("Draw Spells Cooldown Bar", "Trackers.CooldownTracker.SCBar", true);
			Menu::SliderInt("Spells Cooldown Bar Width", "Trackers.CooldownTracker.SCWidth", 3, 1, 10);
			SdkUiText("^-> Default 3");
			Menu::SliderInt("Spells Cooldown Bar Length", "Trackers.CooldownTracker.SCLength", 23, 15, 40);
			SdkUiText("^-> Default 23");
			Menu::SliderInt("Spells Cooldown Gap Between Bars", "Trackers.CooldownTracker.SCGap", 26, 10, 50);
			SdkUiText("^-> Default 26, This needs to be bigger than bar legnth");

			Menu::Checkbox("Fill out the Background of Spells Cooldown Bar", "Trackers.CooldownTracker.SCBarFillOut", true);
			SdkUiText("^-> It will use - Spells Cooldown Bar NotLeanred Color - ");

			Menu::SliderInt("Spells Cooldown Bar Position X-axis", "Trackers.CooldownTracker.SCDrawingX", 0, -200, 200);
			Menu::SliderInt("Spells Cooldown Bar Position Y-axis", "Trackers.CooldownTracker.SCDrawingY", 0, -200, 200);
			Menu::DropList("^-> Spells Cooldown Bar Color", "Trackers.CooldownTracker.BarColor", ColorMenuList, 5);
			Menu::DropList("^-> Spells Cooldown Bar OutLine Color", "Trackers.CooldownTracker.BarOutLineColor", ColorMenuList, 0);
			Menu::DropList("^-> Spells Cooldown Bar NotLeanred Color", "Trackers.CooldownTracker.BarNotLearnColor", ColorMenuList, 6);
			Menu::DropList("^-> Spells Cooldown Bar NotLeanred OutLine Color", "Trackers.CooldownTracker.BarNotLearnOutlineColor", ColorMenuList, 0);

			Menu::Checkbox("Draw Spells Cooldown Timer", "Trackers.CooldownTracker.SCTimer", true);
			Menu::DropList("Spells Cooldown Timer Format", "Trackers.CooldownTracker.SCTFormat", std::vector<std::string>{ "MM:SS", "SS" , "SS But MM:SS on R"}, 1);
			Menu::SliderInt("Spells Cooldown Timer Position X-axis", "Trackers.CooldownTracker.SCTDrawingX", 0, -200, 200);
			Menu::SliderInt("Spells Cooldown Timer Position Y-axis", "Trackers.CooldownTracker.SCTDrawingY", 0, -200, 200);
			Menu::SliderInt("Spells Cooldown Timer Font Height", "Trackers.CooldownTracker.SCTFontSize", 13, 10, 30);
			Menu::SliderInt("Spells Cooldown Timer Font Width", "Trackers.CooldownTracker.SCTFontSize2", 4, 1, 10);
			Menu::DropList("^-> Spells Cooldown Timer Color", "Trackers.CooldownTracker.SCTColor", ColorMenuList, 11);
			Menu::DropList("^-> Spells Cooldown Timer OutLine Color", "Trackers.CooldownTracker.SCTOutLineColor", ColorMenuList, 0);

		});

		Menu::Tree("Summoner Spells Cooldown", "Trackers.CooldownTracker.SS", false, []()
		{
			Menu::Checkbox("Draw Summoner Spells Icons", "Trackers.CooldownTracker.SSIcon", true);
			Menu::SliderInt("Summoner Spells Icon Position X-axis", "Trackers.CooldownTracker.SSDrawingX", 0, -200, 200);
			Menu::SliderInt("Summoner Spells Icon Position Y-axis", "Trackers.CooldownTracker.SSDrawingY", 0, -200, 200);

			Menu::Checkbox("Draw Summoner Spells Cooldown Timer", "Trackers.CooldownTracker.SSTimer", true);
			Menu::DropList("Summoner Spells Cooldown Timer Format", "Trackers.CooldownTracker.SSTFormat", std::vector<std::string>{ "MM:SS", "SS" }, 0);
			Menu::SliderInt("Summoner Spells Cooldown Timer Position X-axis", "Trackers.CooldownTracker.SSTDrawingX", 20, -200, 200);
			Menu::SliderInt("Summoner Spells Cooldown Timer Position Y-axis", "Trackers.CooldownTracker.SSTDrawingY", 0, -200, 200);
			Menu::SliderInt("Summoner Spells Cooldown Timer Font Height", "Trackers.CooldownTracker.SSTFontSize", 13, 10, 30);
			Menu::SliderInt("Summoner Spells Cooldown Timer Font Width", "Trackers.CooldownTracker.SSTFontSize2", 4, 1, 10);
			Menu::DropList("^-> Summoner Spells Cooldown Timer Color", "Trackers.CooldownTracker.SSTColor", ColorMenuList, 11);
			Menu::DropList("^-> Summoner Spells Cooldown Timer OutLine Color", "Trackers.CooldownTracker.SSTOutLineColor", ColorMenuList, 0);


		});


	});


}

void CooldownTracker::TickLoader()
{
	/*
	auto ally1{ pSDK->EntityManager->GetAllyHeroes() };

	if (!ally1.empty())
	{
		for (auto &[netID, hero] : ally1)
		{
			if (hero != nullptr && hero != NULL)
			{
				//22
			}
		}
	}
	*/



}

void CooldownTracker::DrawLoader()
{

	if (!Menu::Get<bool>("Trackers.CooldownTracker.Use"))
	{
		return;
	}
	//Vector2 screenPos{ Player.GetHealthBarScreenPos() };

	//screenPos.x += -69;
	//screenPos.y += -25;


	//screenPos.x += (float)Menu::Get<int>("Trackers.CooldownTracker.DrawingX");
	//screenPos.y += (float)Menu::Get<int>("Trackers.CooldownTracker.DrawingY");
	//SdkDrawSpriteFromResource(MAKEINTRESOURCEA(HUDSELF2), &screenPos, true);
	//auto resol = Renderer::GetResolution();

	//auto temp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_PNG2));

	
	//SdkDrawSpriteFromResource(temp, &screenPos, false);

	//SdkUiConsoleWrite(" r: %d", temp);

	//::FindResource(NULL, "Resources\\CD\\CD_HudSelf_2px.png", RT_RCDATA);


	if (!_spellDatas.empty())
	{
		for (auto &[netID, hero] : _spellDatas)
		{
			if (hero != nullptr && hero != NULL)
			{

				if (!hero->GetPosition().IsValid() || !hero->IsVisible() || !hero->GetHealthBarPos().IsValid() || !hero->GetPosition().IsOnScreen() || !hero->IsAlive() )
				{
					continue;
				}


				if (Menu::Get<bool>("Trackers.CooldownTracker.Myself") && hero->GetNetworkID() == Player.GetNetworkID())
				{
					InsideDrawer(hero, hero->IsAlly());
				}
				else if (Menu::Get<bool>("Trackers.CooldownTracker.Ally") && hero->IsAlly() && hero->GetNetworkID() != Player.GetNetworkID())
				{
					InsideDrawer(hero, hero->IsAlly());
				}
				else if (Menu::Get<bool>("Trackers.CooldownTracker.Enemy") && !hero->IsAlly())
				{
					InsideDrawer(hero, hero->IsAlly());
				}

			}
		}
	}


	


}

void CooldownTracker::InsideDrawer(AIHeroClient* hero, bool isAlly)
{

	Vector2 SpellPosition{ hero->GetHealthBarScreenPos() };
	SpellPosition.x += -43.0f;

	auto spells = hero->GetSpells();
	for (auto &spellInside : spells)
	{
		float time = spellInside.CooldownExpires - Game::Time();
		float spellCD = spellInside.TotalCooldown;

		if (!ActualManualSpellLists.empty())
		{
			for (auto &manual : ActualManualSpellLists)
			{
				if (_stricmp(manual.SpellName, spellInside.ScriptName) == 0 && _stricmp(manual.Champ, hero->GetCharName()) == 0 && manual.IsAlly == isAlly)
				{
					time = manual.CooldownExpires - Game::Time();
					spellCD = manual.Cooldown;
				}
			}

		}
		auto percent = time > 0 && std::abs(spellCD) > FLT_EPSILON
			? 1.0f - time / spellCD
			: 1.0f;


		/*
		if (time > 0.0)
		{
			std::string temp = spellInside.ScriptName;
			temp += " : ";
			temp += std::to_string(time);
			temp += " : ";
			temp += std::to_string(percent);
			//Draw::Text(NULL, &screenPos2, temp, "Arial", &Color::Green, 28, 10, 0, false);
			//Draw::Text(&sPos, NULL, temp, "Arial", &Color::Green, 28, 10, 0, false);

		}
		*/


		std::stringstream ss1;
		ss1.precision(1); //for decimal
		ss1.setf(std::ios_base::fixed, std::ios_base::floatfield);

		int sec = time;
		int mins = sec / 60;
		sec = sec % 60;
		int onlySec = time;

	
		//Menu::Get<int>("Trackers.CooldownTracker.SCTFormat")
		if (spellInside.Level > 0)
		{
			ss1.str("");
			if (int(spellInside.Slot) == int(SpellSlot::Summoner1) || int(spellInside.Slot) == int(SpellSlot::Summoner2))
			{
				Vector2 SSPosition{ hero->GetHealthBarScreenPos() };
				SSPosition.x += 63.0f;
				SSPosition.y += -28.0f;

				SSPosition.x += (float)Menu::Get<int>("Trackers.CooldownTracker.SSDrawingX");
				SSPosition.y += (float)Menu::Get<int>("Trackers.CooldownTracker.SSDrawingY");
				if (int(spellInside.Slot) == int(SpellSlot::Summoner2))
				{
					SSPosition.y += 15.0f; // gap between 2 summoners spell
				}

				if (Menu::Get<bool>("Trackers.CooldownTracker.SSIcon"))
				{
					SdkDrawSpriteFromResource(MAKEINTRESOURCEA(GetSummonerSpells(spellInside.ScriptName, Game::Time() > spellInside.CooldownExpires)), &SSPosition, false);

				}


				if (time > 0.0f && Menu::Get<bool>("Trackers.CooldownTracker.SSTimer"))
				{
					int format = Menu::Get<int>("Trackers.CooldownTracker.SSTFormat");

					if (format == 0) // mm:ss
					{
						ss1 << std::setfill('0') << std::setw(2) << mins << ":" << std::setfill('0') << std::setw(2) << sec;

					}
					else if (format == 1) // ss
					{
						ss1 << std::setfill('0') << std::setw(2) << onlySec;

					}

					DrawHelper::DrawOutlineText(NULL, &Vector2(SSPosition.x + (float)Menu::Get<int>("Trackers.CooldownTracker.SSTDrawingX") + 18.0f, SSPosition.y + (float)Menu::Get<int>("Trackers.CooldownTracker.SSTDrawingY")), ss1.str().c_str(), "Calibri Bold", &Color::White, 15, 4, 0,
						&Color::Black, false);
				}

			}
			else
			{
				
				auto xPos = SpellPosition.x + percent * (float)Menu::Get<int>("Trackers.CooldownTracker.SCLength");
				auto yPos = SpellPosition.y;
				xPos += (float)Menu::Get<int>("Trackers.CooldownTracker.SCDrawingX");
				yPos += (float)Menu::Get<int>("Trackers.CooldownTracker.SCDrawingY");

		

				if (spellInside.Learned)
				{
					if (Menu::Get<bool>("Trackers.CooldownTracker.SCBar"))
					{
						DrawHelper::DrawOutlineLineScreen(&Vector2(SpellPosition.x + (float)Menu::Get<int>("Trackers.CooldownTracker.SCDrawingX"), SpellPosition.y + (float)Menu::Get<int>("Trackers.CooldownTracker.SCDrawingY")), &Vector2(xPos, yPos), (float)Menu::Get<int>("Trackers.CooldownTracker.SCWidth"), &DropLists::GetColor(Menu::Get<int>("Trackers.CooldownTracker.BarColor")), &DropLists::GetColor(Menu::Get<int>("Trackers.CooldownTracker.BarOutLineColor")));
						if (Menu::Get<bool>("Trackers.CooldownTracker.SCBarFillOut") && onlySec > 0)
						{
							//&Vector2(xPos, yPos)

							auto xPosFinished = SpellPosition.x + 1.0 * (float)Menu::Get<int>("Trackers.CooldownTracker.SCLength");
							auto yPosFinished = SpellPosition.y;
							xPosFinished += (float)Menu::Get<int>("Trackers.CooldownTracker.SCDrawingX");
							yPosFinished += (float)Menu::Get<int>("Trackers.CooldownTracker.SCDrawingY");
							DrawHelper::DrawOutlineLineScreen(&Vector2(xPos, yPos), &Vector2(xPosFinished, yPosFinished), (float)Menu::Get<int>("Trackers.CooldownTracker.SCWidth"), &DropLists::GetColor(Menu::Get<int>("Trackers.CooldownTracker.BarNotLearnColor")), &DropLists::GetColor(Menu::Get<int>("Trackers.CooldownTracker.BarNotLearnOutlineColor")));
						}
					}

					if (time > 0.0f&& Menu::Get<bool>("Trackers.CooldownTracker.SCTimer"))
					{
						int format = Menu::Get<int>("Trackers.CooldownTracker.SCTFormat");

						if (format == 0) // mm:ss
						{
							ss1 << std::setfill('0') << std::setw(2) << mins << ":" << std::setfill('0') << std::setw(2) << sec;

						}
						else if (format == 1) // ss
						{
							ss1 << std::setfill('0') << std::setw(2) << onlySec;

						}
						else if (format == 2) // R only mm:ss
						{
							if (int(spellInside.Slot) == int(SpellSlot::R))
							{
								ss1 << std::setfill('0') << std::setw(2) << mins << ":" << std::setfill('0') << std::setw(2) << sec;
							}
							else
							{
								ss1 << std::setfill('0') << std::setw(2) << onlySec;
							}

						}


						DrawHelper::DrawOutlineText(NULL, &Vector2(SpellPosition.x + (float)Menu::Get<int>("Trackers.CooldownTracker.SCTDrawingX"), SpellPosition.y + (float)Menu::Get<int>("Trackers.CooldownTracker.SCTDrawingY") + 5.0f), ss1.str().c_str(), "Calibri Bold", &DropLists::GetColor(Menu::Get<int>("Trackers.CooldownTracker.SCTColor")), 15, 4, 0,
							&DropLists::GetColor(Menu::Get<int>("Trackers.CooldownTracker.SCTOutLineColor")), false);
						//Trackers.CooldownTracker.SCBarFillOut
						
					}
				}
				else if (!spellInside.Learned)
				{
					if (Menu::Get<bool>("Trackers.CooldownTracker.SCBar"))
					{
						DrawHelper::DrawOutlineLineScreen(&Vector2(SpellPosition.x + (float)Menu::Get<int>("Trackers.CooldownTracker.SCDrawingX"), SpellPosition.y + (float)Menu::Get<int>("Trackers.CooldownTracker.SCDrawingY")), &Vector2(xPos, yPos), (float)Menu::Get<int>("Trackers.CooldownTracker.SCWidth"), &DropLists::GetColor(Menu::Get<int>("Trackers.CooldownTracker.BarNotLearnColor")), &DropLists::GetColor(Menu::Get<int>("Trackers.CooldownTracker.BarNotLearnOutlineColor")));
					}
				}
			}

		}

		/*
				Menu::SliderInt("Spells Cooldown Bar Width", "Trackers.CooldownTracker.SCWidth", 3, 1, 10);
			Menu::SliderInt("Spells Cooldown Bar Length", "Trackers.CooldownTracker.SCLength", 23, 15, 40);
			Menu::SliderInt("Spells Cooldown Gap Between Bars", "Trackers.CooldownTracker.SCGap", 25, 10, 40);
		*/
		SpellPosition.x += (float)Menu::Get<int>("Trackers.CooldownTracker.SCGap"); // gab between spells
	}
}


void CooldownTracker::SpellCastStart(void * AI, PSDK_SPELL_CAST SpellCast, void * UserData)
{
	
	UNREFERENCED_PARAMETER(UserData);

	if (!Menu::Get<bool>("Trackers.CooldownTracker.Use"))
	{
		return;
	}
	/*
	if (!Menu::Get<bool>("Trackers.DashJumpTracker.Use"))
	{

		return;
	}


	if (Destinations.empty())
	{
		return;
	}
	*/

	auto sender = pSDK->EntityManager->GetObjectFromPTR(AI);
	if (AI == nullptr || sender == nullptr || sender == NULL || !sender->IsHero()) //later need to add not ally
	{
		return;
	}

	auto spellName = SpellCast->Spell.ScriptName;

	//SdkUiConsoleWrite("1Manual na Added: %s", sender->AsAIHeroClient()->GetCharName());

	//SdkUiConsoleWrite("1Manual Spell Added: %s", spellName);

	//SdkUiConsoleWrite("diff: %f", SpellCast->Spell.CooldownExpires - Game::Time());
	//auto objectPos = sender->GetPosition();

	if (spellName == NULL || spellName == nullptr )
	{
		return ;
	}

	for (auto &manual : ActualManualSpellLists)
	{
		if (_stricmp(manual.SpellName, spellName) == 0)
		{
			if (SpellCast->Spell.CooldownExpires - Game::Time() < 0.5)
			{
				//SdkUiConsoleWrite("na2: %s", manual.Champ);
				//float cooldown = manual.Cooldowns[SpellCast->Spell.Level - 1];
				//SdkUiConsoleWrite("Level: %d", SpellCast->Spell.Level);
				float cooldown = manual.Cooldowns[SpellCast->Spell.Level - 1];
				float cdr = sender->AsAIHeroClient()->GetCooldownReduction() * -1.0f * 100.0f;

				if (cdr == -0.0)
				{
					cdr = 0.0;
				}
				else if (cdr > 45.0f)
				{
					cdr = 45.0f;
				}
				else if (cdr < 0.0)
				{
					cdr = 0.0;
				}

				manual.Cooldown = cooldown - ((cooldown / 100.0f) * cdr) + manual.Additional;
				manual.CooldownExpires = Game::Time() + manual.Cooldown;
				//SdkUiConsoleWrite("Manual cd %f cdr %f result %f Final %f Game: %f", cooldown, cdr , manual.Cooldown, manual.CooldownExpires, Game::Time());
			}
		}
	}
}


unsigned int CooldownTracker::GetSummonerSpells(const char* name, bool isReady)
{
	if (_stricmp(name, "Summoner_Dash") == 0)
	{
		if (isReady)
		{
			return Summoner_Dash;
		}
		else
		{
			return Summoner_Dash_Cooldown;
		}
	}

	if (_stricmp(name, "Summoner_Empty") == 0)
	{
		if (isReady)
		{
			return Summoner_Empty;
		}
		else
		{
			return Summoner_Empty_Cooldown;
		}
	}

	if (_stricmp(name, "Summoner_flashPerksHextechFlashtraption") == 0)
	{
		if (isReady)
		{
			return Summoner_flashPerksHextechFlashtraption;
		}
		else
		{
			return Summoner_flashPerksHextechFlashtraption_Cooldown;
		}
	}

	if (_stricmp(name, "Summoner_flashPerksHextechFlashtraptionBlue") == 0)
	{
		if (isReady)
		{
			return Summoner_flashPerksHextechFlashtraptionBlue;
		}
		else
		{
			return Summoner_flashPerksHextechFlashtraptionBlue_Cooldown;
		}
	}

	if (_stricmp(name, "Summoner_Mark") == 0)
	{
		if (isReady)
		{
			return Summoner_Mark;
		}
		else
		{
			return Summoner_Mark_Cooldown;
		}
	}

	if (_stricmp(name, "SummonerBarrier") == 0)
	{
		if (isReady)
		{
			return SummonerBarrier;
		}
		else
		{
			return SummonerBarrier_Cooldown;
		}
	}

	if (_stricmp(name, "SummonerBoost") == 0)
	{
		if (isReady)
		{
			return SummonerBoost;
		}
		else
		{
			return SummonerBoost_Cooldown;
		}
	}

	if (_stricmp(name, "SummonerClairvoyance") == 0)
	{
		if (isReady)
		{
			return SummonerClairvoyance;
		}
		else
		{
			return SummonerClairvoyance_Cooldown;
		}
	}

	if (_stricmp(name, "SummonerDot") == 0)
	{
		if (isReady)
		{
			return SummonerDot;
		}
		else
		{
			return SummonerDot_Cooldown;
		}
	}

	if (_stricmp(name, "SummonerExhaust") == 0)
	{
		if (isReady)
		{
			return SummonerExhaust;
		}
		else
		{
			return SummonerExhaust_Cooldown;
		}
	}

	if (_stricmp(name, "SummonerFlash") == 0)
	{
		if (isReady)
		{
			return SummonerFlash;
		}
		else
		{
			return SummonerFlash_Cooldown;
		}
	}

	if (_stricmp(name, "SummonerFlashPerksHextechFlashtraptionV2") == 0)
	{
		if (isReady)
		{
			return SummonerFlashPerksHextechFlashtraptionV2;
		}
		else
		{
			return SummonerFlashPerksHextechFlashtraptionV2_Cooldown;
		}
	}
	if (_stricmp(name, "SummonerHaste") == 0)
	{
		if (isReady)
		{
			return SummonerHaste;
		}
		else
		{
			return SummonerHaste_Cooldown;
		}
	}

	if (_stricmp(name, "SummonerHeal") == 0)
	{
		if (isReady)
		{
			return SummonerHeal;
		}
		else
		{
			return SummonerHeal_Cooldown;
		}
	}

	if (_stricmp(name, "SummonerMana") == 0)
	{
		if (isReady)
		{
			return SummonerMana;
		}
		else
		{
			return SummonerMana_Cooldown;
		}
	}
	if (_stricmp(name, "SummonerExhaust") == 0)
	{
		if (isReady)
		{
			return SummonerExhaust;
		}
		else
		{
			return SummonerExhaust_Cooldown;
		}
	}

	if (_stricmp(name, "SummonerSmite") == 0)
	{
		if (isReady)
		{
			return SummonerSmite;
		}
		else
		{
			return SummonerSmite_Cooldown;
		}
	}
	if (_stricmp(name, "SummonerSnowball") == 0)
	{
		if (isReady)
		{
			return SummonerSnowball;
		}
		else
		{
			return SummonerSnowball_Cooldown;
		}
	}

	if (_stricmp(name, "SummonerTeleport") == 0)
	{
		if (isReady)
		{
			return SummonerTeleport;
		}
		else
		{
			return SummonerTeleport_Cooldown;
		}
	}
	if (_stricmp(name, "TeleportCancel") == 0)
	{
		if (isReady)
		{
			return TeleportCancel;
		}
		else
		{
			return TeleportCancel_Cooldown;
		}
	}

	return Summoner_Empty;
}