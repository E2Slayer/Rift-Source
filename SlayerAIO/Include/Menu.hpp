#pragma once

#pragma warning(push, 0)
#pragma warning(disable: 4365)
#include <cctype>
#include <map>
#pragma warning(pop)

#include "Vectors.hpp"
#include "sdkapi.h"

extern void* g_LocalPlayer;
extern PSDK_CONTEXT SDK_CONTEXT_GLOBAL;

struct HotKey_t {
	static unsigned int lastId;

	const char * Name;	
	int VirtualKey; 	

	bool Clicked;
	bool Changed;
	unsigned int Id;
	char buffer[16]; 
	int  buffer_size;	

	HotKey_t(const char* name, int virtualKey) {
		Id			= lastId++;
		Name		= name;
		VirtualKey	= virtualKey;		
		buffer_size = sizeof(buffer);

		Changed = false;
		Clicked = false;

		long ScanCode{ (long)MapVirtualKeyA((UINT)virtualKey, 0) };
		long lParam{ (ScanCode << 16) };
		memset(buffer, 0, (size_t)buffer_size);
		GetKeyNameTextA(lParam, buffer, buffer_size);				
	}
};

__declspec(selectany) std::map<unsigned int, bool> ExcludeOnWnd {
	{13u, true},		//RETURN
	//{77u, true},   //M
	//{83u, true},   //S
	{123u, true},	//F12
	{132u, true},	//F21
	{258u, true},	
	{259u, true}, 	
	{274u, true}, 	
	{512u, true}, 	
	{522u, true}, 	
	{641u, true}		
};
__declspec(selectany) std::map<unsigned int, bool> clickedOnWnd = { {256u, true}, {257u, false}, {260u, true}, {261u, false} };

class MakeMenu {
	static bool hkInit;
	static std::vector<HotKey_t*> requestKeyChange;	

public:
	static void Bool(const char* Name, bool* Value) {
		bool changed = false;
		SdkUiCheckbox(Name, Value, &changed);
		if (changed)
			SdkSetSettingBool(Name, *Value);
	}
	static void Slider(const char* Name, float* Value, float MinValue, float MaxValue, const char* Format = "%.0f") {
		bool changed = false;
		SdkUiDragFloat(Name, Value, MinValue, MaxValue, Format, &changed);
		if (changed)
			SdkSetSettingFloat(Name, *Value);
	}
	
	static void ColorPicker(const char * Name, Vector3 * Value) {
		bool changed = false;

		Value->x = min(Value->x / 255.0f, 1.f);
		Value->y = min(Value->y / 255.0f, 1.f);
		Value->z = min(Value->z / 255.0f, 1.f);
		SdkUiColorPicker(Name, Value, &changed);
		Value->x = min(std::ceil(Value->x * 255.0f), 255.f);
		Value->y = min(std::ceil(Value->y * 255.0f), 255.f);
		Value->z = min(std::ceil(Value->z * 255.0f), 255.f);

		if (changed) {
			std::string red(Name); red += "_R";
			std::string green(Name); green += "_G";
			std::string blue(Name); blue += "_B";

			SdkSetSettingFloat(red.c_str(), Value->x);
			SdkSetSettingFloat(green.c_str(), Value->y);
			SdkSetSettingFloat(blue.c_str(), Value->z);
		}
	}

	static void DropList(const char* Name, int* Value, const char* Buffer[], int Size) {
		bool changed = false;
		SdkUiCombo(Name, Value, Buffer, Size, &changed);
		if (changed)
			SdkSetSettingNumber(Name, *Value);
	}

