#pragma once

#pragma warning(push, 0)
#pragma warning(disable: 4365)
#include <vector>
#include <map>
#pragma warning(pop)

#include "Vectors.hpp"
#include "sdkapi.h"

#include "Macros.hpp"
#include "GameObjects.hpp"

#define TEAM_JUNGLE 300
#define TEAM_ALLY Player.GetTeamID()
#define TEAM_ENEMY TEAM_JUNGLE-TEAM_ALLY

enum class SpellSlot {
	Q,
	W,
	E,
	R,
	Summoner1,
	Summoner2,
	Item1,
	Item2,
	Item3,
	Item4,
	Item5,
	Item6,
	Trinket,
	Recall,
	BasicAttack = 65,
	Unknown
};

#define UGLYDIRTYMACRO(VAL) case(SpellSlot::##VAL): return #VAL;

#pragma warning(push)
#pragma warning(disable: 4062)
inline std::string SlotToString(SpellSlot val) {
	switch (val) {
		UGLYDIRTYMACRO(Q);
		UGLYDIRTYMACRO(W);
		UGLYDIRTYMACRO(E);
		UGLYDIRTYMACRO(R);
		UGLYDIRTYMACRO(Summoner1);
		UGLYDIRTYMACRO(Summoner2);
		UGLYDIRTYMACRO(Item1);
		UGLYDIRTYMACRO(Item2);
		UGLYDIRTYMACRO(Item3);
		UGLYDIRTYMACRO(Item4);
		UGLYDIRTYMACRO(Item5);
		UGLYDIRTYMACRO(Item6);
		UGLYDIRTYMACRO(Trinket);
		UGLYDIRTYMACRO(Recall);
	}
	return "";
}
#pragma warning(pop)

enum class SpellState {
	Ready = SPELL_CAN_CAST_OK,
	NotLearned = SPELL_CAN_CAST_FLAG_NOT_LEARNED,
	Disabled = SPELL_CAN_CAST_FLAG_DISABLED,
	Surpressed = SPELL_CAN_CAST_FLAG_CROWD_CONTROLLED,
	Cooldown = SPELL_CAN_CAST_FLAG_COOLDOWN0,
	NoMana = SPELL_CAN_CAST_FLAG_NO_MANA,
	Unknown = SPELL_CAN_CAST_FLAG_UNKNOWN0
};

enum class PingType {
	Default,
	Charge,
	Danger,
	Missing,
	OnMyWay,
	Back,
	Come,
	Warded,
	SmallIcon,
	BigIcon
};

class Game {
public:
	static float Time() {
		float time;
		CHECKRAWFAIL(SdkGetGameTime(&time));
		return time;
	}
	static unsigned int Ping() {
		unsigned int tmp1;
		CHECKRAWFAIL(SdkGetNetPing(&tmp1));
		return tmp1;
	}

	static void DisableInput() {
		SdkDisableInput();
	}

	static void EnableInput() {
		SdkEnableInput();
	}

	static bool IsOverlayOpen() {
		bool res;
		CHECKRAWFAIL(SdkUiIsOverlayVisible(&res));
		return res;
	}

	static bool IsChatOpen() {
		bool res;
		CHECKRAWFAIL(SdkIsChatActive(&res));
		return res;
	}

	static bool IsAvailable() {
		HWND handle; SdkUiGetWindow(&handle);
		return GetForegroundWindow() == handle && !IsChatOpen(); //&& !IsOverlayOpen()
	}

	static bool IsKeyPressed(int key) {
		return (GetAsyncKeyState(key) & 0x8000);
	}

	static bool SendPing(Vector3& pos, PingType type, bool playAudio) {
		return SDKSTATUS_SUCCESS(SdkPingMap(&pos, (unsigned char)type, playAudio));
	}

	static void PrintChat(std::string Text, unsigned int Flags = 0x40) {
		SdkDisplayChatLocalPlayer(Text.c_str(), Flags);
	}

	static void PrintChat(const char* Text, unsigned int Flags = 0x40) {
		SdkDisplayChatLocalPlayer(Text, Flags);
	}

	static void SendChat(std::string Text) {
		SdkSendChatLocalPlayer(Text.c_str());
	}

	static void SendChat(const char* Text) {
		SdkSendChatLocalPlayer(Text);
	}

	static void ShowEmote(unsigned char EMOTE_TYPE_ID) {
		SdkShowEmoteLocalPlayer(EMOTE_TYPE_ID);
	}

	static bool IsHeroInGame(std::string Name, bool EnemiesOnly = false) {
		return GameObject::IsHeroInGame(Name, EnemiesOnly);
	}
};