#include "stdafx.h"
#include "Cleansers.h"


std::map<unsigned int, AIHeroClient*> AllyChampionsCleanse;

std::vector<ItemStruct> CleanseItemList;

DWORD LastTimeTickCountClean = 0;

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

}





void Cleansers::MenuLoader()
{
	Menu::Tree("Cleansers", "Activator.Cleansers", false, []()
	{

		Menu::Tree("Cleanse", "Activator.Cleansers.Cleanse", false, []()
		{
			Menu::Checkbox("Use Cleanse", "Activator.Cleansers.CleanseUse", true);

			Menu::SliderInt("Cleanse Delay (ms)", "Activator.Cleansers.CleanseDelay", 150, 10, 1000);


			Menu::DropList("Cleanse Style", "Activator.Cleansers.CleanseStyle", std::vector<std::string>{ "Always", "Combo" }, 0);

			Menu::Tree("Cleanse Config", "Activator.Cleansers.CleanseConfig", false, []()
			{
				Menu::Checkbox("Use Cleanse On Slow", "Activator.Cleansers.CleanseUseSlow", true);
				Menu::Checkbox("Use Cleanse On Stun", "Activator.Cleansers.CleanseUseStun", true);
				Menu::Checkbox("Use Cleanse On Charm", "Activator.Cleansers.CleanseUseCharm", true);
				Menu::Checkbox("Use Cleanse On Asleep", "Activator.Cleansers.CleanseUseAsleep", true);
				Menu::Checkbox("Use Cleanse On Grounded", "Activator.Cleansers.CleanseUseGrounded", true);
				Menu::Checkbox("Use Cleanse On KnockUp", "Activator.Cleansers.CleanseUseKnockUp", true);
				Menu::Checkbox("Use Cleanse On KnockBack", "Activator.Cleansers.CleanseUseKnockBack", true);
				Menu::Checkbox("Use Cleanse On Taunt", "Activator.Cleansers.CleanseUseTaunt", true);
				Menu::Checkbox("Use Cleanse On Fear", "Activator.Cleansers.CleanseUseFear", true);
				Menu::Checkbox("Use Cleanse On Snare", "Activator.Cleansers.CleanseUseSnare", true);
				Menu::Checkbox("Use Cleanse On Silence", "Activator.Cleansers.CleanseUseSilence", true);
				Menu::Checkbox("Use Cleanse On Suppression", "Activator.Cleansers.CleanseUseSuppression", true);
				Menu::Checkbox("Use Cleanse On Polymorph", "Activator.Cleansers.CleanseUsePolymorph", true);
				Menu::Checkbox("Use Cleanse On Blind", "Activator.Cleansers.CleanseUseBlind", true);
				Menu::Checkbox("Use Cleanse On Poison", "Activator.Cleansers.CleanseUsePoison", true);

			});


		});

		Menu::Tree("Quick Silver Sash/Mercurial Scimitar", "Activator.Cleansers.QSS", false, []()
		{
			Menu::Checkbox("Use Quick Silver Sash/Mercurial Scimitar", "Activator.Cleansers.QSSUse", true);

			Menu::SliderInt("Quick Silver Sash/Mercurial Scimitar Delay (ms)", "Activator.Cleansers.QSSDelay", 150, 10, 1000);


			Menu::DropList("Quick Silver Sash/Mercurial Scimitar Style", "Activator.Cleansers.QSSStyle", std::vector<std::string>{ "Always", "Combo" }, 0);

			Menu::Tree("QSS Config", "Activator.Cleansers.QSSConfig", false, []()
			{
				Menu::Checkbox("Use QSS On Slow", "Activator.Cleansers.QSSUseSlow", true);
				Menu::Checkbox("Use QSS On Stun", "Activator.Cleansers.QSSUseStun", true);
				Menu::Checkbox("Use QSS On Charm", "Activator.Cleansers.QSSUseCharm", true);
				Menu::Checkbox("Use QSS On Asleep", "Activator.Cleansers.QSSUseAsleep", true);
				Menu::Checkbox("Use QSS On Grounded", "Activator.Cleansers.QSSUseGrounded", true);
				Menu::Checkbox("Use QSS On KnockUp", "Activator.Cleansers.QSSUseKnockUp", true);
				Menu::Checkbox("Use QSS On KnockBack", "Activator.Cleansers.QSSUseKnockBack", true);
				Menu::Checkbox("Use QSS On Taunt", "Activator.Cleansers.QSSUseTaunt", true);
				Menu::Checkbox("Use QSS On Fear", "Activator.Cleansers.QSSUseFear", true);
				Menu::Checkbox("Use QSS On Snare", "Activator.Cleansers.QSSUseSnare", true);
				Menu::Checkbox("Use QSS On Silence", "Activator.Cleansers.QSSUseSilence", true);
				Menu::Checkbox("Use QSS On Suppression", "Activator.Cleansers.QSSUseSuppression", true);
				Menu::Checkbox("Use QSS On Polymorph", "Activator.Cleansers.QSSUsePolymorph", true);
				Menu::Checkbox("Use QSS On Blind", "Activator.Cleansers.QSSUseBlind", true);
				Menu::Checkbox("Use QSS On Poison", "Activator.Cleansers.QSSUsePoison", true);

			});



		});


		Menu::Tree("Mikael's Crucible", "Activator.Cleansers.MikaelsCrucible", false, []()
		{

			Menu::Checkbox("Use Mikael's Crucible For Ally", "Activator.Cleansers.MikaelsCrucibleAllyUse", true);

			Menu::SliderInt("Mikael's Crucible Delay For Ally (ms)", "Activator.Cleansers.MikaelsCrucibleAllyDelay", 150, 10, 1000);


			Menu::DropList("Mikael's Crucible Ally Style", "Activator.Cleansers.MikaelsCrucibleAllyStyle", std::vector<std::string>{ "Always", "Combo" }, 0);


			Menu::Tree("Use Mikael's Crucible For", "Activator.Cleansers.MikaelsCrucibleUseTo", true, []()
			{
				if (!AllyChampionsCleanse.empty())
				{
					for (auto const &ent : AllyChampionsCleanse)
					{
						if (Player.GetNetworkID() != (&ent)->second->GetNetworkID())
						{
							std::string menuID = "Activator.Cleansers.MikaelsCrucibleUseFor";
							menuID += (&ent)->second->GetCharName();
							Menu::Checkbox((&ent)->second->GetCharName(), menuID, true);
						}

					}
				}
			});



			Menu::Tree("Mikael's Crucible Config For Ally", "Activator.Cleansers.MikaelsCrucibleConfigAlly", false, []()
			{
				Menu::Checkbox("Use Mikael's Crucible Ally On Slow", "Activator.Cleansers.MikaelsCrucibleAllyUseSlow", true);
				Menu::Checkbox("Use Mikael's Crucible Ally On Stun", "Activator.Cleansers.MikaelsCrucibleAllyUseStun", true);
				Menu::Checkbox("Use Mikael's Crucible Ally On Charm", "Activator.Cleansers.MikaelsCrucibleAllyUseCharm", true);
				Menu::Checkbox("Use Mikael's Crucible Ally On Taunt", "Activator.Cleansers.MikaelsCrucibleAllyUseTaunt", true);
				Menu::Checkbox("Use Mikael's Crucible Ally On Fear", "Activator.Cleansers.MikaelsCrucibleAllyUseFear", true);
				Menu::Checkbox("Use Mikael's Crucible Ally On Snare", "Activator.Cleansers.MikaelsCrucibleAllyUseSnare", true);
				Menu::Checkbox("Use Mikael's Crucible Ally On Silence", "Activator.Cleansers.MikaelsCrucibleAllyUseSilence", true);
			});



		});
	});
}

