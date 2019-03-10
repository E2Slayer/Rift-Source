#include "stdafx.h"
#include "KalistaDamage.h"


float Damage::GetRendDamage(AIBaseClient* target, int StackCount)
{
	if (StackCount <= 0) // no need to calculate if there is no rend stack
	{
		return 0;
	}
	else if (target == nullptr || target == NULL)
	{
		return 0;
	}


	float rawRendDamage[] = { 20, 30, 40, 50, 60 };
	float rawRendDamageMultiplier[] = { 0.6f, 0.6f, 0.6f, 0.6f, 0.6f };
	float rawRendDamagePerSpear[] = { 5, 9, 14, 25, 32 };
	float rawRendDamagePerSpearMultiplier[] = { 0.2f, 0.2375f, 0.275f, 0.315f, 0.35f };


	// 10, 14, 19, 

	int CurrentELevel = Player.GetSpell((char)SpellSlot::E).Level - 1;
	float attackDmg = Player.GetAttackDamage();


	float rawDmg = (rawRendDamage[CurrentELevel] + rawRendDamageMultiplier[CurrentELevel] * attackDmg) + // Base damage
		(static_cast<float>(StackCount - 1) * (rawRendDamagePerSpear[CurrentELevel] + rawRendDamagePerSpearMultiplier[CurrentELevel] * attackDmg)); // Damage per spear

	auto done = pSDK->DamageLib->CalculatePhysicalDamage(Player.AsAIBaseClient(), target, rawDmg);


	if (target->GetName() != NULL)
	{
		std::string s1(target->GetName());

		if (strstr(s1.c_str(), "SRU_Dragon") || strstr(s1.c_str(), "SRU_Baron") || strstr(s1.c_str(), "SRU_RiftHerald")) //SRU_RiftHerald
		{
			done = done / 2.0f;
		}
	}

	
	if (Player.HasBuff("SummonerExhuast"), false)
	{
		done = done * 0.6f;
	}

	return done;

}


int Damage::NumberOfRendBuff(AIBaseClient* target)
{
	return pSDK->BuffManager->GetBuffCount(target->GetNetworkID(), "kalistaexpungemarker", false);
}

float Damage::RendDamageToHealth(AIBaseClient* target, bool rawDamage)
{

	auto hi2 = pSDK->BuffManager->GetBuffStacks(target->GetNetworkID(), "kalistaexpungemarker", false);

	float dmg = GetRendDamage(target, hi2);

	if (rawDamage)
	{
		return dmg;
	}

	auto health = target->GetHealth();

	float totalHealth = health.Current + health.AllShield;

	float calc = ((dmg / totalHealth) * 100.0f);
	return calc;
}


bool Damage::IsRendKillable(AIBaseClient* target)
{

	auto hi2 = pSDK->BuffManager->GetBuffStacks(target->GetNetworkID(), "kalistaexpungemarker", false);

	float dmg = GetRendDamage(target, hi2);

	auto health = target->GetHealth();

	float totalHealth = health.Current + health.AllShield;

	float calc = ((dmg / totalHealth) * 100.0f);

	return calc >= 100.0f;
}

float Damage::GetPierceDamage(AIBaseClient* target)
{
	if (target == NULL || target == nullptr)
	{
		return 0.0f;
	}

	auto damage = 0.0f;
	const auto spellLevel = Player.GetSpell((char)SpellSlot::Q).Level - 1;
	const float baseDamage[] = { 20.0f, 80.0f, 150.0f, 215.0f, 280.0f };

	damage = baseDamage[spellLevel] + Player.GetAttackDamage() * 1.0f;
	damage = pSDK->DamageLib->CalculateMagicalDamage(&Player, target, damage);

	if (Player.HasBuff("SummonerExhuast"), false)
	{
		damage = damage * 0.6f;
	}


	return damage;
}