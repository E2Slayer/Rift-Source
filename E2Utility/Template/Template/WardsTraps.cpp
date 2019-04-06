#include "stdafx.h"
#include "WardsTraps.h"
#include <string.h>
#include "DrawHelper.h"
#include <iomanip>
#include "resource.h"


std::vector< HeroWard> _heroNoWards;
std::vector< WardObject> _wardObjects;
std::vector< WardStruct> _wardStructs;
std::vector<unsigned int> trinketAdjust;


DWORD WardLastCheck = 0.0f;

const float visionDuration = 50000000.0f;
Vector3 visionDirection = Vector3(100.0f, 100.0f, 100.0f);
bool WardsTrapsFirstRun = false;


bool TrackVisionRange = false;
bool VisionRangeEnable = false;
bool TrackWardsTraps = false;
bool TrackWards = false;
bool TrackTraps = false;
bool MinimapTrack = false;


void WardsTraps::Init()
{
	_heroNoWards.clear();
	_wardObjects.clear();
	_wardStructs.clear();
	WardLastCheck = 0.0f;
	WardsTrapsFirstRun = false;

	TrackVisionRange = Menu::Get<bool>("Trackers.WardsTraps.VisionRange.Use");
	VisionRangeEnable = false;
	MinimapTrack = Menu::Get<bool>("Trackers.WardsTraps.Wards.Minimap");

	//Trackers.WardsTraps.Wards.Minimap
	TrackWardsTraps = Menu::Get<bool>("Trackers.WardsTraps.Use");
	TrackWards = Menu::Get<bool>("Trackers.WardsTraps.Wards.Use");
	TrackTraps = Menu::Get<bool>("Trackers.WardsTraps.Traps.Use");

	/*
	spellcast: LootedTrinketTotem
	SightWard 
	*/

	// LootedTrinketOrb / SightWard
	//TrinketOrbLvl3 / SightWard

	// VisionWard on create - ItemGhostWard on Spell Cast

	//oncreate Cupcake Trap / spellcast : CaitynYordleTrap

	
	_wardStructs.emplace_back(90.0f, 100.0f, 1000.0f,"YellowTrinket", "TrinketTotemLvl1", WardType::Trinket); //Normal Trinket Ward  Duration 84.706 + (5.294*level)

	_wardStructs.emplace_back(150.0f, 100.0f, 1000.0f, "SightWard", "ItemGhostWard", WardType::Green); //ward from Supporter items
	_wardStructs.emplace_back(150.0f, 100.0f, 1000.0f, "YellowTrinket", "LootedTrinketTotem", WardType::Trinket); //ward from Supporter items
	_wardStructs.emplace_back(visionDuration, 75.0f, 700.0f, "BlueTrinket", "LootedTrinketOrb", WardType::Blue); //JammerDevice


	_wardStructs.emplace_back(visionDuration, 75.0f, 1000.0f,"JammerDevice", "JammerDevice", WardType::Pink); //JammerDevice
	_wardStructs.emplace_back(visionDuration, 75.0f, 700.0f, "BlueTrinket", "TrinketOrbLvl3", WardType::Blue); //JammerDevice

	_wardStructs.emplace_back(30.0f, 80.0f, 80.0f, "CaitlynTrap", "CaitlynYordleTrap", WardType::Trap); //JammerDevice
	_wardStructs.emplace_back(120.0f, 80.0f, 0.0f, "NidaleeSpear", "Bushwhack", WardType::Trap); //JammerDevice
	_wardStructs.emplace_back(60.0f, 100.0f, 850.0f, "ShacoBox", "JackInTheBox", WardType::Trap); //JammerDevice
	_wardStructs.emplace_back(300.0f, 150.0f, 450.0f, "TeemoMushroom", "BantamTrap", WardType::Trap); //JammerDevice
	_wardStructs.emplace_back(120.0f, 125.0f, 450.0f, "JhinTrap", "JhinETrap", WardType::Trap); //JammerDevice
	

	pSDK->EventHandler->RegisterCallback(CallbackEnum::CreateObject, WardsTraps::OnCreate);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::DeleteObject, WardsTraps::OnDelete);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::SpellCastEnd, WardsTraps::SpellCastStart);
}