	static void Hotkey(HotKey_t& KeyStruct) {		
		if (!hkInit) {
			SdkRegisterOnKeyPress(MakeMenu::WndProc, NULL);
			hkInit = true;
		}

		SdkUiText("%s: ", KeyStruct.Name);
		SdkUiForceOnSameLine();

		if (KeyStruct.Clicked) {
			SdkUiSmallButton("Please Press Any Key", NULL);	
			for (auto Key : requestKeyChange) {	if (Key->Id == KeyStruct.Id) {return;}	}
			requestKeyChange.push_back(&KeyStruct);
		}				
		else {
			std::string str(KeyStruct.buffer);
			str += "###"; str+= KeyStruct.Name;
			SdkUiSmallButton(str.c_str(), &KeyStruct.Clicked);				
		}		
		
		if (KeyStruct.Changed) {
			std::string name(KeyStruct.Name);
			name.erase(std::remove_if(name.begin(), name.end(), std::isspace), name.end());
			SdkSetSettingString(name.c_str(), KeyStruct.buffer);

			name += "_VirtualKey";
			int tmpKey{ KeyStruct.VirtualKey > 0 ? (int)KeyStruct.VirtualKey : 0 }; 
			SdkSetSettingNumber(name.c_str(), tmpKey);			
					
			KeyStruct.Changed = false;
		}
	}

	static void WndProc(bool KeyDown, unsigned int VirtualKey, unsigned short Repeated, unsigned char ScanCode, bool IsExtended, bool IsAltDown, bool PreviousState, bool TransitionState, void* UserData) {
		UNREFERENCED_PARAMETER(UserData);
		UNREFERENCED_PARAMETER(KeyDown);

		if (ExcludeOnWnd.find(VirtualKey) == ExcludeOnWnd.end() && !requestKeyChange.empty()) {
			if (VirtualKey != VK_ESCAPE) {
				requestKeyChange.back()->VirtualKey = (int)VirtualKey;

				long lParam{ Repeated | (ScanCode << 16) | (IsExtended << 24) | (IsAltDown << 29) | (PreviousState << 30) | (TransitionState << 31) };
				GetKeyNameTextA(lParam, requestKeyChange.back()->buffer, requestKeyChange.back()->buffer_size);

				requestKeyChange.back()->Changed = true;
				//SdkUiConsoleWrite("Changed Value of %s to %s", requestKeyChange.back()->Name, requestKeyChange.back()->buffer);

				requestKeyChange.back()->Clicked = false;
				requestKeyChange.pop_back();
			}
			else {
				//SdkUiConsoleWrite("Change of Keys Cancelled");
				for (auto Key : requestKeyChange) { 
					Key->Clicked = false; 
					Key->Changed = false;
				}
				requestKeyChange.clear();
			}									
		}
	}
};

class GetMenu {
public:
	static bool Initialized;	
	
	static void Bool(const char* Name, bool* Value, bool Default = false) {
		SdkGetSettingBool(Name, Value, Default);
	}

	static void Float(const char* Name, float* Value, float Default = 0.0f) {
		SdkGetSettingFloat(Name, Value, Default);
	}
	static void ColorPicker(const char * Name, Vector3 * Value) {
		std::string red(Name);		red += "_R";
		std::string green(Name);	green += "_G";
		std::string blue(Name);		blue += "_B";

		SdkGetSettingFloat(red.c_str(), &(Value->x), 255.0f);
		SdkGetSettingFloat(green.c_str(), &(Value->y), 255.0f);
		SdkGetSettingFloat(blue.c_str(), &(Value->z), 255.0f);
	}

	static void Hotkey(HotKey_t& KeyStruct) {	
		std::string name(KeyStruct.Name);
		name.erase(std::remove_if(name.begin(), name.end(), std::isspace), name.end());
		SdkGetSettingString(name.c_str(), KeyStruct.buffer, (size_t)KeyStruct.buffer_size, KeyStruct.buffer) ;

		int tmpKey{}; name += "_VirtualKey";
		SdkGetSettingNumber(name.c_str(), &tmpKey, (int)KeyStruct.VirtualKey);		
		KeyStruct.VirtualKey = tmpKey;		
	}

	static void DropList(const char * Name, int * Value, int Default) {
		SdkGetSettingNumber(Name, Value, Default);
	}
};

