#pragma once
#include "Activator.h"

extern DWORD LastTimeTickCountClean;
const std::vector<std::string> CleanseStyle{ "Always", "Combo" };

const enum CleanseDebuffTypes
{
	Slow = (1 << 0),
	Stun = (1 << 1),
	Charm = (1 << 2),
	Asleep = (1 << 3),
	Grounded = (1 << 4),
	KnockUp = (1 << 5),
	KnockBack = (1 << 6),
	Taunt = (1 << 7),
	Fear = (1 << 8),
	Flee = (1 << 9),
	Snare = (1 << 10),
	Silence = (1 << 11),
	Suppression = (1 << 12),
	Polymorph = (1 << 13),
	Blind = (1 << 14),
	Poison = (1 << 15)
};


const enum CleanseType
{
	Cleanse,
	QSS,
	Mikaels
};


struct CleanseDetail
{
	int DebuffType;
	const char* DebuffName;
	bool DefaultBool;
	bool Enable;
	float Duration;

	CleanseDetail(int _DebuffType, const char* _DebuffName, bool _DefaultBool)
		: DebuffType(_DebuffType), DebuffName(_DebuffName), DefaultBool(&DefaultBool)
	{
		Enable = false;
		Duration = 0.0f;
	}
};


struct MikaelAllyHero
{
	unsigned int NetID;
	AIHeroClient* AllyHero;
	bool Enabled;

	MikaelAllyHero(unsigned int _NetID, AIHeroClient* _AllyHero, bool _Enabled)
		: NetID(_NetID), AllyHero(_AllyHero), Enabled(_Enabled)
	{
		//Enabled = false;

	}
};

struct CleanseStruct
{


	CleanseType CleanseKey;
	std::string SubCategory;
	const char* MenuID;
	CleanseDebuffTypes EnumState;
	const char* DisplayName;
	//bool Default;
	std::vector<CleanseDetail> DebuffInfo;
	bool IsEnable;
	float Delay;
	int Style;
	Spell::Active cleanseSpell = Spell::Active(SpellSlot::Unknown, 750.0f, DamageType::True);

	Spell::Targeted MikaelSpell = Spell::Targeted(SpellSlot::Unknown, 650.0f, DamageType::True);
	std::vector<MikaelAllyHero> MikaelAllyList;
	//	std::map<int, const char*> CleanseDebuffList;

