#pragma once

#pragma warning(push, 0)
#pragma warning(disable: 4774)
#include <string>
#pragma warning(pop)

#include "sdkapi.h"

struct PerkInstance {
	unsigned int ID;
	std::string Name;
};

struct BuffInstance {
	std::string Name;
	unsigned char Type;
	const char* TypeStr;
	float StartTime;
	float EndTime;
	bool HasCount;
	int Count;
	PSDK_SPELL Spell;
	void* Caster;
	int Stacks;

	inline bool IsValid() {
		float time = 0.0f;
		SdkGetGameTime(&time);
		return Stacks > 0 && time < EndTime;
	}
};

class IBuffManager {
public:
	//Avoid Using This, its slow.
	virtual std::vector<BuffInstance> GetBuffs(unsigned int nID) = 0;

	virtual bool HasBuff(unsigned int nID, std::string buffName, bool partialName = false) = 0;
	virtual bool HasBuffType(unsigned int nID, unsigned char buffType) = 0;
	virtual BuffInstance GetBuff(unsigned int nID, std::string buffName, bool partialName = false) = 0;
	virtual BuffInstance GetBuffByType(unsigned int nID, unsigned char buffType) = 0;	
	virtual int  GetBuffStacks(unsigned int nID, std::string buffName, bool partialName = false) = 0;
	virtual int  GetBuffCount(unsigned int nID, std::string buffName, bool partialName = false) = 0;	

	virtual std::vector<PerkInstance> GetPerks(AIHeroClient* Hero) = 0;
	virtual bool HasPerk(AIHeroClient* Hero, unsigned int ID) = 0;
	virtual bool HasPerk(AIHeroClient* Hero, std::string Name) = 0;
};