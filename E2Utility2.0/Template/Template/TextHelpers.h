#pragma once
#include "../../Include/Renderer.hpp"

const enum TimerStyle { SSS, SS, MMSS };


class TextHelpers
{
public:
	TextHelpers();
	~TextHelpers();

	void static DrawOutlineText(PSDKVECTOR WorldPos, PSDKPOINT ScreenPos, const std::string&, const char* Face = "Calibri Bold",
		PSDKCOLOR Color = &Color::White, int Height = 20, int Width = 0, int Weight = 0, PSDKCOLOR OutlineColor = &Color::Black);

	std::string static TimeFormat(const float& Seconds, TimerStyle timerStlye) ;
};

