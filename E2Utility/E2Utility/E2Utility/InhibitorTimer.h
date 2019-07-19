#pragma once
#include "Trackers.h"


/*

		private class InhibitorObject
		{
			public InhibitorObject(Obj_BarracksDampener inhibitor)
			{
				Object = inhibitor;
				Destroyed = false;
				NextRespawnTime = -1;
				RespawnTime = 300;
				LastHealth = float.MinValue;
			}

			public Obj_BarracksDampener Object { get; private set; }
			public bool Destroyed { get; set; }
			public int RespawnTime { get; private set; }
			public int NextRespawnTime { get; set; }
			public float LastHealth { get; set; }
		}
*/

struct InhibitorObject
{
	AttackableUnit* Object;
	bool Destroyed;
	float NextRespawnTime;
	float RespawnTime;
	float LastHealth;
	Vector3 Position;

	InhibitorObject(AttackableUnit* _Object)
		: Object(_Object)
	{
		Destroyed = false;
		NextRespawnTime = -1.0f;

		RespawnTime = 300.0f;
		LastHealth = -1.0f;
		Position = _Object->GetPosition();
	}
};


class InhibitorTimer
{
public:
	static void Init();

	static void MenuLoader();
	static void TickLoader();
	static void DrawLoader();

};

