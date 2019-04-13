#pragma once

#include "IEntityManager.h"
#include "IGapcloser.h"
#include "IEventHandler.h"
#include "IControl.h"
#include "IDamageLib.h"
#include "IHealthPrediction.h"
#include "ICollision.h"
#include "IPrediction.h"
#include "IBuffManager.h"
#include "IInterruptible.h"

struct SDK {
	std::shared_ptr<IControl>			Control;
	std::shared_ptr<IEventHandler>		EventHandler;
	std::shared_ptr<IEntityManager>		EntityManager;
	std::shared_ptr<IDamageLib>			DamageLib;
	std::shared_ptr<IHealthPred>		HealthPred;
	std::shared_ptr<ICollision>			Collision;
	std::shared_ptr<IPrediction>		Prediction;
	std::shared_ptr<IBuffManager>		BuffManager;
	std::shared_ptr<IInterruptible>		InterruptibleManager;
	std::shared_ptr<IGapclose>			GapcloserManager;
};

#include "ITargetSelector.h"
#include "IOrbwalker.h"

struct Essentials {
	std::shared_ptr<ITargetSelector> TS;
	std::shared_ptr<IOrbwalker>		 Orbwalker;
};


__declspec(selectany) std::shared_ptr<SDK> pSDK;
__declspec(selectany) std::shared_ptr<Essentials> pCore;