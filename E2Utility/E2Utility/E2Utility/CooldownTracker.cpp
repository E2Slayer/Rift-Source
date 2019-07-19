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

constexpr int CDRequiredExp[] = { 0, 280, 660, 1140, 1720, 2400, 3180, 4060, 5040, 6120, 7300, 8580, 9960, 11440, 13020, 14700, 16480, 18360, 9999999999999 };


std::vector<ManualSpell> ManualSpellLists;

std::vector<ManualSpell> ActualManualSpellLists;

std::vector< CooldownChamp> CooldownChampList;

std::map<unsigned int, AIHeroClient*> _spellDatas;

unsigned int PlayerNetID;

CooldownPositionList CooldownPos = CooldownPositionList(Vector3(0.0f, 0.0f, 0.0f));

CooldownMenu CooldownMenuList;

bool CDMySelf;
bool CDAlly;
bool CDEnemy;

bool ExpBar;

bool CDFirstRun = false;

int SCTformat = 0;

DWORD CooldownTrackerTick = 0;
//std::map<unsigned int, std::vector<_SDK_SPELL>> _summonerDatas;

//const SpellSlot _spellSlots[] { SpellSlot::Q, SpellSlot::W, SpellSlot::E, SpellSlot::R };
//const SpellSlot _summonerSlots[] { SpellSlot::Summoner1, SpellSlot::Summoner2 };

