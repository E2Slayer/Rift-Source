#pragma once
#include "Parent.h"


template <typename T>
class Child : public T
{
public:
	std::string subCategory;
	std::string categoryMenuID;
	Child(std::string ParentName) : Parent(ParentName) {};
	~Child();

};

