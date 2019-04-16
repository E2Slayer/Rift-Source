#include "stdafx.h"
#include "WardsTraps.h"
#include <iomanip>
#include "resource.h"
#include "TextHelpers.h"
#include "Loader.h"

constexpr float visionDuration = FLT_MAX; // Since vision wards and blue orbs has unlimited duration


Vector3 visionDirection = Vector3(100.0f, 100.0f, 100.0f);

const std::vector<WardStruct> WardsTraps::_wardStructs
{
	// Duration, Circle Size, Vision Range, On Create Name, SpellCast Name, Types //

	{90.0f, 100.0f, 1000.0f, "YellowTrinket", "TrinketTotemLvl1", Trinket}, // Yellow Trinket 
	
	{150.0f, 100.0f, 1000.0f, "SightWard", "ItemGhostWard", Green}, // Wards From Supporters Items
	{150.0f, 100.0f, 1000.0f, "YellowTrinket", "LootedTrinketTotem", Trinket}, // Kleptomancy Ward
	{visionDuration, 75.0f, 700.0f, "BlueTrinket", "LootedTrinketOrb", Blue}, // Kleptomancy Farsight Orb 
	{visionDuration, 75.0f, 1000.0f, "JammerDevice", "JammerDevice", Pink}, // Pink Ward
	{visionDuration, 75.0f, 700.0f, "BlueTrinket", "TrinketOrbLvl3", Blue}, // Farsight Orb

	{30.0f, 80.0f, 80.0f, "CaitlynTrap", "CaitlynYordleTrap", Trap}, // Caitlyn's Trap 
	{120.0f, 80.0f, 0.0f, "NidaleeSpear", "Bushwhack", Trap}, // Nidalee's Trap
	{60.0f, 100.0f, 850.0f, "ShacoBox", "JackInTheBox", Trap}, // Shaco's Trap
	{300.0f, 150.0f, 450.0f, "TeemoMushroom", "BantamTrap", Trap}, // Teemo's Trap
	{120.0f, 125.0f, 450.0f, "JhinTrap", "JhinETrap", Trap}, // Jhin's Trap
	
};

std::vector<WardObject> WardsTraps::_wardObjects;
std::vector<unsigned int> WardsTraps::trinketAdjust;

struct
{
	bool TrackVisionRange = false;
	bool VisionRangeEnable = false;
	bool TrackWardsTraps = false;
	bool TrackWards = false;
	bool TrackTraps = false;
	bool MinimapTrack = false;

	bool VisionRangeKey = false;
	bool VisionRangeToggle = false;
} WardsTrapsSettings;


WardsTraps::WardsTraps()
{
	SdkUiConsoleWrite("here? !!");
	//Parent::OnInit();
}

WardsTraps::~WardsTraps()
{
	SdkUiConsoleWrite("Dstroy? !!"); // todo better way to disable functions
}

void WardsTraps::OnTick(void* userData)
{
	//SdkUiConsoleWrite("Ward Tick? !!");
	if (Game::IsOverlayOpen())
	{
		SettingsUpdate();
	}

	if (!WardsTrapsSettings.TrackWardsTraps)
	{
		return;
	}

	if (WardsTrapsSettings.TrackVisionRange)
	{
		WardsTrapsSettings.VisionRangeToggle = Menu::Get<KeyToggle_t>("Trackers.WardsTraps.VisionRange.Toggle").Toggle;
		WardsTrapsSettings.VisionRangeKey = Menu::Get<Hotkey>("Trackers.WardsTraps.VisionRange.Key").Active;

		if (WardsTrapsSettings.VisionRangeToggle || WardsTrapsSettings.VisionRangeKey)
		{
			WardsTrapsSettings.VisionRangeEnable = true;
		}
		else
		{
			WardsTrapsSettings.VisionRangeEnable = false;
		}
	}


	//Remove expired wards and adjust Trinket wards durations 
	//Because if a sender is inivisible, it can't check sender's level so adjust duration by using buff duration.

	if (!_wardObjects.empty())  
	{
		auto obj = std::begin(_wardObjects);

		while (obj != std::end(_wardObjects))
		{
			if ((*obj).Data.Type == WardTypes::Trinket && (*obj).Object && (*obj).Object != nullptr)
			{
				const auto singleBuff = (*obj).Object->GetBuff("sharedwardbuff", false);
				(*obj).OverrideDuration = singleBuff.EndTime - singleBuff.StartTime + 1.0f;
			}

			if ((*obj).Data.Duration != visionDuration && ((*obj).EndTime() - Game::Time() < 0.0f))
			{
				obj = _wardObjects.erase(obj);
			}
			else
			{
				++obj;
			}

		}
	}




}

