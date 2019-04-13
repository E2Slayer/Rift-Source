#pragma once

#include "Vectors.hpp"
#include "sdkapi.h"

#include "KeyCodes.h"
#include "ItemID.h"
#include "Macros.hpp"
#include "Common.hpp"
#include "Renderer.hpp"
#include "Game.hpp"
#include "GameObjects.hpp"
#include "Menu.hpp"

#include "Interface Structs.h"
#include "Spell.hpp"





#define LIBRARY_SDK "Rift SDK Extensions"
#define LIBRARY_VERSION_SDK 4
#define LIBRARY_IMPORT_SDK "RETURN_SDK"

#define LIBRARY_ESSENTIALS "Rift Essentials"
#define LIBRARY_VERSION_ESSENTIALS 4
#define LIBRARY_IMPORT_ESSENTIALS "RETURN_ESSENTIALS"

__declspec(selectany) bool DevMode = false;