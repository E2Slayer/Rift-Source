#pragma once


#include "../../Include/SDK Extensions.h"

class JungleTimer
{
private:
	//static AttackableUnit CurrentTarget;
	//static AttackableUnit* OrbTarget;
	static std::map<unsigned int, GameObject*> GameObjects;
public:

	static void Init();

	static void	__cdecl	Tick(_In_ void* UserData);
	static void	__cdecl	Update(_In_ void* UserData);
	static void	__cdecl	Draw(_In_ void* UserData);
	static void __cdecl DrawMenu(_In_ void* UserData);

	static bool __cdecl OnCreate(_In_ void* Object, _In_ unsigned int NetworkID, _In_opt_ void* UserData);
	static bool __cdecl OnDelete(_In_ void* Object, _In_ unsigned int NetworkID, _In_opt_ void* UserData);
	
};



class testClass
{
public:
	bool testBool;
};

struct Monster
{
	Monster(const char* _MonsterName, bool _isBig, bool _isDead)
	{
		MonsterName = _MonsterName;
		isBig = _isBig;
		isDead = _isDead;
	}

	const char* MonsterName;
	bool isBig;
	bool isDead;
};



struct MonsterList
{

	MonsterList(float spawnTime, float respawnTime, Vector3 position, std::vector<Monster> mobs, bool isBig, bool isDead)
	{
		SpawnTime = spawnTime;
		RespawnTime = respawnTime;
		Position = position;
		Mobs = mobs;
		IsBig = isBig;
		IsDead = isDead;
	}


	void DeadSetter()
	{
		if (IsDead)
		{
			IsDead = false;
		}
		else
		{
			IsDead = true;
		}
	}

	float SpawnTime;
	float RespawnTime;
	Vector3 Position;
	std::vector<Monster> Mobs;
	bool IsBig;
	bool IsDead;
};




void JungleMonstersAdder();
