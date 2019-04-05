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
		//Draw::Text(NULL, &Vector2(ScreenPos->x + outlineWidth, ScreenPos->y + outlineWidth), Text, Face, OutlineColor, Height, Width, Weight, Italic);
		//Draw::Text(NULL, &Vector2(ScreenPos->x - outlineWidth, ScreenPos->y), Text, Face, OutlineColor, Height, Width, Weight, Italic);
		//Draw::Text(NULL, &Vector2(ScreenPos->x + outlineWidth, ScreenPos->y), Text, Face, OutlineColor, Height, Width, Weight, Italic);

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
		//Draw::LineScreen(&Vector2(StartPos->x - outlineWidth, StartPos->y - outlineWidth), &Vector2(EndPos->x + outlineWidth, EndPos->y - outlineWidth), Width + outlineWidth, OutlineColor);

		//Draw::LineScreen(&Vector2(StartPos->x - outlineWidth, StartPos->y), &Vector2(EndPos->x + outlineWidth, EndPos->y), Width + outlineWidth, OutlineColor);
		Draw::LineScreen(StartPos, EndPos, Width, Color);
	}



	void static DrawCircleMinimap(Vector3 center, float radius, PSDKCOLOR color, float thickness, int quality)
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

	void static DrawCircleMap(Vector3 center, float radius, PSDKCOLOR color, float thickness, int quality)
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

};