#include "stdafx.h"
#include "TextHelpers.h"
#include <iomanip>


TextHelpers::TextHelpers()
{
}


TextHelpers::~TextHelpers()
{
}

void TextHelpers::DrawOutlineText(PSDKVECTOR WorldPos, PSDKPOINT ScreenPos, const std::string& Text, const char* Face,
	PSDKCOLOR Color, int Height, int Width, int Weight, PSDKCOLOR OutlineColor)
{
	Draw::Text(nullptr, &Vector2(ScreenPos->x + 1.0, ScreenPos->y + 1.0), Text, "Calibri Bold", OutlineColor, Height, Width, Weight);
	Draw::Text(nullptr, ScreenPos, Text, "Calibri Bold", Color, Height, Width, Weight);
}

std::string TextHelpers::TimeFormat(float Seconds, TimerStyle timerStlye)
{

	std::stringstream ss1;
	ss1.precision(1); //for decimal
	ss1.setf(std::ios_base::fixed, std::ios_base::floatfield);


	if (timerStlye == TimerStyle::MMSS)
	{
		ss1 << std::setfill('0') << std::setw(2) << static_cast<int>(Seconds / 60.0F) << ":" << std::setfill('0') << std::setw(2) << static_cast<int>(Seconds) % 60;
	}
	else if (timerStlye == TimerStyle::SSS)
	{
		ss1 << std::setfill('0') << std::setw(2) << Seconds;
	}
	else if (timerStlye == TimerStyle::SS)
	{
		ss1.precision(0);
		ss1 << std::setfill('0') << std::setw(2) << Seconds;
	}

	return ss1.str();
}


