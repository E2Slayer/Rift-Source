#pragma once
#include "../../Include/SDK Extensions.h"
#include <string.h>
#include "resource.h"

const std::string ChampionNameList[]
{
	"Aatrox",

	"Ahri",

	"Akali",

	"Alistar",

	"Amumu",

	"Anivia",

	"Annie",

	"Ashe",

	"AurelionSol",

	"Azir",

	"Bard",

	"Blitzcrank",

	"Brand",

	"Braum",

	"Caitlyn",

	"Camille",

	"Cassiopeia",

	"Chogath",

	"Corki",

	"Darius",

	"Default",

	"Diana",

	"Draven",

	"DrMundo",

	"Ekko",

	"Elise",

	"Evelynn",

	"Ezreal",

	"FiddleSticks",

	"Fiora",

	"Fizz",

	"Galio",

	"Gangplank",

	"Garen",

	"Gnar",

	"Gragas",

	"Graves",

	"Hecarim",

	"Heimerdinger",

	"Illaoi",

	"Irelia",

	"Ivern",

	"Janna",

	"JarvanIV",

	"Jax",

	"Jayce",

	"Jhin",

	"Jinx",

	"Kaisa",

	"Kalista",

	"Karma",

	"Karthus",

	"Kassadin",

	"Katarina",

	"Kayle",

	"Kayn",

	"Kennen",

	"Khazix",

	"Kindred",

	"Kled",

	"KogMaw",

	"Leblanc",

	"LeeSin",

	"Leona",

	"Lissandra",

	"Lucian",

	"Lulu",

	"Lux",

	"Malphite",

	"Malzahar",

	"Maokai",

	"MasterYi",

	"MissFortune",

	"MonkeyKing",

	"Mordekaiser",

	"Morgana",

	"Nami",

	"Nasus",

	"Nautilus",

	"Neeko",

	"Nidalee",

	"Nocturne",

	"Nunu",

	"Olaf",

	"Orianna",

	"Ornn",

	"Pantheon",

	"Poppy",

	"PracticeTool_TargetDummy",

	"Pyke",

	"Quinn",

	"Rakan",

	"Rammus",

	"RekSai",

	"Renekton",

	"Rengar",

	"Riven",

	"Rumble",

	"Ryze",

	"Sejuani",

	"Shaco",

	"Shen",

	"Shyvana",

	"Singed",

	"Sion",

	"Sivir",

	"Skarner",

	"Sona",

	"Soraka",

	"Swain",

	"Sylas",

	"Syndra",

	"TahmKench",

	"Taliyah",

	"Talon",

	"Taric",

	"Teemo",

	"Thresh",

	"Tristana",

	"Trundle",

	"Tryndamere",

	"TwistedFate",

	"Twitch",

	"Udyr",

	"Urgot",

	"Varus",

	"Vayne",

	"Veigar",

	"Velkoz",

	"Vi",

	"Viktor",

	"Vladimir",

	"Volibear",

	"Warwick",

	"Xayah",

	"Xerath",

	"XinZhao",

	"Yasuo",

	"Yorick",

	"Zac",

	"Zed",

	"Ziggs",

	"Zilean",

	"Zoe",

	"Zyra"


};