typedef struct Hotkey_t{
	int Key;
	bool Active;
} Hotkey;
typedef struct KeyToggle_t{
	int Key;
	bool Toggle;
} KeyToggle;
class Menu {	
	static std::map<std::string, float> m_Float;
	static std::map<std::string, bool>  InitializedFloat;

	static std::map<std::string, int>  m_Int;
	static std::map<std::string, bool> InitializedInt;

	static std::map<std::string, bool> m_Bool;
	static std::map<std::string, bool> InitializedBool;

	static std::map<std::string, SDKVECTOR> m_Vector;
	static std::map<std::string, bool>		InitializedVector;

	struct Hotkey_t {
		int VirtualKey;

		static unsigned int lastId;
		bool Clicked;
		bool Changed;
		unsigned int Id;
		std::string Buffer;

		Hotkey_t(int virtualKey = 0) : Id(lastId++), Changed(false), Clicked(false) {
			VirtualKey = virtualKey;

			static char lpszName[256];
			long ScanCode{ (long)MapVirtualKeyA((UINT)virtualKey, 0) };
			if (GetKeyNameTextA(ScanCode << 16, lpszName, sizeof(lpszName))) {
				Buffer = lpszName;
			}
			else { Buffer = ""; }
		}
	};
	static std::map<std::string, Hotkey_t>  m_Hotkey;
	static std::map<std::string, bool>		InitializedHotkey;

	struct Toggle_t {
		Hotkey_t hotkey;
		bool Active;
		bool Changed;
	};
	static std::map<std::string, Toggle_t>  m_KeyToggle;
	static std::map<std::string, bool>		InitializedKeyToggle;

public:
	static void ForceOnSameLine() {	SdkUiForceOnSameLine();	}	
	static void BulletPoint()  { SdkUiBulletPoint();}

	#pragma region Get Values
	template <typename T> static T Get(std::string NameID);

	template<> static float		Get<float>(std::string NameID) {
		if (m_Float.count(NameID) > 0) {
			return m_Float[NameID];
		}
		return 0.0f;
	}
			   
	template<> static int		Get<int>(std::string NameID) {
		if (m_Int.count(NameID) > 0) {
			return m_Int[NameID];
		}
		return 0;		
	}
			   
	template<> static bool		Get<bool>(std::string NameID) {		
		if (m_Bool.count(NameID) > 0) {
			return m_Bool[NameID];
		}
		return false; 		
	}

	template<> static SDKVECTOR Get<SDKVECTOR>(std::string NameID) {
		if (m_Vector.count(NameID) > 0) {
			return m_Vector[NameID];
		}
		return SDKVECTOR{};
	}

	template<> static SDKCOLOR  Get<SDKCOLOR>(std::string NameID) {
		if (m_Vector.count(NameID) > 0) {
			auto color_vec{ m_Vector[NameID] };
			return {
				static_cast<unsigned char>(color_vec.z * 255),
				static_cast<unsigned char>(color_vec.y * 255),
				static_cast<unsigned char>(color_vec.x * 255),
				255
			};
		}
		return {
			static_cast<unsigned char>(0),
			static_cast<unsigned char>(0),
			static_cast<unsigned char>(0),
			255
		};
	}

	template<> static Hotkey    Get<Hotkey>(std::string NameID) {
		int key{ 0 };
		if (m_Hotkey.count(NameID) > 0) {
			key = m_Hotkey[NameID].VirtualKey;
		}
		return { key, Game::IsKeyPressed(key) };
	}

	template<> static KeyToggle Get<KeyToggle>(std::string NameID) {
		auto toggle_struct{ m_KeyToggle[NameID] };
		if (m_KeyToggle.count(NameID) > 0) {
			toggle_struct = m_KeyToggle[NameID];
			return { toggle_struct.hotkey.VirtualKey, toggle_struct.Active };
		}
		return { 0, false };
	}
	#pragma endregion
	
