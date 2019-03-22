#pragma once

#pragma warning(push, 0)
#pragma warning(disable: 4242 4244)
#include <algorithm>
#pragma warning(pop)

#define DEBUG_PRINT false

#define CheckSDK(NAME) \
UNREFERENCED_PARAMETER(hinstDLL); \
\
if (fdwReason != DLL_PROCESS_ATTACH) \
return TRUE; \
\
SDK_EXTRACT_CONTEXT(lpvReserved); \
if (!SDK_CONTEXT_GLOBAL || !SDKSTATUS_SUCCESS(SdkNotifyLoadedModule(NAME, SDK_VERSION))) { \
	return FALSE; \
}

#define CheckLocalPlayer() \
SdkGetLocalPlayer(&g_LocalPlayer); \
if (!g_LocalPlayer) {\
	SdkUiConsoleWrite("[error]Couldn't Retrieve Local Player");\
	return;\
} 

#define CHECKFAIL(CODE) \
int status = CODE;\
if ((Object == NULL || !SDKSTATUS_SUCCESS(status)) && DEBUG_PRINT) { \
	SdkUiConsoleWrite("[error][SDK Extensions] Error! %s  errorCode: %d \n", __FUNCTION__, (int)CODE);\
} \


#define CHECKRAWFAIL(CODE)\
int status = CODE;\
if ((!SDKSTATUS_SUCCESS(status)) && DEBUG_PRINT) { \
	SdkUiConsoleWrite("[error][SDK Extensions] Error! %s\n", __FUNCTION__);\
}

#define MAKE_GET_DEF(NAME,TYPE,FUNCTIONDEF, DEFAULT)                    \
TYPE Get##NAME##(){                                        \
    TYPE _##NAME {DEFAULT};                                        \
    SDKSTATUS _STATE = FUNCTIONDEF(Object,&_##NAME);    \
    if ((!SDKSTATUS_SUCCESS(_STATE)) && DEBUG_PRINT) {                    \
        SdkUiConsoleWrite("[error][SDK Extensions] Error! MAKE_GET Could not retrieve %s for obj %p errorCode: %i.\n", __FUNCTION__, Object, (int)_STATE); \
    }                                                    \
    return _##NAME ? _##NAME : DEFAULT;                                     \
}

#define MAKE_GET(NAME,TYPE,FUNCTIONDEF)                    \
TYPE Get##NAME##(){                                        \
    TYPE _##NAME {};                                        \
    SDKSTATUS _STATE = FUNCTIONDEF(Object,&_##NAME);    \
    if ((!SDKSTATUS_SUCCESS(_STATE)) && DEBUG_PRINT) {                    \
        SdkUiConsoleWrite("[error][SDK Extensions] Error! MAKE_GET Could not retrieve %s for obj %p errorCode: %i.\n", __FUNCTION__, Object, (int)_STATE); \
    }                                                    \
    return _##NAME;                                     \
}

#define MAKE_RAW_DEF(NAME,TYPE,FUNCTIONDEF, DEFAULT)                    \
TYPE NAME##(){                                        \
    TYPE _##NAME {DEFAULT};                                        \
    SDKSTATUS _STATE = FUNCTIONDEF(Object,&_##NAME);    \
    if ((!SDKSTATUS_SUCCESS(_STATE)) && DEBUG_PRINT) {                    \
        SdkUiConsoleWrite("[error][SDK Extensions] MAKE_RAW Error! Could not retrieve %s for obj %p errorCode: %i.\n", __FUNCTION__, Object, (int)_STATE); \
    }                                                    \
    return _##NAME;                                     \
}

#define MAKE_RAW(NAME,TYPE,FUNCTIONDEF)                    \
TYPE NAME##(){                                        \
    TYPE _##NAME {};                                        \
    SDKSTATUS _STATE = FUNCTIONDEF(Object,&_##NAME);    \
    if ((!SDKSTATUS_SUCCESS(_STATE)) && DEBUG_PRINT) {                    \
        SdkUiConsoleWrite("[error][SDK Extensions] MAKE_RAW Error! Could not retrieve %s for obj %p errorCode: %i.\n", __FUNCTION__, Object, (int)_STATE); \
    }                                                    \
    return _##NAME;                                     \
}

#define BAD_BUFFER_CHECK(Buffer, Actual, Expected) \
if (Actual < Expected) { \
	SdkUiConsoleWrite("[error] Bad buffer size: need at least %zu bytes, but got only %zu bytes instead.\n", (size_t)Expected, (size_t)Actual); \
	return false; \
} \
if (IsBadWritePtr(Buffer, Actual)) { \
	SdkUiConsoleWrite("[error] Bad buffer: not writable (0x%p, %zu bytes).\n", Buffer, Actual); \
	return false; \
}

// 
#define PLUGIN_SETUP(NAME, LOAD_FUNC) \
\
void __cdecl LOAD_FUNC(void* UserData);\
\
BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {\
	_CrtSetDbgFlag(0); \
    CheckSDK(NAME);\
    \
	SdkRegisterOnLoad(LOAD_FUNC, NULL);\
	\
	return TRUE;\
}

#define MAKE_INTERFACE(TYPE, NAME) \
std::shared_ptr<TYPE>  NAME;

#define IMPORT_LIBRARY(NAME, IMPORT, VERSION, PTR) \
size_t SizeOf##IMPORT = sizeof(PTR); \
SDKSTATUS Status##IMPORT = SdkGetLibraryImport(NAME, IMPORT, VERSION, &PTR, &SizeOf##IMPORT); \
 \
if (Status##IMPORT > 0) { \
	SdkUiConsoleWrite("[error] Dependency %s From Lib %s Didnt Load Correctly. Error Code: %d", IMPORT, NAME, Status##IMPORT); \
	return; \
}

#define LOAD_ENVIRONMENT() \
CheckLocalPlayer(); \
IMPORT_LIBRARY(LIBRARY_SDK, LIBRARY_IMPORT_SDK, LIBRARY_VERSION_SDK, pSDK); \
IMPORT_LIBRARY(LIBRARY_ESSENTIALS, LIBRARY_IMPORT_ESSENTIALS, LIBRARY_VERSION_ESSENTIALS, pCore); \
Player = pSDK->EntityManager->GetLocalPlayer(); \
pSDK->EventHandler->RegisterCallback(CallbackEnum::Tick, Game::UpdateLocalPlayer);
