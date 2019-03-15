#pragma once

#include "Activator.h"


class Consumables
{
private:
	const enum ItemTypes { Health, Mana, HealthMana, Instant };
	static bool expandedArray[50];
public:
	static void Init();

	static void	__cdecl	Tick(_In_ void* UserData);
	//static void	__cdecl	Update(_In_ void* UserData);
	//static void	__cdecl	Draw(_In_ void* UserData);
	static void __cdecl DrawMenu(_In_ void* UserData);

	static void __cdecl InventoryUpdate(int Event, int Slot, int Extra, void* UserData);


	static void UseItems(std::map<int, std::string> inputMap, ItemTypes itemType, int targetID, SDK_ITEM targetItem);
	static void ItemMenuGenerator(std::map<int, std::string> inputMap, ItemTypes itemType, int arrayStart);

	static const char* GetItemBuffName(int itemID);
	//static void __cdecl HealCheck();
	//static void __cdecl BarrierCheck();

};

