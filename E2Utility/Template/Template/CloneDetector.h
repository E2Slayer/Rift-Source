#pragma once
#include "Detector.h"


struct CloneDraw
{
	Vector3 Position;
	GameObject* Object;
	float End;
	const char* TargetName;

	CloneDraw(Vector3 _Position, GameObject* _Object, float _End, const char* _TargetName) :
		Position(_Position), Object(_Object), End(_End), TargetName(_TargetName)
	{

	}
};


class CloneDetector
{
public:
	static void Init();
	static void MenuLoader();
	static void TickLoader();
	static void DrawLoader();

	static bool __cdecl OnCreate(void* Object, unsigned int NetworkID, void* UserData);
	static bool __cdecl OnDelete(void* Object, unsigned int NetworkID, void* UserData);
};