void Cleansers::TickLoader(ItemStruct currentItem)
{
	if (Player.IsAlive() && !Player.IsRecalling())
	{


			if (currentItem.GetItemID() == 0 || (LastTimeTickCountClean + Menu::Get<int>("Activator.Config.HumanizerDelay") >= GetTickCount()))
			{
				return;
			}
			//LastTimeTickCountClean




			if (Menu::Get<bool>("Activator.Cleansers.CleanseUse"))
			{
				Cleansers::CleanseCheck();
			}

			if (Menu::Get<bool>("Activator.Cleansers.QSSUse") || Menu::Get<bool>("Activator.Cleansers.MikaelsCrucibleAllyUse"))
			{
				for (auto const &value : CleanseItemList)
				{
					if (currentItem.GetItemID() == value.GetItemID())
					{


						if (currentItem.GetItemID() == (int)ItemID::QuicksilverSash || currentItem.GetItemID() == (int)ItemID::MercurialScimitar)
						{
							Cleansers::QSSCheck(currentItem.GetItemID(), currentItem.GetItemSlot());
						}
						else if (currentItem.GetItemID() == (int)ItemID::MikaelsCrucible)
						{
							Cleansers::MikaelCheck(currentItem.GetItemID(), currentItem.GetItemSlot(), value.GetSpellRange());
						}

					}
				}


			}
		
	}

}