void WardsTraps::MenuLoader()
{
	Menu::Tree("Wards Traps", "Trackers.WardsTraps", false, []()
	{
		Menu::Checkbox("Enable Wards Traps Tracker", "Trackers.WardsTraps.Use", true);

		Menu::Checkbox("Track Wards", "Trackers.WardsTraps.Wards.Use", true);
		Menu::Checkbox("Draw Vision Wards on the Minimap", "Trackers.WardsTraps.Wards.Minimap", true);
		Menu::Checkbox("Track Traps", "Trackers.WardsTraps.Traps.Use", true);


		
		Menu::Tree("Vision Range", "Trackers.WardsTraps.VisionRange", false, []()
		{

			Menu::Checkbox("Use Vision Range", "Trackers.WardsTraps.VisionRange.Use", true);
			Menu::Hotkey("Vision Key", "Trackers.WardsTraps.VisionRange.Key", 85); //32 is space bar
			Menu::HotkeyToggle("Vision Toggle Key", "Trackers.WardsTraps.VisionRange.Toggle", 74, false);
		});
		
	});

}

void WardsTraps::TickLoader()
{
	if (Game::IsOverlayOpen || WardsTrapsFirstRun == false)
	{
		WardsTrapsFirstRun = true;
		TrackVisionRange = Menu::Get<bool>("Trackers.WardsTraps.VisionRange.Use");
		TrackWardsTraps = Menu::Get<bool>("Trackers.WardsTraps.Use");
		TrackWards = Menu::Get<bool>("Trackers.WardsTraps.Wards.Use");
		TrackTraps = Menu::Get<bool>("Trackers.WardsTraps.Traps.Use");
		MinimapTrack = Menu::Get<bool>("Trackers.WardsTraps.Wards.Minimap");
		//SdkUiConsoleWrite("trackvision %d", TrackVisionRange);
	}

	if (!TrackWardsTraps)
	{
		return;
	}

	if (TrackVisionRange)
	{
		//SdkUiConsoleWrite("ishere 23");
		if (Menu::Get<KeyToggle_t>("Trackers.WardsTraps.VisionRange.Toggle").Toggle || Menu::Get<Hotkey>("Trackers.WardsTraps.VisionRange.Key").Active)
		{
			//SdkUiConsoleWrite("ishere 2");
			VisionRangeEnable = true;
		}
		else
		{
			VisionRangeEnable = false;
		}
	}


	//SdkUiConsoleWrite("ishere 1");

	if (WardLastCheck + 300 > GetTickCount())
	{
		return;
	}
	WardLastCheck = GetTickCount();

	if (!_wardObjects.empty())
	{
		auto obj = std::begin(_wardObjects);

		while (obj != std::end(_wardObjects))
		{
			if ( (*obj).Data.Type == WardType::Trinket && (*obj).Object != NULL && (*obj).Object != nullptr)
			{
				//auto correct = pSDK->EntityManager->GetObjectFromID((*obj).Object->GetNetworkID());
				
				auto singleBuff = (*obj).Object->GetBuff("sharedwardbuff", false);
				(*obj).OverrideDuration = singleBuff.EndTime - singleBuff.StartTime + 2.0f;
				

			}


			if ((*obj).Data.Duration != visionDuration && ((*obj).EndTime() - Game::Time() < 0.0f ))
			{
				obj = _wardObjects.erase(obj);
			}
			else
			{
				++obj;
			}
			
		}
	}


	if (!trinketAdjust.empty())
	{
		auto obj = std::begin(trinketAdjust);

		auto minions{ pSDK->EntityManager->GetAllyWards() };


		while (obj != std::end(trinketAdjust))
		{
			for (auto &[netID, mini] : minions)
			{
				if ((*obj) == netID)
				{
					for (auto &value : _wardObjects)
					{
						if (value.Object->GetNetworkID() == netID)
						{
							value.Data.Duration = mini->GetResource().Current;
						}
					}


					obj = trinketAdjust.erase(obj);
					break;
				}

			}

			++obj;
			

		}
	}

	//trinketAdjust
	/*

	_wardObjects.RemoveAll(
		w = >
		(w.Data.Duration != int.MaxValue && w.EndTime <= Game.Time) ||
		(w.Object != null && !w.Object.IsValid));
	foreach(var hw in _heroNoWards.ToArray())
	{
		if (hw.Hero.IsVisible)
		{
			hw.LastVisible = Game.Time;
		}
		else
		{
			if (Game.Time - hw.LastVisible >= 15)
			{
				_heroNoWards.Remove(hw);
			}
		}
	}*/

}

