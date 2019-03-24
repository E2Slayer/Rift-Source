#pragma once

#pragma warning(push, 0)
#include <memory>
#pragma warning(pop)

#include "Vectors.hpp"
#include "sdkapi.h"
#include "Macros.hpp"
#include "Interface Structs.h"
#include "Common.hpp"


extern std::shared_ptr<SDK> pSDK;
__declspec(selectany) std::map<std::string, bool> LoadedHeroes;

class AIHeroClient;
struct NavData {
	void* AI;
	SDKVECTOR StartPos;
	SDKVECTOR EndPos;
	size_t NextWaypoint;
	size_t NumberOfWaypoints;
	SDKVECTOR* Waypoints;
	SDKVECTOR Velocity;
	bool IsDashing;
	float DashSpeed;
	float DashGravity;
};

enum class GameObjectType {
	AIHeroClient = 1,
	obj_AI_Minion = 3,
	obj_AI_Turret = 6,
	obj_GeneralParticleEmitter = 8,
	MissileClient = 10,
	obj_BarracksDampener = 13,
	obj_HQ = 21,
	LevelPropGameObject = 23,
	obj_Shop = 25,
};

__declspec(selectany) std::map<unsigned char, bool> cantProcessAttackBuffs {
	{ (unsigned char)BUFF_TYPE_STUN, true},
	{ (unsigned char)BUFF_TYPE_TAUNT, true},
	{ (unsigned char)BUFF_TYPE_POLYMORPH, true},
	{ (unsigned char)BUFF_TYPE_FEAR, true} ,
	{ (unsigned char)BUFF_TYPE_CHARM, true} ,
	{ (unsigned char)BUFF_TYPE_SUPPRESSION, true} ,
	{ (unsigned char)BUFF_TYPE_BLIND, true}, 
	{ (unsigned char)BUFF_TYPE_FLEE, true},
	{ (unsigned char)BUFF_TYPE_KNOCKUP, true} ,
	{ (unsigned char)BUFF_TYPE_KNOCKBACK, true}, 
	{ (unsigned char)BUFF_TYPE_DISARM, true}, 
	{ (unsigned char)BUFF_TYPE_ASLEEP, true} 
};

__declspec(selectany) std::map<unsigned char, bool> cantProcessMovementBuffs{
	{ (unsigned char)BUFF_TYPE_STUN, true},
	{ (unsigned char)BUFF_TYPE_TAUNT, true},	
	{ (unsigned char)BUFF_TYPE_SNARE, true},
	{ (unsigned char)BUFF_TYPE_FEAR, true},
	{ (unsigned char)BUFF_TYPE_CHARM, true},
	{ (unsigned char)BUFF_TYPE_SUPPRESSION, true},
	{ (unsigned char)BUFF_TYPE_FLEE, true},
	{ (unsigned char)BUFF_TYPE_KNOCKUP, true},
	{ (unsigned char)BUFF_TYPE_KNOCKBACK, true},
	{ (unsigned char)BUFF_TYPE_ASLEEP, true}
};

__declspec(selectany) std::map<unsigned char, bool> immobileBuffs{
	{ (unsigned char)BUFF_TYPE_STUN, true},
	{ (unsigned char)BUFF_TYPE_SNARE, true},
	{ (unsigned char)BUFF_TYPE_SUPPRESSION, true},
	{ (unsigned char)BUFF_TYPE_KNOCKUP, true},
	{ (unsigned char)BUFF_TYPE_ASLEEP, true}
};

__declspec(selectany) std::map<std::string, bool> InvalidTargets{
	{("JarvanIVStandard")			, true},
	{("ZyraSeed")					, true},
	{("TestCubeRender10Vision")		, true},
	{("PreSeason_Turret_Shield")	, true},
	{("SyndraSphere")				, true},
};

class GameObject {
public:
	void* Object; // object ptr
	unsigned int NetworkID;

	GameObject() {};
	GameObject(void*obj) : Object(obj) {
		SdkGetObjectNetworkID(Object, &NetworkID);		
	};

	~GameObject() {};

	static void* GetObjFromID(unsigned int ID) {
		void* Obj = NULL;
		SdkGetObjectFromID(ID, &Obj);
		return Obj;
	};	

	MAKE_GET(Position, SDKVECTOR, SdkGetObjectPosition);
	MAKE_GET(Orientation, SDKVECTOR, SdkGetObjectOrientation);
	MAKE_GET(BBox, SDKBOX, SdkGetObjectBoundingBox);
	MAKE_GET(TeamID, int, SdkGetObjectTeamID);
	MAKE_GET(TypeID, int, SdkGetObjectTypeID);
	MAKE_GET(TypeFlags, int, SdkGetObjectTypeFlags);
	MAKE_RAW(IsZombie, bool, SdkIsObjectZombie);
	MAKE_GET_DEF(Name, const char*, SdkGetObjectName, "");
	MAKE_RAW(IsVisible, bool, SdkIsUnitVisible);
	MAKE_RAW(IsVisibleOnScreen, bool, SdkIsObjectVisibleOnScreen);
	MAKE_GET(NetworkID, unsigned int, SdkGetObjectNetworkID);

	bool operator < (GameObject& b) {
		return GetNetworkID() < b.GetNetworkID();
	}

