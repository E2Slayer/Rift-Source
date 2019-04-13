#pragma once

#pragma warning(push, 0)
#pragma warning(disable: 4774)
#include <string>
#pragma warning(pop)

#include "Vectors.hpp"

struct GapcloseData {
	std::string SpellName;
	bool IsBlink;
	bool IsTargetted;

	bool  CanVaryInLength;
	float MaxRange;
	float Speed;
	float WindUp;     //Time hero stays immobile before dash/blink
	float WindDown;   //Time hero stays immobile before dash/blink	

	bool IsValid() {
		return SpellName != "";
	}
};

struct GapcloseDataInst {	
	AIHeroClient* Sender;
	float EndTime;

	SDK_SPELL_CAST Spell;
	GapcloseData Data;
};

class IGapclose {
public:
	virtual bool IsBlinking(unsigned int netID) = 0;
	virtual bool IsDashing (unsigned int netID) = 0;	
	virtual GapcloseDataInst GetGapcloseDataInst(unsigned int nID) = 0;

	virtual bool HasGapclose(std::string championName) = 0;
	virtual GapcloseData GetGapcloseData(std::string championName, std::string spellName) = 0;
	virtual std::vector<GapcloseData> GetGapcloseData(std::string championName) = 0;

	virtual void AddGapclose(std::string spellName, GapcloseData& data) = 0;
	virtual void RemoveGapclose(std::string championName, std::string spellName) = 0;
};
