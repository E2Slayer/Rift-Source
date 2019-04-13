#pragma once

#include "../../Include/SDK Extensions.h"

class MainCategory 
{
private:
	static std::string category_name;
	static bool enabled;

public:
	MainCategory();
	~MainCategory();

	static void OnInit();
	static void OnEnabled();
	static void OnDisabled();


	static void AdditionalMenu()
	{
		Menu::Tree((category_name+" Configs").c_str(), category_name+".Config", false, []()
		{
			Menu::Checkbox(("Enable "+ category_name).c_str(), category_name + ".Config.Enable", true);
		});
	}

	static void	__cdecl	Tick(_In_ void* UserData);
	static void	__cdecl	Update(_In_ void* UserData);
	static void	__cdecl	Draw(_In_ void* UserData);
	static void __cdecl DrawMenu(_In_ void* UserData);
};