	GameObject* GetEntity() {
		return pSDK->EntityManager->GetObjectFromID(GetNetworkID());
	}
	GameObject* GetObjectOwner() {
		unsigned int netID{0};
		SdkGetObjectOwner(Object, &netID, NULL);
		return pSDK->EntityManager->GetObjectFromID(netID);
	}
	GameObject* GetParticleOwner() {
		void* obj{ NULL };
		SdkGetParticleOwner(Object, &obj);
		return pSDK->EntityManager->GetObjectFromPTR(obj);
	}

	AttackableUnit*	AsAttackableUnit() {
		return (IsAttackable()) ? (AttackableUnit*)this : NULL;
	}
	AIBaseClient* AsAIBaseClient() {
		return (IsAI()) ? (AIBaseClient*)this : NULL;
	}
	AIHeroClient* AsAIHeroClient() {
		return (IsHero()) ? (AIHeroClient*)this : NULL;
	}
	AIMinionClient*	AsAIMinionClient() {
		return (IsMinion()) ? (AIMinionClient*)this : NULL;
	}
	AITurretClient*	AsAITurretClient() {
		return (IsTurret()) ? (AITurretClient*)this : NULL;
	}
	MissileClient* AsMissileClient() {
		return (IsMissile()) ? (MissileClient*)this : NULL;
	}

	static bool IsHeroInGame(std::string Name, bool EnemiesOnly = false);

	float GetBoundingRadius() {
		float bR = 0.0f;
		SdkGetObjectBoundingRadius(Object, &bR);
		return bR;
	}

	bool IsEnemy() {		
		int tmpID{-1};
		CHECKFAIL(SdkGetObjectTeamID(g_LocalPlayer, &tmpID));
		return (300 - tmpID) == GetTeamID();
	}

	bool IsAlly() {		
		int tmpID{-1};
		CHECKFAIL(SdkGetObjectTeamID(g_LocalPlayer, &tmpID));
		return tmpID == GetTeamID();
	}
	bool IsAlive() {		
		bool tmpb{true};
		CHECKFAIL(SdkIsObjectDead(Object, &tmpb));
		return !tmpb;
	}

	bool IsAttackable() {
		return Object != NULL && SDKSTATUS_SUCCESS(SdkIsObjectUnit(Object));
	}
	bool IsAI() {
		return Object != NULL && SDKSTATUS_SUCCESS(SdkIsObjectAI(Object));
	}
	bool IsMinion() {
		return Object != NULL && SDKSTATUS_SUCCESS(SdkIsObjectMinion(Object));
	}
	bool IsHero() {
		return Object != NULL && SDKSTATUS_SUCCESS(SdkIsObjectHero(Object));
	}
	bool IsTurret() {
		return Object != NULL && SDKSTATUS_SUCCESS(SdkIsObjectTurret(Object));
	}
	bool IsMissile() {
		return Object != NULL && SDKSTATUS_SUCCESS(SdkIsObjectSpellMissile(Object));
	}
	bool IsStructure() {
		return Object != NULL && (GetTypeFlags() & OBJECT_FLAG_BUILDING);
	}
	bool IsHQ() {
		return Object != NULL && (GetTypeID() == (int)GameObjectType::obj_HQ);
	}
	bool IsShop() {
		return Object != NULL && (GetTypeID() == (int)GameObjectType::obj_Shop);
	}
	bool IsInhibitor() {
		return Object != NULL && (GetTypeID() == (int)GameObjectType::obj_BarracksDampener);
	}
	bool IsParticle() {
		return Object != NULL && (GetTypeID() == (int)GameObjectType::obj_GeneralParticleEmitter);
	}

	bool IsValid() {
		return Object != NULL && pSDK->EntityManager->IsValidObj(GetNetworkID());
	}

	void DrawHitBox() {
		auto bBox = GetBBox();
		auto &max{ bBox.Max };
		auto &min{ bBox.Min };

		std::vector<Vector3> Points{
			{min.x, max.y, min.z },	{min.x, max.y, max.z },	{max.x, max.y, max.z },	{max.x, max.y, min.z },
			{min.x, min.y, min.z },	{min.x, min.y, max.z },	{max.x, min.y, max.z },	{max.x, min.y, min.z },
		};
		for (size_t i = 0; i < 8; ++i) {
			if (i < 4)
				Draw::Line(&(Points[i]), &(Points[i + 4]), 2.0f, &Color::Red);
			if (i == 3 || i == 7)
				Draw::Line(&(Points[i]), &(Points[i - 3]), 2.0f, &Color::Red);
			else
				Draw::Line(&(Points[i]), &(Points[i + 1]), 2.0f, &Color::Red);
		}
	}

	float Distance(GameObject* b) {
		if (b == NULL || !b->IsValid()) { return HUGE_VALF; };
		Vector3 Pos{ b->GetPosition() };
		return GetPosition().Distance(Pos);
	}

	float Distance(Vector3& b) {
		if (b == NULL || !b.IsValid()) { return HUGE_VALF; };
		return GetPosition().Distance(b);
	}

	float Distance(Vector2& b) {
		if (b == NULL || !b.IsValid()) { return HUGE_VALF; };
		return GetPosition().To2D().Distance(b);
	}

