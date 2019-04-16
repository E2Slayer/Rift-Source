#include "stdafx.h"
#include "CooldownTracker.h"
#include "resource.h"


// Got the information from https://leagueoflegends.fandom.com/wiki/Experience_(summoner)


void CooldownTracker::SettingsUpdate() const // Updating settings section
{
	CooldownTrackerSettings.EnableCooldownTracker = Menu::Get<bool>("Trackers.CooldownTracker.Use");

	CooldownTrackerSettings.TrackMyself = Menu::Get<bool>("Trackers.CooldownTracker.Myself");
	CooldownTrackerSettings.TrackAlly = Menu::Get<bool>("Trackers.CooldownTracker.Ally");
	CooldownTrackerSettings.TrackEnemy = Menu::Get<bool>("Trackers.CooldownTracker.Enemy");

	CooldownTrackerSettings.EnableExpbar = Menu::Get<bool>("Trackers.CooldownTracker.EXPBar.Use");
	CooldownTrackerSettings.ExpbarWidth = Menu::Get<int>("Trackers.CooldownTracker.EXPBar.Width");
	CooldownTrackerSettings.ExpbarPosX = Menu::Get<int>("Trackers.CooldownTracker.EXPBar.DrawingX");
	CooldownTrackerSettings.ExpbarPosY = Menu::Get<int>("Trackers.CooldownTracker.EXPBar.DrawingY");


	CooldownTrackerSettings.EnableHUD = Menu::Get<bool>("Trackers.CooldownTracker.HUD.Use");


	CooldownTrackerSettings.EnableCDTimer = false;

	CooldownTrackerSettings.CDBarWidth = Menu::Get<int>("Trackers.CooldownTracker.SCWidth");
	CooldownTrackerSettings.CDBarLength = Menu::Get<int>("Trackers.CooldownTracker.SCLength");
	CooldownTrackerSettings.CDBarGab = Menu::Get<int>("Trackers.CooldownTracker.SCGap");
	CooldownTrackerSettings.CDBarPosX = Menu::Get<int>("Trackers.CooldownTracker.SCDrawingX");
	CooldownTrackerSettings.CDBarPosY = Menu::Get<int>("Trackers.CooldownTracker.SCDrawingY");


	CooldownTrackerSettings.EnableCDTimer = Menu::Get<bool>("Trackers.CooldownTracker.SCTUse");
	CooldownTrackerSettings.CDTimerFormat = Menu::Get<int>("Trackers.CooldownTracker.SCTFormat");
	CooldownTrackerSettings.CDTimerFontSize = Menu::Get<int>("Trackers.CooldownTracker.SCTFontSize");
	CooldownTrackerSettings.CDTimerFontSize2 = Menu::Get<int>("Trackers.CooldownTracker.SCTFontSize2");
	CooldownTrackerSettings.CDTimerPosX = Menu::Get<int>("Trackers.CooldownTracker.SCTDrawingX");
	CooldownTrackerSettings.CDTimerPosY = Menu::Get<int>("Trackers.CooldownTracker.SCTDrawingY");


	CooldownTrackerSettings.SSPosX = Menu::Get<int>("Trackers.CooldownTracker.SSDrawingX");
	CooldownTrackerSettings.SSPosY = Menu::Get<int>("Trackers.CooldownTracker.SSDrawingY");

	CooldownTrackerSettings.EnableSSTimer = Menu::Get<bool>("Trackers.CooldownTracker.SSUse");
	CooldownTrackerSettings.SSTimerFormat = Menu::Get<int>("Trackers.CooldownTracker.SSTFormat");
	CooldownTrackerSettings.SSTimerFontSize = Menu::Get<int>("Trackers.CooldownTracker.SSTFontSize");
	CooldownTrackerSettings.SSTimerFontSize2 = Menu::Get<int>("Trackers.CooldownTracker.SSTFontSize2");
	CooldownTrackerSettings.SSTimerPosX = Menu::Get<int>("Trackers.CooldownTracker.SSTDrawingX");
	CooldownTrackerSettings.SSTimerPosY = Menu::Get<int>("Trackers.CooldownTracker.SSTDrawingY");
}