void WardsTraps::DrawLoader()
{
	if (!TrackWardsTraps)
	{
		return;
	}
	//return;
	/*
	auto minions { pSDK->EntityManager->GetAllyWards( 1000.0f, &Player.GetPosition()) };


	for (auto &[netID, mini] : minions)
	{

		

		auto buffs = mini->GetBuffs();
		Vector2 screenPos{ Renderer::WorldToScreen(mini->GetPosition()) };
		for (auto &value : buffs)
		{

			
			//for (auto const& value : currentItems)


			if (screenPos.IsValid())
			{


				screenPos.y -= 20.0f;

				//auto hi = Menu::Get<Hotkey>("Activator.Config.ComboKey");



				//hi.Active;

				//Menu::Hotkey
				Draw::Text(NULL, &screenPos, value.Name, "Arial", &Color::White, 24, 6);

				screenPos.y -= 20.0f;

				Draw::Text(NULL, &screenPos, std::to_string(value.Count), "Arial", &Color::White, 24, 6);

				screenPos.y -= 20.0f;
				Draw::Text(NULL, &screenPos, std::to_string(value.EndTime - value.StartTime), "Arial", &Color::Green, 24, 6);

				screenPos.y -= 20.0f;
				Draw::Text(NULL, &screenPos, std::to_string(value.Stacks), "Arial", &Color::Green, 24, 6);
				screenPos.y -= 20.0f;
				Draw::Text(NULL, &screenPos, std::to_string(mini->GetResource().Current), "Arial", &Color::Blue, 24, 6);

			}

			//SdkUiConsoleWrite("Buff[%s]- Count : %d Endtime %f / Stacks %d / StartTime %f", value.Name, value.Count, value.EndTime, value.Stacks, value.StartTime);

		}
	}

	*/




	if (_wardObjects.empty())
	{
		return;
	}

	std::stringstream ss1;
	ss1.precision(1); //for decimal
	ss1.setf(std::ios_base::fixed, std::ios_base::floatfield);
	int sec = 0;
	int mins = 0;

	Vector2 screenPos2;

	for (auto& ward : _wardObjects)
	{
		if (ward.Position.IsOnScreen(50.0f) && ward.Position.IsValid())
		{
			ss1.str("");
			if (ward.Data.Type == WardType::Pink && TrackWards)
			{
				/*
				auto ve2 = Renderer::WorldToScreen(ward.Position);
				ve2.x += 5.0f;


				Draw::LineScreen(&Renderer::WorldToScreen(ward.Position), &ve2, 5.0f, &Color::Red);
				*/


				Draw::Circle(&ward.Position, ward.Data.Range, &Color::Red, 0, &visionDirection);

				if (MinimapTrack)
				{
					//Vector2 mini = Renderer::WorldToMinimap(ward.Position);
					SdkDrawSpriteFromResource(MAKEINTRESOURCEA(WT_Pink), &Renderer::WorldToMinimap(ward.Position), true);
				}
				//MinimapTrack

			}
			else if (ward.Data.Type == WardType::Blue && TrackWards)
			{
				Draw::Circle(&ward.Position, ward.Data.Range, &Color::Blue, 0, &visionDirection);
			}
			else if ( ( (ward.Data.Type == WardType::Green || ward.Data.Type == WardType::Trinket) && TrackWards || (ward.Data.Type == WardType::Trap && TrackTraps))  && !ward.Data.Duration != visionDuration)
			{

				Draw::Circle(&ward.Position, ward.Data.Range, &(ward.Data.Type == WardType::Trap ? Color::Magenta : Color::Green), 0, &visionDirection);

				screenPos2 = Renderer::WorldToScreen(ward.Position);
				screenPos2.x += -5.0f;
				screenPos2.y += -5.0f;

				sec = ward.EndTime() - Game::Time();

				mins = sec / 60;
				sec = sec % 60;

				ss1 << std::setfill('0') << std::setw(2) << mins << ":" << std::setfill('0') << std::setw(2) << sec;

				DrawHelper::DrawOutlineText(NULL, &screenPos2, ss1.str().c_str(), "Calibri Bold", &Color::White, 26, 6, 0,
					&Color::Black, false);

			}

			if (VisionRangeEnable)
			{
				Draw::Circle(&ward.Position, ward.Data.VisionRange, &Color::DarkBlue, 0, &visionDirection);
			}
		}




		//Draw::Circle(&ward.Position, 300.0f, &Color::Red, 0, &Vector3(100.0f, 100.0f, 100.0f));
	}

}