void CooldownTracker::Init()
{
	CDFirstRun = false;
	PlayerNetID = Player.GetNetworkID();
	_spellDatas.clear();
	ManualSpellLists.clear();
	CooldownTrackerTick = 0;
	/*
	CDMySelf = Menu::Get<bool>("Trackers.CooldownTracker.Myself");
	CDAlly = Menu::Get<bool>("Trackers.CooldownTracker.Ally");
	CDEnemy = Menu::Get<bool>("Trackers.CooldownTracker.Enemy");
	SCTformat = 0;
	*/
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

				CooldownChampList.emplace_back(CooldownChamp(hero, netID));

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
					CooldownChampList.emplace_back(CooldownChamp(hero, netID));

					for (auto &manual : ManualSpellLists)
					{
						if (_stricmp(manual.Champ, hero->GetCharName()) == 0)
						{
							manual.IsAlly = false;
							ActualManualSpellLists.emplace_back(manual);
							//SdkUiConsoleWrite("Champname: %s", hero->GetCharName());
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
			//Menu::Checkbox("Draw Spells Cooldown Bar", "Trackers.CooldownTracker.SCBar", true);

			/*
			Menu::SliderInt("Spells Cooldown Bar Width", "Trackers.CooldownTracker.SCWidth", 3, 1, 10);
			SdkUiText("^-> Default 3");
			Menu::SliderInt("Spells Cooldown Bar Length", "Trackers.CooldownTracker.SCLength", 23, 15, 40);
			SdkUiText("^-> Default 23");
			Menu::SliderInt("Spells Cooldown Gap Between Bars", "Trackers.CooldownTracker.SCGap", 26, 10, 50);
			SdkUiText("^-> Default 26, This needs to be bigger than bar legnth");

			Menu::Checkbox("Fill out the Background of Spells Cooldown Bar", "Trackers.CooldownTracker.SCBarFillOut", true);
			SdkUiText("^-> It will use - Spells Cooldown Bar NotLeanred Color - ");
			*/

			//Menu::SliderInt("Spells Cooldown Bar Position X-axis", "Trackers.CooldownTracker.SCDrawingX", 0, -200, 200);
			//Menu::SliderInt("Spells Cooldown Bar Position Y-axis", "Trackers.CooldownTracker.SCDrawingY", 0, -200, 200);
			//Menu::DropList("^-> Spells Cooldown Bar Color", "Trackers.CooldownTracker.BarColor", ColorMenuList, 5);
			//Menu::DropList("^-> Spells Cooldown Bar OutLine Color", "Trackers.CooldownTracker.BarOutLineColor", ColorMenuList, 0);
			//Menu::DropList("^-> Spells Cooldown Bar NotLeanred Color", "Trackers.CooldownTracker.BarNotLearnColor", ColorMenuList, 6);
			//Menu::DropList("^-> Spells Cooldown Bar NotLeanred OutLine Color", "Trackers.CooldownTracker.BarNotLearnOutlineColor", ColorMenuList, 0);

			Menu::Checkbox("Track Spells", "Trackers.CooldownTracker.SCTUse", true);
			Menu::DropList("Spells Cooldown Timer Format", "Trackers.CooldownTracker.SCTFormat", std::vector<std::string>{ "MM:SS", "SS", "SS But MM:SS on R"}, 1);
			Menu::SliderInt("Spells Cooldown Timer Position X-axis", "Trackers.CooldownTracker.SCTDrawingX", 0, -200, 200);
			Menu::SliderInt("Spells Cooldown Timer Position Y-axis", "Trackers.CooldownTracker.SCTDrawingY", 0, -200, 200);
			Menu::SliderInt("Spells Cooldown Timer Font Height", "Trackers.CooldownTracker.SCTFontSize", 18, 10, 30);
			Menu::SliderInt("Spells Cooldown Timer Font Width", "Trackers.CooldownTracker.SCTFontSize2", 6, 1, 10);
			//Menu::DropList("^-> Spells Cooldown Timer Color", "Trackers.CooldownTracker.SCTColor", ColorMenuList, 11);
			//Menu::DropList("^-> Spells Cooldown Timer OutLine Color", "Trackers.CooldownTracker.SCTOutLineColor", ColorMenuList, 0);
		});

		Menu::Tree("Summoner Spells Cooldown", "Trackers.CooldownTracker.SS", false, []()
		{
			//Menu::Checkbox("Draw Summoner Spells Icons", "Trackers.CooldownTracker.SSIcon", true);
			//Menu::SliderInt("Summoner Spells Icon Position X-axis", "Trackers.CooldownTracker.SSDrawingX", 0, -200, 200);
			//Menu::SliderInt("Summoner Spells Icon Position Y-axis", "Trackers.CooldownTracker.SSDrawingY", 0, -200, 200);

			Menu::Checkbox("Track Summoner Spells", "Trackers.CooldownTracker.SSUse", true);
			Menu::DropList("Summoner Spells Cooldown Timer Format", "Trackers.CooldownTracker.SSTFormat", std::vector<std::string>{ "MM:SS", "SS" }, 0);
			Menu::SliderInt("Summoner Spells Cooldown Timer Position X-axis", "Trackers.CooldownTracker.SSTDrawingX", 0, -200, 200);
			Menu::SliderInt("Summoner Spells Cooldown Timer Position Y-axis", "Trackers.CooldownTracker.SSTDrawingY", 0, -200, 200);
			Menu::SliderInt("Summoner Spells Cooldown Timer Font Height", "Trackers.CooldownTracker.SSTFontSize", 16, 10, 30);
			Menu::SliderInt("Summoner Spells Cooldown Timer Font Width", "Trackers.CooldownTracker.SSTFontSize2", 4, 1, 10);
			//Menu::DropList("^-> Summoner Spells Cooldown Timer Color", "Trackers.CooldownTracker.SSTColor", ColorMenuList, 11);
			//Menu::DropList("^-> Summoner Spells Cooldown Timer OutLine Color", "Trackers.CooldownTracker.SSTOutLineColor", ColorMenuList, 0);
		});

		
		Menu::Tree("HUD Settings", "Trackers.CooldownTracker.HUD", false, []()
		{
			Menu::Checkbox("Draw HUD", "Trackers.CooldownTracker.HUD.Use", true);
			Menu::SliderInt("Summoner Spells Icon Position X-axis", "Trackers.CooldownTracker.HUD.DrawingX", 0, -200, 200);
			Menu::SliderInt("Summoner Spells Icon Position Y-axis", "Trackers.CooldownTracker.HUD.DrawingY", 0, -200, 200);
		});


		Menu::Tree("Experience Bar Settings", "Trackers.CooldownTracker.EXPBar", false, []()
		{
			Menu::Checkbox("DrawExperience Bar", "Trackers.CooldownTracker.EXPBar.Use", false);

			//Menu::SliderInt("Summoner Spells Icon Position X-axis", "Trackers.CooldownTracker.HUD.DrawingX", 0, -200, 200);
			//Menu::SliderInt("Summoner Spells Icon Position Y-axis", "Trackers.CooldownTracker.HUD.DrawingY", 0, -200, 200);
		});
	});
}