	#pragma region Set Values
	template <typename T> static void Set(std::string NameID, bool Value);
	template <typename T> static void Set(std::string NameID, float Value);
	template <typename T> static void Set(std::string NameID, int Value);
	template <typename T> static void Set(std::string NameID, SDKVECTOR Value);
	template <typename T> static void Set(std::string NameID, SDKCOLOR Value);

	template<> static void Set<float>(std::string NameID, float Value)
	{
		if (m_Float.count(NameID) > 0)
			m_Float[NameID] = Value;
	}

	template<>static void Set<int>(std::string NameID, int Value)
	{
		if (m_Int.count(NameID) > 0)
			m_Int[NameID] = Value;
	}

	template<> static void Set<bool>(std::string NameID, bool Value)
	{
		if (m_Bool.count(NameID) > 0)
			m_Bool[NameID] = Value;
	}

	template<> static void Set<SDKVECTOR>(std::string NameID, SDKVECTOR Value)
	{
		if (m_Vector.count(NameID) > 0)
			m_Vector[NameID] = Value;
	}

	template<> static void Set<SDKCOLOR>(std::string NameID, SDKCOLOR Value)
	{
		if (m_Vector.count(NameID) > 0)
			m_Vector[NameID] = {static_cast<float>(Value.R) / 255, static_cast<float>(Value.G) / 255, static_cast<float>(Value.B) / 255};
	}

	template<> static void Set<KeyToggle>(std::string NameID, bool Value)
	{
		if (m_KeyToggle.count(NameID) > 0)
		{
			m_KeyToggle[NameID].Active = Value;
			m_KeyToggle[NameID].Changed = true;
		}
	}
	#pragma endregion
	
	#pragma region Float	   
	static void Float(const char* DisplayName, std::string NameID, float Default = 0.0f) {
		//Load
		if (InitializedFloat.count(NameID) == 0) {
			SdkGetSettingFloat(NameID.c_str(), &(m_Float[NameID]), Default);
			InitializedFloat[NameID] = true;
		}

		//Update
		bool changed = false;
		SdkUiInputFloat(DisplayName, &(m_Float[NameID]), 3, &changed);

		//Save
		if (changed) {
			SdkSetSettingFloat(NameID.c_str(), m_Float[NameID]);
		}
	}
	#pragma endregion

	#pragma region Int
	static void Int(const char* DisplayName, std::string NameID, int Default = 0) {
		//Load
		if (InitializedInt.count(NameID) == 0) {
			SdkGetSettingNumber(NameID.c_str(), &(m_Int[NameID]), Default);
			InitializedInt[NameID] = true;
		}

		//Update
		bool changed = false;
		SdkUiInputNumber(DisplayName, &(m_Int[NameID]), &changed);

		//Save
		if (changed) {
			SdkSetSettingNumber(NameID.c_str(), m_Int[NameID]);
		}
	}
	#pragma endregion
	
	#pragma region Checkbox
	static void Checkbox(const char* DisplayName, std::string NameID, bool Default = false) {		
		//Load
		if (InitializedBool.count(NameID) == 0) {
			SdkGetSettingBool(NameID.c_str(), &(m_Bool[NameID]), Default);
			InitializedBool[NameID] = true;
		}

		//Update
		bool changed = false;
		SdkUiCheckbox(DisplayName, &(m_Bool[NameID]), &changed);

		//Save
		if (changed) {
			SdkSetSettingBool(NameID.c_str(), m_Bool[NameID]);
		}
	}
	#pragma endregion
	
	#pragma region SliderFloat
	static void SliderFloat(const char* DisplayName, std::string NameID, float Default, float MinValue, float MaxValue, const char* Format = "%.3f") {
		//Load
		if (InitializedFloat.count(NameID) == 0) {
			SdkGetSettingFloat(NameID.c_str(), &(m_Float[NameID]), Default);
			InitializedFloat[NameID] = true;
		}

		//Update
		bool changed = false;
		SdkUiDragFloat(DisplayName, &(m_Float[NameID]), MinValue, MaxValue, Format, &changed);

		//Save
		if (changed) {
			SdkSetSettingFloat(NameID.c_str(), m_Float[NameID]);
		}			
	}
	#pragma endregion