CooldownTracker::CooldownTracker()
= default;


CooldownTracker::~CooldownTracker()
= default;

void CooldownTracker::OnTick(void* userData)
{

	//SdkUiConsoleWrite("tracker Tick? !!");
	if (Game::IsOverlayOpen())
	{
		SettingsUpdate();
	}

	if (!CooldownTrackerSettings.EnableCooldownTracker)
	{
		return;
	}

	//return;

	if (!CooldownChampList.empty())
	{
		const float currentTime = Game::Time();

		for (auto& value : CooldownChampList)
		{
			value.UpdateCDChamp();

			for (int i = 0; i < 6; ++i)
			{
				float time = value.CooldownSpells[i].Spell.CooldownExpires - currentTime;
				//	SdkUiConsoleWrite(" %d Time :  %f ", i, time);
				float spellCD = value.CooldownSpells[i].Spell.TotalCooldown;

				if (!ActualManualSpellLists.empty() && i != 4 && i != 5)
				{
					for (auto& manual : ActualManualSpellLists)
					{
						if ((_stricmp(manual.SpellName, value.CooldownSpells[i].Spell.ScriptName) == 0 ||
							int(manual.Slot) == int(value.CooldownSpells[i].Spell.Slot)) && _stricmp(
							manual.Champ, value.Hero->GetCharName()) == 0 && manual.IsAlly == value.Hero->IsAlly())
						{
							//SdkUiConsoleWrite(" Script : %s Time: %f ", manual.SpellName, time);
							if (manual.CooldownExpires - currentTime > 0.0f)
							{
								time = manual.CooldownExpires - currentTime;
								// modify cooldowns because they don't be tracked correctly by riot's spell datas
								spellCD = manual.Cooldown;
							}
							//SdkUiConsoleWrite(" Script : %s Time: %f ", manual.SpellName, time);
						}
					}
				}

				value.CooldownSpells[i].UpdateInfoManual(time, spellCD);
			}
		}
	}
}

