#include "stdafx.h"
#include "WardsTraps.h"
#include <iomanip>

constexpr float visionDuration = FLT_MAX; // Since vision wards and blue orbs has unlimited duration

Vector3 visionDirection = Vector3(100.0f, 100.0f, 100.0f);

const std::vector< WardStruct> WardsTraps::_wardStructs 
{
	// Duration, Circle Size, Vision Range, On Create Name, SpellCast Name, Types //

	{90.0f, 100.0f, 1000.0f,"YellowTrinket", "TrinketTotemLvl1", WardTypes::Trinket},	// Yellow Trinket 
	{150.0f, 100.0f, 1000.0f, "SightWard", "ItemGhostWard", WardTypes::Green},			// Wards From Supporters Items
	{150.0f, 100.0f, 1000.0f, "YellowTrinket", "LootedTrinketTotem", WardTypes::Trinket}, // Kleptomancy Ward
	{visionDuration, 75.0f, 700.0f, "BlueTrinket", "LootedTrinketOrb", WardTypes::Blue}, // Kleptomancy Farsight Orb 
	{visionDuration, 75.0f, 1000.0f,"JammerDevice", "JammerDevice", WardTypes::Pink},	// Pink Ward
	{visionDuration, 75.0f, 700.0f, "BlueTrinket", "TrinketOrbLvl3", WardTypes::Blue},	// Farsight Orb

	{30.0f, 80.0f, 80.0f, "CaitlynTrap", "CaitlynYordleTrap", WardTypes::Trap},			// Caitlyn's Trap 
	{120.0f, 80.0f, 0.0f, "NidaleeSpear", "Bushwhack", WardTypes::Trap},				// Nidalee's Trap
	{60.0f, 100.0f, 850.0f, "ShacoBox", "JackInTheBox", WardTypes::Trap},				// Shaco's Trap
	{300.0f, 150.0f, 450.0f, "TeemoMushroom", "BantamTrap", WardTypes::Trap},			// Teemo's Trap
	{120.0f, 125.0f, 450.0f, "JhinTrap", "JhinETrap", WardTypes::Trap},					// Jhin's Trap

};

std::vector< WardObject> WardsTraps::_wardObjects;
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
= default;

void WardsTraps::OnTick(void* userData)
{
	if(Game::IsOverlayOpen())
	{
		SettingsUpdate();
	}

	if (!WardsTrapsSettings.TrackWardsTraps)
	{
		return;
	}

	if (WardsTrapsSettings.TrackVisionRange)
	{
		if (WardsTrapsSettings.VisionRangeToggle || WardsTrapsSettings.VisionRangeKey)
		{
			WardsTrapsSettings.VisionRangeEnable = true;
		}
		else
		{
			WardsTrapsSettings.VisionRangeEnable = false;
		}
	}






}

void WardsTraps::OnDraw(void* userData)
{
	if (!WardsTrapsSettings.TrackWardsTraps || _wardObjects.empty())
	{
		return;
	}

	

	std::stringstream ss1;
	ss1.precision(1); //for decimal
	ss1.setf(std::ios_base::fixed, std::ios_base::floatfield);
	int sec = 0;
	int mins = 0;

	for (auto& ward : _wardObjects)
	{
		if (ward.Position.IsValid())
		{
			ss1.str("");



			if (ward.Position.IsOnScreen(50.0f))
			{


				Draw::Circle(&ward.Position, ward.Data.Range, &(ward.Data.Type == WardTypes::Trap ? Color::Magenta : Color::Green), 0, &visionDirection);

			}





			/*
			if (ward.Data.Type == WardTypes::Pink && WardsTrapsSettings.TrackWards)
			{
			

				if (ward.Position.IsOnScreen(50.0f))
				{
					Draw::Circle(&ward.Position, ward.Data.Range, &Color::Red, 0, &visionDirection);
				}

				if (WardsTrapsSettings.MinimapTrack)
				{
					//Vector2 mini = Renderer::WorldToMinimap(ward.Position);
					//SdkDrawSpriteFromResource(MAKEINTRESOURCEA(WT_Pink), &Renderer::WorldToMinimap(ward.Position), true);
				}
				//MinimapTrack

			}
			else if (ward.Data.Type == WardTypes::Blue && WardsTrapsSettings.TrackWards)
			{
				if (ward.Position.IsOnScreen(50.0f))
				{
					Draw::Circle(&ward.Position, ward.Data.Range, &Color::Blue, 0, &visionDirection);
				}
			}
			else if (((ward.Data.Type == WardTypes::Green || ward.Data.Type == WardTypes::Trinket) && WardsTrapsSettings.TrackWards || (ward.Data.Type == WardTypes::Trap && WardsTrapsSettings.TrackTraps)) && !ward.Data.Duration != visionDuration)
			{
				if (ward.Position.IsOnScreen(50.0f))
				{

					
					Draw::Circle(&ward.Position, ward.Data.Range, &(ward.Data.Type == WardTypes::Trap ? Color::Magenta : Color::Green), 0, &visionDirection);

					Vector2 screenPos2 = Renderer::WorldToScreen(ward.Position);
					screenPos2.x += -5.0f;
					screenPos2.y += -5.0f;

					sec = ward.EndTime() - Game::Time();

					mins = sec / 60;
					sec = sec % 60;

					ss1 << std::setfill('0') << std::setw(2) << mins << ":" << std::setfill('0') << std::setw(2) << sec;
					
					//DrawHelper::DrawOutlineText(NULL, &screenPos2, ss1.str().c_str(), "Calibri Bold", &Color::White, 26, 6, 0,
						//&Color::Black, false);
				}

				if (WardsTrapsSettings.MinimapTrack && (ward.Data.Type == WardTypes::Green || ward.Data.Type == WardTypes::Trinket))
				{
					//Vector2 mini = Renderer::WorldToMinimap(ward.Position);
				//	SdkDrawSpriteFromResource(MAKEINTRESOURCEA(WT_Green), &Renderer::WorldToMinimap(ward.Position), true);
				}

			}
			
			if (WardsTrapsSettings.VisionRangeEnable)
			{
				if (ward.Position.IsOnScreen(50.0f))
				{
					Draw::Circle(&ward.Position, ward.Data.VisionRange, &Color::DarkBlue, 0, &visionDirection);
				}
			}*/


		}




		//Draw::Circle(&ward.Position, 300.0f, &Color::Red, 0, &Vector3(100.0f, 100.0f, 100.0f));
	}

}