	float Distance(Vector3* b) {
		if (b == NULL || !b->IsValid()) { return HUGE_VALF; };
		return GetPosition().Distance(*b);
	}

	float Distance(Vector2* b) {
		if (b == NULL || !b->IsValid()) { return HUGE_VALF; };
		return GetPosition().To2D().Distance(*b);
	}	

	void* PTR() {
		return Object;
	}	
	
	bool IsRealTarget();
	bool CanSpellHit();

	bool IsValidTarget(bool CheckIfSpellCanHit = true) {
		return IsValid() && IsAlive() && !IsZombie() && IsVisible() && IsRealTarget() && (!CheckIfSpellCanHit || CanSpellHit());
	}

	bool IsRealWard();
	bool IsJunglePlant();
	bool IsBarrel();
};

class AttackableUnit : public GameObject
{
public:
	AttackableUnit() {};
	AttackableUnit(void* obj) : GameObject(obj) {};
	~AttackableUnit() {};
	
	MAKE_GET(ServerPosition, SDKVECTOR, SdkGetAIServerPosition)
	MAKE_RAW(IsMouseOver, bool, SdkGetUnitIsMouseOver);
	MAKE_GET(DeathTime, float, SdkGetUnitDeathTime);
	MAKE_GET(Health, SDK_HEALTH, SdkGetUnitHealth);
	MAKE_RAW(IsCasting, bool, SdkIsAICasting);
	MAKE_GET(Targetability, int, SdkGetUnitTargetability);
	MAKE_RAW(IsInvulnerable, bool, SdkIsUnitInvulnerable);
	MAKE_GET(HealthBarPos, Vector3, SdkGetAIHealthbarWorldPosition);
	MAKE_GET(HealthBarScreenPos, Vector2, SdkGetAIHealthbarScreenPosition);
	MAKE_GET_DEF(CharName, const char*, SdkGetAIName, "");
	MAKE_GET(VisionRadius, float, SdkGetUnitVisionRadius);
	
	NavData CreateNavPath(PSDKVECTOR DesiredEndPosition, bool SmoothPath = false) {
		NavData res{Object};
		res.NextWaypoint = 1;
		SdkCreateAIPath(Object, DesiredEndPosition, SmoothPath, &(res.StartPos), &(res.EndPos), &(res.NumberOfWaypoints), &(res.Waypoints));		
		return res;
	}	

	bool IsTargetable() {
		int res = GetTargetability();
		return (res & TARGET_FLAG_TARGETABLE_ALL_TEAMS);
	}

	bool IsTargetableToTeam() {
		int res = GetTargetability();
		return (res & TARGET_FLAG_UNTARGETABLE_ALLY_TEAM) == 0;
	}

	bool IsMonster() {
		return GetTeamID() == TEAM_TYPE_NEUTRAL;
	}
};

class AIBaseClient : public AttackableUnit {
public:
	AIBaseClient() {};
	AIBaseClient(void* obj) : AttackableUnit(obj) {};
	~AIBaseClient() {};

	MAKE_GET(SkinID, int, SdkGetAISkinID);
	MAKE_RAW(PlayerControlled, bool, SdkIsAIPlayerControlled);
	MAKE_RAW(CanAttack, bool, SdkCanAIAttack);
	MAKE_RAW(CanCrit, bool, SdkCanAICrit);
	MAKE_RAW(CanCast, bool, SdkCanAICast);
	MAKE_RAW(CanMove, bool, SdkCanAIMove);
	MAKE_RAW(Stealthed, bool, SdkIsAIStealthed);
	MAKE_RAW(Taunted, bool, SdkIsAITaunted);
	MAKE_RAW(Feared, bool, SdkIsAIFeared);
	MAKE_RAW(Fleeing, bool, SdkIsAIFleeing);
	MAKE_RAW(Suppressed, bool, SdkIsAISuppressed);
	MAKE_RAW(Sleeping, bool, SdkIsAISleeping);
	MAKE_RAW(NearSighted, bool, SdkIsAINearSighted);
	MAKE_RAW(Ghosted, bool, SdkIsAIGhosted);
	MAKE_RAW(Charmed, bool, SdkIsAICharmed);
	MAKE_RAW(Slowed, bool, SdkIsAISlowed);
	MAKE_RAW(Selectable, bool, SdkIsAISelectable);
	MAKE_RAW(CritImmune, bool, SdkIsAICritImmune);
	MAKE_RAW(Grounded, bool, SdkIsAIGrounded);
	MAKE_RAW(Obscured, bool, SdkIsAIObscured);
	MAKE_RAW(Killable, bool, SdkIsAIKillable);
	MAKE_GET(Armor, float, SdkGetAIArmor);
	MAKE_GET(Lethality, float, SdkGetAILethality);
	MAKE_GET(AttackSpeed, float, SdkGetAIAttackSpeed);
	MAKE_GET(AbilityPower, float, SdkGetAIAbilityPower);
	MAKE_GET(AttackDamage, float, SdkGetAIAttackDamage);
	MAKE_GET(AttackRange, float, SdkGetAIAttackRange);
	MAKE_GET(MagicResist, float, SdkGetAIMagicResist);
	MAKE_GET(Tenacity, float, SdkGetAITenacity);
	MAKE_GET(MovementSpeed, float, SdkGetAIMovementSpeed);
	MAKE_GET(CritChance, float, SdkGetAICritChance);
	MAKE_GET(CritDamageMultiplier, float, SdkGetAICritDamageMultiplier);
	MAKE_GET(DodgeChance, float, SdkGetAIDodgeChance);
	MAKE_GET(HealthRegen, float, SdkGetAIHealthRegen);
	MAKE_GET(LifeSteal, float, SdkGetAILifeSteal);
	MAKE_GET(SpellVamp, float, SdkGetAISpellVamp);
	MAKE_GET(CooldownReduction, float, SdkGetAICooldownReduction);
	MAKE_RAW(IsBot, bool, SdkIsAIBot);
	MAKE_GET(CombatType, int, SdkGetAICombatType);
	MAKE_GET(DeathDuration, float, SdkGetAIDeathDuration);
	MAKE_RAW(IsAttacking, bool, SdkIsAIAttacking);
	MAKE_RAW(IsMoving, bool, SdkIsAIMoving);
	MAKE_GET(AttackDelay, float, SdkGetAIAttackDelay);
	MAKE_GET(AttackCastDelay, float, SdkGetAIAttackCastDelay);
	MAKE_GET(AttackData, SDK_SPELL, SdkGetAIBasicAttack);
	MAKE_GET(BaseAttackDamage, float, SdkGetAIBaseAttackDamage);
	MAKE_GET(BonusAttackDamage, float, SdkGetAIBonusAttackDamage);
	MAKE_GET(Direction, SDKVECTOR, SdkGetAIFacingDirection);	

