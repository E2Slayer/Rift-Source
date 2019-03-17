#pragma once

#include "../../Include/SDK Extensions.h"
#include "ItemStruct.h"

class ItemRetriever : public ItemStruct
{
private:
	//static PlayerItem currentPlayerItem[7]; //0~5 player 6 item slots, 6 is for tricket slot. 

	ItemRetriever();
	~ItemRetriever();
public:
	static void Init();
	static void	__cdecl	Tick(_In_ void* UserData);
	static ItemStruct* GetAllPlayerItems();


};

