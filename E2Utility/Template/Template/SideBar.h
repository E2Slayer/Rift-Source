#pragma once
#include "Trackers.h"
#include "TeleportDecode.h"
#include "ChampionNames.h"
#include <string.h>
/*
private class EnemyObject
		{
			private Packet.S2C.Teleport.Status _teleportStatus;

			public EnemyObject(Obj_AI_Hero unit, Texture texture)
			{
				TeleportStatus = Packet.S2C.Teleport.Status.Unknown;
				Unit = unit;
				Texture = texture;
				RSpell = unit.GetSpell(SpellSlot.R);
				LastVisible = Game.Time;
				LastPosition = Vector3.Zero;
			}

			public Texture Texture { get; private set; }
			public SpellDataInst RSpell { get; private set; }
			public Obj_AI_Hero Unit { get; private set; }
			public float DeathEndTime { get; set; }
			public float LastVisible { get; set; }
			public Vector3 LastPosition { get; set; }
			public float LastTeleportStatusTime { get; private set; }

			public Packet.S2C.Teleport.Status TeleportStatus
			{
				get { return _teleportStatus; }
				set
				{
					_teleportStatus = value;
					LastTeleportStatusTime = Game.Time;
				}
			}
		}
*/

struct EnemyObject
{
	TeleportTypes _teleportStatus;
	SDK_SPELL RSpell;
	AIHeroClient* Unit;
	float DeathEndTime;
	float LastVisible;
	Vector3 LastPosition;
	float LastTeleportStatusTime;
	int ChampIMG;
	int SpellIMG1;
	int SpellIMG2;

	EnemyObject(AIHeroClient* _Unit)
		: Unit(_Unit)
	{

		_teleportStatus = TeleportTypes::UnknownStatus;
		RSpell = _Unit->GetSpell((unsigned char)SpellSlot::R);

		LastVisible = Game::Time();
		LastPosition = Vector3(0.0f, 0.0f, 0.0f);

		DeathEndTime = 0.0f;
		LastTeleportStatusTime = 0.0f;
		//SdkUiConsoleWrite("img nmber1 %s", _Unit->GetCharName());
		ChampIMG = ChampionNames::GetChampionIMG(_Unit->GetCharName(), 145);
	//	SpellIMG1 = ChampionNames::GetSummonerSpellIMG(_Unit->GetSpell((unsigned char)SpellSlot::Summoner1).ScriptName, 297);
		//SpellIMG2 = ChampionNames::GetSummonerSpellIMG(_Unit->GetSpell((unsigned char)SpellSlot::Summoner2).ScriptName, 297);



	//	SdkUiConsoleWrite("img nmber %d", ChampIMG);
		/*
			TeleportStatus = Packet.S2C.Teleport.Status.Unknown;
				Unit = unit;
				Texture = texture;
				RSpell = unit.GetSpell(SpellSlot.R);
				LastVisible = Game.Time;
				LastPosition = Vector3.Zero;
		*/
	}
};


class SideBar
{
public:
	static void Init();

	static void MenuLoader();
	static void TickLoader();
	static void DrawLoader();


	static void __cdecl RecallTrack(void* Unit, const char* Name, const char* Type, void* UserData);
};