void Cleansers::CleanseCheck()
{
	if (!Menu::Get<bool>("Activator.Cleansers.CleanseUse"))
	{
		return;
	}

	if (Menu::Get<int>("Activator.Cleansers.CleanseStyle") == 0 || (Menu::Get<int>("Activator.Cleansers.CleanseStyle") == 1 && Menu::Get<Hotkey>("Activator.Config.ComboKey").Active))
	{
		
	

		unsigned char cleanse = Player.GetSpellSlotFromName("SummonerBoost");

		if (cleanse == (unsigned char)SpellSlot::Unknown || cleanse == NULL)
		{
			return;
			//SdkUiConsoleWrite("You have Ignite");
		}

		if (cleanse != (unsigned char)SpellSlot::Summoner1 && cleanse != (unsigned char)SpellSlot::Summoner2)
		{
			return;
		}


		Spell::Active cleanseSpell = Spell::Active(cleanse, 750.0f, DamageType::True);


		if (!cleanseSpell.IsReady())
		{
			return;
		}




		if (!pSDK->EntityManager->GetLocalPlayer().GetPosition().IsValid())
		{
			return;
		}

		int delay = Menu::Get<int>("Activator.Cleansers.CleanseDelay");


		if (delay == 0)
		{
			return;
		}


		if (Menu::Get<bool>("Activator.Cleansers.CleanseUseSlow") && Player.HasBuffType((unsigned char)BUFF_TYPE_SLOW) ||
			Menu::Get<bool>("Activator.Cleansers.CleanseUseStun") && Player.HasBuffType((unsigned char)BUFF_TYPE_STUN) ||
			Menu::Get<bool>("Activator.Cleansers.CleanseUseCharm") && Player.HasBuffType((unsigned char)BUFF_TYPE_CHARM) ||
			Menu::Get<bool>("Activator.Cleansers.CleanseUseAsleep") && Player.HasBuffType((unsigned char)BUFF_TYPE_ASLEEP) ||
			Menu::Get<bool>("Activator.Cleansers.CleanseUseGrounded") && Player.HasBuffType((unsigned char)BUFF_TYPE_GROUNDED) ||
			Menu::Get<bool>("Activator.Cleansers.CleanseUseKnockUp") && Player.HasBuffType((unsigned char)BUFF_TYPE_KNOCKUP) ||
			Menu::Get<bool>("Activator.Cleansers.CleanseUseKnockBack") && Player.HasBuffType((unsigned char)BUFF_TYPE_KNOCKBACK) ||
			Menu::Get<bool>("Activator.Cleansers.CleanseUseTaunt") && Player.HasBuffType((unsigned char)BUFF_TYPE_TAUNT) ||
			Menu::Get<bool>("Activator.Cleansers.CleanseUseFear") && Player.HasBuffType((unsigned char)BUFF_TYPE_FEAR) ||
			Menu::Get<bool>("Activator.Cleansers.CleanseUseSnare") && Player.HasBuffType((unsigned char)BUFF_TYPE_SNARE) ||
			Menu::Get<bool>("Activator.Cleansers.CleanseUseSilence") && Player.HasBuffType((unsigned char)BUFF_TYPE_SILENCE) ||
			Menu::Get<bool>("Activator.Cleansers.CleanseUseSuppression") && Player.HasBuffType((unsigned char)BUFF_TYPE_SUPPRESSION) ||
			Menu::Get<bool>("Activator.Cleansers.CleanseUsePolymorph") && Player.HasBuffType((unsigned char)BUFF_TYPE_POLYMORPH) ||
			Menu::Get<bool>("Activator.Cleansers.CleanseUseBlind") && Player.HasBuffType((unsigned char)BUFF_TYPE_BLIND) ||
			Menu::Get<bool>("Activator.Cleansers.CleanseUsePoison") && Player.HasBuffType((unsigned char)BUFF_TYPE_POISON) 
			)
		{


			
			//SdkUiConsoleWrite("Cast Cleanse Actual : %f" , Game::Time());
			pSDK->EventHandler->DelayedAction([cleanse]()
			{ 
				Spell::Active cleanseSpell = Spell::Active(cleanse, 750.0f, DamageType::True);
				cleanseSpell.Cast();
				LastTimeTickCountClean = GetTickCount();
				//SdkUiConsoleWrite("Cast Cleanse Delayed : %f", Game::Time());
			}, delay);
		}


	}


}

