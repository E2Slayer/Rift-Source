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





const enum WardType {Green, Pink, Trap};

struct WardStruct
{
	float Duration;
	const char* ObjectBaseSkinName;
	float Range;
	const char* SpellName;
	WardType Type;


	/*
	            public readonly int Duration;
            public readonly string ObjectBaseSkinName;
            public readonly int Range;
            public readonly string SpellName;
            public readonly WardType Type;
	*/

	WardStruct(float _Duration, float _Range, const char* _ObjectBaseSkinName, const char* _SpellName, WardType _Type)
		: Duration(_Duration), Range(_Range), ObjectBaseSkinName(_ObjectBaseSkinName), SpellName(_SpellName), Type(_Type)
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
	bool IsFromMissile;
	float OverrideDuration;
	WardStruct Data;


	WardObject(WardStruct data, Vector3 position, float startT, AIBaseClient* wardObject,
		bool isFromMissile, Vector3 startPosition, AIBaseClient* spellCaster)
		: Data(data)
	{
		Vector3 pos = position;
		StartT = startT;

		
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
		
		IsFromMissile = isFromMissile;
		Data = data;

		Position = RealPosition(pos);

		

		EndPosition = (Position == position || Corrected) ? position : RealPosition(position);
		MinimapPosition = Renderer::WorldToMinimap(Position).To3D();

		StartPosition = (startPosition == Vector3(0.0f, 0.0f, 0.0f) || Corrected) ? startPosition : RealPosition(startPosition);
		Object = wardObject;
		OverrideDuration = 0.0f;
		


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
			for (int i = 0; i < 500; i = i + 5)
			{
				auto circleVector = Geometry::Circle(end, i, 20).Points;


				float Closest = 5000.0f;
				Vector2 ClosetPos;
				for (auto& circle : circleVector)
				{
					if (circle.Distance(end) <= Closest && !ClosetPos.IsWall())
					{
						Closest = circle.Distance(end);
						ClosetPos = circle;
					}

					//tempStruct.emplace_back(circle, circle.Distance(end));
				}

				return Vector3(ClosetPos.x, ClosetPos.GetTerrainHeight(), ClosetPos.y);

				/*
				std::sort(tempStruct.begin(), tempStruct.end(), cmd);


				for (auto& circle : tempStruct)
				{
					if (!circle.Position.IsWall())
					{
						return circle.Position;
					}
				}*/


			}
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

