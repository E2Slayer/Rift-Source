#pragma once
#include "../../Include/SDK Extensions.h"



//std::vector<std::string> DrawHelperFontNameList = { "Calibri", "Calibri Bold", "Arial", "Tahoma", "Verdana", "Times New Roman", "Lucida Console" };
//std::string DrawHelperFontName("Calibri Bold");


class DrawHelper
{
private:
	static std::string DrawHelperFontName;
	static bool OutlineUse;


public:
 

	void static SetFont(int Selection, bool Outline);

	void static DrawOutlineText(PSDKVECTOR WorldPos, PSDKPOINT ScreenPos, const char* Text, const char* Face = "Calibri Bold",
		PSDKCOLOR Color = &Color::White, int Height = 20, int Width = 0, int Weight = 0, PSDKCOLOR OutlineColor = &Color::Black, bool Italic = false);

	void static DrawOutlineLineScreen(PSDKPOINT  StartPos, PSDKPOINT  EndPos, float Width = 3, PSDKCOLOR Color = &Color::White, PSDKCOLOR OutlineColor = &Color::Black);


	void static DrawCircleMinimap(Vector3 center, float radius, PSDKCOLOR color, float thickness, int quality);

	void static DrawCircleMap(Vector3 center, float radius, PSDKCOLOR color, float thickness, int quality);

};