	bool IsFacing(Vector2& position) {
		auto Pos{ position - GetPosition().To2D() };
		return (IsValid() && GetDirection().AngleBetween(Pos) < 90.0f);
	}

	bool IsFacing(Vector3& position) {
		auto Pos{ (position - GetPosition()).To2D() };
		return (IsValid() && GetDirection().AngleBetween(Pos) < 90.0f);
	}

	bool IsFacing(Vector2* position) {
		auto Pos{ *position - GetPosition().To2D() };
		return (IsValid() && GetDirection().AngleBetween(Pos) < 90.0f);
	}

	bool IsFacing(Vector3* position) {
		auto Pos{ (*position - GetPosition()).To2D() };
		return (IsValid() && GetDirection().AngleBetween(Pos) < 90.0f);
	}

	bool IsFacing(AIBaseClient* target) {
		auto position { target->GetPosition() };

		return IsFacing(&position);
	}

	float GetFlatArmorPen() {
		float tmp{};
		CHECKFAIL(SdkGetAIArmorPenetration(Object, &tmp, NULL));
		return tmp;
	}
	float GetPercentArmorPen() {
		float tmp{};
		CHECKFAIL(SdkGetAIArmorPenetration(Object, NULL, &tmp));
		return tmp;
	}
	float GetFlatMagicPen() {
		float tmp{};
		CHECKFAIL(SdkGetAIMagicPenetration(Object, &tmp, NULL));
		return tmp;
	}
	float GetPercentMagicPen() {
		float tmp{};
		CHECKFAIL(SdkGetAIMagicPenetration(Object, NULL, &tmp));
		return tmp;
	}

	SDK_ABILITY_RESOURCE GetResource(unsigned char Slot = ABILITY_SLOT_PRIMARY) {
		SDK_ABILITY_RESOURCE tmp{};
		CHECKFAIL(SdkGetUnitAbilityResource(Object, Slot, &tmp));
		return tmp;
	}

	float GetHealthPercent() {
		auto tmp{ GetHealth() };
		return (tmp.Current / tmp.Max) * 100.0f;
	}

	float GetManaPercent() {
		auto tmp{ GetResource() };
		return (tmp.Current / tmp.Max) * 100.0f;
	}

	float GetAbilityResourceRegens(unsigned char Slot = ABILITY_SLOT_PRIMARY) {
		float tmp{};
		CHECKFAIL(SdkGetAIAbilityResourceRegen(Object, Slot, &tmp));
		return tmp;
	}

	struct ActiveSpell {
		void* AI;
		bool  Valid;
		SDK_SPELL_CAST SpellCast;
		void* ParticleObject;
		float EndTime;
		bool  SpellWasCast;
		bool  IsCharging;
		bool  IsChanneling;
		float ChannelEndTime;
		bool  IsStopped;
	};

	ActiveSpell GetActiveSpell() {
		ActiveSpell t{};
		CHECKFAIL(SdkGetAIActiveSpell(Object, &(t.Valid), &(t.SpellCast), &(t.ParticleObject), &(t.EndTime), &(t.SpellWasCast), &(t.IsCharging), &(t.IsChanneling), &(t.ChannelEndTime), &(t.IsStopped)));
		return t;
	}

	bool IsChannelling() {
		auto spell{ GetActiveSpell() };
		return spell.Valid && spell.IsChanneling;
	}

	bool IsCharging() {
		auto spell{ GetActiveSpell() };
		return spell.Valid && spell.IsCharging;
	}

	SDK_SPELL GetSpell(unsigned char Slot) {
		SDK_SPELL Spell{};
		CHECKFAIL(SdkGetAISpell(Object, Slot, &Spell));		
		return Spell;
	}