void CooldownTracker::OnDraw(void* userData)
{


	if (!CooldownTrackerSettings.EnableCooldownTracker)
	{
		return;
	}

	if (CooldownChampList.empty())
	{
		return;
	}


	

	for (auto& value : CooldownChampList)
	{
		if (!value.Hero->GetPosition().IsValid() || !value.Hero->IsVisible() || !value.Hero->GetHealthBarPos().IsValid()
			|| !value.Hero->GetPosition().IsOnScreen() || !value.Hero->IsAlive() || value.Hero->IsZombie())
		{
			continue;
		}


		if (CooldownTrackerSettings.TrackMyself && value.Hero->GetNetworkID() == Player.GetNetworkID() || // Me 
			(CooldownTrackerSettings.TrackAlly && value.Hero->IsAlly() && value.Hero->GetNetworkID() != Player.
				GetNetworkID()) || // "Ally" is included localplayer
			CooldownTrackerSettings.TrackEnemy && !value.Hero->IsAlly()) // Enemy
		{
			value.UpdatePosition();

			if (CooldownTrackerSettings.EnableHUD)
			{
				auto HUDPosition = Vector2(value.HeroHealthPos.x + 6.0F, value.HeroHealthPos.y - 8.0f);
				SdkDrawSpriteFromResource(MAKEINTRESOURCEA(CD_HudSelf), &HUDPosition, true);


				Draw::LineScreen(&value.SpellsStartPosition[0],
				                 &Vector2(value.SpellsStartPosition[0].x + 105.0f, value.SpellsStartPosition[0].y),
				                 7.0f, &Color::Grey);
			}

			//Draw::LineScreen(&SpellPosition, &Vector2(SpellPosition.x + 105.0f, SpellPosition.y), 7.0f, &Color::Grey);
			for (int i = 0; i != 6; ++i)
			{
				if (i == 4 || i == 5) // Summoners Spells
				{
					if (CooldownTrackerSettings.EnableCDTimer)
					{
						SdkDrawSpriteFromResource(MAKEINTRESOURCEA(value.CooldownSpells[i].SpellIMG), &value.
SpellsStartPosition[i], false);


						//TextHelpers::TimeFormat(value.CooldownSpells[i].Cooldown, true);
					}

					if (value.CooldownSpells[i].Cooldown > 0.0f)
					{
						TextHelpers::DrawOutlineText(nullptr, &value.SpellsTimerPosition[i],
						                             (TextHelpers::TimeFormat(value.CooldownSpells[i].Cooldown,TimerStyle(CooldownTrackerSettings.SSTimerFormat))), "",
						                             &Color::White
						                             , 18, 4, 0, &Color::Black);
					}
				}
				else // Regular Spells Q - R
				{
					SDKCOLOR tempColor = Color::DarkGreen;

					if (value.CooldownSpells[i].Percent < 0.99f && value.CooldownSpells[i].Percent >= 0.50f)
					{
						tempColor = Color::Orange;
					}
					else if (value.CooldownSpells[i].Percent < 0.50f && value.CooldownSpells[i].Percent > 0.00f)
					{
						tempColor = Color::Red;
					}


					Draw::LineScreen(&value.SpellsStartPosition[i], &value.SpellsEndPosition[i],
					                 static_cast<float>(CooldownTrackerSettings.CDBarWidth), &tempColor);

					if (value.CooldownSpells[i].Cooldown > 0.0f)
					{
						TextHelpers::DrawOutlineText(nullptr, &value.SpellsTimerPosition[i],
						                             (TextHelpers::TimeFormat(
							                             value.CooldownSpells[i].Cooldown,
							                             TimerStyle(CooldownTrackerSettings.CDTimerFormat))), "",
						                             &Color::White
						                             , 18, 4, 0, &Color::Black);
					}
				}

			}


			if (CooldownTrackerSettings.EnableExpbar)
			{
				Draw::LineScreen(&value.SpellsStartPosition[6], &value.SpellsEndPosition[6],
				                 static_cast<float>(CooldownTrackerSettings.ExpbarWidth), &Color::Purple);
			}
		}
	}
}

void CooldownTracker::OnProcessSpell(void* AI, PSDK_SPELL_CAST SpellCast, void* UserData)
{
	UNREFERENCED_PARAMETER(UserData);

	if (!CooldownTrackerSettings.EnableCooldownTracker || ActualManualSpellLists.empty())
	{
		return;
	}

	auto sender = pSDK->EntityManager->GetObjectFromPTR(AI);
	if (AI == nullptr || sender == nullptr || !sender->IsHero()) //later need to add not ally
	{
		return;
	}

	const auto spellName = SpellCast->Spell.ScriptName;
	if (spellName == nullptr)
	{
		return;
	}

	for (auto& manual : ActualManualSpellLists)
	{
		if (_stricmp(manual.SpellName, spellName) == 0)
		{
			//if (SpellCast->Spell.CooldownExpires - Game::Time() < 0.5)
			{
				//SdkUiConsoleWrite("na2: %s", manual.Champ);
				//float cooldown = manual.Cooldowns[SpellCast->Spell.Level - 1];
				//SdkUiConsoleWrite("Level: %d", SpellCast->Spell.Level);
				const float cooldown = manual.Cooldowns[SpellCast->Spell.Level - 1];
				float cdr = sender->AsAIHeroClient()->GetCooldownReduction() * -1.0f * 100.0f;

				if (cdr == -0.0 || cdr < 0.0)
				{
					cdr = 0.0;
				}
				else if (cdr > 45.0f)
				{
					cdr = 45.0f;
				}

				manual.Cooldown = cooldown - ((cooldown / 100.0f) * cdr) + manual.Additional;
				manual.CooldownExpires = Game::Time() + manual.Cooldown;
			}
		}
	}
}

