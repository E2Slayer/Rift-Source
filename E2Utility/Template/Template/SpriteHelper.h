#pragma once
#include "../../Include/SDK Extensions.h"
#include "resource.h"


class SpriteHelper
{
public:
	void static SpriteDraw()
	{
		Vector2 screenPos{ Player.GetHealthBarScreenPos() };

		screenPos.x += 5.0f;
	    screenPos.y += -10.0f;
		//SdkUiConsoleWrite("ind");
		SdkDrawSpriteFromResource(MAKEINTRESOURCEA(HUDSELF), &screenPos, true);
	}

};