	AttackableUnit* GetTarget() {
		void* Target{ NULL };
		CHECKFAIL(SdkGetAIEnemy(Object, &Target, NULL));
		auto tar{ pSDK->EntityManager->GetObjectFromPTR(Target) };
		return (tar ? tar->AsAttackableUnit() : NULL);
	}

	SDK_ABILITY_RESOURCE GetAbilitySlots(unsigned char index) {
		SDK_ABILITY_RESOURCE tmp1{};
		CHECKFAIL(SdkGetUnitAbilityResource(Object, index, &tmp1));
		return tmp1;
	}

	float GetCurrentGold() {
		float Gold{};
		CHECKFAIL(SdkGetAIGold(Object, &Gold, NULL));
		return Gold;
	}
	float GetTotalGold() {
		float Gold{};
		CHECKFAIL(SdkGetAIGold(Object, NULL, &Gold));
		return Gold;
	}
	void* GetCurrentTarget() {
		void* Target{ NULL };
		CHECKFAIL(SdkGetAIEnemy(Object, &Target, NULL));
		return Target;
	}

	NavData NavInfo() {
		NavData temp{Object, Vector3(), Vector3(), 0, 0, NULL, Vector3(), false, 0.0f, 0.0f};
		SdkGetAINavData(Object, &temp.StartPos, &temp.EndPos, &temp.NextWaypoint, &temp.NumberOfWaypoints, &temp.Waypoints, &temp.Velocity, &temp.IsDashing, &temp.DashSpeed, &temp.DashGravity);
		return temp;
	}

	std::vector<Vector3> GetWaypoints() {
		std::vector<Vector3> result;
		auto nav{ NavInfo() };
		for (size_t i = nav.NextWaypoint; i < nav.NumberOfWaypoints; ++i) {
			result.push_back(nav.Waypoints[i]);
		}
		return result;
	}

	std::vector<Vector2> GetWaypoints2D() {
		std::vector<Vector2> result;
		auto nav{ NavInfo() };
		for (size_t i = nav.NextWaypoint; i < nav.NumberOfWaypoints; ++i) {
			result.push_back(nav.Waypoints[i].To2D());
		}
		return result;
	}

	std::vector<Vector3> GetWaypoints(NavData info) {
		std::vector<Vector3> result;
		auto nav{ info };
		for (size_t i = nav.NextWaypoint; i < nav.NumberOfWaypoints; ++i) {
			result.push_back(nav.Waypoints[i]);
		}
		return result;
	}

	bool IsDashing() {
		bool IsDashing{ false };
		SdkGetAINavData(Object, NULL, NULL, NULL, NULL, NULL, NULL, &IsDashing, NULL, NULL);
		return IsDashing;
	}

	bool IsRunningFrom(GameObject* Unit) {
		if (!IsMoving()) { return false; }

		auto nav{ NavInfo() };
		auto pos{ Unit->GetPosition() };
		return nav.NumberOfWaypoints >= 2 && nav.NextWaypoint < nav.NumberOfWaypoints && nav.Waypoints &&
			   nav.Waypoints[nav.NextWaypoint].DistanceSqr(pos) > GetPosition().DistanceSqr(pos);
	}	

	std::vector<SDK_SPELL> GetSpells(size_t maxSlot = 6) {
		std::vector<SDK_SPELL> spells;
		for (size_t i = 0; i < min(maxSlot, SPELL_SLOT_MAX); ++i) {
			SDK_SPELL Spell{};
			SdkGetAISpell(Object, (unsigned char)i, &Spell);
			spells.push_back(Spell);
		}
		return spells;
	}

	unsigned char GetSpellSlotFromName(std::string name) {
		for (auto &spell : GetSpells()) {
			if (spell.ScriptName == name) {
				return spell.Slot;
			}
		}
		return 66;
	}	

	std::vector<BuffInstance> GetBuffs() {		
		std::vector<BuffInstance> output;
		SdkEnumAIBuffs(Object,
			[](unsigned char Type, float StartTime, float EndTime, const char* Name, void* Caster, unsigned int CasterID,
				int Stacks, bool HasCount, int Count, PSDK_SPELL Spell, void* UserData)  -> bool 
			{
				UNREFERENCED_PARAMETER(CasterID);

				if (!UserData) {
					SdkUiConsoleWrite("[SDK Extensions] EnumBuffs Error");
					return false;
				}
				std::vector<BuffInstance>* Buffs = (std::vector<BuffInstance>*)UserData;
				float CurrentTime;
				SdkGetGameTime(&CurrentTime);

				if (Buffs && CurrentTime < EndTime && Stacks > 0) {
					BuffInstance Buff;
					Buff.Type = Type;
					Buff.Name = Name;
					Buff.StartTime = StartTime;
					Buff.EndTime = EndTime;
					Buff.Count = Count;
					Buff.Spell = Spell;
					Buff.Stacks = Stacks;
					Buff.HasCount = HasCount;
					Buff.Caster = Caster;

					SdkBuffTypeToString(Buff.Type, &Buff.TypeStr);

					Buffs->push_back(Buff);
				}
				return true;
			},
			&output
		);
		return output;
	};	