const std::string LastPostionChampionNameList[]
{
	"Aatrox",

	"Ahri",

	"Akali",

	"Alistar",

	"Amumu",

	"Anivia",

	"Annie",

	"Ashe",

	"AurelionSol",

	"Azir",

	"Bard",

	"Blitzcrank",

	"Brand",

	"Braum",

	"Caitlyn",

	"Camille",

	"Cassiopeia",

	"Chogath",

	"Corki",

	"Darius",

	"Default",

	"Diana",

	"Draven",

	"DrMundo",

	"Ekko",

	"Elise",

	"Evelynn",

	"Ezreal",

	"FiddleSticks",

	"Fiora",

	"Fizz",

	"Galio",

	"Gangplank",

	"Garen",

	"Gnar",

	"Gragas",

	"Graves",

	"Hecarim",

	"Heimerdinger",

	"Illaoi",

	"Irelia",

	"Ivern",

	"Janna",

	"JarvanIV",

	"Jax",

	"Jayce",

	"Jhin",

	"Jinx",

	"Kaisa",

	"Kalista",

	"Karma",

	"Karthus",

	"Kassadin",

	"Katarina",

	"Kayle",

	"Kayn",

	"Kennen",

	"Khazix",

	"Kindred",

	"Kled",

	"KogMaw",

	"Leblanc",

	"LeeSin",

	"Leona",

	"Lissandra",

	"Lucian",

	"Lulu",

	"Lux",

	"Malphite",

	"Malzahar",

	"Maokai",

	"MasterYi",

	"MissFortune",

	"MonkeyKing",

	"Mordekaiser",

	"Morgana",

	"Nami",

	"Nasus",

	"Nautilus",

	"Neeko",

	"Nidalee",

	"Nocturne",

	"Nunu",

	"Olaf",

	"Orianna",

	"Ornn",

	"Pantheon",

	"Poppy",

	"Pyke",

	"Quinn",

	"Rakan",

	"Rammus",

	"RekSai",

	"Renekton",

	"Rengar",

	"Riven",

	"Rumble",

	"Ryze",

	"Sejuani",

	"Shaco",

	"Shen",

	"Shyvana",

	"Singed",

	"Sion",

	"Sivir",

	"Skarner",

	"Sona",

	"Soraka",

	"Swain",

	"Sylas",

	"Syndra",

	"TahmKench",

	"Taliyah",

	"Talon",

	"Taric",

	"Teemo",

	"Thresh",

	"Tristana",

	"Trundle",

	"Tryndamere",

	"TwistedFate",

	"Twitch",

	"Udyr",

	"Urgot",

	"Varus",

	"Vayne",

	"Veigar",

	"Velkoz",

	"Vi",

	"Viktor",

	"Vladimir",

	"Volibear",

	"Warwick",

	"Xayah",

	"Xerath",

	"XinZhao",

	"Yasuo",

	"Yorick",

	"Zac",

	"Zed",

	"Ziggs",

	"Zilean",

	"Zoe",

	"Zyra"


};

const std::string SummonerSpellsList[9]
{
	"summonerbarrier",
	"summonerboost",
	"summonerdot",
	"summonerexhaust",
	"summonerflash",
	"summonerhaste",
	"summonerheal",
	"summonersmite",
	"summonerteleport"
};

class ChampionNames
{

private:

public:
	static unsigned int GetChampionIMG(const char* name, int start)
	{
		//SdkUiConsoleWrite("Size %d", sizeof(ChampionNameList)/sizeof(*ChampionNameList));
		int ArraySize = sizeof(ChampionNameList) / sizeof(*ChampionNameList);
		int temp = start;
		for (int i = 0; i < ArraySize; i++)
		{

			//SdkUiConsoleWrite("inside check %s", ChampionNameList[i].c_str());
			if (_stricmp(name, ChampionNameList[i].c_str()) == 0)
			{
				return temp;
			}
			temp++;
		}


		return 165; //165 is default
	}

	static unsigned int GetSummonerSpellIMG(const char* name, int start)
	{
		//SdkUiConsoleWrite("Size %d", sizeof(SummonerSpellsList));
		int temp = start;
		int ArraySize = sizeof(SummonerSpellsList) / sizeof(*SummonerSpellsList);
		
		for (int i = 0; i < ArraySize; i++)
		{

			//SdkUiConsoleWrite("inside check %s", ChampionNameList[i].c_str());
			if (_stricmp(name, SummonerSpellsList[i].c_str()) == 0)
			{
				return temp;
			}
			temp++;
		}
		

		return 304; //165 is default
	}

	static unsigned int GetLastPostionChampionIMG(const char* name, int start)
	{
		//SdkUiConsoleWrite("Size %d", sizeof(ChampionNameList)/sizeof(*ChampionNameList));
		int ArraySize = sizeof(LastPostionChampionNameList) / sizeof(*LastPostionChampionNameList);
		int temp = start;
		for (int i = 0; i < ArraySize; i++)
		{

			//SdkUiConsoleWrite("inside check %s", ChampionNameList[i].c_str());
			if (_stricmp(name, LastPostionChampionNameList[i].c_str()) == 0)
			{
				return temp;
			}
			temp++;
		}


		return 328; //165 is default
	}


