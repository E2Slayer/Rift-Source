#pragma once
#include "Activator.h"



class Offensives
{
private:
	const enum ItemTypes { Active, Timats, Target, HextTechs };
public:
	static void Init();

	static void	__cdecl	Tick(_In_ void* UserData);
	//static void	__cdecl	Update(_In_ void* UserData);
	//static void	__cdecl	Draw(_In_ void* UserData);
	static void __cdecl DrawMenu(_In_ void* UserData);
	static void __cdecl PostAttack(AttackableUnit* Target);


	static void UseItems(std::map<int, std::string> inputMap, ItemTypes itemType, int targetID, SDK_ITEM targetItem);
	static void UseTimats(int targetID, int spellSlot, std::string menuNameOrigninal, bool fromPostAttack);

	static void ItemMenuGenerator(std::map<int, std::string> inputMap, ItemTypes itemType);

};

