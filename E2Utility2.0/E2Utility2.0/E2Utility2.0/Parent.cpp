#include "stdafx.h"
#include "Parent.h"




Parent::~Parent()
{

}


void Parent::OnDrawMenu(void * userData)
{
	//SdkUiConsoleWrite("Are you hrer");
	Menu::Tree(category_name.c_str(), category_name+".Menu", false, [this]
	{
		Menu();

		Menu::Tree((category_name + " Configs").c_str(), category_name + ".Config", false, [this]()
		{
			//SdkUiCheckbox(("Enable " + category_name).c_str(), &Enabled, NULL);

			Menu::Checkbox(("Enable " + category_name).c_str(), category_name + ".Config.Enable", true);
		});
	});


}

/*
std::function<void()> Parent::ParentMenu()
{

}
*/