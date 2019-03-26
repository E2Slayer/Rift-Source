#pragma once
#include "Trackers.h"
#include "resource.h"

/*
internal class ManualSpell
	{
		public ManualSpell(string champ, string spell, SpellSlot slot, float[] cooldowns, float additional = 0)
		{
			Champ = champ;
			Spell = spell;
			Slot = slot;
			Cooldowns = cooldowns;
			Additional = additional;
		}

		public string Champ { get; private set; }
		public string Spell { get; private set; }
		public SpellSlot Slot { get; private set; }
		public float[] Cooldowns { get; set; }
		public float Additional { get; set; }
		public float Cooldown { get; set; }
		public float CooldownExpires { get; set; }
	}
*/

struct ManualSpell
{
	const char* Champ;
	const char* SpellName;
	SpellSlot Slot;
	float Cooldowns[5];
	float Additional;
	float Cooldown;
	float CooldownExpires;

	ManualSpell(const char* _Champ, const char* _SpellName, SpellSlot _Slot, float(&_Coooldowns)[5], float _Additional)
		: Champ(_Champ), SpellName(_SpellName), Slot(_Slot), Additional(_Additional)
	{

		for (int i = 0; i < 5; ++i) 
		{
			Cooldowns[i] = _Coooldowns[i];
		}

		Cooldown = 0.0f;
		CooldownExpires = 0.0f;
	}
};


class CooldownTracker
{
public:
	static void Init();
	static void MenuLoader();
	static void TickLoader();
	static void DrawLoader();

	static void __cdecl SpellCastStart(void* AI, PSDK_SPELL_CAST SpellCast, void* UserData);
};

