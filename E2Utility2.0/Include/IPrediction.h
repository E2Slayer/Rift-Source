#pragma once

#include "GameObjects.hpp"
#include "Game.hpp"
#include "ICollision.h"

#pragma region Enums

enum class SkillType {
	Line,
	Circle,
	Cone,
	Arc,
	None
};
enum class SkillshotType {
	Line,
	Circle,
	Cone
};
enum class HitChance {
	Impossible = -1,
	Collision,
	OutOfRange,
	Low,
	Medium,
	High,
	VeryHigh,
	Dashing,
	Immobile
};
#pragma endregion



class IPrediction {
public:
	struct Input {		
		float Delay;
		float Width;
		float Range;
		float MissileSpeed;		
		bool  Collision;
		CollisionFlags Flags;
		Vector3 From;
		Vector3 RangeCheckFrom;
		SkillshotType SkillType;	

		Input(
			SkillshotType _Type,
			float _Range,
			float _Delay = 0.25f,
			float _MissileSpeed = HUGE_VALF,
			float _Width = 0.0f,
			bool  _Collision = false,
			CollisionFlags _Flags = CollisionFlags::Default,
			Vector3 _From = Vector3()
		) {
			Delay = _Delay;
			MissileSpeed = _MissileSpeed;
			Width = _Width;
			Range = _Range;
			Collision = _Collision;
			SkillType = _Type;
			Flags = _Flags;
			From = _From;
			RangeCheckFrom = _From;
		};
	};

	struct Output {
		AIBaseClient*	  Unit;
		Vector3			  CastPosition;
		Vector3			  UnitPosition;
		HitChance         Hitchance;
		std::shared_ptr<IPrediction::Input> Input;
		std::shared_ptr<ICollision::Output> CollisionResult;
	};

	virtual Vector2 GetFastPrediction(AIBaseClient * target, float delay, float missileSpeed = HUGE_VALF, Vector2* from = NULL, float distanceSet = 0) = 0;

	virtual std::shared_ptr<IPrediction::Output> GetPrediction(AIHeroClient* target, std::shared_ptr <IPrediction::Input> Data) = 0;
	virtual std::shared_ptr<IPrediction::Output> GetPrediction(
		AIHeroClient* target, 
		float width = 100.0f, 
		float delay = 0.25f, 
		float missileSpeed = HUGE_VALF,
		float range = HUGE_VALF, 
		SkillshotType type = SkillshotType::Line,
		bool collisionable = false, 		
		CollisionFlags Flags = CollisionFlags::Default,
		Vector3 From = Vector3(),
		Vector3 RangeCheckFrom = Vector3()
	) = 0;
};

