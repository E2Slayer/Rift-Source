#pragma once

#include "Vectors.hpp"
#include "sdkapi.h"

#include "Macros.hpp"
#include "Common.hpp"

#pragma warning(push, 0)
#pragma warning(disable: 4774)
#include <string>
#include <vector>
#pragma warning(pop)

typedef struct {
	int Width;
	int Height;
} Resolution;

#pragma region Colors
class Color {
public:
	static SDKCOLOR Black;
	static SDKCOLOR White;
	static SDKCOLOR Yellow;
	static SDKCOLOR Green;
	static SDKCOLOR Red;
	static SDKCOLOR Purple;
	static SDKCOLOR DarkBlue;
	static SDKCOLOR Magenta;
};

__declspec(selectany) SDKCOLOR Color::Black		= { 0, 0, 0, 255 };
__declspec(selectany) SDKCOLOR Color::White		= { 255, 255, 255, 255 };
__declspec(selectany) SDKCOLOR Color::Yellow	= { 0, 255, 255, 255 };
__declspec(selectany) SDKCOLOR Color::Green		= { 0, 255, 0, 255 };
__declspec(selectany) SDKCOLOR Color::Red		= { 0, 0, 255, 255 };
__declspec(selectany) SDKCOLOR Color::Purple	= { 128, 0, 128, 255 };
__declspec(selectany) SDKCOLOR Color::DarkBlue	= { 92, 0, 40, 255 };
__declspec(selectany) SDKCOLOR Color::Magenta	= { 63, 12, 144, 255 };
#pragma endregion

class Renderer {
public:
	static Vector3 ScreenToWorld(Vector2 ScreenPos) {
		Vector3 WorldPos{};
		CHECKRAWFAIL(SdkScreenToWorld(&ScreenPos, &WorldPos) );
		return WorldPos;
	}

	static Vector2 WorldToScreen(Vector3 WorldPos) {
		Vector2 ScreenPos{};
		bool Valid{ false };
		CHECKRAWFAIL(SdkWorldToScreenEx(&WorldPos, &ScreenPos, &Valid));
		return Valid ? ScreenPos : Vector2();
	}

	static Vector2 WorldToMinimap(Vector3 WorldPos) {
		Vector2 ScreenPos{};
		CHECKRAWFAIL(SdkWorldToMinimap(&WorldPos, &ScreenPos));
		return ScreenPos;
	}

	static Resolution GetResolution() {
		RECT lpRect{};
		HWND Handle;
		SdkUiGetWindow(&Handle);
		GetClientRect(Handle, &lpRect);
		return Resolution{ (lpRect.right - lpRect.left), (lpRect.bottom - lpRect.top) };
	}

	static Vector3 MousePos() {
		auto screenPos = CursorPos();
		return ScreenToWorld(screenPos);
	}

	static Vector2 CursorPos() {
		POINT Cursor;
		GetCursorPos(&Cursor);

		HWND Window = NULL;
		SdkUiGetWindow(&Window);
		ScreenToClient(Window, &Cursor);

		return Vector2((float)Cursor.x, (float)Cursor.y);;
	}
};

class Draw {
public:
	//official SDK
	static void Circle(PSDKVECTOR Pos, float Radius, PSDKCOLOR Color = &Color::White, int Texture = 0, PSDKVECTOR Direction = &Vector3::DirectionVector){
		SdkDrawCircle(Pos, Radius, Color, Texture, Direction);
	}

	static void Circle(PSDKPOINT  Pos, float Radius, PSDKCOLOR Color = &Color::White, int Texture = 0, PSDKVECTOR Direction = &Vector3::DirectionVector){
		auto position { Pos->To3D() };

		SdkDrawCircle(&position, Radius, Color, Texture, Direction);
	}

	static void Cone(PSDKVECTOR Pos, float Length, float Angle, PSDKVECTOR Direction, PSDKCOLOR Color = &Color::White, int Texture = 0) {
		SdkDrawCone(Pos, Length, Angle, Direction, Color, Texture);
	}
	static void Line(PSDKVECTOR StartPos, PSDKVECTOR EndPos, float Width = 3, PSDKCOLOR Color = &Color::White, int Texture = 0) {
		if (StartPos == NULL || !StartPos->IsValid() || EndPos == NULL || !EndPos->IsValid() ) { return; }
		//auto screenPos1{ Renderer::WorldToScreen(*StartPos) };
		//auto screenPos2{ Renderer::WorldToScreen(*EndPos) };
		//SdkUiConsoleWrite("Drawing Line From {%f , %f} to {%f, %f}", screenPos1.x, screenPos1.y, screenPos2.x, screenPos2.y);
		//if (StartPos->IsOnScreen() || EndPos->IsOnScreen()) {
			SdkDrawLine(StartPos, EndPos, Width, Color, Texture);
		//}		
	}
	static void Line(PSDKPOINT  StartPos, PSDKPOINT  EndPos, float Width = 3, PSDKCOLOR Color = &Color::White, int Texture = 0) {
		if (StartPos == NULL || !StartPos->IsValid() || EndPos == NULL || !EndPos->IsValid()) { return; }
		//auto screenPos1{ Renderer::WorldToScreen(*StartPos) };
		//auto screenPos2{ Renderer::WorldToScreen(*EndPos) };
		//SdkUiConsoleWrite("Drawing Line From {%f , %f} to {%f, %f}", screenPos1.x, screenPos1.y, screenPos2.x, screenPos2.y);

		auto start { StartPos->To3D() };
		auto end { EndPos->To3D() };

		SdkDrawLine(&start, &end, Width, Color, Texture);
	}
	static void Text(PSDKVECTOR WorldPos, PSDKPOINT ScreenPos, const char* Text, const char* Face = "Arial Narrow", 
		PSDKCOLOR Color = &Color::White, int Height = 20, int Width = 0, int Weight = 0, bool Italic = false) {
		SdkDrawText(WorldPos, ScreenPos, Text, Face, Color, Height, Width, Weight, Italic);
	}
	static void Text(PSDKVECTOR WorldPos, PSDKPOINT ScreenPos, std::string Text, const char* Face = "Arial Narrow", 
		PSDKCOLOR Color = &Color::White, int Height = 20, int Width = 0, int Weight = 0, bool Italic = false) {
		SdkDrawText(WorldPos, ScreenPos, Text.c_str(), Face, Color, Height, Width, Weight, Italic);
	}