bool __cdecl WardsTraps::OnCreate(void * Object, unsigned int NetworkID, void * UserData)
{
	UNREFERENCED_PARAMETER(UserData);
	//UNREFERENCED_PARAMETER(NetworkID);
	if (!TrackWardsTraps)
	{
		return false;
	}
	auto sender = pSDK->EntityManager->GetObjectFromPTR(Object);
	if (Object == nullptr || sender == nullptr || sender == NULL || NetworkID == 0 || sender->IsAlly())
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
//	SdkUiConsoleWrite("3rd Ward Create GetName [%s]", objectName);




	/*
		if (sender->IsMissile())
	{
		auto missile = sender->AsMissileClient();
		if (missile->IsValid() && missile->GetOwner() != NULL && !missile->GetOwner()->IsAlly())
		{
			Vector3 sPos = missile->GetStartPos();
			Vector3 ePos = missile->GetEndPos();

		//	SdkUiConsoleWrite("Missile Ward Create %s ", missile->GetName());

		}
	}
	else 
	*/

	//return false;
	if ( sender->IsRealWard() || sender->IsMinion())
	{

		

		auto wardObject = sender->AsAIMinionClient();

		/*
		auto buffs = wardObject->GetBuffs();

		for (auto &value : buffs)
		{
			//SdkUiConsoleWrite("BuffName [%s] ", value.Name);
			//SdkUiConsoleWrite("BuffName [%d] ", value.Caster);

		//value.EndTime - Game::Time();
			//sender->GetObjectOwner
			
			auto caster = pSDK->EntityManager->GetObjectFromPTR(value.Caster);
			if (value.Caster == nullptr || caster == nullptr || caster == NULL || !caster->IsHero())
			{
				continue;
			}
			
			auto heroCaster = caster->AsAIHeroClient();
			

			SdkUiConsoleWrite("BuffName [%s] Timeleft [%f] ", value.Name, value.EndTime );

		}*/


		//auto ownder = wardObject->GetObjectOwner();
		//SdkUiConsoleWrite("3rd Ward Created GetCharName [%s]", wardObject->GetCharName());
		
		if (wardObject != NULL && wardObject != nullptr) //gotta add ally checker later
		{
			for (auto& ward : _wardStructs)
			{
				if ((_stricmp(ward.ObjectBaseSkinName, wardObject->GetCharName()) == 0))
				{

					auto obj = std::begin(_wardObjects);

					while (obj != std::end(_wardObjects))
					{
						Vector3 wardPos = wardObject->GetPosition();
						if ((*obj).Position.Distance(wardPos) < 300.0f && Game::Time()- (*obj).StartT < 0.5f)
						{
							obj = _wardObjects.erase(obj);
						}
						else if ((*obj).Data.Type == WardType::Pink && ward.Type == WardType::Pink)
						{
							obj = _wardObjects.erase(obj);
						}
						else
						{
							++obj;
						}
					}

					float time = 0.0f;

					if (ward.Type == WardType::Green) //|| ward.Type == WardType::KillableTrap
					{

						/*

						auto buffs = wardObject->GetBuffs();
						for (auto &value : buffs)
						{
							SdkUiConsoleWrite("buff Added %s ", value.Name);
						}
						//wardObject->GetBaseMana();
						time = wardObject->GetResource().Current;*/

						time = 91.0f;

						trinketAdjust.emplace_back(NetworkID);

						//SdkUiConsoleWrite("wobj Create Time [%f] Type %d", time, int(sender->AsAIMinionClient()->GetResource().Type));

					}


					WardObject wObj = WardObject(ward, wardObject->GetPosition(), Game::Time() , wardObject, time);
					_wardObjects.emplace_back(wObj);
					//SdkUiConsoleWrite("wobj Added  ");

				}

			}

		}


	}


	return false;
}

