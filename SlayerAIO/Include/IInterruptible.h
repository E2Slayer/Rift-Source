#pragma once

#pragma warning(push, 0)
#pragma warning(disable: 4774)
#include <string>
#include <vector>
#pragma warning(pop)


class AIHeroClient;

enum class SpellDangerLevel {
	Low,
	Medium,
	High,
	VeryHigh
};
struct InterruptibleSpellData {
	unsigned char Slot;
	std::string Name;
	SpellDangerLevel DangerLevel;
	bool MovementInterrupts;

	bool IsValid() {
		return Slot <= SPELL_SLOT_MAX;
	}
};
struct InterruptibleSpellDataInst {
	AIHeroClient* Sender;
	float EndTime;
	SDK_SPELL_CAST Spell;
	InterruptibleSpellData Data;
};


class IInterruptible {	
public:	   
	virtual bool IsCastingInterruptibleSpell(unsigned int nID, float delay = 0.0f) = 0;
	virtual InterruptibleSpellDataInst GetInterruptibleSpellDataInst(unsigned int nID) = 0;

	virtual bool IsInterruptibleSpell (std::string championName, unsigned char slot) = 0;
	virtual bool IsInterruptibleSpell (std::string championName, std::string spellName) = 0;
	virtual bool HasInterruptibleSpell(std::string championName) = 0;

	virtual InterruptibleSpellData GetInterruptibleSpellData(std::string championName, unsigned char slot) = 0;
	virtual InterruptibleSpellData GetInterruptibleSpellData(std::string championName, std::string spellName) = 0;
	virtual std::vector<InterruptibleSpellData> GetInterruptibleSpellData(std::string championName) = 0;

	virtual void AddInterruptibleSpell(std::string championName, InterruptibleSpellData& data) = 0;
	virtual void RemoveInterruptibleSpell(std::string championName, std::string spellName) = 0;
};