	bool HasBuff(std::string input, bool partialName = false) {
		return pSDK->BuffManager->HasBuff(GetNetworkID(), input, partialName);
	}

	BuffInstance GetBuff(std::string input, bool partialName = false) {
		return pSDK->BuffManager->GetBuff(GetNetworkID(), input, partialName);
	}

	int GetBuffStacks(std::string input, bool partialName = false) {
		return pSDK->BuffManager->GetBuffStacks(GetNetworkID(), input, partialName);
	}

	int GetBuffCount(std::string input, bool partialName = false) {
		return pSDK->BuffManager->GetBuffCount(GetNetworkID(), input, partialName);
	}

	bool HasBuffType(unsigned char buffType) {
		return pSDK->BuffManager->HasBuffType(GetNetworkID(), buffType);
	}

	BuffInstance GetBuffByType(unsigned char buffType) {
		return pSDK->BuffManager->GetBuffByType(GetNetworkID(), buffType);
	}

	bool CanProcessAttack(float TimeLeft = 0.f) {
		float CurrentTime;	SdkGetGameTime(&CurrentTime);

		for (auto &[type, _] : cantProcessAttackBuffs) {
			auto buff{ GetBuffByType(type) };
			if (buff.IsValid() && buff.EndTime > (CurrentTime + TimeLeft)) {
				return false;
			}
		}

		return true;
	}

	bool CanProcessMove(float TimeLeft = 0.f) {
		float CurrentTime;	CHECKFAIL(SdkGetGameTime(&CurrentTime));

		for (auto &[type, _] : cantProcessMovementBuffs) {
			auto buff{ GetBuffByType(type) };
			if (buff.IsValid() && buff.EndTime > (CurrentTime + TimeLeft)) {
				return false;
			}
		}

		return true;
	}

	bool IsImmobile(float TimeLeft = 0.f) {
		float CurrentTime;	CHECKFAIL(SdkGetGameTime(&CurrentTime));

		for (auto &[type, _] : immobileBuffs) {
			auto buff{ GetBuffByType(type) };
			if (buff.IsValid() && buff.EndTime > (CurrentTime + TimeLeft)) {
				return true;
			}
		}

		return false;
	}

	float ImmobileTimeLeft() {
		float CurrentTime;	CHECKFAIL(SdkGetGameTime(&CurrentTime));
		float bestTime{ 0.0f };

		for (auto &[type, _] : immobileBuffs) {
			auto buff{ GetBuffByType(type) };
			if (buff.IsValid() && buff.EndTime > bestTime) {
				bestTime = buff.EndTime;
			}
		}
		return bestTime > EPSILON ? (bestTime - CurrentTime) : 0.0f;
	}	

	bool IsMelee() {
		int tmp{}; CHECKFAIL(SdkGetAICombatType(Object, &tmp));
		return tmp == COMBAT_TYPE_MELEE;
	}

	std::string SummonerName() {		
		const char* Name = "";
		CHECKFAIL(SdkGetObjectName(Object, &Name));
		return std::string(Name);
	}
	const char* GetSummonerName() {		
		const char* Name = "";
		CHECKFAIL(SdkGetObjectName(Object, &Name));
		return Name;
	}
};

class AIHeroClient : public AIBaseClient
{
public:
	AIHeroClient(void* obj) : AIBaseClient(obj) {};
	AIHeroClient() {};
	~AIHeroClient() {};

	int GetLevel() {
		int Level = 1;
		CHECKFAIL(SdkGetHeroExperience(Object, NULL, &Level));
		return Level;
	}
	float GetExperience() {
		float Exp = 0.0f;
		CHECKFAIL(SdkGetHeroExperience(Object, &Exp, NULL));
		return Exp;
	}

	SDK_ITEM GetItem(unsigned char Slot) {		
		SDK_ITEM Item{0};
		CHECKFAIL(SdkGetHeroItem(Object, Slot, &Item));
		return Item;
	}

	std::map<int, SDK_ITEM> GetItems() {		
		std::map<int, SDK_ITEM> Items;
		for (uint8_t i = 0; i <= 6; ++i) {
			SDK_ITEM Item{0};
			CHECKFAIL(SdkGetHeroItem(Object, i, &Item));
			Items[Item.ItemID] = Item;
		}
		return Items;
	}

	int GetItemSlot(int ItemID) {
		for (uint8_t i = 0; i <= 6; ++i) {
			SDK_ITEM Item{};
			CHECKFAIL(SdkGetHeroItem(Object, i, &Item));
			if (Item.ItemID == ItemID) {
				return Item.Slot;
			}
		}
		return -1;
	}

	bool HasItem(int ItemID) {
		return GetItemSlot(ItemID) != -1;
	}

	bool HasItem(int ItemID, std::map<int, SDK_ITEM>& ItemList) {
		return ItemList.find(ItemID) != ItemList.end();
	}

	std::vector<PerkInstance> GetPerks() {
		return pSDK->BuffManager->GetPerks(this);
	}

	bool HasPerk(unsigned int ID) {
		return pSDK->BuffManager->HasPerk(this, ID);
	}

	bool HasPerk(std::string Name) {
		return pSDK->BuffManager->HasPerk(this, Name);
	}

