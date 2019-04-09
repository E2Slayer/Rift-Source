#include "stdafx.h"
#include "Cleansers.h"


std::map<unsigned int, AIHeroClient*> AllyChampionsCleanse;

std::vector<ItemStruct> CleanseItemList;

DWORD LastTimeTickCountClean = 0;
unsigned int Cleansers::TickCount = 0;

CleanseStruct _Clenase = CleanseStruct(CleanseType::Cleanse, "Activator", "Cleansers", "Cleanse",
	CleanseDebuffTypes::Slow | CleanseDebuffTypes::Stun | CleanseDebuffTypes::Charm | CleanseDebuffTypes::Asleep | CleanseDebuffTypes::Taunt |
	CleanseDebuffTypes::Fear | CleanseDebuffTypes::Flee | CleanseDebuffTypes::Snare | CleanseDebuffTypes::Silence | CleanseDebuffTypes::Polymorph |
	CleanseDebuffTypes::Blind | CleanseDebuffTypes::Poison);

CleanseStruct _QSS = CleanseStruct(CleanseType::Cleanse, "Activator", "Cleansers", "QSS",
	CleanseDebuffTypes::Slow | CleanseDebuffTypes::Stun | CleanseDebuffTypes::Charm | CleanseDebuffTypes::Asleep | CleanseDebuffTypes::Taunt |
	CleanseDebuffTypes::Grounded | CleanseDebuffTypes::KnockUp | CleanseDebuffTypes::KnockUp |
	CleanseDebuffTypes::Fear | CleanseDebuffTypes::Flee | CleanseDebuffTypes::Snare | CleanseDebuffTypes::Silence | CleanseDebuffTypes::Suppression | CleanseDebuffTypes::Polymorph |
	CleanseDebuffTypes::Blind | CleanseDebuffTypes::Poison);

CleanseStruct _Mikael = CleanseStruct(CleanseType::Cleanse, "Activator", "Cleansers", "Mikael",
	CleanseDebuffTypes::Slow | CleanseDebuffTypes::Stun | CleanseDebuffTypes::Charm | CleanseDebuffTypes::Taunt |
	CleanseDebuffTypes::Fear | CleanseDebuffTypes::Flee | CleanseDebuffTypes::Snare | CleanseDebuffTypes::Silence);



void Cleansers::Init()
{


	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick, Cleansers::Tick);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Update, Summoners::Update);
	//pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, Cleansers::DrawMenu);


	CleanseItemList.emplace_back(ItemStruct((int)ItemID::QuicksilverSash, "Quick silver Sash", "QuicksilverSash", "Cleansers", MenuTypes::None, SpellTypes::Active, 700.0f));
	CleanseItemList.emplace_back(ItemStruct((int)ItemID::MercurialScimitar, "Mercurial Scimitar", "MercurialScimitar", "Cleansers", MenuTypes::None, SpellTypes::Active, 700.0f));
	CleanseItemList.emplace_back(ItemStruct((int)ItemID::MikaelsCrucible, "Mikaels Crucible", "MikaelsCrucible", "Cleansers", MenuTypes::None, SpellTypes::Active, 650.0f));
	//Mercurial
	AllyChampionsCleanse.clear();
	LastTimeTickCountClean = 0;

	AllyChampionsCleanse = pSDK->EntityManager->GetAllyHeroes();



	_Clenase = CleanseStruct(CleanseType::Cleanse, "Activator", "Cleansers", "Cleanse",
		CleanseDebuffTypes::Slow | CleanseDebuffTypes::Stun | CleanseDebuffTypes::Charm | CleanseDebuffTypes::Asleep | CleanseDebuffTypes::Taunt |
		CleanseDebuffTypes::Fear | CleanseDebuffTypes::Flee | CleanseDebuffTypes::Snare | CleanseDebuffTypes::Silence | CleanseDebuffTypes::Polymorph |
		CleanseDebuffTypes::Blind | CleanseDebuffTypes::Poison);

	_QSS = CleanseStruct(CleanseType::QSS, "Activator", "Cleansers", "QSS",
		CleanseDebuffTypes::Slow | CleanseDebuffTypes::Stun | CleanseDebuffTypes::Charm | CleanseDebuffTypes::Asleep | CleanseDebuffTypes::Taunt |
		CleanseDebuffTypes::Grounded | CleanseDebuffTypes::KnockUp | CleanseDebuffTypes::KnockUp |
		CleanseDebuffTypes::Fear | CleanseDebuffTypes::Flee | CleanseDebuffTypes::Snare | CleanseDebuffTypes::Silence | CleanseDebuffTypes::Suppression | CleanseDebuffTypes::Polymorph |
		CleanseDebuffTypes::Blind | CleanseDebuffTypes::Poison);

	_Mikael = CleanseStruct(CleanseType::Mikaels, "Activator", "Cleansers", "Mikael",
		CleanseDebuffTypes::Slow | CleanseDebuffTypes::Stun | CleanseDebuffTypes::Charm | CleanseDebuffTypes::Taunt |
		CleanseDebuffTypes::Fear | CleanseDebuffTypes::Flee | CleanseDebuffTypes::Snare | CleanseDebuffTypes::Silence);

	_Clenase.RetriveInformation();
	_QSS.RetriveInformation();
	_Mikael.RetriveInformation();

}





void Cleansers::MenuLoader()
{
	Menu::Tree("Cleansers", "Activator.Cleansers", false, []()
	{

		Menu::Tree("Cleanse", "Activator.Cleansers.Cleanse", false, []()
		{
			_Clenase.MenuGenerator();


		});

		Menu::Tree("Quick Silver Sash/Mercurial Scimitar", "Activator.Cleansers.QSS", false, []()
		{
			_QSS.MenuGenerator();


		});


		Menu::Tree("Mikael's Crucible", "Activator.Cleansers.MikaelsCrucible", false, []()
		{
			_Mikael.MenuGenerator();


			/*
			Menu::Tree("Use Mikael's Crucible For", "Activator.Cleansers.MikaelsCrucibleUseTo", true, []()
			{
				if (!AllyChampionsCleanse.empty())
				{
					for (auto const &[NetID, Hero] : AllyChampionsCleanse)
					{
						if (Player.GetNetworkID() != Hero->GetNetworkID())
						{
							Menu::Checkbox(Hero->GetCharName(), "Activator.Cleansers.MikaelsCrucibleUseFor" + std::string(Hero->GetCharName()), true);
						}

					}
				}
			});
			*/


		});
	});
}

void Cleansers::UpdateLoader()
{

	if (Player.IsAlive() && !Player.IsRecalling())
	{
		_Clenase.CleanseTick();
		_QSS.QSSTick();
		_Mikael.MikaelTick();
	}
}

void Cleansers::TickLoader()
{

	if (Game::IsOverlayOpen())
	{
		_Clenase.RetriveInformation();
		_QSS.RetriveInformation();
		_Mikael.RetriveInformation();
	}

	if (Cleansers::TickCount + 1000 > GetTickCount())
	{
		return;
	}
	Cleansers::TickCount = GetTickCount();

	if (Player.IsAlive() && !Player.IsRecalling())
	{
		_Clenase.GetSpells();
		_QSS.GetSpells();
		_Mikael.GetSpells();
	}

}


