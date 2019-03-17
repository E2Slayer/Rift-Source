#pragma once
#include "Activator.h"

const enum MenuTypes
{
	Instant = (0 << 0),
	AfterAA = (1 << 0),
	MyHealth = (1 << 1),
	MyMana = (1 << 2),
	EnemyHealth = (1 << 3),
	EnemyNumber = (1 << 4),
	AllyHealth = (1 << 5),
	AllyNumber = (1 << 6),
	None = (1 << 7)
};


const enum SpellTypes
{
	Active,
	Ranged,
	SkillShot,
	SkillBase,
	Targeted
};

class ItemStruct
{

private:
	unsigned int itemID;
	_SDK_ITEM sdkItem;
	SpellSlot itemSlot;
	const char* displayName;
	const char* menuID;
	const char* menuSubCategory;
	MenuTypes enumState;
	SpellTypes spellType;
	SpellSlot ItemSlotAdjust(unsigned char slot);
public:
	ItemStruct();
	~ItemStruct();
	ItemStruct(int _itemID, _SDK_ITEM _sdkItem, unsigned char _itemSlot);
	ItemStruct(int _itemID, _SDK_ITEM _sdkItem, unsigned char _itemSlot, const char* _displayName, const char* _menuID, const char* _menuSubCategory, MenuTypes _enumState, SpellTypes _spellType);
	ItemStruct(int _itemID, const char* _displayName, const char* _menuID, const char* _menuSubCategory, MenuTypes _enumState, SpellTypes _spellType);
	void MenuGenerator();
	//const char* MenuString(const char* targetName);
	//const char* DisplayString(const char* displayName, const char* targetName);

	void CastItem();
	const char* GetItemBuffName(int itemID);
	unsigned int GetItemID() const;
	_SDK_ITEM GetSDKItem() const;
	SpellSlot GetItemSlot() const;
	const char* GetDisplayName() const;
	const char* GetMenuID() const;
	MenuTypes GetMenuTypes() const;
	SpellTypes GetSpellTypes() const;

};

/*
inline MenuTypes operator|(MenuTypes a, MenuTypes b)
{
	return static_cast<MenuTypes>(static_cast<int>(a) | static_cast<int>(b));
}
*/

inline MenuTypes operator |(MenuTypes lhs, MenuTypes rhs) {
	return static_cast<MenuTypes> (
		static_cast<std::underlying_type<MenuTypes>::type>(lhs) |
		static_cast<std::underlying_type<MenuTypes>::type>(rhs)
		);
}

inline std::underlying_type<MenuTypes>::type operator &(MenuTypes lhs, MenuTypes rhs) {
	return (
		static_cast<std::underlying_type<MenuTypes>::type>(lhs) &
		static_cast<std::underlying_type<MenuTypes>::type>(rhs)
		);
}

