#pragma once
#include "Trackers.h"
#include "DropLists.h"
#include "../../Include/Geometry.hpp"
#include <vector>
#include <algorithm>




struct HeroWard
{
	AIHeroClient* Hero;
	float LastVisible;



	HeroWard(AIHeroClient* _Hero, float _LastVisible)
		: Hero(_Hero), LastVisible(_LastVisible)
	{

	}
};





const enum WardType {Green, Pink, Trap, Trinket, Blue};

struct WardStruct
{
	float Duration;
	float Range;
	float VisionRange;
	const char* ObjectBaseSkinName;
	const char* SpellName;
	//const char* DisplayName;
	WardType Type;


	WardStruct(float _Duration, float _Range, float _VisionRange, const char* _ObjectBaseSkinName, const char* _SpellName, WardType _Type)
		: Duration(_Duration), Range(_Range), VisionRange(_VisionRange), ObjectBaseSkinName(_ObjectBaseSkinName), SpellName(_SpellName), Type(_Type)
	{
		
	}
};


struct WardObject
{



	bool Corrected;
	Vector3 EndPosition;
	Vector3 MinimapPosition;
	AIBaseClient* Object;
	Vector3 StartPosition;
	float StartT;
	Vector3 Position;
	//bool IsFromMissile;
	float OverrideDuration;
	WardStruct Data;
	//WardType CurrentWardType;


	WardObject(WardStruct data, Vector3 position, float startT, AIBaseClient* wardObject, float _OverrideDuration) //bool isFromMissile, Vector3 startPosition, AIBaseClient* spellCaster
		: Data(data), OverrideDuration(_OverrideDuration)
	{
		Vector3 pos = position;
		StartT = startT;

		/*
		
		if (isFromMissile)
		{
			Vector3 newPos = GuessPosition(startPosition, position);

			if (position.x != newPos.x || position.y != newPos.y)
			{
				pos = newPos;
				Corrected = true;
			}

			if (!Corrected)
			{
				pos = startPosition;
			}

		}
		
		*/


		//IsFromMissile = isFromMissile;

		Data = data;

		Position = RealPosition(pos);

		//float(wardObject->GetResource().Max - wardObject->GetResource().Current)

		MinimapPosition = Renderer::WorldToMinimap(Position).To3D();

		/*
		StartPosition = (startPosition == Vector3(0.0f, 0.0f, 0.0f) || Corrected) ? startPosition : RealPosition(startPosition);
		EndPosition = (Position == position || Corrected) ? position : RealPosition(position);
		*/

		//StartPosition = RealPosition(startPosition);
		EndPosition = RealPosition(position);


		Object = wardObject;

		
		//OverrideDuration = 0.0f;
		


	}

	float EndTime()
	{
		if (OverrideDuration > 0.0f)
		{

			return this->StartT + OverrideDuration;
		}

		return this->StartT + this->Data.Duration;
	}

	int sign(int x)
	{
		return (x > 0) - (x < 0);
	}

	Vector3 GuessPosition(Vector3 start, Vector3 end)
	{
		std::vector<Vector3> grass;
		grass.clear();
		float distance = start.Distance(end);

		for (int i = 0; i < distance; i++)
		{
			auto pos = start.Extended(end, i);


			if (pos.IsGrass())
			{
				grass.emplace_back(pos);
			}
			/*
			if (NavMesh.IsWallOfGrass(pos, 1))
			{
				grass.Add(pos);
			}
			*/
		}

		//return grass.Count > 0 ? grass[(int) (grass.Count/2d + 0.5d*Math.Sign(grass.Count/2d))] : end;


		if (grass.size() > 0)
		{



			auto temp = (grass.size() / 2) + 0.5*sign(grass.size() / 2);
			return grass[temp];

		}

		return end;

	}


	Vector3 RealPosition(Vector3 end)
	{
		if (end.IsWall())
		{
			return end.GetClosestNonWallPosition(500.0f);

			/*
			for (int i = 0; i < 500; i = i + 5)
			{
				std::vector<Vector2> circleVector = Geometry::Circle(end, i, 20).Points;

				if (!circleVector.empty())
				{
					float Closest = 500.0f;
					Vector2 ClosetPos = Vector2(0.0f, 0.0f, 0.0f);
					for (auto& circle : circleVector)
					{
						if (circle.Distance(end) <= Closest)
						{
							Closest = circle.Distance(end);
							ClosetPos = circle;
						
						}
					}

					if (!ClosetPos.IsWall())
					{
						return Vector3(ClosetPos.x, ClosetPos.GetTerrainHeight(), ClosetPos.y);
					}
				}


			}

		*/
		}

		return end;

	}



};



class WardsTraps
{
public:
	static void Init();

	static void MenuLoader();
	static void TickLoader();
	static void DrawLoader();

	static bool __cdecl OnCreate(void* Object, unsigned int NetworkID, void* UserData);
	static bool __cdecl OnDelete(void* Object, unsigned int NetworkID, void* UserData);
	static void __cdecl SpellCastStart(void* AI, PSDK_SPELL_CAST SpellCast, void* UserData);

	static void CheckDuplicateWards(WardObject wObj);
};