	#pragma region SliderInt
	static void SliderInt(const char* DisplayName, std::string NameID, int Default, int MinValue, int MaxValue) {
		//Load
		if (InitializedInt.count(NameID) == 0) {
			SdkGetSettingNumber(NameID.c_str(), &(m_Int[NameID]), Default);
			InitializedInt[NameID] = true;
		}

		//Update
		bool changed = false;
		float val = (float) m_Int[NameID];
		SdkUiDragFloat(DisplayName, &val, (float)MinValue, (float)MaxValue, "%.0f", &changed);
		m_Int[NameID] = (int)std::floor(val);

		//Save
		if (changed) {
			SdkSetSettingNumber(NameID.c_str(), m_Int[NameID]);
		}
	}
	#pragma endregion

	#pragma region Vector
	static void Vector(const char* DisplayName, std::string NameID, SDKVECTOR Default) {
		//Load
		if (InitializedVector.count(NameID) == 0) {
			SdkGetSettingFloat((NameID + "_X").c_str(),  &(m_Vector[NameID].x), Default.x);
			SdkGetSettingFloat((NameID + "_Y").c_str(),  &(m_Vector[NameID].y), Default.y);
			SdkGetSettingFloat((NameID + "_Z").c_str(),  &(m_Vector[NameID].z), Default.z);
			InitializedVector[NameID] = true;
		}

		//Update
		bool changed = false;
		SdkUiInputVector(DisplayName, &(m_Vector[NameID]), 3, &changed);

		//Save
		if (changed) {
			SdkSetSettingFloat((NameID + "_X").c_str(), m_Vector[NameID].x);
			SdkSetSettingFloat((NameID + "_Y").c_str(), m_Vector[NameID].y);
			SdkSetSettingFloat((NameID + "_Z").c_str(), m_Vector[NameID].z);
		}
	}
	#pragma endregion

	#pragma region ColorPicker
	static void ColorPicker(const char* DisplayName, std::string NameID, SDKVECTOR Default) {
		//Load
		if (InitializedVector.count(NameID) == 0) {
			SdkGetSettingFloat((NameID + "_R").c_str(),  &(m_Vector[NameID].x), min(Default.x > 1.f ? Default.x / 255.f : Default.x, 1.f));
			SdkGetSettingFloat((NameID + "_G").c_str(),  &(m_Vector[NameID].y), min(Default.y > 1.f ? Default.y / 255.f : Default.y, 1.f));
			SdkGetSettingFloat((NameID + "_B").c_str(),  &(m_Vector[NameID].z), min(Default.z > 1.f ? Default.z / 255.f : Default.z, 1.f));			
			InitializedVector[NameID] = true;
		}

		//Update
		bool changed = false;
		SdkUiColorPicker(DisplayName, &(m_Vector[NameID]), &changed);

		//Save
		if (changed) {
			SdkSetSettingFloat((NameID + "_R").c_str(), min(m_Vector[NameID].x > 1.f ? m_Vector[NameID].x / 255.f : m_Vector[NameID].x, 1.f));
			SdkSetSettingFloat((NameID + "_G").c_str(), min(m_Vector[NameID].y > 1.f ? m_Vector[NameID].y / 255.f : m_Vector[NameID].y, 1.f));
			SdkSetSettingFloat((NameID + "_B").c_str(), min(m_Vector[NameID].z > 1.f ? m_Vector[NameID].z / 255.f : m_Vector[NameID].z, 1.f));
		}
	}
	#pragma endregion

	#pragma region DropList
	static void DropList(const char* DisplayName, std::string NameID, std::vector<std::string> Items, int Default = 0) {
		//Load
		if (InitializedInt.count(NameID) == 0) {
			SdkGetSettingNumber(NameID.c_str(), &(m_Int[NameID]), Default);
			InitializedInt[NameID] = true;
		}

		//Update
		bool changed = false;

		std::vector<const char*> values;
		for (const auto& item : Items)
		{
			values.push_back(item.c_str());
		}

		SdkUiCombo(DisplayName, &(m_Int[NameID]), values.data(), (int)Items.size(), &changed);		

		//Save
		if (changed) {
			SdkSetSettingNumber(NameID.c_str(), m_Int[NameID]);
		}
	}
	#pragma endregion

