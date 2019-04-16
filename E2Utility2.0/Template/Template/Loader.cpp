#include "stdafx.h"
#include "Loader.h"
#include "WardsTraps.h"
#include "Parent.h"
#include "CooldownTracker.h"
#include "SideBar.h"


/*
static std::vector<std::shared_ptr<CallBacks>> vectorCallbacks
{
	{std::make_shared<WardsTraps>()},
	{std::make_shared<CooldownTracker>()}

	};
	*/

static std::vector<std::shared_ptr<CallBacks>> ListCallBacks;

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

	//ListCallBacks.emplace_back(std::make_shared<WardsTraps>());

	ListCallBacks.emplace_back(std::make_shared<CooldownTracker>());

	ListCallBacks.emplace_back(std::make_shared<SideBar>());

	
	for (std::shared_ptr<CallBacks> & value : ListCallBacks)
	{
		value->OnInit(value);
	}

	
	//GetCallBacks();

}

std::vector<std::shared_ptr<CallBacks>> Loader::GetVector()
{

	return ListCallBacks;

	//GetCallBacks();

}

std::shared_ptr<CallBacks> Loader::GetCallBacks()
{
	
	if(callBacks)
	{
		return callBacks;
	}
	
	//championPlugin = std::make_shared<AshePlugin>();
	callBacks = std::make_shared<WardsTraps>();

	callBacks = std::make_shared<CooldownTracker>();

	//callBacks = std::make_shared<SideBar>();

	if (callBacks)
		//(callBacks)->OnInit();

	SdkUiConsoleWrite("Loader Int");
	
	//callBacks.reset(new WardsTraps);

	return callBacks;
}



//template <class T>
void Loader::DisableFunction()
{
	//callBacks.reset();
}




