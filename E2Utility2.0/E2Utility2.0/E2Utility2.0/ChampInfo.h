#pragma once
#include <climits>

constexpr int RequiredExp[] = {
0, 280, 660, 1140, 1720, 2400, 3180, 4060, 5040, 6120, 7300, 8580, 9960, 11440, 13020, 14700, 16480, 18360, INT_MAX
};
class ChampInfo
{
private:


public:

	static float ExperiencePercentage(const int& currentLevel, const float& currentExp)
	{
		if (currentLevel >= 18)
			return 0.0F;
		return (100.0f / (RequiredExp[currentLevel] - RequiredExp[currentLevel - 1]) * (
			currentExp - RequiredExp[currentLevel - 1]));
	}

};