void Cleansers::QSSCheck(int targetID, SpellSlot itemSlot)
{
	if (!Menu::Get<bool>("Activator.Cleansers.QSSUse"))
	{
		return;
	}

	if (Menu::Get<int>("Activator.Cleansers.QSSStyle") == 0 || (Menu::Get<int>("Activator.Cleansers.QSSStyle") == 1 && Menu::Get<Hotkey>("Activator.Config.ComboKey").Active))
	{

		if (targetID == 0 || itemSlot == SpellSlot::Unknown)
		{
			return;
		}

		if (!Player.HasItem(targetID) || Player.IsRecalling())
		{
			return;
		}


		Spell::Active item = Spell::Active(itemSlot); //temporary

		if (!item.IsValid() || !item.IsReady())
		{
			return;
		}


		if (!pSDK->EntityManager->GetLocalPlayer().GetPosition().IsValid())
		{
			return;
		}

		int delay = Menu::Get<int>("Activator.Cleansers.QSSDelay");


		if (delay == 0)
		{
			return;
		}



		if (Menu::Get<bool>("Activator.Cleansers.QSSUseSlow") && Player.HasBuffType((unsigned char)BUFF_TYPE_SLOW) ||
			Menu::Get<bool>("Activator.Cleansers.QSSUseStun") && Player.HasBuffType((unsigned char)BUFF_TYPE_STUN) ||
			Menu::Get<bool>("Activator.Cleansers.QSSUseCharm") && Player.HasBuffType((unsigned char)BUFF_TYPE_CHARM) ||
			Menu::Get<bool>("Activator.Cleansers.QSSUseAsleep") && Player.HasBuffType((unsigned char)BUFF_TYPE_ASLEEP) ||
			Menu::Get<bool>("Activator.Cleansers.QSSUseGrounded") && Player.HasBuffType((unsigned char)BUFF_TYPE_GROUNDED) ||
			Menu::Get<bool>("Activator.Cleansers.QSSUseKnockUp") && Player.HasBuffType((unsigned char)BUFF_TYPE_KNOCKUP) ||
			Menu::Get<bool>("Activator.Cleansers.QSSUseKnockBack") && Player.HasBuffType((unsigned char)BUFF_TYPE_KNOCKBACK) ||
			Menu::Get<bool>("Activator.Cleansers.QSSUseTaunt") && Player.HasBuffType((unsigned char)BUFF_TYPE_TAUNT) ||
			Menu::Get<bool>("Activator.Cleansers.QSSUseFear") && Player.HasBuffType((unsigned char)BUFF_TYPE_FEAR) ||
			Menu::Get<bool>("Activator.Cleansers.QSSUseSnare") && Player.HasBuffType((unsigned char)BUFF_TYPE_SNARE) ||
			Menu::Get<bool>("Activator.Cleansers.QSSUseSilence") && Player.HasBuffType((unsigned char)BUFF_TYPE_SILENCE) ||
			Menu::Get<bool>("Activator.Cleansers.QSSUseSuppression") && Player.HasBuffType((unsigned char)BUFF_TYPE_SUPPRESSION) ||
			Menu::Get<bool>("Activator.Cleansers.QSSUsePolymorph") && Player.HasBuffType((unsigned char)BUFF_TYPE_POLYMORPH) ||
			Menu::Get<bool>("Activator.Cleansers.QSSUseBlind") && Player.HasBuffType((unsigned char)BUFF_TYPE_BLIND) ||
			Menu::Get<bool>("Activator.Cleansers.QSSUsePoison") && Player.HasBuffType((unsigned char)BUFF_TYPE_POISON)
			)
		{


			//SdkUiConsoleWrite("Cast Cleanse Actual : %f" , Game::Time());
			pSDK->EventHandler->DelayedAction([itemSlot]()
			{
				Spell::Active cleanseSpell = Spell::Active(itemSlot);
				cleanseSpell.Cast();
				LastTimeTickCountClean = GetTickCount();
				//SdkUiConsoleWrite("Cast Cleanse Delayed : %f", Game::Time());
			}, delay);
		}
	}
}