	static void LineScreen(PSDKPOINT  StartPos, PSDKPOINT  EndPos, float Width = 3, PSDKCOLOR Color = &Color::White) {
		if (StartPos == NULL || !StartPos->IsValid() || EndPos == NULL || !EndPos->IsValid()) { return; }
		//SdkUiConsoleWrite("Drawing Line From {%f , %f} to {%f, %f}", StartPos->x, StartPos->y, EndPos->x, EndPos->y);
		SdkDrawLineScreen(StartPos, EndPos, Width, Color);
	}
	//Custom
	static void RectOutline(PSDKVECTOR StartPos, PSDKVECTOR EndPos, float Width, PSDKCOLOR Color = &Color::White) {
		if (StartPos == NULL || !StartPos->IsValid() || EndPos == NULL || !EndPos->IsValid()) { return; }
		auto vec3 = (*EndPos - *StartPos).To2D();
		auto vec4 = (*StartPos - *EndPos).To2D();
		auto A = (StartPos->To2D() - (vec3.Perpendicular().Normalized() * Width / 2)).To3D(StartPos->y);
		auto B = (StartPos->To2D() + (vec3.Perpendicular().Normalized() * Width / 2)).To3D(StartPos->y);
		auto C = (EndPos->To2D()   - (vec4.Perpendicular().Normalized() * Width / 2)).To3D(StartPos->y);
		auto D = (EndPos->To2D()   + (vec4.Perpendicular().Normalized() * Width / 2)).To3D(StartPos->y);

		Line(&A, &B, 2, Color, 0);
		Line(&B, &C, 2, Color, 0);
		Line(&C, &D, 2, Color, 0);
		Line(&D, &A, 2, Color, 0);
	}

	static void LinePerpendicular(PSDKVECTOR Pos, PSDKVECTOR PerpendicularTo, float Width, PSDKCOLOR Color = &Color::White) {
		auto vec3 = (*PerpendicularTo - *Pos).To2D();

		auto A = (Pos->To2D() - (vec3.Perpendicular().Normalized() * Width / 2)).To3D(PerpendicularTo->y);
		auto B = (Pos->To2D() + (vec3.Perpendicular().Normalized() * Width / 2)).To3D(PerpendicularTo->y);

		Line(&A, &B, 3, Color, 0);
	}

	static void LinePerpendicular(PSDKPOINT Pos, PSDKPOINT PerpendicularTo, float Width, PSDKCOLOR Color = &Color::White) {
		auto vec3 = (*PerpendicularTo - *Pos);

		auto A = (*Pos - (vec3.Perpendicular().Normalized() * Width / 2)).To3D();
		auto B = (*Pos + (vec3.Perpendicular().Normalized() * Width / 2)).To3D();

		Line(&A, &B, 3, Color, 0);
	}

	static void TriangleMark(PSDKVECTOR Pos, float Size = 150, PSDKCOLOR Color = &Color::Red) {

		auto Offset = *Pos; Offset.z += Size;
		float RotateAngle = (float)((Common::GetTickCount() % 3600)*M_PI / 1800); //*M_PI/3600
		float Mod = (float)(2.f / 3.f * M_PI);

		std::vector<SDKVECTOR> Points;
		Points.push_back(*Pos);
		Points.push_back(Offset.RotatedAroundPoint(*Pos, RotateAngle));
		Points.push_back(Offset.RotatedAroundPoint(*Pos, RotateAngle + Mod));
		Points.push_back(Offset.RotatedAroundPoint(*Pos, RotateAngle + 2.f * Mod));

		for (size_t i = 0; i < Points.size(); i++) {
			for (size_t j = i + 1; j < Points.size(); j++) {				
				Line(&(Points[i]), &(Points[j]), 1, Color, 0);
			}
		}
	}

};