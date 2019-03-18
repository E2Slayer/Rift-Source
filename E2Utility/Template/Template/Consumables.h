#pragma once

#include "Activator.h"
#include "ItemRetriever.h"
#include "ItemStruct.h"

extern DWORD LastTimeTickCountCon;

class Consumables
{
private:

public:
	static void Init();

	//static void	__cdecl	Tick(_In_ void* UserData);
	//static void	__cdecl	Update(_In_ void* UserData);
	//static void	__cdecl	Draw(_In_ void* UserData);
	//static void __cdecl DrawMenu(_In_ void* UserData);
	static void MenuLoader();
	static void TickLoader(ItemStruct currentItem);
	static void __cdecl InventoryUpdate(int Event, int Slot, int Extra, void* UserData);


	//static void UseItems(std::map<int, std::string> inputMap, ItemTypes itemType, int targetID, SDK_ITEM targetItem);
	//static void ItemMenuGenerator(std::map<int, std::string> inputMap, ItemTypes itemType, int arrayStart);

	//static const char* GetItemBuffName(int itemID);
	//static void __cdecl HealCheck();
	//static void __cdecl BarrierCheck();

};

