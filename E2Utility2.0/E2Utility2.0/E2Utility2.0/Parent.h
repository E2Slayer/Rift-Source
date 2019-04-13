#pragma once
#include <utility>
#include "CallBacks.h"


class Parent : public CallBacks
{
protected:
	std::string category_name;
	bool Enabled = true;
public:
	Parent() = default;;
	Parent(std::string _category_name)
		: category_name(std::move(_category_name)) {};
	virtual ~Parent();

	//void OnInit();

	//void OnUpdate(void* userData) override;
	//void OnTick(void* userData) override;
	//void OnDraw(void* userData) override;
	void OnDrawMenu(void* userData) override;

	//virtual std::function<void()> ParentMenu() = 0;

	virtual void Menu() = 0;
};

