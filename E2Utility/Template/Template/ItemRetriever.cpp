#include "stdafx.h"
#include "ItemRetriever.h"


//0~5 player 6 item slots, 6 is for tricket slot. 
//std::vector<ItemStruct> currentPlayerItems;
ItemStruct currentPlayerItems[7];

ItemRetriever::ItemRetriever() //constructor
{

}

ItemRetriever::~ItemRetriever() //distructor
{

}


void ItemRetriever::Init()
{

	pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick, ItemRetriever::Tick);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, Summoners::Update);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, Consumables::DrawMenu);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::InventoryUpdate, Consumables::InventoryUpdate);

}

void ItemRetriever::Tick(void * UserData)
{

		/*
		auto items = Player.GetItems();

		//Player.GetItem((SpellSlot::Item1);


		for (auto &[netID, itemsz] : items)
		{

			//itemsz.Slot
			SdkUiConsoleWrite("ID : %d Name : %s Slot : %d", netID, itemsz.DisplayName, itemsz.Slot );

		}
		*/
		if (!Player.IsValid())
		{
			return;
		}

		std::map<int, _SDK_ITEM> playerItems{ Player.GetItems() };

		if (playerItems.empty())
		{
			return;
		}

		for (auto const&[key, val] : playerItems)
		{

			//Activator.Consumables.Level1
			//if (!Menu::Get<bool>("Activator.Summoners.HealUse"))

			/*
			switch ((unsigned char)val.Slot)
			{
				case NULL: //strangly slot 0 returns as NUL
					currentPlayerItem[0] = PlayerItem(key, val, (unsigned char)val.Slot, "test");
					break;
				case 1:
					currentPlayerItem[0] = PlayerItem(key, val, (unsigned char)val.Slot, "test");
					break;
				case 2:
					currentPlayerItem[0] = PlayerItem(key, val, (unsigned char)val.Slot, "test");
					break;
				case 3:
					currentPlayerItem[0] = PlayerItem(key, val, (unsigned char)val.Slot, "test");
					break;
				case 4:
					currentPlayerItem[0] = PlayerItem(key, val, (unsigned char)val.Slot, "test");
					break;
				case 5:
					currentPlayerItem[0] = PlayerItem(key, val, (unsigned char)val.Slot, "test");
					break;
				case 6:
					break;
			}*/

			//currentPlayerItem[0] = PlayerItem(key, val, (unsigned char)val.Slot, "test");

				//currentPlayerItems.emplace_back(ItemStruct(key, val, (unsigned char)val.Slot));
				//currentPlayerItems.insert((unsigned char)val.Slot, ItemStruct(key, val, (unsigned char)val.Slot));
				
			if (val.Slot == NULL)
			{

				//currentPlayerItems.emplace_back(ItemStruct(key, val, (unsigned char)val.Slot));

				currentPlayerItems[0] = ItemStruct(key, val, (unsigned char)val.Slot);
			}
			else
			{
				//currentPlayerItems.emplace_back(ItemStruct(key, val, (unsigned char)val.Slot));

				currentPlayerItems[(unsigned char)val.Slot] = ItemStruct(key, val, (unsigned char)val.Slot);
			}
			
			
			/*
			if (Menu::Get<bool>("Activator.Consumables.Level1") && Player.GetLevel() == 1)
			{
				if (((key != (int)ItemID::RefillablePotion) && (key != (int)ItemID::HuntersPotion) && (key != (int)ItemID::CorruptingPotion)))
				{
					continue;
				}
			}*/

			/*
			UseItems(HealthItems, ItemTypes::Health, key, val);
			UseItems(HealthManaItems, ItemTypes::HealthMana, key, val);
			UseItems(ManaItems, ItemTypes::Mana, key, val);
			UseItems(InstantItems, ItemTypes::Instant, key, val);
			*/

		}



	
}

ItemStruct* ItemRetriever::GetAllPlayerItems()
{
	return currentPlayerItems;
}