void CooldownTracker::Initialized()
{
	ManualSpellListInit();

	SettingsUpdate();

	auto ally1{pSDK->EntityManager->GetAllyHeroes()};
	auto enemy1{pSDK->EntityManager->GetEnemyHeroes()};

	ally1.insert(enemy1.begin(), enemy1.end());

	if (!ally1.empty())
	{
		for (auto& [netID, hero] : ally1)
		{
			if (hero != nullptr)
			{
				_spellDatas.try_emplace(netID, hero);
				CooldownChampList.emplace_back(CooldownChamp(hero, netID));

				// Find champions that need to be manually tracked.

				for (auto& manual : ManualSpellLists)
				{
					if (_stricmp(manual.Champ, hero->GetCharName()) == 0)
					{
						manual.IsAlly = hero->IsAlly();
						ActualManualSpellLists.emplace_back(manual);
					}
				}
			}
		}
	}


	/*
	auto hi = this->GetMenu();
	if (hi)
		hi->menuList = std::make_shared<CooldownTracker>();
	*/
}

void CooldownTracker::OnDrawMenu(void* userData)
{
	Menu::Tree("Cooldown Tracker", "Trackers.CooldownTracker", false, []()
	{
		Menu::Checkbox("Use Cooldown Trackers", "Trackers.CooldownTracker.Use", true);

		Menu::Checkbox("Track MySelf", "Trackers.CooldownTracker.Myself", false);
		Menu::Checkbox("Track Ally", "Trackers.CooldownTracker.Ally", true);
		Menu::Checkbox("Track Enemy", "Trackers.CooldownTracker.Enemy", true);

		Menu::Checkbox("Draw HUD", "Trackers.CooldownTracker.HUD.Use", true);
		//SdkUiText("^-> If You Changed one of them, Reloading the Plugin is Recommended for Better Perfomances.");

		Menu::Tree("Spells Cooldown", "Trackers.CooldownTracker.SC", false, []()
		{
			//Menu::Checkbox("Draw Spells Cooldown Bar", "Trackers.CooldownTracker.SCBar", true);


			Menu::SliderInt("Spells Cooldown Bar Width", "Trackers.CooldownTracker.SCWidth", 6, 1, 10);
			SdkUiText("^-> Default 6");
			Menu::SliderInt("Spells Cooldown Bar Length", "Trackers.CooldownTracker.SCLength", 25, 15, 40);
			SdkUiText("^-> Default 25");
			Menu::SliderInt("Spells Cooldown Gap Between Bars", "Trackers.CooldownTracker.SCGap", 27, 10, 50);
			SdkUiText("^-> Default 27, This needs to be bigger than bar legnth");
			Menu::Checkbox("Fill out the Background of Spells Cooldown Bar", "Trackers.CooldownTracker.SCBarFillOut",
			               true);
			SdkUiText("^-> It will use - Spells Cooldown Bar NotLeanred Color - ");


			Menu::SliderInt("Spells Cooldown Bar Position X-axis", "Trackers.CooldownTracker.SCDrawingX", 0, -200, 200);
			Menu::SliderInt("Spells Cooldown Bar Position Y-axis", "Trackers.CooldownTracker.SCDrawingY", 0, -200, 200);
			//Menu::DropList("^-> Spells Cooldown Bar Color", "Trackers.CooldownTracker.BarColor", ColorMenuList, 5);
			//Menu::DropList("^-> Spells Cooldown Bar OutLine Color", "Trackers.CooldownTracker.BarOutLineColor", ColorMenuList, 0);
			//Menu::DropList("^-> Spells Cooldown Bar NotLeanred Color", "Trackers.CooldownTracker.BarNotLearnColor", ColorMenuList, 6);
			//Menu::DropList("^-> Spells Cooldown Bar NotLeanred OutLine Color", "Trackers.CooldownTracker.BarNotLearnOutlineColor", ColorMenuList, 0);

			Menu::Checkbox("Track Spells", "Trackers.CooldownTracker.SCTUse", true);
			Menu::DropList("Spells Cooldown Timer Format", "Trackers.CooldownTracker.SCTFormat",
			               std::vector<std::string>{"SS.S", "SS", "MM:SS"}, 0);
			Menu::SliderInt("Spells Cooldown Timer Position X-axis", "Trackers.CooldownTracker.SCTDrawingX", 0, -200,
			                200);
			Menu::SliderInt("Spells Cooldown Timer Position Y-axis", "Trackers.CooldownTracker.SCTDrawingY", 0, -200,
			                200);
			Menu::SliderInt("Spells Cooldown Timer Font Height", "Trackers.CooldownTracker.SCTFontSize", 18, 10, 30);
			Menu::SliderInt("Spells Cooldown Timer Font Width", "Trackers.CooldownTracker.SCTFontSize2", 6, 1, 10);
			//Menu::DropList("^-> Spells Cooldown Timer Color", "Trackers.CooldownTracker.SCTColor", ColorMenuList, 11);
			//Menu::DropList("^-> Spells Cooldown Timer OutLine Color", "Trackers.CooldownTracker.SCTOutLineColor", ColorMenuList, 0);
		});

		Menu::Tree("Summoner Spells Cooldown", "Trackers.CooldownTracker.SS", false, []()
		{
			//Menu::Checkbox("Draw Summoner Spells Icons", "Trackers.CooldownTracker.SSIcon", true);
			Menu::SliderInt("Summoner Spells Icon Position X-axis", "Trackers.CooldownTracker.SSDrawingX", 0, -200,
			                200);
			Menu::SliderInt("Summoner Spells Icon Position Y-axis", "Trackers.CooldownTracker.SSDrawingY", 0, -200,
			                200);

			Menu::Checkbox("Track Summoner Spells", "Trackers.CooldownTracker.SSUse", true);
			Menu::DropList("Summoner Spells Cooldown Timer Format", "Trackers.CooldownTracker.SSTFormat",
			               std::vector<std::string>{"SS.S", "SS", "MM:SS"}, 2);
			Menu::SliderInt("Summoner Spells Cooldown Timer Position X-axis", "Trackers.CooldownTracker.SSTDrawingX", 0,
			                -200, 200);
			Menu::SliderInt("Summoner Spells Cooldown Timer Position Y-axis", "Trackers.CooldownTracker.SSTDrawingY", 0,
			                -200, 200);
			Menu::SliderInt("Summoner Spells Cooldown Timer Font Height", "Trackers.CooldownTracker.SSTFontSize", 16,
			                10, 30);
			Menu::SliderInt("Summoner Spells Cooldown Timer Font Width", "Trackers.CooldownTracker.SSTFontSize2", 4, 1,
			                10);
			//Menu::DropList("^-> Summoner Spells Cooldown Timer Color", "Trackers.CooldownTracker.SSTColor", ColorMenuList, 11);
			//Menu::DropList("^-> Summoner Spells Cooldown Timer OutLine Color", "Trackers.CooldownTracker.SSTOutLineColor", ColorMenuList, 0);
		});

		/*
		Menu::Tree("HUD Settings", "Trackers.CooldownTracker.HUD", false, []()
		{
			Menu::Checkbox("Draw HUD", "Trackers.CooldownTracker.HUD.Use", true);
			Menu::SliderInt("Summoner Spells Icon Position X-axis", "Trackers.CooldownTracker.HUD.DrawingX", 0, -200, 200);
			Menu::SliderInt("Summoner Spells Icon Position Y-axis", "Trackers.CooldownTracker.HUD.DrawingY", 0, -200, 200);
		});
		*/


		Menu::Tree("Experience Bar Settings", "Trackers.CooldownTracker.EXPBar", false, []()
		{
			Menu::Checkbox("Draw Experience Bar", "Trackers.CooldownTracker.EXPBar.Use", false);

			Menu::SliderInt("Experience Bar Width", "Trackers.CooldownTracker.EXPBar.Width", 3, 1, 10);
			SdkUiText("^-> Default 3");
			Menu::SliderInt("Experience Bar Position X-axis", "Trackers.CooldownTracker.EXPBar.DrawingX", 0, -200, 200);
			Menu::SliderInt("Experience Bar Position Y-axis", "Trackers.CooldownTracker.EXPBar.DrawingY", 0, -200, 200);
		});
	});
}


