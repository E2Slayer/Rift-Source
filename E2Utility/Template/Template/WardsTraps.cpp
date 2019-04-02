#include "stdafx.h"
#include "WardsTraps.h"
#include <string.h>
std::vector< HeroWard> _heroNoWards;
std::vector< WardObject> _wardObjects;
std::vector< WardStruct> _wardStructs;

const float visionDuration = 50000000.0f;

void WardsTraps::Init()
{
	_heroNoWards.clear();
	_wardObjects.clear();
	_wardStructs.clear();

	/*
	spellcast: LootedTrinketTotem
	SightWard 
	*/

	// LootedTrinketOrb / SightWard
	//TrinketOrbLvl3 / SightWard

	_wardStructs.emplace_back(60 * 1, 1100, "SightWard", "SightWard", WardType::Green); //TrinketTotemLvl1
	_wardStructs.emplace_back(visionDuration, 1100, "JammerDevice", "JammerDevice", WardType::Pink); //JammerDevice

	pSDK->EventHandler->RegisterCallback(CallbackEnum::CreateObject, WardsTraps::OnCreate);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::DeleteObject, WardsTraps::OnDelete);
	pSDK->EventHandler->RegisterCallback(CallbackEnum::SpellCastEnd, WardsTraps::SpellCastStart);
}

void WardsTraps::MenuLoader()
{
}

void WardsTraps::TickLoader()
{
}

void WardsTraps::DrawLoader()
{
	for (auto& ward : _wardObjects)
	{
		Draw::Circle(&ward.Position, 300.0f, &Color::Red, 0, &Vector3(100.0f, 100.0f, 100.0f));
	}

}

bool __cdecl WardsTraps::OnCreate(void * Object, unsigned int NetworkID, void * UserData)
{
	UNREFERENCED_PARAMETER(UserData);
	UNREFERENCED_PARAMETER(NetworkID);
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
	if (Object == nullptr || sender == nullptr || sender == NULL || NetworkID == 0)
	{
		return false;
	}


	/*
	 if (missile.SpellCaster != null && !missile.SpellCaster.IsAlly && missile.SData != null)
                    {
                        if (missile.SData.Name.Equals("itemplacementmissile", StringComparison.OrdinalIgnoreCase) &&
                            !missile.SpellCaster.IsVisible)
                        {
                            var sPos = missile.StartPosition;
                            var ePos = missile.EndPosition;

                            Utility.DelayAction.Add(
                                1000, delegate
                                {
                                    if (
                                        !_wardObjects.Any(
                                            w =>
                                                w.Position.To2D().Distance(sPos.To2D(), ePos.To2D(), false) < 300 &&
                                                ((int) Game.Time - w.StartT < 2)))
                                    {
                                        var wObj = new WardObject(
                                            GetWardStructForInvisible(sPos, ePos),
                                            new Vector3(ePos.X, ePos.Y, NavMesh.GetHeightForPosition(ePos.X, ePos.Y)),
                                            (int) Game.Time, null, true,
                                            new Vector3(sPos.X, sPos.Y, NavMesh.GetHeightForPosition(sPos.X, sPos.Y)),
                                            missile.SpellCaster);
                                        CheckDuplicateWards(wObj);
                                        _wardObjects.Add(wObj);
                                    }
                                });
                        }
                    }
	*/

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
	SdkUiConsoleWrite("Ward Create %s ", objectName);

	if (sender->IsMissile())
	{
		auto missile = sender->AsMissileClient();
		if (missile->IsValid() && missile->GetOwner() != NULL && !missile->GetOwner()->IsAlly())
		{
			Vector3 sPos = missile->GetStartPos();
			Vector3 ePos = missile->GetEndPos();

			SdkUiConsoleWrite("Missile Ward Create %s ", missile->GetName());

		}
	}
	else if ( sender->IsRealWard())
	{
		auto wardObject = sender->AsAIBaseClient();
		if (wardObject != NULL && wardObject != nullptr) //gotta add ally checker later
		{
			for (auto& ward : _wardStructs)
			{
				if ((_stricmp(ward.ObjectBaseSkinName, wardObject->GetName()) == 0))
				{

					auto obj = std::begin(_wardObjects);

					while (obj != std::end(_wardObjects))
					{
						Vector3 wardPos = wardObject->GetPosition();
						if ((*obj).Position.Distance(wardPos) < 300.0f && Game::Time()- (*obj).StartT < 0.5f)
						{
							obj = _wardObjects.erase(obj);
						}
						else
						{
							++obj;
						}
					}

					WardObject wObj = WardObject(ward, wardObject->GetPosition(), (Game::Time() - float(wardObject->GetResource().Max - wardObject->GetResource().Current)), wardObject, false, Vector3(0.0f, 0.0f, 0.0f), nullptr);
					_wardObjects.emplace_back(wObj);
					SdkUiConsoleWrite("wobj Added %s ", wardObject->GetPosition());

				}

			}

		}

		/*
		 if (wardObject != null && wardObject.IsValid && !wardObject.IsAlly)
                    {
                        foreach (var ward in _wardStructs)
                        {
                            if (wardObject.CharData.BaseSkinName.Equals(
                                ward.ObjectBaseSkinName, StringComparison.OrdinalIgnoreCase))
                            {
                                _wardObjects.RemoveAll(
                                    w =>
                                        w.Position.Distance(wardObject.Position) < 300 &&
                                        ((int) Game.Time - w.StartT < 0.5));
                                var wObj = new WardObject(
                                    ward,
                                    new Vector3(wardObject.Position.X, wardObject.Position.Y, wardObject.Position.Z),
                                    (int) (Game.Time - (int) (wardObject.MaxMana - wardObject.Mana)), wardObject);
                                CheckDuplicateWards(wObj);
                                _wardObjects.Add(wObj);
                            }
                        }
                    }
		*/
	}

	//sender->AsMissileClient()->IsValid()
	/*
	auto objectName = sender->GetName();


	auto objectPos = sender->GetPosition();

	if (objectName == NULL || objectName == nullptr || !objectPos.IsValid())
	{
		return false;
	}

	SdkUiConsoleWrite("Ward Create %s ", objectName);
	*/

	return false;
}

bool __cdecl WardsTraps::OnDelete(void * Object, unsigned int NetworkID, void * UserData)
{
	return false;
}

void __cdecl WardsTraps::SpellCastStart(void * AI, PSDK_SPELL_CAST SpellCast, void * UserData)
{
	UNREFERENCED_PARAMETER(UserData);

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

	auto sender = pSDK->EntityManager->GetObjectFromPTR(AI);
	if (AI == nullptr || sender == nullptr || sender == NULL || !sender->IsHero()) //later need to add not ally
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
	SdkUiConsoleWrite("ScriptName: %s", spellName);
	SdkUiConsoleWrite("MissileEffectName: %s", SpellCast->Spell.MissileEffectName);
}

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

			if ((*obj).IsFromMissile && !(*obj).Corrected)
			{
				auto newPoint = (*obj).StartPosition.Extended((*obj).EndPosition, -(range*1.5f));
				if (wObj.Position.Distance(newPoint) < range)
				{
					obj = _wardObjects.erase(obj);
					return;
				}
			}
			else
			{
				++obj;
			}
		}
	}
	else
	{
		auto obj = std::begin(_wardObjects);

		while (obj != std::end(_wardObjects))
		{
			/*
			w.Data.Duration != int.MaxValue && w.IsFromMissile &&
                                w.Position.Distance(wObj.Position) < 100)
			*/

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
