#pragma once
#include "../../Include/SDK Extensions.h"
//#include "TurnAround.h"
#include "SharedExperience.h"
#include "Teleport.h"

class Detector
{
public:
	static void Init();

	virtual void InitLoader();
	virtual void MenuLoader();
	virtual void TickLoader();
	virtual void DrawLoader();
	static void __cdecl Update(_In_ void* UserData);
	static void	__cdecl	Tick(_In_ void* UserData);
	static void __cdecl DrawMenu(_In_ void* UserData);
	static void __cdecl Draw(_In_ void* UserData);

};