void CooldownTracker::ManualSpellListInit()
{
	float skillcds[] = {10.0f, 9.5f, 9.0f, 8.5f, 8.0f};
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

	skillcds[0] = 20.0f;
	skillcds[1] = 18.0f;
	skillcds[2] = 16.0f;
	skillcds[3] = 14.0f;
	skillcds[4] = 12.0f;
	ManualSpellLists.emplace_back(ManualSpell("Thresh", "ThreshQInternal", SpellSlot::Q, skillcds, 0.0f, false));

	skillcds[0] = 1.0f;
	skillcds[1] = 1.0f;
	skillcds[2] = 1.0f;
	skillcds[3] = 1.0f;
	skillcds[4] = 1.0f;
	ManualSpellLists.emplace_back(ManualSpell("Karthus", "KarthusLayWasteA1", SpellSlot::Q, skillcds, 0.0f, false));

	/*
	skillcds[0] = 1.0f;
	skillcds[1] = 1.0f;
	skillcds[2] = 1.0f;
	skillcds[3] = 1.0f;
	skillcds[4] = 1.0f;
	ManualSpellLists.emplace_back(ManualSpell("Veigar", "VeigarDarkMatterCastLockout", SpellSlot::W, skillcds, 0.0f, false));
	*/

	/*
	skillcds[0] = 16.0f;
	skillcds[1] = 15.5f;
	skillcds[2] = 15.0f;
	skillcds[3] = 14.5f;
	skillcds[4] = 14.0f;
	ManualSpellLists.emplace_back(ManualSpell("Shaco", "Deceive", SpellSlot::Q, skillcds, 0.0f, false));
	//Deceive 16 / 15.5 / 15 / 14.5 / 14
	*/

	skillcds[0] = 10.0f;
	skillcds[1] = 9.0f;
	skillcds[2] = 8.0f;
	skillcds[3] = 7.0f;
	skillcds[4] = 6.0f;
	ManualSpellLists.emplace_back(ManualSpell("Kennen", "KennenLightningRush", SpellSlot::E, skillcds, 2.0f, false));

	skillcds[0] = 10.0f;
	skillcds[1] = 9.0f;
	skillcds[2] = 8.0f;
	skillcds[3] = 7.0f;
	skillcds[4] = 6.0f;
	ManualSpellLists.emplace_back(ManualSpell("Kennen", "KennenLRCancel", SpellSlot::E, skillcds, 0.0f, false));

	skillcds[0] = 6.0f;
	skillcds[1] = 6.0f;
	skillcds[2] = 6.0f;
	skillcds[3] = 6.0f;
	skillcds[4] = 6.0f;
	ManualSpellLists.emplace_back(ManualSpell("TwistedFate", "GoldCardPreAttack", SpellSlot::W, skillcds, 0.0f, false));

	ManualSpellLists.emplace_back(ManualSpell("TwistedFate", "BlueCardPreAttack", SpellSlot::W, skillcds, 0.0f, false));

	ManualSpellLists.emplace_back(ManualSpell("TwistedFate", "RedCardPreAttack", SpellSlot::W, skillcds, 0.0f, false));
}