	unsigned int CountEnemiesInRange(float f) {
		unsigned int result{ 0 };
		auto position { GetPosition() };
		for (auto &[_, Enemy] : pSDK->EntityManager->GetEnemyHeroes(f, &position)) {
			if (pCore->TS->IsValidTarget(Enemy)) {
				++result;
			}
		}
		
		return result;
	}

	unsigned int CountAlliesInRange(float f) {
		unsigned int result{ 0 };
		auto position { GetPosition() };
		for (auto &[_, Allyy] : pSDK->EntityManager->GetAllyHeroes(f, &position)) {
			if (pCore->TS->IsValidTarget(Allyy)) {
				++result;
			}
		}

		return result;
	}

	float GetTrueAttackRange(AttackableUnit* Target = NULL) {
		return pCore->Orbwalker->GetTrueAutoAttackRange(this, Target);
	}

	bool IsRecalling() {
		return HasBuff("recall", false);
	}

	bool IsChannelingImportantSpell(float delay = 0.0f, bool bCheckInterruptedByMove = false) {
		float CurrentTime;	CHECKFAIL(SdkGetGameTime(&CurrentTime));

		auto data{ pSDK->InterruptibleManager->GetInterruptibleSpellDataInst(GetNetworkID()) };
		if (data.Sender && data.Data.IsValid()) {
			if (data.EndTime > (CurrentTime + delay) && (!bCheckInterruptedByMove || data.Data.MovementInterrupts)) {
				return true;
			}
		}		
		return false;
	}

	bool HasUndyingBuff(bool HealthCheck = false) {
		std::map<std::string, std::function<bool()>> selfImmortalLambdas{
			{"Aatrox",     [&]() {return HasBuff("aatroxpassivedeath"); }},
			{"Fiora" ,     [&]() {return HasBuff("FioraW"); }},
			{"Tryndamere", [&]() {return HasBuff("UndyingRage") && (!HealthCheck || GetHealth().Current <= 70.0f); }},
			{"Vladimir"  , [&]() {return HasBuff("VladimirSanguinePool"); }},
		};

		std::map<std::string, std::function<bool()>> allyImmortalLambdas{
			{"Kayle"  , [&]() {return  HasBuff("JudicatorIntervention"); }},
			{"Kindred", [&]() {return  HasBuff("kindredrnodeathbuff") && (!HealthCheck || GetHealthPercent() <= 10.0f); }},
			{"Zilean" , [&]() {return (HasBuff("ChronoShift") || HasBuff("chronorevive")) && (!HealthCheck || GetHealthPercent() <= 10.0f); }},			
		};
		
		auto charNamePtr{ GetCharName() };
		std::string charName{ charNamePtr ? charNamePtr : "" };
		
		if (selfImmortalLambdas.count(charName) > 0) {			
			if (selfImmortalLambdas.at(charName)()) { return true; }
		}
		
		for (auto &[sourceName, func] : allyImmortalLambdas) {
			if (GameObject::IsHeroInGame(sourceName)) {
				if (func()) { return true; }
			}
		}

		return false;
	}

	bool HasSpellShield()	{
		if (HasBuffType(BUFF_TYPE_SPELL_SHIELD) || HasBuffType(BUFF_TYPE_SPELL_IMMUNE)) {
			return true;
		}		
		return false;
	}

	MAKE_GET(NeutralKills, int, SdkGetHeroNeutralKills);

	void DrawDamageOnHP(float dmg) {
		static int constexpr barHeight  {11};
		static int constexpr barWidth   {105};
		static int constexpr barXOffset {-45};
		static int constexpr barYOffset {-23};
		static SDKCOLOR DmgColor { 110, 14, 76, 195 }; 

		auto barPos{ GetHealthBarScreenPos() };
		barPos.x += (float)barXOffset;
		barPos.y += (float)barYOffset;

		if (Renderer::ScreenToWorld(barPos).IsOnScreen()) {
			auto h{ GetHealth() };
			auto percentHealthAfterDamage{  (std::max<float>(0, 100.0f*(h.Current - dmg)/h.Max) ) };
			Vector2 curHealthPos{ barPos.x + barWidth * (h.Current / h.Max), barPos.y };
			Vector2 endPos{ barPos.x + percentHealthAfterDamage,  barPos.y };

			Draw::LineScreen(&endPos, &curHealthPos, barHeight, &DmgColor);
		}		
	}
};

class AIMinionClient : public AIBaseClient {
public:
	AIMinionClient() {};
	AIMinionClient(void* obj) : AIBaseClient(obj) {};
	~AIMinionClient() {};

	MAKE_GET(SpawnTime, float, SdkGetMinionSpawnTime);
	MAKE_GET(CampNumber, int, SdkGetMinionCampNumber);
	MAKE_RAW(IsLaneMinion, bool, SdkIsMinionLaneMinion);
	MAKE_GET(Type, int, SdkGetMinionType);
	MAKE_GET(Level, int, SdkGetMinionLevel);
	MAKE_RAW(IsWard, bool, SdkIsMinionWard);	

	bool IsJungleMob() {
		return GetType() == MINION_TYPE_JUNGLE_MONSTER;
	}
	bool IsSiegeMinion() {
		return strstr(GetCharName(), "Siege");
	}

