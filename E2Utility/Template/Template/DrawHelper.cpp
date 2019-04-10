#include "stdafx.h"
#include "DrawHelper.h"


std::string DrawHelper::DrawHelperFontName;
const std::vector<std::string> DrawHelperFontNameList{ "Calibri", "Calibri Bold", "Arial", "Tahoma", "Verdana", "Times New Roman", "Lucida Console" };

bool DrawHelper::OutlineUse = true;

void DrawHelper::SetFont(int Selection, bool Outline)
{
	DrawHelper::DrawHelperFontName = DrawHelperFontNameList[Selection];
	OutlineUse = Outline;
}

void DrawHelper::DrawOutlineText(PSDKVECTOR WorldPos, PSDKPOINT ScreenPos, const char* Text, const char* Face,
	PSDKCOLOR Color, int Height, int Width, int Weight, PSDKCOLOR OutlineColor , bool Italic)
{
	//float outlineWidth = 1.0f;
	if (OutlineUse)
	{
		//Draw::Text(NULL, &Vector2(ScreenPos->x - 1.0, ScreenPos->y - 1.0), Text, DrawHelper::DrawHelperFontName.c_str(), OutlineColor, Height, Width, Weight, Italic);
		Draw::Text(NULL, &Vector2(ScreenPos->x + 1.0, ScreenPos->y + 1.0), Text, DrawHelper::DrawHelperFontName.c_str(), OutlineColor, Height, Width, Weight, Italic);
	}

	//Draw::Text(NULL, &Vector2(ScreenPos->x + outlineWidth, ScreenPos->y + outlineWidth), Text, Face, OutlineColor, Height, Width, Weight, Italic);
	//Draw::Text(NULL, &Vector2(ScreenPos->x - outlineWidth, ScreenPos->y), Text, Face, OutlineColor, Height, Width, Weight, Italic);
	//Draw::Text(NULL, &Vector2(ScreenPos->x + outlineWidth, ScreenPos->y), Text, Face, OutlineColor, Height, Width, Weight, Italic);

	Draw::Text(NULL, ScreenPos, Text, DrawHelper::DrawHelperFontName.c_str(), Color, Height, Width, Weight, Italic);
}

void DrawHelper::DrawOutlineLineScreen(PSDKPOINT  StartPos, PSDKPOINT  EndPos, float Width, PSDKCOLOR Color, PSDKCOLOR OutlineColor)
{
	float outlineWidth = 1.0f;
	/*
	Draw::LineScreen(&Vector2(StartPos->x - outlineWidth , StartPos->y - outlineWidth), &Vector2(EndPos->x - outlineWidth, EndPos->y - outlineWidth), Width, OutlineColor);
	Draw::LineScreen(&Vector2(StartPos->x + outlineWidth, StartPos->y + outlineWidth), &Vector2(EndPos->x + outlineWidth, EndPos->y + outlineWidth), Width, OutlineColor);
	Draw::LineScreen(&Vector2(StartPos->x - outlineWidth, StartPos->y), &Vector2(EndPos->x - outlineWidth, EndPos->y), Width, OutlineColor);
	Draw::LineScreen(&Vector2(StartPos->x + outlineWidth, StartPos->y), &Vector2(EndPos->x + outlineWidth, EndPos->y), Width, OutlineColor);
	*/
	//Draw::LineScreen(&Vector2(StartPos->x - outlineWidth, StartPos->y - outlineWidth), &Vector2(EndPos->x + outlineWidth, EndPos->y - outlineWidth), Width + outlineWidth, OutlineColor);

	//Draw::LineScreen(&Vector2(StartPos->x - outlineWidth, StartPos->y), &Vector2(EndPos->x + outlineWidth, EndPos->y), Width + outlineWidth, OutlineColor);
	Draw::LineScreen(StartPos, EndPos, Width, Color);
}



void DrawHelper::DrawCircleMinimap(Vector3 center, float radius, PSDKCOLOR color, float thickness, int quality)
{
	std::vector<Vector3> pointList;
	pointList.clear();


	for (int i = 0; i < quality; i++)
	{
		float angle = float(i * M_PI * 2.0f / quality);
		Vector3 tempVector = Vector3(center.x + radius * (float(std::cos(angle))), center.y, center.z + (radius * float(std::sin(angle))));
		pointList.emplace_back(tempVector);

	}

	for (int i = 0; i < pointList.size(); i++)
	{
		auto a = pointList[i];
		auto b = pointList[i == pointList.size() - 1 ? 0 : i + 1];

		Vector2 aonScreen = Renderer::WorldToMinimap(a);
		Vector2 bonScreen = Renderer::WorldToMinimap(b);
		Draw::LineScreen(&aonScreen, &bonScreen, thickness, color);
	}
}

void DrawHelper::DrawCircleMap(Vector3 center, float radius, PSDKCOLOR color, float thickness, int quality)
{
	std::vector<Vector3> pointList;
	pointList.clear();


	for (int i = 0; i < quality; i++)
	{
		float angle = float(i * M_PI * 2.0f / quality);
		Vector3 tempVector = Vector3(center.x + radius * (float(std::cos(angle))), center.y, center.z + (radius * float(std::sin(angle))));
		pointList.emplace_back(tempVector);

	}

	for (int i = 0; i < pointList.size(); i++)
	{
		auto a = pointList[i];
		auto b = pointList[i == pointList.size() - 1 ? 0 : i + 1];

		Draw::Line(&a, &b, thickness, color);
	}
}