bool __cdecl WardsTraps::OnDelete(void * Object, unsigned int NetworkID, void * UserData)
{
	UNREFERENCED_PARAMETER(UserData);
	if (!TrackWardsTraps)
	{
		return false;
	}
	//UNREFERENCED_PARAMETER(NetworkID);
	/*
	if (!Menu::Get<bool>("Trackers.AbilityTimer.Use"))
	{
		return false;
	}


	if (AbilitiesMap.empty())
	{
		return false;
	}
	*/
	auto sender = pSDK->EntityManager->GetObjectFromPTR(Object);
	if (Object == nullptr || sender == nullptr || sender == NULL || NetworkID == 0 || sender->IsAlly())
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
	//SdkUiConsoleWrite("3rd Ward deleted [%s]", objectName);

	if (sender->IsRealWard() || sender->IsMinion())
	{
		auto wardObject = sender->AsAIBaseClient();
		//SdkUiConsoleWrite("3rd Ward deleted [%s]", wardObject->GetCharName());

		if (wardObject != NULL && wardObject != nullptr) //gotta add ally checker later
		{
			for (auto& ward : _wardStructs)
			{
				if ((_stricmp(ward.ObjectBaseSkinName, wardObject->GetCharName()) == 0) )
				{

					auto obj = std::begin(_wardObjects);

					while (obj != std::end(_wardObjects))
					{
						Vector3 wardPos = wardObject->GetPosition();
						if ((*obj).Object != NULL && (*obj).Object != nullptr && (*obj).Object->GetNetworkID() == NetworkID)
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

void __cdecl WardsTraps::SpellCastStart(void * AI, PSDK_SPELL_CAST SpellCast, void * UserData)
{
	UNREFERENCED_PARAMETER(UserData);
	if (!TrackWardsTraps)
	{
		return;
	}
	//return;
	/*
	if (!Menu::Get<bool>("Trackers.CooldownTracker.Use"))
	{
		return;
	}
	*/
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

	//return;

	auto sender = pSDK->EntityManager->GetObjectFromPTR(AI);
	if (AI == nullptr || sender == nullptr || sender == NULL || !sender->IsHero() || sender->IsAlly()) //later need to add not ally
	{
		return;
	}

	auto spellName = SpellCast->Spell.ScriptName;

	//SpellCast->Spell.MissileEffectName;

	//SdkUiConsoleWrite("1Manual na Added: %s", sender->AsAIHeroClient()->GetCharName());

	//SdkUiConsoleWrite("1Manual Spell Added: %s", spellName);

	//SdkUiConsoleWrite("diff: %f", SpellCast->Spell.CooldownExpires - Game::Time());
	//auto objectPos = sender->GetPosition();

	if (spellName == NULL || spellName == nullptr)
	{
		return;
	}

	//sender->AsAIHeroClient()->GetLevel()


	//SdkUiConsoleWrite("4th SpellName: %s Level : %d ", spellName, sender->AsAIHeroClient()->GetLevel());
	//SdkUiConsoleWrite("MissileEffectName: %s", SpellCast->Spell.MissileEffectName);


	/*
	foreach (var ward in _wardStructs)
                {
                    if (args.SData.Name.Equals(ward.SpellName, StringComparison.OrdinalIgnoreCase))
                    {
                        var wObj = new WardObject(
                            ward, ObjectManager.Player.GetPath(args.End).LastOrDefault(), (int) Game.Time);
                        CheckDuplicateWards(wObj);
                        _wardObjects.Add(wObj);
                    }
                }
	*/

	for (auto &ward : _wardStructs)
	{
		if (_strcmpi(ward.SpellName, spellName) == 0)
		{


			float time = 0.0f;

			if (ward.Type == WardType::Green || ward.Type == WardType::Trinket )
			{
				if (strcmp(ward.SpellName, "TrinketTotemLvl1") == 0)
				{
					//int level = sender->AsAIHeroClient()->GetLevel();

					time = 84.706 + (5.294* (sender->AsAIHeroClient()->GetLevel()));
					//SdkUiConsoleWrite("wobj SpellCast [%f]", time);
				}
				
			}



			auto wObj = WardObject(ward, SpellCast->EndPosition, Game::Time(), nullptr, time);
			//CheckDuplicateWards(wObj);

			_wardObjects.emplace_back(wObj);
			//SdkUiConsoleWrite("ScriptName: added %s", spellName);
		}

	}




}



/*
 private void CheckDuplicateWards(WardObject wObj)
		{
			try
			{
				var range = Menu.Item(Name + "FilterWards").GetValue<Slider>().Value;
				if (wObj.Data.Duration != int.MaxValue)
				{
					foreach (var obj in _wardObjects.Where(w => w.Data.Duration != int.MaxValue).ToList())
					{
						if (wObj.Position.Distance(obj.Position) < range)
						{
							_wardObjects.Remove(obj);
							return;
						}
						if (obj.IsFromMissile && !obj.Corrected)
						{
							var newPoint = obj.StartPosition.Extend(obj.EndPosition, -(range*1.5f));
							if (wObj.Position.Distance(newPoint) < range)
							{
								_wardObjects.Remove(obj);
								return;
							}
						}
					}
				}
				else
				{
					foreach (var obj in
						_wardObjects.Where(
							w =>
								w.Data.Duration != int.MaxValue && w.IsFromMissile &&
								w.Position.Distance(wObj.Position) < 100).ToList())
					{
						_wardObjects.Remove(obj);
						return;
					}
				}
			}
			catch (Exception ex)
			{
				Global.Logger.AddItem(new LogItem(ex));
			}
		}

*/




void WardsTraps::CheckDuplicateWards(WardObject wObj)
{

	float range = 5000.0f;

	if (wObj.Data.Duration != visionDuration)
	{

		auto obj = std::begin(_wardObjects);

		while (obj != std::end(_wardObjects))
		{
			if (wObj.Position.Distance((*obj).Position) < range)
			{

				obj = _wardObjects.erase(obj);
				return;
			}
			/*
			if ((*obj).IsFromMissile && !(*obj).Corrected)
			{
				auto newPoint = (*obj).StartPosition.Extended((*obj).EndPosition, -(range*1.5f));
				if (wObj.Position.Distance(newPoint) < range)
				{
					obj = _wardObjects.erase(obj);
					return;
				}
			}*/
			else
			{
				++obj;
			}
		}
	}
	/*
	else
	{
		auto obj = std::begin(_wardObjects);

		while (obj != std::end(_wardObjects))
		{

			if ((*obj).Data.Duration == visionDuration || (*obj).IsFromMissile || (*obj).Position.Distance(wObj.Position) > 100.0f)
			{

				obj = _wardObjects.erase(obj);
				return;
			}
			else
			{
				++obj;
			}
		}
	}
	*/
	/*
	var range = Menu.Item(Name + "FilterWards").GetValue<Slider>().Value;
                if (wObj.Data.Duration != int.MaxValue)
                {
                    foreach (var obj in _wardObjects.Where(w => w.Data.Duration != int.MaxValue).ToList())
                    {
                        if (wObj.Position.Distance(obj.Position) < range)
                        {
                            _wardObjects.Remove(obj);
                            return;
                        }
                        if (obj.IsFromMissile && !obj.Corrected)
                        {
                            var newPoint = obj.StartPosition.Extend(obj.EndPosition, -(range*1.5f));
                            if (wObj.Position.Distance(newPoint) < range)
                            {
                                _wardObjects.Remove(obj);
                                return;
                            }
                        }
                    }
                }
                else
                {
                    foreach (var obj in
                        _wardObjects.Where(
                            w =>
                                w.Data.Duration != int.MaxValue && w.IsFromMissile &&
                                w.Position.Distance(wObj.Position) < 100).ToList())
                    {
                        _wardObjects.Remove(obj);
                        return;
                    }
                }
	*/
}
