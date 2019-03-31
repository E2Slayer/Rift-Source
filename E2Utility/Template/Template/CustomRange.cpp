#include "stdafx.h"
#include "CustomRange.h"
#include "DropLists.h"


const char* champName;


Spell::Active spellQCustom = Spell::Active(SpellSlot::Q, 1000.0f, DamageType::Physical);
Spell::Active spellWCustom = Spell::Active(SpellSlot::W, 1000.0f, DamageType::Physical);
Spell::Active spellECustom = Spell::Active(SpellSlot::E, 1000.0f, DamageType::Physical);
Spell::Active spellRCustom = Spell::Active(SpellSlot::R, 1200.0f, DamageType::Physical);

void CustomRange::Init()
{
	champName = Player.GetCharName();
}


std::string CustomRange::StringCombine(std::string fisrt, std::string last, std::string input)
{

	std::string tempString ;


	tempString += fisrt;
	tempString += input;

	tempString += last;
	//tempString += last;

	return tempString;
}

void CustomRange::MenuLoader()
{
	Menu::Tree("Custom Range", "Drawings.CustomRange", false, []()
	{
		/*
		Summoners::MenuLoader();
		Consumables::MenuLoader();
		Offensives::MenuLoader();
		Defensives::MenuLoader();
		Cleansers::MenuLoader();
		*/
		Menu::Checkbox("Enable Custom Range", "Drawings.CustomRange.Enable", true);
		for (int i = 1; i < 6; i++)
		{
			std::string ChampName = champName;

			std::string temp;

			temp += champName;
			temp += std::to_string(i);

			std::string display;

			display += champName;
			display += " Range ";
			display += std::to_string(i);

			Menu::Tree(StringCombine(display, "", "").c_str(), StringCombine("Drawings.CustomRange.", "", temp), false, [temp, display, i]()
			{
				Menu::Checkbox(StringCombine(display, " Enable", "").c_str(), StringCombine("Drawings.CustomRange.", ".Enable", temp), false);
				Menu::SliderInt(StringCombine(display, " Range", "").c_str(), StringCombine("Drawings.CustomRange.", ".Range", temp), 1000, 100, 5000);
				Menu::DropList(StringCombine(display, " Style", "").c_str(), StringCombine("Drawings.CustomRange.", ".Style", temp), std::vector<std::string>{ "Circle", "Line To Mouse" }, 0);
				Menu::DropList(StringCombine(display, " Bind Spell", "").c_str(), StringCombine("Drawings.CustomRange.", ".BindSpell", temp), std::vector<std::string>{ "None", "Q", "W", "E", "R" }, 0);
				Menu::DropList(StringCombine(display, " Color", "").c_str(), StringCombine("Drawings.CustomRange.", ".Color", temp), ColorMenuList, 1);
				Menu::SliderInt(StringCombine(display, " Line Width", "").c_str(), StringCombine("Drawings.CustomRange.", ".LineWidth", temp), 5, 1, 10);

			});
		}
	});
}

void CustomRange::TickLoader()
{

}

void CustomRange::DrawLoader()
{
	
	if (!Menu::Get<bool>("Drawings.CustomRange.Enable"))
	{
		return;
	}
	
	for (int i = 1; i < 6; i++)
	{
		std::string ChampName = champName;

		std::string temp;

		temp += champName;
		temp += std::to_string(i);

		std::string display;

		display += champName;
		display += " Range ";
		display += std::to_string(i);

		if (!Menu::Get<bool>(StringCombine("Drawings.CustomRange.", ".Enable", temp)))
		{
			continue;
		}

		int bindSpell = Menu::Get<int>(StringCombine("Drawings.CustomRange.", ".BindSpell", temp));

		if (bindSpell != 0 )
		{
			if (bindSpell == 1) //q
			{
				if (!spellQCustom.IsLearned() || !spellQCustom.IsReady())
				{
					return;
				}
			}
			else if (bindSpell == 2) //w
			{
				if (!spellWCustom.IsLearned() || !spellWCustom.IsReady())
				{
					return;
				}
			}
			else if (bindSpell == 3) //e
			{
				if (!spellECustom.IsLearned() || !spellECustom.IsReady())
				{
					return;
				}
			}
			else if (bindSpell == 4) //r
			{
				if (!spellRCustom.IsLearned() || !spellRCustom.IsReady())
				{
					return;
				}
			}
		}


		float range = (float) Menu::Get<int>(StringCombine("Drawings.CustomRange.", ".Range", temp));

		int style = Menu::Get<int>(StringCombine("Drawings.CustomRange.", ".Style", temp));

		if (style == 0)
		{
			auto playerPos = Player.GetPosition();
			if (playerPos.IsValid() && playerPos.IsOnScreen())
			{
				Draw::Circle(&playerPos, range, &DropLists::GetColor(Menu::Get<int>(StringCombine("Drawings.CustomRange.", ".Color", temp))));
			}

		}
		else if (style == 1)
		{
			auto playerPos = Player.GetPosition();
			if (playerPos.IsValid() && playerPos.IsOnScreen())
			{


				auto lineColor = DropLists::GetColor(Menu::Get<int>(StringCombine("Drawings.CustomRange.", ".Color", temp)));

				//Drawings.GankAlerter.LineWidth
				float width = (float)Menu::Get<int>(StringCombine("Drawings.CustomRange.", ".LineWidth", temp));

				auto mousepos{ Renderer::MousePos() };
				if (mousepos != nullptr && mousepos != NULL)
				{
					Draw::Line(&playerPos, &playerPos.Extended(mousepos, range), width, &lineColor);
				}


			}
		}




	}



}
