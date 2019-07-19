#pragma once
#include "Parent.h"

class Drawings : public Parent
{

public:
	Drawings() : Parent("Drawings") {};
	~Drawings() {};
	//void Menu() override = 0;

	//void OnDrawMenu(void * userData) override;
};

