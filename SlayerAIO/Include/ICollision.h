#pragma once

enum class CollisionFlags {
	Nothing   = (0 << 0),
	Minions   = (1 << 0),
	Heroes    = (1 << 1),
	Wall      = (1 << 2),
	BraumWall = (1 << 3),
	YasuoWall = (1 << 4),
	Default   = ((1 << 0) | (1 << 4))
};

class ICollision {
public:
	struct Output {
		std::vector<AIBaseClient*>	 Units;
		CollisionFlags				 Objects = CollisionFlags::Nothing;
	};	

	virtual bool CheckWallCollision(Vector2& from, Vector2& to, float width) = 0;
	virtual bool CheckWallCollision(Vector3& from, Vector3& to, float width) = 0;

	virtual bool CheckHeroCollision(Vector2& from, Vector2& to, float width, float delay, float missileSpeed = HUGE_VALF, bool AllyCollision = false, bool isArc = false) = 0;
	virtual bool CheckHeroCollision(Vector3& from, Vector3& to, float width, float delay, float missileSpeed = HUGE_VALF, bool AllyCollision = false, bool isArc = false) = 0;

	virtual bool CheckMinionCollision(Vector2& from, Vector2& to, float width, float delay, float missileSpeed = HUGE_VALF, bool AllyCollision = false, bool isArc = false) = 0;
	virtual bool CheckMinionCollision(Vector3& from, Vector3& to, float width, float delay, float missileSpeed = HUGE_VALF, bool AllyCollision = false, bool isArc = false) = 0;

	virtual bool CheckYasuoWallCollision(Vector2& from, Vector2& to, float width, bool isArc = false) = 0;
	virtual bool CheckYasuoWallCollision(Vector3& from, Vector3& to, float width, bool isArc = false) = 0;	
	
	virtual bool CheckCollisions(Vector2& from, Vector2& to, float width, float delay, float missileSpeed = HUGE_VALF, bool AllyCollision = false, CollisionFlags CollisionFlags = CollisionFlags::Default, bool isArc = false) = 0;
	virtual bool CheckCollisions(Vector3& from, Vector3& to, float width, float delay, float missileSpeed = HUGE_VALF, bool AllyCollision = false, CollisionFlags CollisionFlags = CollisionFlags::Default, bool isArc = false) = 0;
	
	virtual std::shared_ptr<ICollision::Output> GetCollisions(Vector2& from, Vector2& to, float width, float delay, float missileSpeed = HUGE_VALF, bool AllyCollision = false, CollisionFlags CollisionFlags = CollisionFlags::Default, bool isArc = false) = 0;
	virtual std::shared_ptr<ICollision::Output> GetCollisions(Vector3& from, Vector3& to, float width, float delay, float missileSpeed = HUGE_VALF, bool AllyCollision = false, CollisionFlags CollisionFlags = CollisionFlags::Default, bool isArc = false) = 0;
};

inline CollisionFlags operator |(CollisionFlags lhs, CollisionFlags rhs) {
	return static_cast<CollisionFlags> (
		static_cast<std::underlying_type<CollisionFlags>::type>(lhs) |
		static_cast<std::underlying_type<CollisionFlags>::type>(rhs)
	);
}

inline std::underlying_type<CollisionFlags>::type operator &(CollisionFlags lhs, CollisionFlags rhs) {
	return (
		static_cast<std::underlying_type<CollisionFlags>::type>(lhs) & 
		static_cast<std::underlying_type<CollisionFlags>::type>(rhs)
	);
}