	#pragma region Hotkey && HotkeyToggle
	private:
	static bool hkInit;
	static std::vector<Hotkey_t*> requestKeyChange;	
	static size_t maxLength;

	static void WndProc(bool KeyDown, unsigned int VirtualKey, unsigned short Repeated, unsigned char ScanCode, bool IsExtended, bool IsAltDown, bool PreviousState, bool TransitionState, void* UserData) {
		UNREFERENCED_PARAMETER(UserData);
		UNREFERENCED_PARAMETER(KeyDown);

		if (ExcludeOnWnd.find(VirtualKey) == ExcludeOnWnd.end() && Game::IsAvailable()) {
			if (VirtualKey != VK_ESCAPE) {
				if (!requestKeyChange.empty()) {
					requestKeyChange.back()->VirtualKey = (int)VirtualKey;

					static char buffer[256];
					long lParam{ Repeated | (ScanCode << 16) | (IsExtended << 24) | (IsAltDown << 29) | (PreviousState << 30) | (TransitionState << 31) };
					GetKeyNameTextA(lParam, buffer, sizeof(buffer));

					requestKeyChange.back()->Buffer = buffer;
					requestKeyChange.back()->Changed = true;
					requestKeyChange.back()->Clicked = false;
					requestKeyChange.pop_back();
					//SdkUiConsoleWrite("Changed Value of %s to %s", requestKeyChange.back()->Name, requestKeyChange.back()->buffer);

				}				

				if (!PreviousState) {
					for (auto &[_, val] : m_KeyToggle) {
						if (VirtualKey == (unsigned int)val.hotkey.VirtualKey) {
							val.Active = !val.Active;
							val.Changed = true;
						}
					}
				}				
			}
			else {
				if (!requestKeyChange.empty()) {
					//SdkUiConsoleWrite("Change of Keys Cancelled");
					for (auto Key : requestKeyChange) {
						Key->Clicked = false;
						Key->Changed = false;
					}
					requestKeyChange.clear();
				}				
			}
		}
	}
	public:
	#pragma region Hotkey
	static void Hotkey(const char* DisplayName, std::string NameID, int Default) {
		if (!hkInit) {
			SdkRegisterOnKeyPress(WndProc, NULL);
			hkInit = true;
		}

		//Load
		if (InitializedHotkey.count(NameID) == 0) {
			m_Hotkey[NameID] = Hotkey_t{ Default };

			static char Buffer[256];
			SdkGetSettingString(NameID.c_str(), Buffer, sizeof(Buffer), m_Hotkey[NameID].Buffer.c_str());			
			m_Hotkey[NameID].Buffer = Buffer;

			int tmpKey{};
			SdkGetSettingNumber((NameID + "_VirtualKey").c_str(), &tmpKey, (int)m_Hotkey[NameID].VirtualKey);
			m_Hotkey[NameID].VirtualKey = (tmpKey > 0 ? tmpKey : 0);

			InitializedHotkey[NameID] = true;
		}		

		///Alignment Stuff
		if (strlen(DisplayName) > maxLength) {maxLength = strlen(DisplayName);}
		std::string format{ ("%-" + std::to_string(maxLength)) + "s - "}; 

		SdkUiText(format.c_str(), DisplayName);
		SdkUiForceOnSameLine();

		if (m_Hotkey[NameID].Clicked) {
			SdkUiSmallButton("Please Press Any Key", NULL);
			for (auto Key : requestKeyChange) { if (Key->Id == m_Hotkey[NameID].Id) { return; } }
			requestKeyChange.push_back(&m_Hotkey[NameID]);
		}
		else {
			SdkUiSmallButton((m_Hotkey[NameID].Buffer + ("###" + NameID)).c_str(), &m_Hotkey[NameID].Clicked);
		}

		SdkUiForceOnSameLine();
		if (Game::IsKeyPressed(m_Hotkey[NameID].VirtualKey)) {
			SdkUiColoredText(&(Color::Green), " %-3s", "ON");
		}
		else {
			SdkUiColoredText(&(Color::Red), " %-3s", "OFF");
		}

		//Save
		if (m_Hotkey[NameID].Changed) {
			SdkSetSettingString(NameID.c_str(), m_Hotkey[NameID].Buffer.c_str());
			m_Hotkey[NameID].Changed = false;

			int tmpKey{ m_Hotkey[NameID].VirtualKey > 0 ? (int)m_Hotkey[NameID].VirtualKey : 0 };
			SdkSetSettingNumber((NameID + "_VirtualKey").c_str(), tmpKey);
		}
	}
	#pragma endregion