void Cleansers::MikaelCheck(int targetID, SpellSlot itemSlot, float spellRange)
{
	if (Menu::Get<int>("Activator.Cleansers.MikaelsCrucibleAllyStyle") == 0 || (Menu::Get<int>("Activator.Cleansers.MikaelsCrucibleAllyStyle") == 1 && Menu::Get<Hotkey>("Activator.Config.ComboKey").Active))
	{
		if (!Menu::Get<bool>("Activator.Cleansers.MikaelsCrucibleAllyUse"))
		{
			return;
		}



		if (targetID == 0 || itemSlot == SpellSlot::Unknown)
		{
			return;
		}

		if (!Player.HasItem(targetID) || Player.IsRecalling())
		{
			return;
		}


		Spell::Targeted item = Spell::Targeted(itemSlot, spellRange, DamageType::Magical); //temporary

		if (!item.IsValid() || !item.IsReady())
		{
			return;
		}


		if (!pSDK->EntityManager->GetLocalPlayer().GetPosition().IsValid())
		{
			return;

		}

		int delayAlly = Menu::Get<int>("Activator.Cleansers.MikaelsCrucibleAllyDelay");


		if (delayAlly == 0)
		{
			return;
		}

		if (Menu::Get<bool>("Activator.Cleansers.MikaelsCrucibleAllyUse") && (Player.CountAlliesInRange(spellRange) - 1 >= 1))
		{

			auto heroes_ptr
			{
				pSDK->EntityManager->GetAllyHeroes(spellRange, &pSDK->EntityManager->GetLocalPlayer().GetPosition())
			};

			if (heroes_ptr.empty())
			{
				return;
			}

			for (auto &[netID, heroes] : heroes_ptr)
			{
				if (heroes != nullptr && heroes != NULL)
				{
					if (heroes->IsAlive() && !heroes->IsZombie())
					{
						std::string menuID = "Activator.Cleansers.MikaelsCrucibleUseFor";
						menuID += heroes->GetCharName();
						if (Menu::Get<bool>(menuID))
						{
							if (Menu::Get<bool>("Activator.Cleansers.MikaelsCrucibleAllyUseSlow") && heroes->HasBuffType((unsigned char)BUFF_TYPE_SLOW) ||
								Menu::Get<bool>("Activator.Cleansers.MikaelsCrucibleAllyUseStun") && heroes->HasBuffType((unsigned char)BUFF_TYPE_STUN) ||
								Menu::Get<bool>("Activator.Cleansers.MikaelsCrucibleAllyUseCharm") && heroes->HasBuffType((unsigned char)BUFF_TYPE_CHARM) ||
								Menu::Get<bool>("Activator.Cleansers.MikaelsCrucibleAllyUseTaunt") && heroes->HasBuffType((unsigned char)BUFF_TYPE_TAUNT) ||
								Menu::Get<bool>("Activator.Cleansers.MikaelsCrucibleAllyUseFear") && heroes->HasBuffType((unsigned char)BUFF_TYPE_FEAR) ||
								Menu::Get<bool>("Activator.Cleansers.MikaelsCrucibleAllyUseSnare") && heroes->HasBuffType((unsigned char)BUFF_TYPE_SNARE) ||
								Menu::Get<bool>("Activator.Cleansers.MikaelsCrucibleAllyUseSilence") && heroes->HasBuffType((unsigned char)BUFF_TYPE_SILENCE)
								)
							{
								//SdkUiConsoleWrite("Mika Ally Cast Cleanse Actual : %f", Game::Time());
								pSDK->EventHandler->DelayedAction([itemSlot, spellRange, heroes]()
								{
									Spell::Targeted cleanseSpell = Spell::Targeted(itemSlot, spellRange, DamageType::Magical);
									cleanseSpell.Cast(heroes);
									LastTimeTickCountClean = GetTickCount();
									//SdkUiConsoleWrite("Mika Ally Cast Cleanse Delayed : %f", Game::Time());
								}, delayAlly);
							}





						}
					}

				}
			}



		}
	}



}