void WardsTraps::OnDraw(void* userData)
{
	Draw::Circle(&Player.GetPosition(), 800.0f, &Color::Blue);

	if (!WardsTrapsSettings.TrackWardsTraps || _wardObjects.empty())
	{
		return;
	}



	for (auto& ward : _wardObjects)
	{
		if (ward.Position.IsValid())
		{

			
			if (ward.Data.Type == WardTypes::Pink && WardsTrapsSettings.TrackWards)
			{
			

				if (ward.Position.IsOnScreen())
				{
					Draw::Circle(&ward.Position, ward.Data.Range, &Color::Red, 0, &visionDirection);
				}

				if (WardsTrapsSettings.MinimapTrack)
				{
					SdkDrawSpriteFromResource(MAKEINTRESOURCEA(WT_Pink), &Renderer::WorldToMinimap(ward.Position), true);
				}
				//MinimapTrack

			}
			else if (ward.Data.Type == WardTypes::Blue && WardsTrapsSettings.TrackWards)
			{
				if (ward.Position.IsOnScreen())
				{
					Draw::Circle(&ward.Position, ward.Data.Range, &Color::Blue, 0, &visionDirection);
				}
			}
			else if (((ward.Data.Type == WardTypes::Green || ward.Data.Type == WardTypes::Trinket) && WardsTrapsSettings.TrackWards || (ward.Data.Type == WardTypes::Trap && WardsTrapsSettings.TrackTraps)) && ward.Data.Duration != visionDuration)
			{
				if (ward.Position.IsOnScreen())
				{
					Draw::Circle(&ward.Position, ward.Data.Range, &(ward.Data.Type == WardTypes::Trap ? Color::Magenta : Color::Green), 0, &visionDirection);

					Vector2 screenPos2 = Renderer::WorldToScreen(ward.Position);
					screenPos2.x += -5.0f;
					screenPos2.y += -5.0f;


					TextHelpers::DrawOutlineText(nullptr, &screenPos2, TextHelpers::TimeFormat(ward.EndTime() - Game::Time(), TimerStyle::MMSS).c_str(), "Calibri Bold", &Color::White, 26, 6, 0,
						&Color::Black);
				}

				if (WardsTrapsSettings.MinimapTrack && (ward.Data.Type == WardTypes::Green || ward.Data.Type == WardTypes::Trinket))
				{
					SdkDrawSpriteFromResource(MAKEINTRESOURCEA(WT_Green), &Renderer::WorldToMinimap(ward.Position), true);
				}

			}
			
			if (WardsTrapsSettings.VisionRangeEnable)
			{
				if (ward.Position.IsOnScreen())
				{
					Draw::Circle(&ward.Position, ward.Data.VisionRange, &Color::DarkBlue, 0, &visionDirection);
				}
			}
		}


		//Draw::Circle(&ward.Position, 300.0f, &Color::Red, 0, &Vector3(100.0f, 100.0f, 100.0f));
	}
}


void WardsTraps::Initialized()
{
	SdkUiConsoleWrite("Done !!");
	this->subCategory = "WardsTraps";
	this->categoryMenuID = this->category_name + "." + this->subCategory + ".";
	SdkUiConsoleWrite("Menu :%s", categoryMenuID);

	_wardObjects.shrink_to_fit();
	trinketAdjust.shrink_to_fit();

	


}


void WardsTraps::OnDrawMenu(void* userData)
{
	Menu::Tree("Wards Traps", categoryMenuID + "Tree", false, [this]()
	{
		Menu::Checkbox("Enable Wards Traps Tracker", categoryMenuID + "Use", true);

		Menu::Checkbox("Track Wards", categoryMenuID + "Wards.Use", true);
		Menu::Checkbox("Draw Wards (Green, Pink) on the Minimap", categoryMenuID + "Wards.Minimap", true);
		Menu::Checkbox("Track Traps", categoryMenuID + "Traps.Use", true);


		Menu::Tree("Vision Range", categoryMenuID + "VisionRange", false, [this]()
		{
			Menu::Checkbox("Use Vision Range", categoryMenuID + "VisionRange.Use", true);
			Menu::Hotkey("Vision Key", categoryMenuID + "VisionRange.Key", 85); //32 is space bar
			Menu::HotkeyToggle("Vision Toggle Key", categoryMenuID + "VisionRange.Toggle", 74, false);
		});
	
	});


}

void WardsTraps::SettingsUpdate() const
{
	WardsTrapsSettings.TrackVisionRange = Menu::Get<bool>("Trackers.WardsTraps.VisionRange.Use");
	WardsTrapsSettings.TrackWardsTraps = Menu::Get<bool>("Trackers.WardsTraps.Use");
	WardsTrapsSettings.TrackWards = Menu::Get<bool>("Trackers.WardsTraps.Wards.Use");
	WardsTrapsSettings.TrackTraps = Menu::Get<bool>("Trackers.WardsTraps.Traps.Use");
	WardsTrapsSettings.MinimapTrack = Menu::Get<bool>("Trackers.WardsTraps.Wards.Minimap");



}