	#pragma region HotkeyToggle
	static void HotkeyToggle(const char* DisplayName, std::string NameID, int Default, bool DefaultBool) {
		if (!hkInit) {
			SdkRegisterOnKeyPress(WndProc, NULL);
			hkInit = true;
		}

		//Load
		if (InitializedKeyToggle.count(NameID) == 0) {
			m_KeyToggle[NameID] = Toggle_t{ Hotkey_t{ Default },  DefaultBool};

			static char Buffer[256];
			SdkGetSettingString((NameID + "_Hotkey").c_str(), Buffer, sizeof(Buffer), m_KeyToggle[NameID].hotkey.Buffer.c_str());
			m_KeyToggle[NameID].hotkey.Buffer = Buffer;

			int tmpKey{};
			SdkGetSettingNumber((NameID + "_VirtualKey").c_str(), &tmpKey, m_KeyToggle[NameID].hotkey.VirtualKey);
			m_KeyToggle[NameID].hotkey.VirtualKey = (tmpKey > 0 ? tmpKey : 0);	

			SdkGetSettingBool(NameID.c_str(), &(m_KeyToggle[NameID].Active), m_KeyToggle[NameID].Active);			

			InitializedKeyToggle[NameID] = true;
		}

		///Alignment Stuff		
		if (strlen(DisplayName) > maxLength) { maxLength = strlen(DisplayName); }
		std::string format{ ("%-" + std::to_string(maxLength)) + "s - " };

		SdkUiText(format.c_str(), DisplayName);
		SdkUiForceOnSameLine();

		if (m_KeyToggle[NameID].hotkey.Clicked) {
			SdkUiSmallButton("Please Press Any Key", NULL);
			for (auto Key : requestKeyChange) { if (Key->Id == m_KeyToggle[NameID].hotkey.Id) { return; } }
			requestKeyChange.push_back(&(m_KeyToggle[NameID].hotkey));
		}
		else {
			SdkUiSmallButton((m_KeyToggle[NameID].hotkey.Buffer + ("###" + NameID)).c_str(), &(m_KeyToggle[NameID].hotkey.Clicked));
		}

		SdkUiForceOnSameLine();
		if (m_KeyToggle[NameID].Active) {
			SdkUiColoredText(&(Color::Green), " %-3s", "ON");
		}
		else {
			SdkUiColoredText(&(Color::Red), " %-3s", "OFF");
		}
		
		//Save
		if (m_KeyToggle[NameID].Changed) {
			SdkSetSettingBool(NameID.c_str(), m_KeyToggle[NameID].Active);
			m_KeyToggle[NameID].Changed = false;
		}
		if (m_KeyToggle[NameID].hotkey.Changed) {
			SdkSetSettingString((NameID + "_Hotkey").c_str(), m_KeyToggle[NameID].hotkey.Buffer.c_str());
			m_KeyToggle[NameID].hotkey.Changed = false;

			int tmpKey{ m_KeyToggle[NameID].hotkey.VirtualKey > 0 ? (int)m_KeyToggle[NameID].hotkey.VirtualKey : 0 };
			SdkSetSettingNumber((NameID + "_VirtualKey").c_str(), tmpKey);
		}
	}
	#pragma endregion
	#pragma endregion