void CooldownTracker::TickLoader()
{
	/*
	if (!_spellDatas.empty())
	{
		for (auto &[netID, hero] : _spellDatas)
		{
			if (hero != nullptr && hero != NULL)
			{
			}
		}
	}
	*/
	if (Game::IsOverlayOpen() || CDFirstRun == false)
	{
		CDFirstRun = true;
		//FirstRunChecker = true;
		//int screenPosSelection = Menu::Get<int>("Trackers.SideBar.Location");
		//arrayStyle = Menu::Get<int>("Trackers.SideBar.Style");

		//int gabMainFrames = float(Menu::Get<int>("Trackers.SideBar.Main.Gap"));

		Vector2 MainFramePos;

		CDMySelf = Menu::Get<bool>("Trackers.CooldownTracker.Myself");
		CDAlly = Menu::Get<bool>("Trackers.CooldownTracker.Ally");
		CDEnemy = Menu::Get<bool>("Trackers.CooldownTracker.Enemy");

		ExpBar = Menu::Get<bool>("Trackers.CooldownTracker.EXPBar.Use");

		SCTformat = Menu::Get<int>("Trackers.CooldownTracker.SCTFormat");

		CooldownMenuList.CDMySelf = Menu::Get<bool>("Trackers.CooldownTracker.Myself");
		CooldownMenuList.CDAlly = Menu::Get<bool>("Trackers.CooldownTracker.Ally");
		CooldownMenuList.CDEnemy = Menu::Get<bool>("Trackers.CooldownTracker.Enemy");

		CooldownMenuList.SpellEnable = Menu::Get<bool>("Trackers.CooldownTracker.SCTUse");
		CooldownMenuList.SpellFormat = Menu::Get<int>("Trackers.CooldownTracker.SCTFormat");
		CooldownMenuList.SpellFont1 = Menu::Get<int>("Trackers.CooldownTracker.SCTFontSize");
		CooldownMenuList.SpellFont2 = Menu::Get<int>("Trackers.CooldownTracker.SCTFontSize2");
		CooldownMenuList.SpellPosX = Menu::Get<int>("Trackers.CooldownTracker.SCTDrawingX");
		CooldownMenuList.SpellPosY = Menu::Get<int>("Trackers.CooldownTracker.SCTDrawingY");

		CooldownMenuList.SummmonerSpellEnable = Menu::Get<bool>("Trackers.CooldownTracker.SSUse");
		CooldownMenuList.SummmonerSpellFormat = Menu::Get<int>("Trackers.CooldownTracker.SSTFormat");

		CooldownMenuList.SummmonerSpellFont1 = Menu::Get<int>("Trackers.CooldownTracker.SSTFontSize");
		CooldownMenuList.SummmonerSpellFont2 = Menu::Get<int>("Trackers.CooldownTracker.SSTFontSize2");
		CooldownMenuList.SummmonerSpellPosX = Menu::Get<int>("Trackers.CooldownTracker.SSTDrawingX");
		CooldownMenuList.SummmonerSpellPosY = Menu::Get<int>("Trackers.CooldownTracker.SSTDrawingY");

		//Trackers.CooldownTracker.SCTDrawingX

		/*
		if (!CooldownChampList.empty())
		{
			int i = 0;

			for (auto &value : CooldownChampList)
			{
				auto spells = value.Hero->GetSpells();
				int i = 0;
				for (auto &spellInside : spells)
				{
					value.CooldownSpells[i].Spell = spellInside;
				}
			}
		}
		*/

		//SideBarMenuList[0].UpdateInsideFloats(); // Main Frame update

		//float AdjustMainFrameX = float(Menu::Get<int>("Trackers.SideBar.Main.DrawingX"));
		//float AdjustMainFrameY = float(Menu::Get<int>("Trackers.SideBar.Main.DrawingY"));

		/*
		tempPos.x += SideBarMenuList[0].DrawX;
		tempPos.y += SideBarMenuList[0].DrawY;
		MainFramePosition = tempPos;

		if (!_enemyObject.empty())
		{
			for (auto& enemy : _enemyObject)
			{
				enemy.PositionList.HpBarPos = Vector2((tempPos.x - 34.0f + SideBarMenuList[7].DrawX) + enemy.HPlength, tempPos.y + 20.0f + SideBarMenuList[7].DrawY);
				enemy.PositionList.HpBarOrignalPos = Vector2((tempPos.x - 34.0f + SideBarMenuList[7].DrawX), tempPos.y + 20.0f + SideBarMenuList[7].DrawY);

				enemy.PositionList.MpBarPos = Vector2((tempPos.x - 34.0f + SideBarMenuList[8].DrawX) + enemy.MPlength, tempPos.y + 26.0f + SideBarMenuList[8].DrawY);
				enemy.PositionList.MpBarOrignalPos = Vector2((tempPos.x - 34.0f + SideBarMenuList[8].DrawX), tempPos.y + 26.0f + SideBarMenuList[8].DrawY);

				enemy.PositionList.ExpBarPos = Vector2((tempPos.x - 34.0f + SideBarMenuList[9].DrawX) + enemy.EXPlength, tempPos.y + 32.0f + SideBarMenuList[9].DrawY);
				enemy.PositionList.ExpBarOrignalPos = Vector2((tempPos.x - 34.0f + SideBarMenuList[9].DrawX), tempPos.y + 32.0f + SideBarMenuList[9].DrawY);

				enemy.PositionList.DeathTimerPos = Vector2(tempPos.x - 15.0f + SideBarMenuList[5].DrawX, tempPos.y - 25.0f + SideBarMenuList[5].DrawY);

				enemy.PositionList.MissingTimerPos = Vector2(tempPos.x - 15.0f + SideBarMenuList[4].DrawX, tempPos.y - 25.0f + SideBarMenuList[4].DrawY);
				enemy.PositionList.LevelPos = Vector2(tempPos.x + 3.0f + SideBarMenuList[6].DrawX, tempPos.y - 5.0f + SideBarMenuList[6].DrawY);

				enemy.PositionList.UltimatePos = Vector2(tempPos.x - 34.0f + SideBarMenuList[3].DrawX, tempPos.y - 31.0f + SideBarMenuList[3].DrawY);

				enemy.PositionList.SS1Pos = Vector2(tempPos.x + 30.0f + SideBarMenuList[1].DrawX, tempPos.y - 22.0f + SideBarMenuList[1].DrawY);
				enemy.PositionList.SS1TimerPos = Vector2(tempPos.x + 30.0f + SideBarMenuList[2].DrawX, tempPos.y - 22.0f + SideBarMenuList[2].DrawY);

				enemy.PositionList.SS2Pos = Vector2(tempPos.x + 30.0f + SideBarMenuList[1].DrawX, tempPos.y + 4.0f + SideBarMenuList[1].DrawY);

				enemy.PositionList.SS2TimerPos = Vector2(tempPos.x + 30.0f + SideBarMenuList[2].DrawX, tempPos.y + 4.0f + SideBarMenuList[2].DrawY);

				if (arrayStyle == 0) //vertical
				{
					tempPos.y += 100.0f;
				}
				else if (arrayStyle == 1) // horizontal
				{
					tempPos.x += 100.0f;
				}
			}
		}*/
	}

	if (CooldownTrackerTick + 300 > GetTickCount())
	{
		return;
	}
	CooldownTrackerTick = GetTickCount();



	if (!CooldownChampList.empty())
	{
		for (auto &value : CooldownChampList)
		{
			value.UpdateCDChamp();

			for (int i = 0; i < 6; i++)
			{
				float time = value.CooldownSpells[i].Spell.CooldownExpires - Game::Time();
				//	SdkUiConsoleWrite(" %d Time :  %f ", i, time);
				float spellCD = value.CooldownSpells[i].Spell.TotalCooldown;

				if (!ActualManualSpellLists.empty() && i < 4)
				{
					for (auto &manual : ActualManualSpellLists)
					{
						if ((_stricmp(manual.SpellName, value.CooldownSpells[i].Spell.ScriptName) == 0 || int(manual.Slot) == int(value.CooldownSpells[i].Spell.Slot)) && _stricmp(manual.Champ, value.Hero->GetCharName()) == 0 && manual.IsAlly == value.Hero->IsAlly())
						{
							//SdkUiConsoleWrite(" Script : %s Time: %f ", manual.SpellName, time);
							if (manual.CooldownExpires - Game::Time() > 0.0f)
							{
								time = manual.CooldownExpires - Game::Time();
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

void CooldownTracker::DrawLoader()
{
	if (!Menu::Get<bool>("Trackers.CooldownTracker.Use"))
	{
		return;
	}

	std::stringstream ss1;
	ss1.precision(1); //for decimal
	ss1.setf(std::ios_base::fixed, std::ios_base::floatfield);

	if (!CooldownChampList.empty())
	{
		int i = 0;

		for (auto &value : CooldownChampList)
		{
			if (!value.Hero->GetPosition().IsValid() || !value.Hero->IsVisible() || !value.Hero->GetHealthBarPos().IsValid() || !value.Hero->GetPosition().IsOnScreen() || !value.Hero->IsAlive() || value.Hero->IsZombie())
			{
				continue;
			}
			Vector2 HUDPosition{ value.Hero->GetHealthBarScreenPos() };

			if (ExpBar && value.Hero->GetLevel() < 18)
			{

				float percent = (100.0f / (CDRequiredExp[value.Hero->GetLevel()] - CDRequiredExp[value.Hero->GetLevel() - 1]) * (value.Hero->GetExperience() - CDRequiredExp[value.Hero->GetLevel() - 1]));
				

				//EXPlength = percent * 0.73f;
				//float AdjustMainFrameX = float(Menu::Get<int>("Trackers.CooldownTracker.HUD.DrawingX"));
				//float AdjustMainFrameY = float(Menu::Get<int>("Trackers.CooldownTracker.HUD.DrawingY"));

				Vector2 tempPos = HUDPosition;
				tempPos.x += -47.0f;
				tempPos.y += -32.0f;
				Draw::LineScreen(&tempPos, &Vector2(tempPos.x + percent * 1.05f, tempPos.y), 5.0f, &Color::Black);
				
				tempPos.x += 1.0f;
				tempPos.y += 1.0f;
				Draw::LineScreen(&tempPos, &Vector2(tempPos.x + percent*1.05f, tempPos.y), 5.0f, &Color::Purple);

			}


			if (CDMySelf && value.Hero->GetNetworkID() == Player.GetNetworkID() ||
				CDAlly && value.Hero->IsAlly() && value.Hero->GetNetworkID() != Player.GetNetworkID() ||
				CDEnemy && !value.Hero->IsAlly()
				)
			{
				

				SdkDrawSpriteFromResource(MAKEINTRESOURCEA(CD_HudSelf), &Vector2(HUDPosition.x + 6.0f, HUDPosition.y - 8.0f), true);
				auto SpellPosition = HUDPosition;
				SpellPosition.x += -43.0f;
				SpellPosition.y += -3.0f;
				Draw::LineScreen(&SpellPosition, &Vector2(SpellPosition.x + 105.0f, SpellPosition.y), 7.0f, &Color::Grey);
				for (int i = 0; i < 6; i++)
				{
					if (i == 4 || i == 5)
					{
						if (CooldownMenuList.SummmonerSpellEnable)
						{
							Vector2 SSPosition = HUDPosition;
							SSPosition.x += 63.0f;
							SSPosition.y += -24.0f;

							//SSPosition.x += (float)Menu::Get<int>("Trackers.CooldownTracker.SSDrawingX");
							//SSPosition.y += (float)Menu::Get<int>("Trackers.CooldownTracker.SSDrawingY");

							if (i == 5)
							{
								SSPosition.y += 15.0f; // gap between 2 summoners spell
							}

							SdkDrawSpriteFromResource(MAKEINTRESOURCEA(value.CooldownSpells[i].SpellIMG), &SSPosition, false);
							if (value.CooldownSpells[i].Cooldown > 0.0f)
							{
								ss1.str("");
								//SdkUiConsoleWrite("Are we here");

								int sec = value.CooldownSpells[i].Cooldown;
								int mins = sec / 60;
								sec = sec % 60;
								int onlySec = value.CooldownSpells[i].Cooldown;

								if (CooldownMenuList.SummmonerSpellFormat == 0) // mm:ss
								{
									ss1 << std::setfill('0') << std::setw(2) << mins << ":" << std::setfill('0') << std::setw(2) << sec;
								}
								else if (CooldownMenuList.SummmonerSpellFormat == 1) // ss
								{
									ss1 << std::setfill('0') << std::setw(2) << onlySec;
								}

								DrawHelper::DrawOutlineText(NULL, &Vector2(SSPosition.x + float(CooldownMenuList.SummmonerSpellPosX) + 18.0f, SSPosition.y  + float(CooldownMenuList.SummmonerSpellPosY)), ss1.str().c_str(), "Calibri Bold", &Color::White, CooldownMenuList.SummmonerSpellFont1, CooldownMenuList.SummmonerSpellFont2, 0,
									&Color::Black, false);
							}
						}
					}
					else
					{

						auto xPos = SpellPosition.x + value.CooldownSpells[i].Percent * 25.0f;
						auto yPos = SpellPosition.y;

						if (value.CooldownSpells[i].IsLearned && CooldownMenuList.SpellEnable)
						{

							if (value.CooldownSpells[i].Percent < 0.99f && value.CooldownSpells[i].Percent >= 0.50f)
							{
								Draw::LineScreen(&SpellPosition, &Vector2(xPos, yPos), 7.0f, &Color::Orange);
							}
							else if (value.CooldownSpells[i].Percent < 0.50f && value.CooldownSpells[i].Percent > 0.00f)
							{
								Draw::LineScreen(&SpellPosition, &Vector2(xPos, yPos), 7.0f, &Color::Red);
							}
							else if (value.CooldownSpells[i].Percent == 1.00f)
							{
								Draw::LineScreen(&SpellPosition, &Vector2(xPos, yPos), 7.0f, &Color::DarkGreen);
							}

							int sec = value.CooldownSpells[i].Cooldown;
							int mins = sec / 60;
							sec = sec % 60;
							int onlySec = value.CooldownSpells[i].Cooldown;

							if (value.CooldownSpells[i].Cooldown > 0.0f)
							{
								ss1.str("");
								if (SCTformat == 0) // mm:ss
								{
									ss1 << std::setfill('0') << std::setw(2) << mins << ":" << std::setfill('0') << std::setw(2) << sec;
								}
								else if (SCTformat == 1) // ss
								{
									ss1 << std::setfill('0') << std::setw(2) << onlySec;
								}
								else if (SCTformat == 2) // R only mm:ss
								{
									if (i == 3)
									{
										ss1 << std::setfill('0') << std::setw(2) << mins << ":" << std::setfill('0') << std::setw(2) << sec;
									}
									else
									{
										ss1 << std::setfill('0') << std::setw(2) << onlySec;
									}
								}

								//Draw::LineScreen

								//SdkUiConsoleWrite(" First %f Second %f", CooldownMenuList.SpellPos.first, CooldownMenuList.SpellPos.second);
								DrawHelper::DrawOutlineText(NULL, &Vector2(SpellPosition.x + float(CooldownMenuList.SpellPosX) + 2.0f, SpellPosition.y + 9.0f + float(CooldownMenuList.SpellPosY)), ss1.str().c_str(), "Calibri Bold", &Color::White, CooldownMenuList.SpellFont1, CooldownMenuList.SpellFont2, 0,
									&Color::Black, false);
								//Trackers.CooldownTracker.SCBarFillOut
							}
						}
						/*
						else if (!value.CooldownSpells[i].IsLearned)
						{
							//DrawHelper::DrawOutlineLineScreen(&Vector2(SpellPosition.x , SpellPosition.y), &Vector2(xPos, yPos), (float)Menu::Get<int>("Trackers.CooldownTracker.SCWidth"), &DropLists::GetColor(Menu::Get<int>("Trackers.CooldownTracker.BarNotLearnColor")), &DropLists::GetColor(Menu::Get<int>("Trackers.CooldownTracker.BarNotLearnOutlineColor")));
						}*/

					}

					SpellPosition.x += 27.0f;
				}
			}
		}
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

	if (spellName == NULL || spellName == nullptr)
	{
		return;
	}

	for (auto &manual : ActualManualSpellLists)
	{
		if (_stricmp(manual.SpellName, spellName) == 0)
		{
			//if (SpellCast->Spell.CooldownExpires - Game::Time() < 0.5)
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
			}
		}
	}
}