	bool IsSuperMinion() {
		return strstr(GetCharName(), "Super");
	}
};

class AITurretClient : public AIBaseClient {
public:
	AITurretClient() { };
	AITurretClient(void* obj) : AIBaseClient(obj) { };
	~AITurretClient() {};

	int GetTurretPos() {		
		int tmp1{};
		CHECKFAIL(SdkGetTurretInfo(Object, &tmp1, NULL));
		return tmp1;
	}
	int GetLane() {		
		int tmp1{};
		CHECKFAIL(SdkGetTurretInfo(Object, NULL, &tmp1));
		return tmp1;
	}
};

class MissileClient : public GameObject {
public:
	MissileClient(void* obj) : GameObject(obj) {};
	MissileClient() {};
	~MissileClient() {};

	MAKE_RAW(IsAutoAttack, bool, SdkIsMissileAutoAttack);
	MAKE_GET(Spell, SDK_SPELL, SdkGetMissileSpell);
	MAKE_GET(StartPos, Vector3, SdkGetMissileStartPosition);
	MAKE_GET(MissileSpeed, float, SdkGetMissileSpeed);
	MAKE_RAW(HasReached, bool, SdkHasMissileCompleted);
	MAKE_GET(StartTime, float, SdkGetMissileStartTime);
	MAKE_GET(Width, float, SdkGetMissileWidth);

	AIBaseClient* GetOwner() {
		void* Target{ NULL };
		CHECKFAIL(SdkGetMissileCaster(Object, &Target, NULL));
		return (AIBaseClient*)(pSDK->EntityManager->GetObjectFromPTR(Target));
	}

	AttackableUnit* GetTarget() {
		void* Target{ NULL };
		CHECKFAIL(SdkGetMissileTarget(Object, NULL, &Target, NULL));
		return (AttackableUnit*)(pSDK->EntityManager->GetObjectFromPTR(Target));
	}

	MissileClient* GetParentMissile() {
		void* Target{ NULL };
		CHECKFAIL(SdkGetMissileParentMissile(Object, &Target, NULL));
		return (MissileClient*)(pSDK->EntityManager->GetObjectFromPTR(Target));
	}

	Vector3 GetEndPos() {
		SDKVECTOR endPos{};
		CHECKFAIL(SdkGetMissileTarget(Object, &endPos, NULL, NULL));
		return endPos;
	}

	bool IsOutsideOfTheMap() {
		auto pos{ GetPosition() };
		return pos.x < 0.0f || pos.x > 14700.0f || pos.z < 0.0f || pos.z > 14700.0f;
	}

	bool IsValid() {
		return Object != NULL && SDKSTATUS_SUCCESS(SdkIsObjectSpellMissile(Object));
	}
};

inline bool GameObject::IsRealTarget() {
	auto Attackable{ AsAttackableUnit() };
	if (!Attackable) {
		return false;
	}
	else {
		if (IsMinion()) {
			std::string m_charName(Attackable->GetCharName());
			if (InvalidTargets.count(m_charName) > 0) {
				return false;
			}
		}

		auto AI{ AsAIBaseClient() };
		if (AI) {
			bool UntargetableValidTarget{ AI->HasBuff("illaoiespirit") };
			if (!AI->IsTargetableToTeam() && !UntargetableValidTarget) {
				return false;
			}
		}		
	}
	return true;
}

inline bool GameObject::CanSpellHit() {
	auto Attackable{ AsAttackableUnit() };
	if (!Attackable || Attackable->GetHealth().Max <= 10.0f) { return false; }
		
	return IsRealTarget();
}

inline bool GameObject::IsRealWard() {	
	auto Minion{ AsAIMinionClient() };
	if (Minion != NULL && !Minion->IsMonster() && IsRealTarget() ) {
		if (Minion->IsWard() && (Minion->GetType() & MINION_TYPE_LANE_MINION)) {
			return true;
		}
	}
	return false;
}

inline bool GameObject::IsJunglePlant() {
	auto Minion{ AsAIMinionClient() };
	if (Minion != NULL) {
		const char* name{ Minion->GetCharName() };
		if (name && strstr(name, "SRU_Plant_")) {
			return true;
		}		
	}
	return false;
}

inline bool GameObject::IsBarrel() {
	auto Minion{ AsAIMinionClient() };
	if (Minion != NULL) {
		return Minion->HasBuff("gangplankebarrelactive");				
	}
	return false;
}

inline bool GameObject::IsHeroInGame(std::string Name, bool EnemiesOnly) {
	if (LoadedHeroes.size() < 6) {
		auto Heroes{ pSDK->EntityManager->GetEnemyHeroes() };
		auto Allies{ pSDK->EntityManager->GetAllyHeroes() };
		Heroes.insert(Allies.begin(), Allies.end());

		if (LoadedHeroes.size() != Heroes.size()) {
			for (auto &[_, Hero] : Heroes) {
				std::string name{ Hero->GetCharName() };
				if (Hero->IsEnemy())
					LoadedHeroes[name] = true;
				else
					LoadedHeroes[name] = false;
			}
		}		
	}
	return (LoadedHeroes.count(Name) > 0 && (!EnemiesOnly || LoadedHeroes[Name]));
}

__declspec(selectany) AIHeroClient Player;