	CleanseStruct(CleanseType _CleanseKey, const char* _MainCategory, const char* _SubCategory, const char* _MenuID, CleanseDebuffTypes _EnumState)
		: CleanseKey(_CleanseKey), MenuID(_MenuID), EnumState(_EnumState)
	{


		//auto heroes_ptr = pSDK->EntityManager->GetAllyHeroes();


		if (_CleanseKey == CleanseType::Cleanse)
		{
			DisplayName = "Clenase";
		}
		else if (_CleanseKey == CleanseType::QSS)
		{
			DisplayName = "QSS";
		}
		else if (_CleanseKey == CleanseType::Mikaels)
		{
			DisplayName = "MikaelsCrucible";
			auto heroes_ptr = pSDK->EntityManager->GetAllyHeroes();
			
			if (!heroes_ptr.empty())
			{
				unsigned PlayerID = Player.GetNetworkID();
				for (auto &[netID, heroes] : heroes_ptr)
				{
					if (heroes != nullptr && heroes != NULL && PlayerID != heroes->GetNetworkID())
					{
						MikaelAllyList.emplace_back(netID, heroes, Menu::Get<bool>(SubCategory + std::string(this->DisplayName) + "Use" + heroes->GetCharName()));
					}
				}
			}
			
		}
		else
		{
			DisplayName = "None";
		}


		SubCategory = _MainCategory + std::string(".") + _SubCategory + std::string(".") + DisplayName + std::string(".");

		IsEnable = Menu::Get<bool>(SubCategory + std::string(this->DisplayName) + "Use");

		Delay = static_cast<float>(Menu::Get<int>(SubCategory + std::string(this->DisplayName) + "Delay"));

		Style = Menu::Get<int>(SubCategory + std::string(this->DisplayName) + "Style");



		if (_EnumState & CleanseDebuffTypes::Slow)
		{
			DebuffInfo.emplace_back(CleanseDetail(BUFF_TYPE_SLOW, "Slow", false));
		}

		if (_EnumState & CleanseDebuffTypes::Stun)
		{
			DebuffInfo.emplace_back(CleanseDetail(BUFF_TYPE_STUN, "Stun", true));
		}

		if (_EnumState & CleanseDebuffTypes::Charm)
		{
			DebuffInfo.emplace_back(CleanseDetail(BUFF_TYPE_CHARM, "Charm", true));
		}

		if (_EnumState & CleanseDebuffTypes::Asleep)
		{
			DebuffInfo.emplace_back(CleanseDetail(BUFF_TYPE_ASLEEP, "Asleep", true));
		}

		if (_EnumState & CleanseDebuffTypes::Grounded)
		{
			DebuffInfo.emplace_back(CleanseDetail(BUFF_TYPE_GROUNDED, "Grounded", false));
		}

		if (_EnumState & CleanseDebuffTypes::KnockUp)
		{
			DebuffInfo.emplace_back(CleanseDetail(BUFF_TYPE_KNOCKUP, "KnockUp", false));
		}
		if (_EnumState & CleanseDebuffTypes::KnockBack)
		{
			DebuffInfo.emplace_back(CleanseDetail(BUFF_TYPE_KNOCKBACK, "KnockBack", false));
		}

		if (_EnumState & CleanseDebuffTypes::Taunt)
		{
			DebuffInfo.emplace_back(CleanseDetail(BUFF_TYPE_TAUNT, "Taunt", true));
		}

		if (_EnumState & CleanseDebuffTypes::Fear)
		{
			DebuffInfo.emplace_back(CleanseDetail(BUFF_TYPE_FEAR, "Fear", true));
		}

		if (_EnumState & CleanseDebuffTypes::Flee)
		{
			DebuffInfo.emplace_back(CleanseDetail(BUFF_TYPE_FLEE, "Flee", true));
		}
		if (_EnumState & CleanseDebuffTypes::Snare)
		{
			DebuffInfo.emplace_back(CleanseDetail(BUFF_TYPE_SNARE, "Snare", true));
		}

		if (_EnumState & CleanseDebuffTypes::Silence)
		{
			DebuffInfo.emplace_back(CleanseDetail(BUFF_TYPE_SILENCE, "Silence", false));
		}

		if (_EnumState & CleanseDebuffTypes::Suppression)
		{
			DebuffInfo.emplace_back(CleanseDetail(BUFF_TYPE_SUPPRESSION, "Suppression", true));
		}

		if (_EnumState & CleanseDebuffTypes::Polymorph)
		{
			DebuffInfo.emplace_back(CleanseDetail(BUFF_TYPE_POLYMORPH, "Polymorph", true));
		}

		if (_EnumState & CleanseDebuffTypes::Blind)
		{

			DebuffInfo.emplace_back(CleanseDetail(BUFF_TYPE_BLIND, "Blind", false));
		}

		if (_EnumState & CleanseDebuffTypes::Poison)
		{

			DebuffInfo.emplace_back(CleanseDetail(BUFF_TYPE_POISON, "Poison", false));
		}
	}

	void MenuGenerator()
	{

		Menu::Checkbox(("Use " + std::string(this->DisplayName)).c_str(), SubCategory + std::string(this->DisplayName) + "Use", true);

		Menu::SliderInt((std::string(this->DisplayName) + " Delay (ms)").c_str(), SubCategory + std::string(this->DisplayName) + "Delay", 150, 10, 1000);


		Menu::DropList((std::string(this->DisplayName) + " Style").c_str(), SubCategory + std::string(this->DisplayName) + "Style", CleanseStyle, 0);


		if (this->CleanseKey == CleanseType::Mikaels)
		{

			Menu::Tree(("Use " + std::string(this->DisplayName) + " For").c_str(), SubCategory + std::string(this->DisplayName) + "For", true, [this]()
			{
				if (!MikaelAllyList.empty())
				{
					for (auto &value : MikaelAllyList)
					{
						if (value.AllyHero != nullptr && value.AllyHero != NULL)
						{
							Menu::Checkbox(value.AllyHero->GetCharName(), SubCategory + std::string(this->DisplayName) + "Use" + value.AllyHero->GetCharName(), true);
						}
					}
				}

			});
		}

		Menu::Tree((std::string(this->DisplayName) + " Configs").c_str(), SubCategory + std::string(this->DisplayName) + "Configs", false, [this]()
		{
			for (auto const& value : DebuffInfo)
			{
				Menu::Checkbox(("Use " + std::string(this->DisplayName) + " On " + std::string(value.DebuffName)).c_str(), SubCategory + "Use" + std::string(this->DisplayName) + std::string(value.DebuffName), false);
				Menu::SliderInt(("^-> " + std::string(this->DisplayName) + " Minimum " + std::string(value.DebuffName) + " Duration (ms)").c_str(), SubCategory + "Use" + std::string(this->DisplayName) + std::string(value.DebuffName) + "Durtaion", 1500, 500, 5000);

			}
		});
	}

