#include "stdafx.h"
#include "CustomRange.h"



void CustomRange::Init()
{
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
			temp += std::to_string(i);
			
			//StringCombine("Drawings.CustomRange.", ".Enable", (char*)i)
			Menu::Tree(StringCombine("Range ", "", temp).c_str(), StringCombine("Drawings.CustomRange.", "", temp), false, [i]()
			{
				//Menu::Checkbox(StringCombine("Enable Range ", "", (char*)i ), StringCombine("Drawings.CustomRange.", ".Enable", (char*)i), true);
				//Menu::SliderInt(StringCombine("Range ", "", (char*)i), StringCombine("Drawings.CustomRange.", ".Range", (char*)i), 1000, 100, 5000);
				//Menu::DropList(StringCombine("Range ", "Draw Style", (char*)i), StringCombine("Drawings.CustomRange.", ".Style", (char*)i), std::vector<std::string>{ "Circle", "Line To Mouse" }, 0);
				//Menu::DropList(StringCombine("Range ", "Bind Spell", (char*)i), StringCombine("Drawings.CustomRange.", ".BindSpell", (char*)i), std::vector<std::string>{ "None", "Q", "W", "E", "R" }, 0);
				//Menu::DropList(StringCombine("Range ", "Color", (char*)i), StringCombine("Drawings.CustomRange.", ".Color", (char*)i), ColorMenuList, 0);
				//Menu::SliderInt(StringCombine("Range ", "Line Width", (char*)i), StringCombine("Drawings.CustomRange.", ".LineWidth", (char*)i), 5, 1, 10);

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
