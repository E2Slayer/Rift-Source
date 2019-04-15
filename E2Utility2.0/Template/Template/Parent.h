#pragma once
#include <utility>
#include "CallBacks.h"


class Parent : public CallBacks
{
protected:
	std::string category_name;
	bool Enabled = true;

public:
	 std::shared_ptr<Parent> menuList;
	Parent();
	Parent(std::string _category_name)
		: category_name(std::move(_category_name)) 
	{
		SdkUiConsoleWrite("Parent Created2");
		RegisterCallback();

	};
	virtual ~Parent();

	void RegisterCallback();

	// std::shared_ptr<Parent> GetMenu();
	//void OnInit();

	//void OnUpdate(void* userData) override;
	//void OnTick(void* userData) override;
	//void OnDraw(void* userData) override;
	// void OnDrawMenu(void* userData) override;

	//virtual std::function<void()> ParentMenu() = 0;
	//static void DrawMenu(_In_ void* UserData);
	//static void _cdecl Tick(_In_ void* UserData);
    //void  OnDrawMenu(_In_ void* UserData) override;
//	void  OnTick(_In_ void* UserData) override;
	void Initialized() override;
	//virtual void Menu() = 0;
};

