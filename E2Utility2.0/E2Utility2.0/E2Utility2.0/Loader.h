#pragma once
#include <memory>
#include "CallBacks.h"


// Used for casting shared_ptr's between sub & base class references. 
template <typename TOUT, typename TIN>
inline std::shared_ptr<TOUT> cast(TIN in)
{
	return std::static_pointer_cast<TOUT>(in);
}

class Loader
{
public:
	//Loader();
	//~Loader();

	static void OnInit();
	static std::vector<std::shared_ptr<CallBacks>> GetVector();
	static std::shared_ptr<CallBacks> GetCallBacks();
	template <class T>
	static std::shared_ptr<CallBacks> EnableFunction(T Function);
	//template <class T>
	static void DisableFunction();

};