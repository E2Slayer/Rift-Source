#pragma once

#pragma warning(push, 0)
#pragma warning(disable: 4242 4244 4774)
#include <string>
#include <algorithm>
#include <chrono>
#include <random>
#include <functional>

#include <assert.h>
#include <string.h>
#include <sstream>
#pragma warning(pop)

class Common {
public:
	static std::string GetBaseParticleName(std::string& skin) {
		char* First = _strdup(skin.c_str());	
		if (!First)
			return std::string();
		
		char* Context1 = NULL;
		char* Token1 = strtok_s(First, "_", &Context1);		
		std::ostringstream result{};

		for (size_t i = 1; (Token1); ++i) {
			if (i != 1) 
				result << "_";			

			if (i == 2) 
				result << "Base";			
			else 
				result << Token1;			
			
			Token1 = strtok_s(NULL, "_", &Context1);			
		}

		free(First);		
		return (result.str());
	}

	static void ToLower(std::string* str) {
		if (str == NULL) { return; }
		#pragma warning(push)
		#pragma warning(disable: 4242 4244)
		std::transform(str->begin(), str->end(), str->begin(), ::tolower);
		#pragma warning(pop)		
	}
	static unsigned int GetTickCount() {
		return (unsigned int)(std::chrono::high_resolution_clock::now().time_since_epoch().count() / (1000 * 1000));
	}
	static bool IsKeyPressed(int key) {
		return (GetAsyncKeyState(key) & 0x8000);
	}
	static size_t GenerateRandom(size_t min, size_t max) {
		std::mt19937 rng;
		rng.seed(std::random_device()());
		std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);

		return dist(rng);
	}
	static bool CompareLower(const std::string& a, const std::string& b) {
		return std::equal(a.begin(), a.end(),
			b.begin(), b.end(),
			[](char a, char b) {
				return tolower(a) == tolower(b);
			}
		);
	}	
	static float DegToRad(float degrees) {
		return (float)(degrees * M_PI) / 180.0f;
	}
	static float RadToDeg(float radians) {
		return (float)(radians / M_PI) * 180.0f;
	}

	//Thanks @Rift#7189
	static void* GetLambdaAddress(std::function<void()>& func) {
		void** func_table{ *(void***)&func };
		return (func_table) ? func_table[2] : NULL;
	}
};