void WardsTraps::OnObjectCreated(void* Object, unsigned NetworkID, void* UserData)
{
	
	UNREFERENCED_PARAMETER(UserData);
	//UNREFERENCED_PARAMETER(NetworkID);
	if (!WardsTrapsSettings.TrackWardsTraps)
	{
		return;
	}

	auto sender = pSDK->EntityManager->GetObjectFromPTR(Object);

	if (!sender->IsAlly())
	{
		return;
	}


	if (sender->IsRealWard() || sender->IsMinion())
	{
		auto wardObject = sender->AsAIMinionClient();
		if (wardObject)
		{
			for (auto& ward : _wardStructs)
			{
				if ((_stricmp(ward.ObjectBaseSkinName, wardObject->GetCharName()) == 0))
				{
					if ((ward.Type == Green || ward.Type == Pink || ward.Type == Blue || ward.Type == Trinket) &&
						WardsTrapsSettings.TrackWards || ward.Type == Trap && WardsTrapsSettings.TrackTraps)
					{
						auto obj = std::begin(_wardObjects);

						while (obj != std::end(_wardObjects))
						{
							Vector3 wardPos = wardObject->GetPosition();
							if ((*obj).Position.Distance(wardPos) < 300.0f && Game::Time() - (*obj).StartTime < 0.5f)
							{
								obj = _wardObjects.erase(obj);
							}
							else if ((*obj).Data.Type == Pink && ward.Type == Pink)
							{
								obj = _wardObjects.erase(obj);
							}
							else
							{
								++obj;
							}
						}

						float time = 0.0f;
						if (ward.Type == Green)
						{
							time = 91.0f;
							trinketAdjust.emplace_back(NetworkID);
						}
						//SdkUiConsoleWrite("added");
						_wardObjects.emplace_back(WardObject(ward, wardObject->GetPosition(), Game::Time(), wardObject,
						                                     time));
					}
				}
			}
		}
	}
}

void WardsTraps::OnObjectRemoved(void* Object, unsigned NetworkID, void* UserData)
{
	UNREFERENCED_PARAMETER(UserData);
	if (!WardsTrapsSettings.TrackWardsTraps)
	{
		return;
	}

	auto sender = pSDK->EntityManager->GetObjectFromPTR(Object);
	if (sender->IsAlly())
	{
		return;
	}


	if (sender->IsRealWard() || sender->IsMinion())
	{
		auto wardObject = sender->AsAIMinionClient();
		if (wardObject)
		{
			for (auto& ward : _wardStructs)
			{
				if ((_stricmp(ward.ObjectBaseSkinName, wardObject->GetCharName()) == 0))
				{
					auto obj = std::begin(_wardObjects);

					while (obj != std::end(_wardObjects))
					{
						if ((*obj).Object && (*obj).Object->GetNetworkID() == NetworkID)
						{
							obj = _wardObjects.erase(obj);
						}
						else
						{
							++obj;
						}
					}
				}
			}
		}
	}
}

void WardsTraps::OnProcessSpell(void* AI, PSDK_SPELL_CAST SpellCast, void* UserData)
{
	UNREFERENCED_PARAMETER(UserData);
	if (!WardsTrapsSettings.TrackWardsTraps)
	{
		return;
	}


	auto sender = pSDK->EntityManager->GetObjectFromPTR(AI);
	if (sender->IsAlly())
	{
		return;
	}

	for (auto& ward : _wardStructs)
	{
		if (_strcmpi(ward.SpellName, SpellCast->Spell.ScriptName) == 0)
		{
			if ((ward.Type == Green || ward.Type == Pink || ward.Type == Blue || ward.Type == Trinket) &&
				WardsTrapsSettings.TrackWards || ward.Type == Trap && WardsTrapsSettings.TrackTraps)
			{
				if ((ward.Type == Green || ward.Type == Trinket) && WardsTrapsSettings.TrackWards && strcmp(
					ward.SpellName, "TrinketTotemLvl1") == 0)
				{
					// Got the calculation from https://lol.gamepedia.com/Warding_Totem_(Item)
					_wardObjects.emplace_back(WardObject(ward, SpellCast->EndPosition, Game::Time(), nullptr,
						88.235F + (1.765F* (sender->AsAIHeroClient()->GetLevel())
					                                     ) ));
				}
				else
				{
					_wardObjects.emplace_back(WardObject(ward, SpellCast->EndPosition, Game::Time(), nullptr, 0.0F));
				}
			}
		}
	}
}