void WardsTraps::Initialized()
{
	SdkUiConsoleWrite("Done !!");
	this->subCategory = "WardsTraps";
	this->categoryMenuID = this->category_name + "." + this->subCategory + ".";

	_wardObjects.shrink_to_fit();
	trinketAdjust.shrink_to_fit();
}





void WardsTraps::Menu()
{
	Menu::Tree("Wards Traps", categoryMenuID+"Tree", false, [this]()
	{
		Menu::Checkbox("Enable Wards Traps Tracker", categoryMenuID+"Use", true);

		Menu::Checkbox("Track Wards", categoryMenuID+"Wards.Use", true);
		Menu::Checkbox("Draw Wards (Green, Pink) on the Minimap", categoryMenuID + "Wards.Minimap", true);
		Menu::Checkbox("Track Traps", categoryMenuID + "Traps.Use", true);



		Menu::Tree("Vision Range", categoryMenuID+"VisionRange", false, [this]()
		{

			Menu::Checkbox("Use Vision Range", categoryMenuID+"VisionRange.Use", true);
			Menu::Hotkey("Vision Key", categoryMenuID+"VisionRange.Key", 85); //32 is space bar
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


	WardsTrapsSettings.VisionRangeToggle = Menu::Get<KeyToggle_t>("Trackers.WardsTraps.VisionRange.Toggle").Toggle;
	WardsTrapsSettings.VisionRangeKey = Menu::Get<Hotkey>("Trackers.WardsTraps.VisionRange.Key").Active;
}


void WardsTraps::OnObjectCreated(void* object, unsigned NetworkID, void* userData)
{
	UNREFERENCED_PARAMETER(userData);
	//UNREFERENCED_PARAMETER(NetworkID);
	if (!WardsTrapsSettings.TrackWardsTraps )
	{
		return;
	}

	auto sender = pSDK->EntityManager->GetObjectFromPTR(object);

	if(!sender->IsAlly())
	{
		//return;
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
						Vector3 wardPos = wardObject->GetPosition();
						if ((*obj).Position.Distance(wardPos) < 300.0f && Game::Time() - (*obj).StartTime < 0.5f)
						{
							obj = _wardObjects.erase(obj);
						}
						else if ((*obj).Data.Type == WardTypes::Pink && ward.Type == WardTypes::Pink)
						{
							obj = _wardObjects.erase(obj);
						}
						else
						{
							++obj;
						}
					}

					float time = 0.0f;
					if (ward.Type == WardTypes::Green) 
					{
						time = 91.0f;
						trinketAdjust.emplace_back(NetworkID);
					}
					SdkUiConsoleWrite("added");
					_wardObjects.emplace_back(WardObject(ward, wardObject->GetPosition(), Game::Time(), wardObject, time));

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
		//return;
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

void WardsTraps::OnProcessSpell(void * AI, PSDK_SPELL_CAST SpellCast, void * UserData)
{
	UNREFERENCED_PARAMETER(UserData);
	if (!WardsTrapsSettings.TrackWardsTraps)
	{
		//return;
	}


	auto sender = pSDK->EntityManager->GetObjectFromPTR(AI);
	if (sender->IsAlly()) 
	{
		return;
	}

	for (auto &ward : _wardStructs)
	{
		if (_strcmpi(ward.SpellName, SpellCast->Spell.ScriptName) == 0)
		{


			float time = 0.0f;

			if (ward.Type == WardTypes::Green || ward.Type == WardTypes::Trinket)
			{
				if (strcmp(ward.SpellName, "TrinketTotemLvl1") == 0)
				{
					time = 84.706 + (5.294* (sender->AsAIHeroClient()->GetLevel()-1)); // Got the calculation from https://leagueoflegends.fandom.com/wiki/Trinket_item
				}

			}
			SdkUiConsoleWrite("added2");
			_wardObjects.emplace_back(WardObject(ward, SpellCast->EndPosition, Game::Time(), nullptr, time));
		}

	}
}