	void RetriveInformation()
	{



		this->IsEnable = Menu::Get<bool>(SubCategory + std::string(this->DisplayName) + "Use");

		/*
		if (!this->IsEnable)
		{
			return;
		}
		*/
		this->Delay = static_cast<float>(Menu::Get<int>(SubCategory + std::string(this->DisplayName) + "Delay"));

		this->Style = Menu::Get<int>(SubCategory + std::string(this->DisplayName) + "Style");


		
		if (this->CleanseKey == CleanseType::Mikaels)
		{
			for (auto &value : MikaelAllyList)
			{
				if (value.AllyHero != nullptr && value.AllyHero != NULL)
				{
					value.Enabled = Menu::Get<bool>(SubCategory + std::string(this->DisplayName) + "Use" + value.AllyHero->GetCharName());

				}
			}
		}
		

		if (!DebuffInfo.empty())
		{
			for (auto& value : DebuffInfo)
			{
				value.Enable = Menu::Get<bool>(SubCategory + "Use" + std::string(this->DisplayName) + std::string(value.DebuffName));
				value.Duration = static_cast<float>((Menu::Get<int>(SubCategory + "Use" + std::string(this->DisplayName) + "Durtaion" + std::string(value.DebuffName))) / 1000);
			}
		}
	}


	void GetSpells()
	{
		if (!this->IsEnable)
		{
			return;
		}





		if (this->CleanseKey == CleanseType::Cleanse)
		{
			unsigned char cleanse = Player.GetSpellSlotFromName("SummonerBoost");
			if (cleanse == (unsigned char)SpellSlot::Unknown || cleanse == NULL)
			{
				return;
			}

			if (cleanse != (unsigned char)SpellSlot::Summoner1 && cleanse != (unsigned char)SpellSlot::Summoner2)
			{
				return;
			}
			cleanseSpell = Spell::Active(cleanse, 750.0f, DamageType::True);
		}
		else if (this->CleanseKey == CleanseType::QSS)
		{
			if (Player.HasItem(int(ItemID::QuicksilverSash)))
			{
				cleanseSpell = Spell::Active(Player.GetItemSlot(int(ItemID::QuicksilverSash)) + 6);
			}
			else if (Player.HasItem(int(ItemID::MercurialScimitar)))
			{
				cleanseSpell = Spell::Active(Player.GetItemSlot(int(ItemID::MercurialScimitar)) + 6);
			}
		}
		else if (this->CleanseKey == CleanseType::Mikaels)
		{

			if (Player.HasItem(int(ItemID::MikaelsCrucible)))
			{
				MikaelSpell = Spell::Targeted(Player.GetItemSlot(int(ItemID::MikaelsCrucible)) + 6, 650.0f, DamageType::True);
			}
			/*
			DisplayName = "MikaelsCrucible";
			auto heroes_ptr = pSDK->EntityManager->GetAllyHeroes();

			if (!heroes_ptr.empty())
			{
				unsigned PlayerID = Player.GetNetworkID();
				for (auto &[netID, heroes] : heroes_ptr)
				{
					if (heroes != nullptr && heroes != NULL && PlayerID != heroes->GetNetworkID())
					{
						MikaelAllyList.emplace_back(netID, heroes, Menu::Get<bool>(SubCategory + std::string(this->DisplayName) + "Use" + heroes->GetCharName()));
					}
				}
			}*/

		}
		else
		{
			//DisplayName = "None";
		}



		
	}


	bool UseCleanse()
	{
		for (auto const& value : DebuffInfo)
		{
			if (value.Enable && Player.HasBuffType((unsigned char)value.DebuffType) && (Player.GetBuffByType((unsigned char)value.DebuffType).EndTime - Player.GetBuffByType((unsigned char)value.DebuffType).StartTime) >= value.Duration)
			{
				return true;
			}
		}

		return false;
	}