	static unsigned int GetSummonerSpells(const char* name, bool isReady)
	{
		if (_stricmp(name, "Summoner_Dash") == 0)
		{
			if (isReady)
			{
				return Summoner_Dash;
			}
			else
			{
				return Summoner_Dash_Cooldown;
			}
		}

		if (_stricmp(name, "Summoner_Empty") == 0)
		{
			if (isReady)
			{
				return Summoner_Empty;
			}
			else
			{
				return Summoner_Empty_Cooldown;
			}
		}

		if (_stricmp(name, "Summoner_flashPerksHextechFlashtraption") == 0)
		{
			if (isReady)
			{
				return Summoner_flashPerksHextechFlashtraption;
			}
			else
			{
				return Summoner_flashPerksHextechFlashtraption_Cooldown;
			}
		}

		if (_stricmp(name, "Summoner_flashPerksHextechFlashtraptionBlue") == 0)
		{
			if (isReady)
			{
				return Summoner_flashPerksHextechFlashtraptionBlue;
			}
			else
			{
				return Summoner_flashPerksHextechFlashtraptionBlue_Cooldown;
			}
		}

		if (_stricmp(name, "Summoner_Mark") == 0)
		{
			if (isReady)
			{
				return Summoner_Mark;
			}
			else
			{
				return Summoner_Mark_Cooldown;
			}
		}

		if (_stricmp(name, "SummonerBarrier") == 0)
		{
			if (isReady)
			{
				return SummonerBarrier;
			}
			else
			{
				return SummonerBarrier_Cooldown;
			}
		}

		if (_stricmp(name, "SummonerBoost") == 0)
		{
			if (isReady)
			{
				return SummonerBoost;
			}
			else
			{
				return SummonerBoost_Cooldown;
			}
		}

		if (_stricmp(name, "SummonerClairvoyance") == 0)
		{
			if (isReady)
			{
				return SummonerClairvoyance;
			}
			else
			{
				return SummonerClairvoyance_Cooldown;
			}
		}

		if (_stricmp(name, "SummonerDot") == 0)
		{
			if (isReady)
			{
				return SummonerDot;
			}
			else
			{
				return SummonerDot_Cooldown;
			}
		}

		if (_stricmp(name, "SummonerExhaust") == 0)
		{
			if (isReady)
			{
				return SummonerExhaust;
			}
			else
			{
				return SummonerExhaust_Cooldown;
			}
		}

		if (_stricmp(name, "SummonerFlash") == 0)
		{
			if (isReady)
			{
				return SummonerFlash;
			}
			else
			{
				return SummonerFlash_Cooldown;
			}
		}

		if (_stricmp(name, "SummonerFlashPerksHextechFlashtraptionV2") == 0)
		{
			if (isReady)
			{
				return SummonerFlashPerksHextechFlashtraptionV2;
			}
			else
			{
				return SummonerFlashPerksHextechFlashtraptionV2_Cooldown;
			}
		}
		if (_stricmp(name, "SummonerHaste") == 0)
		{
			if (isReady)
			{
				return SummonerHaste;
			}
			else
			{
				return SummonerHaste_Cooldown;
			}
		}

		if (_stricmp(name, "SummonerHeal") == 0)
		{
			if (isReady)
			{
				return SummonerHeal;
			}
			else
			{
				return SummonerHeal_Cooldown;
			}
		}

		if (_stricmp(name, "SummonerMana") == 0)
		{
			if (isReady)
			{
				return SummonerMana;
			}
			else
			{
				return SummonerMana_Cooldown;
			}
		}
		if (_stricmp(name, "SummonerExhaust") == 0)
		{
			if (isReady)
			{
				return SummonerExhaust;
			}
			else
			{
				return SummonerExhaust_Cooldown;
			}
		}

		if (_stricmp(name, "SummonerSmite") == 0)
		{
			if (isReady)
			{
				return SummonerSmite;
			}
			else
			{
				return SummonerSmite_Cooldown;
			}
		}

		if (_stricmp(name, "S5_SummonerSmitePlayerGanker") == 0)
		{
			if (isReady)
			{
				return SummonerSmite;
			}
			else
			{
				return SummonerSmite_Cooldown;
			}
		}

		if (_stricmp(name, "S5_SummonerSmiteDuel") == 0)
		{
			if (isReady)
			{
				return SummonerSmite;
			}
			else
			{
				return SummonerSmite_Cooldown;
			}
		}

		if (_stricmp(name, "SummonerSnowball") == 0)
		{
			if (isReady)
			{
				return SummonerSnowball;
			}
			else
			{
				return SummonerSnowball_Cooldown;
			}
		}

		if (_stricmp(name, "SummonerTeleport") == 0)
		{
			if (isReady)
			{
				return SummonerTeleport;
			}
			else
			{
				return SummonerTeleport_Cooldown;
			}
		}
		if (_stricmp(name, "TeleportCancel") == 0)
		{
			if (isReady)
			{
				return TeleportCancel;
			}
			else
			{
				return TeleportCancel_Cooldown;
			}
		}

		return Summoner_Empty;
	}
};
