#pragma once
class Base
{


protected:
	virtual void OnEnabled();
	virtual void OnDisabled();
	virtual void OnInitialize();
public:
	Base();
	virtual ~Base();


	virtual const char* GetName() = 0;
};

