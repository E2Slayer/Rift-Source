#pragma once
#include "../../Include/SDK Extensions.h"


// Referenced Leaguesharp's Common

const enum TeleportTypes { Start, Abort, Finished, UnknownStatus };



struct TeleportStuct
{
	float Duration;


	DWORD Start;

	TeleportTypes Status;

	RecallType Type;

	unsigned int UnitNetworkId;
};


class TeleportDecode
{

public:
	static std::map<unsigned int, TeleportStuct> TeleportStore;

	static TeleportStuct TeleportDecoderFunction(void*& Unit, const char*& Name, const char*& Type)
	{

		//std::map<unsigned int, TeleportStuct> TeleportStore;

		const int errorGab = 90;
		TeleportStuct result{};

		result.Status = TeleportTypes::UnknownStatus;
		result.Type = RecallType::Unknown;

		auto sender = pSDK->EntityManager->GetObjectFromPTR(Unit);
		if (Unit == nullptr || sender == nullptr  || !sender->IsHero())
		{
			return result;
		}
		result.UnitNetworkId = sender->GetNetworkID();


		const auto it = TeleportStore.find(result.UnitNetworkId);
		if (it == TeleportStore.end())
		{
			TeleportStore.emplace(result.UnitNetworkId, result);


		}



		if (static_cast<unsigned>(strlen(Name)) != 0)
		{
			const RecallType recallType = Game::GetRecallType(Type);
			const auto duration = Game::GetRecallDuration(recallType);
			const auto time = GetTickCount();

			auto it2 = TeleportStore.find(result.UnitNetworkId);
			if (it2 != TeleportStore.end())
			{
				it2->second.Duration = duration;

				it2->second.Type = recallType;

				//SdkUiConsoleWrite("hi inisde2 %d %s", it2->second.Type, Type);
				it2->second.Start = time;
			}

			result.Status = TeleportTypes::Start;
			result.Duration = duration;
			result.Start = time;


		}
		else
		{
			const auto it3 = TeleportStore.find(result.UnitNetworkId);
			if (it3 != TeleportStore.end())
			{
				const DWORD dw = static_cast<DWORD>(it3->second.Duration) * 1000.0f;
				const auto shorter = GetTickCount() - it3->second.Start < dw - errorGab;
				//SdkUiConsoleWrite("Abort %ld %ld", GetTickCount() - it3->second.Start, dw - 90);


				result.Status = shorter ? TeleportTypes::Abort : TeleportTypes::Finished;

				result.Duration = 0;
				result.Start = 0;

			}

		}

		return result;
	};

};