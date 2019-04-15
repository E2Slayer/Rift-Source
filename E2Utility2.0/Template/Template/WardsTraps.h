#pragma once
#include "Trackers.h"
//#include "Drawings.h"


enum WardTypes { Green, Pink, Trap, Trinket, Blue };


struct WardStruct
{
	float Duration;
	float Range;
	float VisionRange;
	const char* ObjectBaseSkinName;
	const char* SpellName;
	WardTypes Type;

	WardStruct(float _Duration, float _Range, float _VisionRange, const char* _ObjectBaseSkinName,
	           const char* _SpellName, WardTypes _Type)
		: Duration(_Duration), Range(_Range), VisionRange(_VisionRange), ObjectBaseSkinName(_ObjectBaseSkinName),
		  SpellName(_SpellName), Type(_Type)
	{
	}
};

struct WardObject
{
	WardStruct Data;
	float StartTime;
	AIBaseClient* Object;

	Vector3 EndPosition;
	Vector3 MinimapPosition;
	Vector3 Position;
	float OverrideDuration;


	WardObject(WardStruct data, Vector3 position, float startTime, AIBaseClient* object, float _OverrideDuration)
		: Data(data), StartTime(startTime), Object(object), OverrideDuration(_OverrideDuration)
	{
		Position = RealPosition(position);
		MinimapPosition = Renderer::WorldToMinimap(Position).To3D();
		EndPosition = RealPosition(position);
	}

	inline float EndTime() const
	{
		if (OverrideDuration > 0.0f)
		{
			return this->StartTime + OverrideDuration;
		}
		return this->StartTime + this->Data.Duration;
	}

	static Vector3 RealPosition(Vector3 end)
	{
		return end.IsWall() ? end.GetClosestNonWallPosition(500.0f) : end;
	}
};


class WardsTraps : public Trackers
{
private:
	std::string subCategory;
	std::string categoryMenuID;

public:

	const static std::vector<WardStruct> _wardStructs;
	static std::vector<WardObject> _wardObjects;
	static std::vector<unsigned int> trinketAdjust;
	WardsTraps();
	~WardsTraps();

	//void OnUpdate(void* userData) override;
	void OnTick(void* userData) override;
	void OnDraw(void* userData) override;
	void OnDrawMenu(void* userData) override;
	void OnObjectCreated(void* Object, unsigned NetworkID, void* UserData) override;
	void OnObjectRemoved(void* Object, unsigned NetworkID, void* UserData) override;
	void OnProcessSpell(void* ai, PSDK_SPELL_CAST cast, void* userData) override;

	void Initialized() override;
	//void Menu() override;

	void SettingsUpdate() const;
};
