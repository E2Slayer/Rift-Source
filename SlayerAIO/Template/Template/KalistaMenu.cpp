#include "stdafx.h"
#include "KalistaMenu.h"


const std::vector<std::string> hitchanceList{ "Low", "Medium", "High", "Very High" };

const std::vector<std::string> ColorMenuList{ "Black", "Blue", "Cyan", "DarkBlue", "DarkGreen", "Green", "Grey", "Magenta", "Orange", "Purple", "Red", "White", "Yellow" };


bool bKalistaExpanded;

bool bComboExpanded;
bool bHarassExpanded;
bool bLaneClearExpanded;
bool bJungleClearExpanded;

bool bSettingsExpanded;
bool bUseEBeforeDieExpanded;
bool bJungleEExpanded;
bool bExploitExpanded;
bool bKillStealExpanded;

bool bDrawExpanded;


void KalistaMenu::Init()
{
	pSDK->EventHandler->RegisterCallback(CallbackEnum::Overlay, KalistaMenu::DrawMenu);
}


///Your menu settings go here
void KalistaMenu::DrawMenu(void * UserData)
{


	UNREFERENCED_PARAMETER(UserData);

	//bool bKalistaExpanded = true;
	Menu::Tree("Kalista", "Kalista.Main", &bKalistaExpanded, []()
	{
		//SdkUiText("MataView 1.1 [Beta] - E2Slayer");
		SdkUiColoredText(&Color::Green, "Kalista 1.4 (Beta)");

		//bool bComboExpanded = false;
		Menu::Tree("Combo", "Kalista.Combo", &bComboExpanded, []()
		{
			//SdkUiColoredText(&Color::Green, "============Q Settings=============");

			//Menu::BulletText("hi");

			Menu::Checkbox("Use Q", "Kalista.ComboQ", true);

			Menu::DropList("Q HitChance", "Kalista.ComboQHit", hitchanceList, 2);



			Menu::Checkbox("Use E", "Kalista.ComboE", true);

			Menu::Checkbox("Reset E when minion is Killable and Enemy has spear(s)", "Kalista.ComboEMinion", true);


			Menu::Checkbox("Chasing Enemy by Using Minions", "Kalista.ComboChase", true);
			//Menu::SliderInt("Chase Range", "Kalista.ComboChaseRange", 1500, 1000, 2000);
			//Menu::Checkbox("Use E on Minions While Chasing", "Kalista.ComboE", true);


		});


		//bool bHarassExpanded = false;
		Menu::Tree("Harass", "Kalista.Harass", &bHarassExpanded, []()
		{

			Menu::Checkbox("Use Q", "Kalista.HarassQ", true);

			Menu::DropList("Q HitChance", "Kalista.HarassQHit", hitchanceList, 2);

			Menu::SliderInt("Q Minimum Mana", "Kalista.HarassQMana", 40, 0, 100);




			Menu::Checkbox("Use E", "Kalista.HarassE", true);

			Menu::Checkbox("Reset E when minion is Killable and Enemy has spear(s)", "Kalista.HarassEMinion", true);

			/*
			Menu::Checkbox("Use E when Enemy is leaving E Range", "Kalista.HarassEleaving", true);

			Menu::SliderInt("^-> Minimum Spear to use", "Kalista.ComboChase", 3, 1, 10);
			*/

		});

		//bool bLaneClearExpanded = false;
		Menu::Tree("LaneClear", "Kalista.LaneClear", &bLaneClearExpanded, []()
		{

			//Menu::Checkbox("Use Q", "Kalista.LaneClearQ", true);
			//SdkUiText("LaneClear Q will be added when Collision is implemented in SDK");
			Menu::Checkbox("Use E", "Kalista.LaneClearE", true);
			Menu::SliderInt("^-> Minimum Killable Minions ", "Kalista.LaneClearEMinimum", 2, 1, 10);


		});



		//bool bJungleClearExpanded = false;
		Menu::Tree("JungleClear", "Kalista.JungleClear", &bJungleClearExpanded, []()
		{

			Menu::Checkbox("Use Q", "Kalista.JungleClearQ", true);
			Menu::SliderInt("^-> Minimum Mana", "Kalista.JungleClearQMana", 40, 0, 100);

			Menu::Checkbox("Use E", "Kalista.JungleClearE", true);



		});


		/*
		bool bFleeExpanded = false;
		Menu::Tree("Flee", "Kalista.Flee", &bFleeExpanded, []()
		{

			Menu::Checkbox("Use Q to do WallJump", "Kalista.WallJump", true);
			//Menu::SliderInt("^-> Minimum Mana", "Kalista.JungleClearQMana", 40, 0, 100);

			//Menu::Checkbox("Use E", "Kalista.JungleClearE", true);



		});
		*/

		//bool bSettingsExpanded = false;
		Menu::Tree("Misc", "Kalista.misc", &bSettingsExpanded, []()
		{
			//Menu::Checkbox("Use ", "Kalista.MiscE", true);


			Menu::Checkbox("Use R to Save Ally", "Kalista.miscR", true);

			Menu::SliderInt("^-> if Ally HP below # %", "Kalista.miscRHP", 20, 1, 100);

			Menu::Checkbox("Auto E when you can't kill a minion with AA", "Kalista.miscEMinion", true);

			Menu::Checkbox("Always Save Mana For E", "Kalista.miscSaveManaE", true);

			Menu::Checkbox("Auto W to Dragon", "Kalista.autoWDrake", true);
			Menu::Checkbox("Auto W to Baron", "Kalista.autoWBaron", true);


			//bool bUseEBeforeDieExpanded = false;
			Menu::Tree("Use E Before Kalista Dies", "Kalista.UseEBeforeDie", &bUseEBeforeDieExpanded, []()
			{
				Menu::Checkbox("Use E Before Kalista Dies", "Kalista.EBeforeDie", true);

				Menu::SliderInt("^-> Minimum Health to Cast E", "Kalista.EBeforeDieMinimumHealth", 10, 1, 100);

				Menu::SliderFloat("^-> Incoming Damage Calculation Duration (Second)", "Kalista.EBeforeDieIncDuration", 1.0f, 0.1f, 2.0f);
				Menu::BulletText("Higher Duration Number = Use Earlier E");
				Menu::BulletText("Lower Durtaion Number = Use E more accurate But may die with not using E");
				Menu::BulletText("Good range is 0.5s to 1.0s");
				//Menu::SliderFloat

			});

			//Menu::SliderInt("t1", "Kalista.t1", 20, 1, 1000);
			//Menu::SliderInt("t2", "Kalista.t2", 20, 1, 1000);



			//Menu::Checkbox("Block Casting Q while Jumping", "Kalista.miscBlockJumpingQ", false);

			//bool bJungleEExpanded = false;
			Menu::Tree("Always E on Jungle Mobs", "Kalista.JungleE", &bJungleEExpanded, []()
			{
				Menu::Checkbox("Always Use E on Dragon", "Kalista.JungleEDragon", true);
				Menu::Checkbox("Always Use E on Baron", "Kalista.JungleEBaron", true);
				Menu::Checkbox("Always Use E on Rift Herald", "Kalista.JungleERift", true);
				Menu::Checkbox("Always Use E on Blue", "Kalista.JungleEBlue", true);
				Menu::Checkbox("Always Use E on Red", "Kalista.JungleERed", true);
				Menu::Checkbox("Always Use E on Scuttler Crab", "Kalista.JungleECrab", true);


			});



			//bool bExploitExpanded = false;
			Menu::Tree("Fly Exploit", "Kalista.ExploitE", &bExploitExpanded, []()
			{
				Menu::Checkbox("Use Fly Exploit", "Kalista.Exploit", true);
				Menu::SliderInt("^-> Fly Adjustment", "Kalista.ExploitAdjustment", 150, 100, 250);
				//SdkUiColoredText(&Color::White, "^-> Higher number = Faster jumps but More Buggy jumps ");
				Menu::BulletText("Higher number = Faster jumps but More Buggy jumps");
				Menu::BulletText("Exploit only works if your Attack Speed is higher than 2.0 ");
				Menu::BulletText("Ideal Range of the exploit adjustment is around 150-180");
				//SdkUiColoredText(&Color::Purple, "^-> Ideal Range is around 150-180");


			});


			bool bKillStealExpanded = false;
			Menu::Tree("KillSteal", "Kalista.KillSteal", &bKillStealExpanded, []()
			{
				Menu::Checkbox("Use Q to Killsteal", "Kalista.KillStealQ", true);
				Menu::Checkbox("Use E to Killsteal", "Kalista.KillStealE", true);
			});





			//Menu::Checkbox("Use E", "Kalista.JungleClearE", true);



		//	Menu::SliderInt("Font Size", "Ability.FontSize", 28, 21, 50); //&_g_CheckTeamAbilities.iFontSize, 21.0f, 50.0f, "%.0f", NULL);
		//	SDKVECTOR defaultColor = { 255, 255, 255 };
		//	Menu::ColorPicker("Color Picker", "Ability.Color", defaultColor);

			//MakeMenu::ColorPicker()
			//SdkUiColorPicker("Color Pick", &_g_CheckTeamAbilities.m_CurrentColor, NULL);
			//SdkUiText("Note: The settings work after pressed the Save Settings");
			//SdkUiColoredText(&_g_ColorGreen, "Note: The settings will apply \nAfter pressed the Save Settings");


		});





		//bool bDrawExpanded = false;
		Menu::Tree("Drawings", "Kalista.draw", &bDrawExpanded, []()
		{
			//Menu::Checkbox("Use ", "Kalista.MiscE", true);


			Menu::Checkbox("Draw Q Range", "Kalista.drawQ", true);
			Menu::DropList("^-> Q Color", "Kalista.drawQColor", ColorMenuList, 11);

			Menu::Checkbox("Draw W Range", "Kalista.drawW", true);
			Menu::DropList("^-> W Color", "Kalista.drawWColor", ColorMenuList, 11);

			Menu::Checkbox("Draw E Range", "Kalista.drawE", true);
			Menu::DropList("^-> E Color", "Kalista.drawEColor", ColorMenuList, 11);

			Menu::Checkbox("Draw R Range", "Kalista.drawR", true);
			Menu::DropList("^-> R Color", "Kalista.drawRColor", ColorMenuList, 11);

			Menu::Checkbox("Show E Damage on Champion - Percentage", "Kalista.drawEDmgPctHero", true);
			Menu::Checkbox("Show E Damage on Jungle Mobs- Percentage", "Kalista.drawEDmgPctJungle", true);



			Menu::Checkbox("Show E Damage - Draw on HPbar (Only Works Champions)", "Kalista.drawEDmgHPBar", true);

			//Menu::Checkbox("Draw R Range", "Kalista.drawR", true);



			//Menu::DropList("Draw E Damage", "Kalista.drawEDMG", )

			//MakeMenu::ColorPicker()
			//SdkUiColorPicker("Color Pick", &_g_CheckTeamAbilities.m_CurrentColor, NULL);
			//SdkUiText("Note: The settings work after pressed the Save Settings");
			//SdkUiColoredText(&_g_ColorGreen, "Note: The settings will apply \nAfter pressed the Save Settings");


		});




	});




}