#include "stdafx.h"
#include "CustomRange.h"
#include "DropLists.h"


const char* champName;

void CustomRange::Init()
{
	champName = Player.GetCharName();
}


std::string CustomRange::StringCombine(std::string fisrt, std::string last, std::string input)
{

	std::string tempString ;


	tempString += fisrt;
	tempString += input;

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
		
		for (int i = 1; i < 6; i++)
		{


			std::string temp;

			temp += champName;
			temp += std::to_string(i);



			//StringCombine("Drawings.CustomRange.", ".Enable", (char*)i)
			Menu::Tree(StringCombine("Range ", "", temp).c_str(), StringCombine("Drawings.CustomRange.", "", temp), false, [temp]()
			{
				Menu::Checkbox(StringCombine("Enable Range ", "", temp).c_str(), StringCombine("Drawings.CustomRange.", ".Enable", temp), true);
				Menu::SliderInt(StringCombine("Range ", "", temp).c_str(), StringCombine("Drawings.CustomRange.", ".Range", temp), 1000, 100, 5000);
				Menu::DropList(StringCombine("Range ", "Draw Style", temp).c_str(), StringCombine("Drawings.CustomRange.", ".Style", temp), std::vector<std::string>{ "Circle", "Line To Mouse" }, 0);
				Menu::DropList(StringCombine("Range ", "Bind Spell", temp).c_str(), StringCombine("Drawings.CustomRange.", ".BindSpell", temp), std::vector<std::string>{ "None", "Q", "W", "E", "R" }, 0);
				Menu::DropList(StringCombine("Range ", "Color", temp).c_str(), StringCombine("Drawings.CustomRange.", ".Color", temp), ColorMenuList, 0);
				Menu::SliderInt(StringCombine("Range ", "Line Width", temp).c_str(), StringCombine("Drawings.CustomRange.", ".LineWidth", temp), 5, 1, 10);

			});
		}
	});
}

void CustomRange::TickLoader()
{

}

void CustomRange::DrawLoader()
{

}
