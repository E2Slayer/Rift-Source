#include "stdafx.h"
#include "Loader.h"
#include "WardsTraps.h"
#include "Parent.h"

static std::shared_ptr<CallBacks> callBacks;

/*
Loader::Loader()
{
}


Loader::~Loader()
{
}
*/

void Loader::OnInit()
{
	GetCallBacks();
	//RegisterCallBacks();
}

std::shared_ptr<CallBacks> Loader::GetCallBacks()
{
	if(callBacks)
	{
		return callBacks;
	}

	//championPlugin = std::make_shared<AshePlugin>();
	callBacks = std::make_shared<WardsTraps>();
	

	if (callBacks) 
		callBacks->OnInit();


	return callBacks;
}