	bool UseCleanseTarget(AIHeroClient* target)
	{
		if (target->IsValid() && target->IsAlive() && !target->IsZombie())
		{
			for (auto const& value : DebuffInfo)
			{
				if (value.Enable && target->HasBuffType((unsigned char)value.DebuffType) && (target->GetBuffByType((unsigned char)value.DebuffType).EndTime - target->GetBuffByType((unsigned char)value.DebuffType).StartTime) >= value.Duration)
				{
					return true;
				}
			}
		}

		return false;
	}

	void CleanseTick()
	{
		if (!this->IsEnable || cleanseSpell.Slot == SpellSlot::Unknown || !cleanseSpell.IsValid() || !pSDK->EntityManager->GetLocalPlayer().GetPosition().IsValid())
		{
			return;
		}

		if ((this->Style == 0 || (this->Style == 1 && Menu::Get<Hotkey>("Activator.Config.ComboKey").Active)) && cleanseSpell.IsReady())
		{


			if (this->UseCleanse())
			{

				pSDK->EventHandler->DelayedAction([this]()
				{

					this->cleanseSpell.Cast();
					//LastTimeTickCountClean = GetTickCount();

				}, this->Delay);
			}
		}
	}

	void QSSTick()
	{
		if (!this->IsEnable || !pSDK->EntityManager->GetLocalPlayer().GetPosition().IsValid() || cleanseSpell.Slot == SpellSlot::Unknown || !cleanseSpell.IsValid())
		{
			return;
		}

		if (!cleanseSpell.IsValid() || !cleanseSpell.IsReady())
		{
			return;
		}

		if ((this->Style == 0 || (this->Style == 1 && Menu::Get<Hotkey>("Activator.Config.ComboKey").Active)) && cleanseSpell.IsReady())
		{
			if (this->UseCleanse())
			{
				pSDK->EventHandler->DelayedAction([this]()
				{

					this->cleanseSpell.Cast();
					//LastTimeTickCountClean = GetTickCount();

				}, this->Delay);
			}
		}
	}

	void MikaelTick()
	{
		if (!this->IsEnable || !pSDK->EntityManager->GetLocalPlayer().GetPosition().IsValid() || MikaelSpell.Slot == SpellSlot::Unknown || !MikaelSpell.IsValid() || (Player.CountAlliesInRange(MikaelSpell.Range) - 1 < 1))
		{
			return;
		}

		if (!MikaelSpell.IsValid() || !MikaelSpell.IsReady())
		{
			return;
		}

		if ((this->Style == 0 || (this->Style == 1 && Menu::Get<Hotkey>("Activator.Config.ComboKey").Active)) && MikaelSpell.IsReady())
		{
			for (auto &value : MikaelAllyList)
			{
				if (this->UseCleanseTarget(value.AllyHero) && value.Enabled && MikaelSpell.IsInRange(value.AllyHero))
				{
					pSDK->EventHandler->DelayedAction([this, &value]()
					{

						this->MikaelSpell.Cast(value.AllyHero);
						//LastTimeTickCountClean = GetTickCount();

					}, this->Delay);
				}
			}
		}
	}

};





class Cleansers
{
private:
	static unsigned int TickCount;
public:
	static void Init();

	//static void	__cdecl	Tick(_In_ void* UserData);
	//static void	__cdecl	Update(_In_ void* UserData);
	//static void	__cdecl	Draw(_In_ void* UserData);
	//static void __cdecl DrawMenu(_In_ void* UserData);
	static void MenuLoader();
	static void TickLoader();
	static void UpdateLoader();

	
};



inline CleanseDebuffTypes operator |(CleanseDebuffTypes lhs, CleanseDebuffTypes rhs) {
	return static_cast<CleanseDebuffTypes> (
		static_cast<std::underlying_type<CleanseDebuffTypes>::type>(lhs) |
		static_cast<std::underlying_type<CleanseDebuffTypes>::type>(rhs)
		);
}

inline std::underlying_type<CleanseDebuffTypes>::type operator &(CleanseDebuffTypes lhs, CleanseDebuffTypes rhs) {
	return (
		static_cast<std::underlying_type<CleanseDebuffTypes>::type>(lhs) &
		static_cast<std::underlying_type<CleanseDebuffTypes>::type>(rhs)
		);
}
