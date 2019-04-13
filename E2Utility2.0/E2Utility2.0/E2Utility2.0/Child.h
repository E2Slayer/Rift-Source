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


	void OnUpdate(void* userData) override;
	void OnTick(void* userData) override;
	void OnDraw(void* userData) override;
};

