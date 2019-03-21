#pragma once
#include "../../Include/SDK Extensions.h"


const std::vector<std::string> ColorMenuList{ "Black", "Blue", "Cyan", "DarkBlue", "DarkGreen", "Green", "Grey", "Magenta", "Orange", "Purple", "Red", "White", "Yellow" };

const std::vector<std::string> PingTypes { "Default", "Charge", "Danger", "Missing", "On the Way", "Back", "Come", "Warded" };

// &GetColor(Menu::Get<int>("Kalista.drawWColor"))
class DropLists
{


public:
	static std::vector<std::string> GetColorMenuList()
	{
		return ColorMenuList;
	}

	static std::vector<std::string> GetPingTypes()
	{
		return PingTypes;
	}

	static SDKCOLOR GetColor(int selectedCombo)
	{
		/*
		{Color::Black, "Black"}, { Color::Blue, "Blue" }, { Color::Cyan, "Cyan" }, { Color::DarkBlue, "DarkBlue" },
		{ Color::DarkGreen, "DarkGreen" }, { Color::Green, "Green" }, { Color::Grey, "Grey" }, { Color::Magenta, "Magenta" },
		{ Color::Orange, "Orange" }, { Color::Purple, "Purple" }, { Color::Red, "Red" }, { Color::White, "White" }, { Color::Yellow, "Yellow" }
		*/


		//int list = Menu::Get<int>(getHitChance);

		if (selectedCombo == NULL)
		{
			return Color::White; //just in case, the fucntion fails.
		}

		switch (selectedCombo)
		{
		case 0:
			return Color::Black;
		case 1:
			return Color::Blue;
		case 2:
			return Color::Cyan;
		case 3:
			return Color::DarkBlue;
		case 4:
			return Color::DarkGreen;
		case 5:
			return Color::Green;
		case 6:
			return Color::Grey;
		case 7:
			return Color::Magenta;
		case 8:
			return Color::Orange;
		case 9:
			return Color::Purple;
		case 10:
			return Color::Red;
		case 11:
			return Color::White;
		case 12:
			return Color::Yellow;
		default:
			return Color::White;
		}

		return Color::White;
	}


	static PingType GetPingType(int selectedCombo)
	{
		/*
		#define PING_TYPE_DEFAULT							0
		#define PING_TYPE_CHARGE							1
		#define PING_TYPE_DANGER							2
		#define PING_TYPE_MISSING							3
		#define PING_TYPE_ON_MY_WAY							4
		#define PING_TYPE_BACK								5
		#define PING_TYPE_COME								6
		#define PING_TYPE_WARDED							7
		#define PING_TYPE_SMALL_ICON						8
		#define PING_TYPE_BIG_ICON							9

		*/


		//int list = Menu::Get<int>(getHitChance);

		if (selectedCombo == NULL)
		{
			return PingType::Default; //just in case, the fucntion fails.
		}

		switch (selectedCombo)
		{
			case 0:
				return PingType::Default;
			case 1:
				return PingType::Charge;
			case 2:
				return PingType::Danger;
			case 3:
				return PingType::Missing;
			case 4:
				return PingType::OnMyWay;
			case 5:
				return PingType::Back;
			case 6:
				return PingType::Come;
			case 7:
				return PingType::Warded;

			default:
				return PingType::Default;
		}

		return PingType::Default;
	}
};