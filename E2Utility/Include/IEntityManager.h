#pragma once

#pragma warning(push, 0)
#pragma warning(disable: 4365)
#include <map>
#pragma warning(pop)

#include "Vectors.hpp"

class GameObject;
class AttackableUnit;
class AIBaseClient;
class AIHeroClient;
class AIMinionClient;
class AITurretClient;
class MissileClient;

class IEntityManager {
public:
	virtual std::map<unsigned int, AIHeroClient*>    GetEnemyHeroes		(float Range = HUGE_VALF, Vector3* From = NULL) = 0;
	virtual std::map<unsigned int, AIHeroClient*>    GetAllyHeroes		(float Range = HUGE_VALF, Vector3* From = NULL) = 0;
	virtual std::map<unsigned int, AIMinionClient*>  GetAllyMinions		(float Range = HUGE_VALF, Vector3* From = NULL) = 0;
	virtual std::map<unsigned int, AIMinionClient*>  GetEnemyMinions	(float Range = HUGE_VALF, Vector3* From = NULL) = 0;
	virtual std::map<unsigned int, AITurretClient*>  GetAllyTurrets		(float Range = HUGE_VALF, Vector3* From = NULL) = 0;
	virtual std::map<unsigned int, AITurretClient*>  GetEnemyTurrets	(float Range = HUGE_VALF, Vector3* From = NULL) = 0;
	virtual std::map<unsigned int, MissileClient*>   GetAllyMissiles	(float Range = HUGE_VALF, Vector3* From = NULL) = 0;
	virtual std::map<unsigned int, MissileClient*>   GetEnemyMissiles	(float Range = HUGE_VALF, Vector3* From = NULL) = 0;
	virtual std::map<unsigned int, AIMinionClient*>  GetJungleMonsters	(float Range = HUGE_VALF, Vector3* From = NULL) = 0;
	virtual std::map<unsigned int, AIMinionClient*>  GetEnemyWards		(float Range = HUGE_VALF, Vector3* From = NULL) = 0;
	virtual std::map<unsigned int, AIMinionClient*>  GetAllyWards		(float Range = HUGE_VALF, Vector3* From = NULL) = 0;
	virtual std::map<unsigned int, GameObject*>		 GetParticles		(float Range = HUGE_VALF, Vector3* From = NULL) = 0;
	virtual std::map<unsigned int, GameObject*>		 GetShops			(float Range = HUGE_VALF, Vector3* From = NULL) = 0;
	virtual std::map<unsigned int, AttackableUnit*>  GetNexus			(float Range = HUGE_VALF, Vector3* From = NULL) = 0;
	virtual std::map<unsigned int, AttackableUnit*>  GetInhibitors		(float Range = HUGE_VALF, Vector3* From = NULL) = 0;
	virtual std::map<unsigned int, AIMinionClient*>  GetAzirSoldiers	(float Range = HUGE_VALF, Vector3* From = NULL) = 0;

	virtual AITurretClient * GetNearestTurret(AIBaseClient* Obj, Vector3* Pos, int Team = 0) = 0;
	//virtual AIHeroClient*	 GetNearestHero  (AIBaseClient* Obj, Vector3* Pos, int Team = 0) = 0;
	//virtual Vector3*		 GetNearestWall  (AIBaseClient* Obj, Vector3* Pos) = 0;

	virtual bool IsValidObj(unsigned int nID) = 0;
	virtual bool IsValidObj(void * Obj) = 0;

	virtual GameObject* GetObjectFromPTR(void * Object) = 0;
	virtual GameObject* GetObjectFromID(unsigned int ID) = 0;

	virtual AIHeroClient GetLocalPlayer() = 0;
};