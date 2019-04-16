#pragma once
#include <string>


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

	"Zyra",

	"PracticeTool_TargetDummy"

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


const std::string SmallSummonerSpellsList[]
{
	"Summoner_Dash",
	"Summoner_Empty",
	"Summoner_flashPerksHextechFlashtraption",
	"Summoner_flashPerksHextechFlashtraptionBlue",
	"Summoner_Mark",
	"SummonerBarrier",
	"SummonerBoost",
	"SummonerClairvoyance",
	"SummonerDot",
	"SummonerExhaust",
	"SummonerFlash",
	"SummonerFlashPerksHextechFlashtraptionV2",
	"SummonerHaste",
	"SummonerHeal",
	"SummonerMana",
	"SummonerSmite",
	"SummonerSnowball",
	"SummonerTeleport"
};


class SpriteImageLoader
{
public:
	static unsigned int GetChampionIMG(const char* name)
	{
		const int ArraySize = (sizeof(ChampionNameList) / sizeof(*ChampionNameList)) ;
		for (int i = 0; i < ArraySize; ++i)
		{
			if (_stricmp(name, ChampionNameList[i].c_str()) == 0)
			{
				return i + 145;
			}
		}


		return 233; //233 is default
	}

	static unsigned int GetSummonerSpellIMG(const char* name)
	{
		const int ArraySize = (sizeof(SummonerSpellsList) / sizeof(*SummonerSpellsList));
		for (int i = 0; i < ArraySize; ++i)
		{
			if (_stricmp(name, SummonerSpellsList[i].c_str()) == 0)
			{
				return i + 290;
			}
		}


		return 233; //233 is default
	}

	static unsigned int GetSmallSummonerSpellIMG(const char* name, bool IsOnCooldown)
	{
		const int ArraySize = (sizeof(SmallSummonerSpellsList) / sizeof(*SmallSummonerSpellsList))*2;
		for (int i = 0; i < ArraySize; ++i)
		{
			//SdkUiConsoleWrite("inside check %s", ChampionNameList[i].c_str());
			if (_stricmp(name, SmallSummonerSpellsList[i].c_str()) == 0)
			{
				//SdkUiConsoleWrite("number : %s %d", name, i);
				return IsOnCooldown ? ((i * 2) + 107) +1 : ((i * 2) + 107);
			}
		}
		return 109; //109 is empty one
	}
};