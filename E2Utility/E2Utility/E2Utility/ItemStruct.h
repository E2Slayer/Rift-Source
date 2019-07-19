#pragma once
#include "../../Include/SDK Extensions.h"

const enum MenuTypes
{
	
	AfterAA = (1 << 0),
	MyHealth = (1 << 1),
	MyMana = (1 << 2),
	EnemyHealth = (1 << 3),
	EnemyNumber = (1 << 4),
	AllyHealth = (1 << 5),
	AllyNumber = (1 << 6),
	Instant = (1 << 7),
	EnemyMinimumRange = (1 << 8),
	None = (1 << 9)
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
	float spellRange;
	SpellSlot ItemSlotAdjust(unsigned char slot);
public:
	ItemStruct();
	~ItemStruct();
	ItemStruct(int _itemID, _SDK_ITEM _sdkItem, unsigned char _itemSlot);
	ItemStruct(int _itemID, _SDK_ITEM _sdkItem, unsigned char _itemSlot, const char* _displayName, const char* _menuID, const char* _menuSubCategory, MenuTypes _enumState, SpellTypes _spellType, float _spellRange);
	ItemStruct(int _itemID, const char* _displayName, const char* _menuID, const char* _menuSubCategory, MenuTypes _enumState, SpellTypes _spellType, float _spellRange);
	void MenuGenerator();
	//const char* MenuString(const char* targetName);
	//const char* DisplayString(const char* displayName, const char* targetName);

	void CastItem();
	const char* GetItemBuffName(int itemID);

	void SpellCaster(SpellTypes type, float range);
	void SpellAllyCaster(SpellTypes type, float range, AIHeroClient* allyTarget);
	unsigned int GetItemID() const;
	_SDK_ITEM GetSDKItem() const;
	SpellSlot GetItemSlot() const;
	const char* GetDisplayName() const;
	const char* GetMenuID() const;
	MenuTypes GetMenuTypes() const;
	SpellTypes GetSpellTypes() const;
	float GetSpellRange() const;

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

