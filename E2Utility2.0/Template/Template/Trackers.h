#pragma once
#include "Parent.h"
#include "Child.h"

class Trackers : public Parent
{

public:
	Trackers() : Parent("Trackers")
	{
		//ParentMenu();
	};
	~Trackers() {};



	//void OnDrawMenu(void* userData) override;

	//void Menu() override = 0;
	

};

