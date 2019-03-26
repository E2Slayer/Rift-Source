#include "stdafx.h"
#include <string.h>
#include "CooldownTracker.h"


//SdkDrawSpriteFromResource(MAKEINTRESOURCEA(102), &barPos, false); //312x8

/*
	const char* Champ;
	const char* SpellName;
	SpellSlot Slot;
	float& Cooldowns;
	float Additional;
*/




std::vector<ManualSpell> ManualSpellLists;


std::map<unsigned int, std::vector<_SDK_SPELL>> _spellDatas;
std::map<unsigned int, std::vector<_SDK_SPELL>> _summonerDatas;

const SpellSlot _spellSlots[] { SpellSlot::Q, SpellSlot::W, SpellSlot::E, SpellSlot::R };
const SpellSlot _summonerSlots[] { SpellSlot::Summoner1, SpellSlot::Summoner2 };

void CooldownTracker::Init()
{
	ManualSpellLists.clear();
	//10 / 9.5 / 9 / 8.5 / 8
	float skillcds[]{ 10.0f, 9.5f, 9.0f, 8.5f, 8.0f };

	ManualSpellLists.emplace_back(ManualSpell("Lux", "LuxLightStrikeKugel", SpellSlot::E, skillcds, 0.0f));



	auto ally1{ pSDK->EntityManager->GetAllyHeroes() };

	auto enemy1{ pSDK->EntityManager->GetEnemyHeroes() };






	//auto ally1{ pSDK->EntityManager->GetAllyHeroes() };

	if (!ally1.empty())
	{

		//Drawings.GankAlerter.Ally.Jungler
		for (auto &[netID, hero] : ally1)
		{
			if (hero != nullptr && hero != NULL)
			{
				//if (hero->GetNetworkID() != Player.GetNetworkID())
				{

					_spellDatas.emplace(netID, hero->GetSpells());

				}
			}
		}
	}
	//SummonerSmite - Smite

	if (!enemy1.empty())
	{
		for (auto &[netID, hero] : enemy1)
		{
			if (hero != nullptr && hero != NULL)
			{
				if (netID != Player.GetNetworkID())
				{

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
		Menu::Checkbox("Enable Cooldown Trackers", "Trackers.CooldownTracker.Enable", true);
		Menu::SliderInt("Cooldown Position X-axis", "Trackers.CooldownTracker.DrawingX", -20, -200, 200);
		Menu::SliderInt("Cooldown Position Y-axis", "Trackers.CooldownTracker.DrawingY", -90, -200, 200);

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
	Vector2 screenPos{ Player.GetHealthBarScreenPos() };

	//auto resol = Renderer::GetResolution();

	screenPos.x += (float)Menu::Get<int>("Trackers.CooldownTracker.DrawingX");
	screenPos.y += (float)Menu::Get<int>("Trackers.CooldownTracker.DrawingY");
	


	auto ally1{ pSDK->EntityManager->GetAllyHeroes() };

	auto enemy1{ pSDK->EntityManager->GetEnemyHeroes() };


	if (!ally1.empty())
	{
		for (auto &[netID, hero] : ally1)
		{
			if (hero != nullptr && hero != NULL)
			{

				if (!hero->GetPosition().IsValid() || !hero->GetHealthBarPos().IsValid())
				{
					continue;
				}

				Vector2 screenPos2{ hero->GetHealthBarScreenPos() };
				Vector2 screenPos3{ hero->GetHealthBarScreenPos() };
				screenPos3.x += -50.0f;
				//screenPos3.y += -90.0f;
				auto spells = hero->GetSpells();
				for (auto &spellInside : spells)
				{
					screenPos2.y += (float)Menu::Get<int>("Trackers.CooldownTracker.DrawingY");

					float time = spellInside.CooldownExpires - Game::Time();
					float spellCD = spellInside.TotalCooldown;
					for (auto &manual : ManualSpellLists)
					{
						if (_stricmp(manual.SpellName, spellInside.ScriptName) == 0 && _stricmp(manual.Champ, hero->GetCharName()) == 0)
						{

							time = manual.CooldownExpires - Game::Time();
							spellCD = manual.Cooldown;

						}
					}

					auto percent = time > 0 && std::abs(spellCD) > FLT_EPSILON
						? 1.0f - time / spellCD
						: 1.0f;

					if (time > 0.0)
					{
						std::string temp = spellInside.ScriptName;
						temp += " : ";
						temp += std::to_string(time);
						temp += " : ";
						temp += std::to_string(percent);
						Draw::Text(NULL, &screenPos2, temp, "Arial", &Color::Green, 28, 10, 0, false);

					}


					if (spellInside.Level > 0)
					{
						
						if (int(spellInside.Slot) == int(SpellSlot::Summoner1) || int(spellInside.Slot) == int(SpellSlot::Summoner2))
						{
							Draw::LineScreen(&screenPos3, &Vector2(screenPos3.x + percent * 23, screenPos3.y), 3.0f, &Color::Blue);
						}
						else
						{
							Draw::LineScreen(&screenPos3, &Vector2(screenPos3.x + percent * 23, screenPos3.y), 3.0f, &Color::Green);
						}


						/*
						_line.Draw(
							new[] { new Vector2(x2, y2), new Vector2(x2 + percent * 23, y2) },
							t > 0 ? new ColorBGRA(235, 137, 0, 255) : new ColorBGRA(0, 168, 25, 255));
							*/
					}
					screenPos3.x += 25.0f;
				}

			
			//	screenPos3.x += -25.0f;

			}
		}
	}

	/*
	if (!enemy1.empty())
	{
		for (auto &[netID, hero] : enemy1)
		{
			if (hero != nullptr && hero != NULL)
			{
				if (netID != Player.GetNetworkID())
				{

					if (!hero->GetPosition().IsValid() || !hero->GetHealthBarPos().IsValid())
					{
						continue;
					}

					Vector2 screenPos2{ hero->GetHealthBarScreenPos() };

					auto spells = hero->GetSpells();
					for (auto &spellInside : spells)
					{
						screenPos2.y += (float)Menu::Get<int>("Trackers.CooldownTracker.DrawingY");

						float time = spellInside.CooldownExpires - Game::Time();
						float spellCD = spellInside.TotalCooldown;
						for (auto &manual : ManualSpellLists)
						{
							if (_stricmp(manual.SpellName, spellInside.ScriptName) == 0 && _stricmp(manual.Champ, hero->GetCharName()) == 0)
							{

								time = manual.CooldownExpires - Game::Time();
								spellCD = manual.Cooldown;

							}
						}

						if (time > 0.0)
						{
							std::string temp = spellInside.ScriptName;
							temp += " : ";
							temp += std::to_string(time);
							Draw::Text(NULL, &screenPos2, temp, "Arial", &Color::White, 28, 10, 0, false);
						}

					}

				}
			}
		}
	}*/

	//SdkDrawSpriteFromResource(MAKEINTRESOURCEA(HUDSELF2), &screenPos, false);
}


void CooldownTracker::SpellCastStart(void * AI, PSDK_SPELL_CAST SpellCast, void * UserData)
{
	
	UNREFERENCED_PARAMETER(UserData);

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

	//SdkUiConsoleWrite("1Manual Spell Added: %f", spellName);
	//auto objectPos = sender->GetPosition();

	if (spellName == NULL || spellName == nullptr )
	{
		return ;
	}

	for (auto &manual : ManualSpellLists)
	{
		if (_stricmp(manual.SpellName, spellName) == 0)
		{
			if (SpellCast->Spell.CooldownExpires - Game::Time() < 0.5)
			{
				SdkUiConsoleWrite("na2: %s", manual.Champ);
				//float cooldown = manual.Cooldowns[SpellCast->Spell.Level - 1];
				SdkUiConsoleWrite("Level: %d", SpellCast->Spell.Level);
				float cooldown = manual.Cooldowns[SpellCast->Spell.Level - 1];
				float cdr = sender->AsAIHeroClient()->GetCooldownReduction()* -1.0f * 100.0f;
				manual.Cooldown = cooldown - cooldown / 100.0f * (cdr > 45.0f ? 45.0f : cdr) + manual.Additional;
				manual.CooldownExpires = Game::Time() + manual.Cooldown;
				SdkUiConsoleWrite("Manual cd %f cdr %f result %f Final %f Game: %f", cooldown, cdr , manual.Cooldown, manual.CooldownExpires, Game::Time());
			}
		}
	}
}