#pragma once

#pragma warning(push, 0)
#include <functional>
#include <tuple>
#pragma warning(pop)

#include "Vectors.hpp"

class AttackableUnit;
class AIHeroClient;
class AIMinionClient;

enum class CallbackEnum {
	/// Official Callbacks
	CreateObject,
	DeleteObject,
	NewPath,
	Attack,
	Update,
	Overlay,
	SpellCastEnd,
	SpellCastStart,
	Recall,
	BuffCreateAndDelete,
	BuffUpdate,
	InventoryUpdate,

	/// Extensions
	Tick,
	PreAttack,
	PreMove,
	PreCast,
	PostAttack,
	GainVision,
	LoseVision,
	UnkillableMinion,
	Interruptible,
	Dash,
	GapCloser,	
};

typedef void(__cdecl* PREMOVECALLBACK)			(bool* Process, Vector3* Position);
typedef void(__cdecl* PREATTACKCALLBACK)		(bool* Process, AttackableUnit** Target);
typedef void(__cdecl* PRECASTCALLBACK)			(bool* Process, PSDK_SPELL Spell, Vector3* Position, AttackableUnit** Target);
typedef void(__cdecl* POSTATTACKCALLBACK)		(AttackableUnit* Target);

typedef void(__cdecl* VISIONCALLBACK)			(AIHeroClient* Target);
typedef void(__cdecl* UNKILLABLECALLBACK)		(AIMinionClient* Target);
typedef void(__cdecl* INTERRUPTIBLECALLBACK)	(AIHeroClient* Source, int Danger, float EndTime, bool CantMove, PSDK_SPELL_CAST Spell);
typedef void(__cdecl* DASHCALLBACK)				(AIHeroClient* Source, PSDKVECTOR StartPos, PSDKVECTOR EndPos, unsigned int StartTick, unsigned int Duration, float Speed);
typedef void(__cdecl* GAPCLOSERCALLBACK)		(AIHeroClient* Source, PSDKVECTOR StartPos, PSDKVECTOR EndPos, unsigned int StartTick, bool IsTargeted, SDK_SPELL_CAST Spell);


class IEventHandler {
public:
	virtual void RegisterCallback(CallbackEnum t, void* func, void* UserData = NULL) = 0;
	virtual void DelayedAction(std::function<void()> func, int delay) = 0;

	//Orb Related:
	virtual void FirePreMove  (bool* Process, Vector3* Position) = 0;
	virtual void FirePreAttack(bool* Process, AttackableUnit** Target) = 0;
	virtual void FirePreCast  (bool* Process, PSDK_SPELL Spell, Vector3* Position, AttackableUnit** Target) = 0;
	virtual void FirePostAttack(AttackableUnit* Target) = 0;
	virtual void FireUnkillable(AIMinionClient* Target) = 0;
};