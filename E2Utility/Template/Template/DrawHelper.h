#pragma once
#include "../../Include/SDK Extensions.h"




class DrawHelper
{
public:
	void static DrawOutlineText(PSDKVECTOR WorldPos, PSDKPOINT ScreenPos, const char* Text, const char* Face = "Calibri Bold",
		PSDKCOLOR Color = &Color::White, int Height = 20, int Width = 0, int Weight = 0, PSDKCOLOR OutlineColor = &Color::Black,  bool Italic = false)
	{
		float outlineWidth = 1.0f;
		Draw::Text(NULL, &Vector2(ScreenPos->x - outlineWidth, ScreenPos->y - outlineWidth), Text, Face, OutlineColor, Height, Width, Weight, Italic);
		Draw::Text(NULL, &Vector2(ScreenPos->x + outlineWidth, ScreenPos->y + outlineWidth), Text, Face, OutlineColor, Height, Width, Weight, Italic);
		Draw::Text(NULL, &Vector2(ScreenPos->x - outlineWidth, ScreenPos->y), Text, Face, OutlineColor, Height, Width, Weight, Italic);
		Draw::Text(NULL, &Vector2(ScreenPos->x + outlineWidth, ScreenPos->y), Text, Face, OutlineColor, Height, Width, Weight, Italic);

		Draw::Text(NULL, ScreenPos, Text, Face, Color, Height, Width, Weight, Italic);
	}

	void static DrawOutlineLineScreen(PSDKPOINT  StartPos, PSDKPOINT  EndPos, float Width = 3, PSDKCOLOR Color = &Color::White, PSDKCOLOR OutlineColor = &Color::Black)
	{
		float outlineWidth = 1.0f;
		/*
		Draw::LineScreen(&Vector2(StartPos->x - outlineWidth , StartPos->y - outlineWidth), &Vector2(EndPos->x - outlineWidth, EndPos->y - outlineWidth), Width, OutlineColor);
		Draw::LineScreen(&Vector2(StartPos->x + outlineWidth, StartPos->y + outlineWidth), &Vector2(EndPos->x + outlineWidth, EndPos->y + outlineWidth), Width, OutlineColor);
		Draw::LineScreen(&Vector2(StartPos->x - outlineWidth, StartPos->y), &Vector2(EndPos->x - outlineWidth, EndPos->y), Width, OutlineColor);
		Draw::LineScreen(&Vector2(StartPos->x + outlineWidth, StartPos->y), &Vector2(EndPos->x + outlineWidth, EndPos->y), Width, OutlineColor);
		*/
		Draw::LineScreen(&Vector2(StartPos->x - outlineWidth, StartPos->y - outlineWidth), &Vector2(EndPos->x + outlineWidth, EndPos->y - outlineWidth), Width + outlineWidth, OutlineColor);

		Draw::LineScreen(&Vector2(StartPos->x - outlineWidth, StartPos->y), &Vector2(EndPos->x + outlineWidth, EndPos->y), Width + outlineWidth, OutlineColor);
		Draw::LineScreen(StartPos, EndPos, Width, Color);
	}

};