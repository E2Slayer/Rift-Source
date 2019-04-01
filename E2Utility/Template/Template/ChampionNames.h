#pragma once
#include "../../Include/SDK Extensions.h"
#include <string.h>

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

};