	#pragma region BulletText
	static void BulletText(std::string format, ...) {
		va_list args;
		va_start(args, format);
		auto size = _vscprintf(format.c_str(), args);
		std::string result((size_t)++size, 0);
		vsnprintf_s(const_cast<char*>(result.c_str()), (size_t)size, _TRUNCATE, format.c_str(), args);
		va_end(args);

		SdkUiBulletText(result.c_str());
	}
	#pragma endregion

	#pragma region Button
	static void Button(std::string DisplayName, const std::function<void()> function) {
		bool clicked = false;
		SdkUiButton(DisplayName.c_str(), &clicked);

		if (clicked) {
			function();
		}
	}
	#pragma endregion

	#pragma region SmallButton
	static void SmallButton(std::string title, const std::function<void()> function) {
		bool clicked = false;
		SdkUiSmallButton(title.c_str(), &clicked);
		if (clicked) {
			function();
		}			
	}
	#pragma endregion

	#pragma region Window
	static void Window(const char* DisplayName, std::string NameID, bool visible, bool collapsed, const std::function<void()> function)	{
		auto v{ NameID + "_Visible" };
		auto c{ NameID + "_Collapsed" };		

		if (m_Bool.count(v) == 0)
			function();

		SDKPOINT position = { 300, 420 };
		SdkUiSetNextWindowPos(&position);

		m_Bool[v] = visible;
		m_Bool[c] = collapsed;

		if (m_Bool[v]) {
			SdkUiBeginWindow(DisplayName, &(m_Bool[v]), &(m_Bool[c]));
		}
		
		if (m_Bool[v] && m_Bool[c]) {
			function();
			SdkUiEndWindow();
		}
	}
	#pragma endregion

	#pragma region Tree
	static void Tree(const char* DisplayName, std::string NameID, bool expanded, const std::function<void()> function) {
		if (m_Bool.count(NameID) == 0) {
			function();
			m_Bool[NameID] = expanded;
		}		
		
		SdkUiBeginTree(DisplayName, &m_Bool[NameID]);
		if (m_Bool[NameID])	{
			function();
			SdkUiEndTree();
		}
	}
	#pragma endregion

};

__declspec(selectany)bool MakeMenu::hkInit;
__declspec(selectany)unsigned int HotKey_t::lastId;
__declspec(selectany)std::vector<HotKey_t*> MakeMenu::requestKeyChange;

__declspec(selectany)bool Menu::hkInit;
__declspec(selectany)unsigned int Menu::Hotkey_t::lastId;
__declspec(selectany)std::vector<Menu::Hotkey_t*> Menu::requestKeyChange;

__declspec(selectany) std::map<std::string, float> Menu::m_Float;
__declspec(selectany) std::map<std::string, bool>  Menu::InitializedFloat;

__declspec(selectany) std::map<std::string, int>  Menu::m_Int;
__declspec(selectany) std::map<std::string, bool> Menu::InitializedInt;

__declspec(selectany) std::map<std::string, bool> Menu::m_Bool;
__declspec(selectany) std::map<std::string, bool> Menu::InitializedBool;

__declspec(selectany) std::map<std::string, SDKVECTOR> Menu::m_Vector;
__declspec(selectany) std::map<std::string, bool>		Menu::InitializedVector;
__declspec(selectany) std::map<std::string, Menu::Hotkey_t>  Menu::m_Hotkey;
__declspec(selectany) std::map<std::string, bool>		Menu::InitializedHotkey;

__declspec(selectany) std::map<std::string, Menu::Toggle_t>  Menu::m_KeyToggle;
__declspec(selectany) std::map<std::string, bool>		Menu::InitializedKeyToggle;
__declspec(selectany) size_t		Menu::maxLength{10};