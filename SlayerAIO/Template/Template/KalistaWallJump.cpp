#include "stdafx.h"
#include "KalistaWallJump.h"


//std::vector<Vector3> wallJumpSpots;
std::map<Vector3, Vector3> wallJumpSpots;



std::map<Vector3, Vector3> WallJump::GetWallJumpSpots()
{
	return wallJumpSpots;
}

void WallJump::WallJumpMode()
{

}

/*
void Kalista::Draw(void * UserData)
{
	//auto Enemies{ pSDK->EntityManager->GetEnemyHeroes() };
	for (auto &[start, end] : wallJumpSpots)
	{
		Vector3 startPos{ start };
		Vector3 endPos{ end };
		SDKVECTOR direction{ 100.f, 100.f, 100.f };

		if (Player.Distance(&startPos) < 10000.0f )
		{
			//Vector3 pos{ Enemy->GetPosition() };

			if (startPos.IsValid() && startPos.IsOnScreen())
			{
				//We get the screen position and offset it a little so it doesnt draw over the above text
				//Vector2 screenPos{ Renderer::WorldToScreen(startPos) };
				//screenPos.y -= 20.0f;

				//Draw::Text(NULL, &screenPos, "In AA Range", "Arial Narrow", &Color::White, 20, 6);

				Draw::Circle(&startPos, 100.0f, &Color::White, 2, &direction);
			}
		}
	}

	//Draw::Circle(&PlayerLocation, spellQ.Range, &Color::White, 2, &direction);
}

*/

void WallJump::WallJumpSpotsInitializer()
{
	wallJumpSpots.clear(); //clear the list


	auto pos0 = Vector3(8252.000000f, 51.129997f, 2924.000000f); //blue side Golem
	auto pos1 = Vector3(8222.000000f, 51.648384f, 3158.000000f);
	wallJumpSpots.emplace(pos0, pos1);
	


	pos0 = Vector3(9022.000000f, 52.581631f, 4458.000000f); //Drake 
	//pos1 = Vector3(9322.000000f, -71.240601f, 4458.000000f);
	//pos1 = Vector3(9315.400391f, -71.240601f, 4456.319336f);
	pos1 = Vector3(9300.000000f, -71.240601f, 4416.000000f);
	wallJumpSpots.emplace(pos0, pos1);



	pos0 = Vector3(11766.542969f, -71.240601f, 4611.030762f); //Bot side river wall
	pos1 = Vector3(11946.282227f, 51.729401f, 4787.229004f);
	wallJumpSpots.emplace(pos0, pos1);


	pos0 = Vector3(11394.136719f, -71.240601f, 5069.042969f); //Bot side river wall 2
	pos1 = Vector3(11575.833008f, 51.807945f, 5228.361328f);

	wallJumpSpots.emplace(pos0, pos1);


	pos0 = Vector3(12275.041992f, 51.729401f, 5470.906738f); //bot side river wall bush
	pos1 = Vector3(12024.841797f, 54.640923f, 5437.476074f);

	wallJumpSpots.emplace(pos0, pos1);

	pos0 = Vector3(10942.713867f, 52.203835f, 7505.490723f); //purple side blue above
	pos1 = Vector3(10972.000000f, 51.723755f, 7208.000000f);
	wallJumpSpots.emplace(pos0, pos1);

	pos0 = Vector3(10792.714844f, 51.722603f, 7208.470703f); //purple side blue above 2 
	pos1 = Vector3(10642.575195f, 51.879730f, 7458.446777f);

	wallJumpSpots.emplace(pos0, pos1);

	pos0 = Vector3(11560.917969f, 62.874905f, 8672.025391f); //purple side wolves right side wall
	pos1 = Vector3(11817.746094f, 50.306786f, 8817.148438f);

	wallJumpSpots.emplace(pos0, pos1);


	pos0 = Vector3(11256.596680f, 52.212971f, 7878.811035f); //purple side wolves bottom side wall
	pos1 = Vector3(11128.000000f, 61.934792f, 8172.000000f);
	wallJumpSpots.emplace(pos0, pos1);


	pos0 = Vector3(8075.475586f, -71.240601f, 6164.311035f); //purple side wolves bottom side wall2
	pos1 = Vector3(7872.000000f, 51.033291f, 6008.000000f);

	wallJumpSpots.emplace(pos0, pos1);


	pos0 = Vector3(8159.947266f, -71.240601f, 6109.645508f); // bottom of mid bush to blue side red
	pos1 = Vector3(8151.123535f, 52.328144f, 5830.718262f);

	wallJumpSpots.emplace(pos0, pos1);


	//pos0 = Vector3(7663.270020f, -6.317760f, 6497.771973f); // bottom of mid bush to blue side red2
	//pos1 = Vector3(7457.260254f, 52.451302f, 6272.562012f);

	//wallJumpSpots.emplace(pos0, pos1);

	pos0 = Vector3(7672.970703f, -13.797449f, 6467.279785f); // bottom of mid bush to blue side red2
	pos1 = Vector3(7641.618164f, 52.451302f, 6179.463867f);
	wallJumpSpots.emplace(pos0, pos1);

	pos0 = Vector3(4124.000000f, 50.628681f, 7980.000000f); //blue side blue thick wall
	pos1 = Vector3(4392.059082f, 48.786247f, 8145.841797f);

	wallJumpSpots.emplace(pos0, pos1);



	/*
	auto pos0 = Vector3(6393.7299804688f, 8341.7451171875f, -63.87451171875f);
	auto pos1 = Vector3(6612.1625976563f, 8574.7412109375f, 56.018413543701f);
	wallJumpSpots.emplace(pos0, pos1);
	auto pos2 = Vector3(7041.7885742188f, 8810.1787109375f, 0.0f);
	auto pos3 = Vector3(7296.0341796875f, 9056.4638671875f, 55.610824584961f);
	wallJumpSpots.emplace(pos2, pos3);
	auto pos4 = Vector3(4546.0258789063f, 2548.966796875f, 54.257415771484f);
	auto pos5 = Vector3(4185.0786132813f, 2526.5520019531f, 109.35539245605f);
	wallJumpSpots.emplace(pos4, pos5);
	auto pos6 = Vector3(2805.4074707031f, 6140.130859375f, 55.182941436768f);
	auto pos7 = Vector3(2614.3215332031f, 5816.9438476563f, 60.193073272705f);
	wallJumpSpots.emplace(pos6, pos7);
	auto pos8 = Vector3(6696.486328125f, 5377.4013671875f, 61.310482025146f);
	auto pos9 = Vector3(6868.6918945313f, 5698.1455078125f, 55.616455078125f);
	wallJumpSpots.emplace(pos8, pos9);
	auto pos10 = Vector3(1677.9854736328f, 8319.9345703125f, 54.923847198486f);
	auto pos11 = Vector3(1270.2786865234f, 8286.544921875f, 50.334892272949f);
	wallJumpSpots.emplace(pos10, pos11);
	auto pos12 = Vector3(2809.3254394531f, 10178.6328125f, -58.759708404541f);
	auto pos13 = Vector3(2553.8962402344f, 9974.4677734375f, 53.364395141602f);
	wallJumpSpots.emplace(pos12, pos13);
	auto pos14 = Vector3(5102.642578125f, 10322.375976563f, -62.845260620117f);
	auto pos15 = Vector3(5483.0f, 10427.0f, 54.5009765625f);
	wallJumpSpots.emplace(pos14, pos15);
	auto pos16 = Vector3(6000.2373046875f, 11763.544921875f, 39.544124603271f);
	auto pos17 = Vector3(6056.666015625f, 11388.752929688f, 54.385917663574f);
	wallJumpSpots.emplace(pos16, pos17);
	auto pos18 = Vector3(1742.34375f, 7647.1557617188f, 53.561042785645f);
	auto pos19 = Vector3(1884.5321044922f, 7995.1459960938f, 54.930736541748f);
	wallJumpSpots.emplace(pos18, pos19);
	auto pos20 = Vector3(3319.087890625f, 7472.4760742188f, 55.027889251709f);
	auto pos21 = Vector3(3388.0522460938f, 7101.2568359375f, 54.486026763916f);
	wallJumpSpots.emplace(pos20, pos21);
	auto pos22 = Vector3(3989.9423828125f, 7929.3422851563f, 51.94282913208f);
	auto pos23 = Vector3(3671.623046875f, 7723.146484375f, 53.906265258789f);
	wallJumpSpots.emplace(pos22, pos23);
	auto pos24 = Vector3(4936.8452148438f, 10547.737304688f, -63.064865112305f);
	auto pos25 = Vector3(5156.7397460938f, 10853.216796875f, 52.951190948486f);
	wallJumpSpots.emplace(pos24, pos25);
	auto pos26 = Vector3(5028.1235351563f, 10115.602539063f, -63.082695007324f);
	auto pos27 = Vector3(5423.0f, 10127.0f, 55.15357208252f);
	wallJumpSpots.emplace(pos26, pos27);
	auto pos28 = Vector3(6035.4819335938f, 10973.666015625f, 53.918266296387f);
	auto pos29 = Vector3(6385.4013671875f, 10827.455078125f, 54.63500213623f);
	wallJumpSpots.emplace(pos28, pos29);
	auto pos30 = Vector3(4747.0625f, 11866.421875f, 41.584358215332f);
	auto pos31 = Vector3(4743.23046875f, 11505.842773438f, 51.196254730225f);
	wallJumpSpots.emplace(pos30, pos31);
	auto pos32 = Vector3(6749.4487304688f, 12980.83984375f, 44.903495788574f);
	auto pos33 = Vector3(6701.4965820313f, 12610.278320313f, 52.563804626465f);
	wallJumpSpots.emplace(pos32, pos33);
	auto pos34 = Vector3(3114.1865234375f, 9420.5078125f, -42.718975067139f);
	auto pos35 = Vector3(2757.0f, 9255.0f, 53.77322769165f);
	wallJumpSpots.emplace(pos34, pos35);
	auto pos36 = Vector3(2786.8354492188f, 9547.8935546875f, 53.645294189453f);
	auto pos37 = Vector3(3002.0930175781f, 9854.39453125f, -53.198081970215f);
	wallJumpSpots.emplace(pos36, pos37);
	auto pos38 = Vector3(3803.9470214844f, 7197.9018554688f, 53.730079650879f);
	auto pos39 = Vector3(3664.1088867188f, 7543.572265625f, 54.18229675293f);
	wallJumpSpots.emplace(pos38, pos39);
	auto pos40 = Vector3(2340.0886230469f, 6387.072265625f, 60.165466308594f);
	auto pos41 = Vector3(2695.6096191406f, 6374.0634765625f, 54.339839935303f);
	wallJumpSpots.emplace(pos40, pos41);
	auto pos42 = Vector3(3249.791015625f, 6446.986328125f, 55.605854034424f);
	auto pos43 = Vector3(3157.4558105469f, 6791.4458007813f, 54.080295562744f);
	wallJumpSpots.emplace(pos42, pos43);
	auto pos44 = Vector3(3823.6242675781f, 5923.9130859375f, 55.420352935791f);
	auto pos45 = Vector3(3584.2561035156f, 6215.4931640625f, 55.6123046875f);
	wallJumpSpots.emplace(pos44, pos45);
	auto pos46 = Vector3(5796.4809570313f, 5060.4116210938f, 51.673671722412f);
	auto pos47 = Vector3(5730.3081054688f, 5430.1635742188f, 54.921173095703f);
	wallJumpSpots.emplace(pos46, pos47);
	auto pos48 = Vector3(6007.3481445313f, 4985.3803710938f, 51.673641204834f);
	auto pos49 = Vector3(6388.783203125f, 4987.0f, 51.673400878906f);
	wallJumpSpots.emplace(pos48, pos49);
	auto pos50 = Vector3(7040.9892578125f, 3964.6728515625f, 57.192108154297f);
	auto pos51 = Vector3(6668.0073242188f, 3993.609375f, 51.671356201172f);
	wallJumpSpots.emplace(pos50, pos51);
	auto pos52 = Vector3(7763.541015625f, 3294.3481445313f, 54.872283935547f);
	auto pos53 = Vector3(7629.421875f, 3648.0581054688f, 56.908012390137f);
	wallJumpSpots.emplace(pos52, pos53);
	auto pos54 = Vector3(4705.830078125f, 9440.6572265625f, -62.586814880371f);
	auto pos55 = Vector3(4779.9809570313f, 9809.9091796875f, -63.09009552002f);
	wallJumpSpots.emplace(pos54, pos55);
	auto pos56 = Vector3(4056.7907714844f, 10216.12109375f, -63.152275085449f);
	auto pos57 = Vector3(3680.1550292969f, 10182.296875f, -63.701038360596f);
	wallJumpSpots.emplace(pos56, pos57);
	auto pos58 = Vector3(4470.0883789063f, 12000.479492188f, 41.59789276123f);
	auto pos59 = Vector3(4232.9799804688f, 11706.015625f, 49.295585632324f);
	wallJumpSpots.emplace(pos58, pos59);
	auto pos60 = Vector3(5415.5708007813f, 12640.216796875f, 40.682685852051f);
	auto pos61 = Vector3(5564.4409179688f, 12985.860351563f, 41.373748779297f);
	wallJumpSpots.emplace(pos60, pos61);
	auto pos62 = Vector3(6053.779296875f, 12567.381835938f, 40.587882995605f);
	auto pos63 = Vector3(6045.4555664063f, 12942.313476563f, 41.211364746094f);
	wallJumpSpots.emplace(pos62, pos63);
	auto pos64 = Vector3(4454.66015625f, 8057.1313476563f, 42.799690246582f);
	auto pos65 = Vector3(4577.8681640625f, 7699.3686523438f, 53.31339263916f);
	wallJumpSpots.emplace(pos64, pos65);
	auto pos66 = Vector3(7754.7700195313f, 10449.736328125f, 52.890430450439f);
	auto pos67 = Vector3(8096.2885742188f, 10288.80078125f, 53.66955947876f);
	wallJumpSpots.emplace(pos66, pos67);
	auto pos68 = Vector3(7625.3139648438f, 9465.7001953125f, 55.008113861084f);
	auto pos69 = Vector3(7995.986328125f, 9398.1982421875f, 53.530490875244f);
	wallJumpSpots.emplace(pos68, pos69);
	auto pos70 = Vector3(9767.0f, 8839.0f, 53.044532775879f);
	auto pos71 = Vector3(9653.1220703125f, 9174.7626953125f, 53.697280883789f);
	wallJumpSpots.emplace(pos70, pos71);
	auto pos72 = Vector3(10775.653320313f, 7612.6943359375f, 55.35241317749f);
	auto pos73 = Vector3(10665.490234375f, 7956.310546875f, 65.222145080566f);
	wallJumpSpots.emplace(pos72, pos73);
	auto pos74 = Vector3(10398.484375f, 8257.8642578125f, 66.200691223145f);
	auto pos75 = Vector3(10176.104492188f, 8544.984375f, 64.849853515625f);
	wallJumpSpots.emplace(pos74, pos75);
	auto pos76 = Vector3(11198.071289063f, 8440.4638671875f, 67.641044616699f);
	auto pos77 = Vector3(11531.436523438f, 8611.0087890625f, 53.454048156738f);
	wallJumpSpots.emplace(pos76, pos77);
	auto pos78 = Vector3(11686.700195313f, 8055.9624023438f, 55.458232879639f);
	auto pos79 = Vector3(11314.19140625f, 8005.4946289063f, 58.438243865967f);
	wallJumpSpots.emplace(pos78, pos79);
	auto pos80 = Vector3(10707.119140625f, 7335.1752929688f, 55.350387573242f);
	auto pos81 = Vector3(1069.03f, 6943.0f, 54.870254516602f);
	wallJumpSpots.emplace(pos80, pos81);
	auto pos82 = Vector3(10395.380859375f, 6938.5009765625f, 54.869094848633f);
	auto pos83 = Vector3(10454.955078125f, 7316.7041015625f, 55.308219909668f);
	wallJumpSpots.emplace(pos82, pos83);
	auto pos84 = Vector3(10358.5859375f, 6677.1704101563f, 54.86909866333f);
	auto pos85 = Vector3(10070.067382813f, 6434.0815429688f, 55.294486999512f);
	wallJumpSpots.emplace(pos84, pos85);
	auto pos86 = Vector3(11161.98828125f, 5070.447265625f, 53.730766296387f);
	auto pos87 = Vector3(10783.0f, 4965.0f, -63.57177734375f);
	wallJumpSpots.emplace(pos86, pos87);
	auto pos88 = Vector3(11167.081054688f, 4613.9829101563f, -62.898971557617f);
	auto pos89 = Vector3(11501.0f, 4823.0f, 54.571090698242f);
	wallJumpSpots.emplace(pos88, pos89);
	auto pos90 = Vector3(11743.823242188f, 4387.4672851563f, 52.005855560303f);
	auto pos91 = Vector3(11379.0f, 4239.0f, -61.565242767334f);
	wallJumpSpots.emplace(pos90, pos91);
	auto pos92 = Vector3(10388.120117188f, 4267.1796875f, -63.61775970459f);
	auto pos93 = Vector3(10033.036132813f, 4147.1669921875f, -60.332069396973f);
	wallJumpSpots.emplace(pos92, pos93);
	auto pos94 = Vector3(8964.7607421875f, 4214.3833007813f, -63.284225463867f);
	auto pos95 = Vector3(8569.0f, 4241.0f, 55.544258117676f);
	wallJumpSpots.emplace(pos94, pos95);
	auto pos96 = Vector3(5554.8657226563f, 4346.75390625f, 51.680099487305f);
	auto pos97 = Vector3(5414.0634765625f, 4695.6860351563f, 51.611679077148f);
	wallJumpSpots.emplace(pos96, pos97);
	auto pos98 = Vector3(7311.3393554688f, 10553.6015625f, 54.153884887695f);
	auto pos99 = Vector3(6938.5209960938f, 10535.8515625f, 54.441242218018f);
	wallJumpSpots.emplace(pos98, pos99);
	auto pos100 = Vector3(7669.353515625f, 5960.5717773438f, -64.488967895508f);
	auto pos101 = Vector3(7441.2182617188f, 5761.8989257813f, 54.347793579102f);
	wallJumpSpots.emplace(pos100, pos101);
	auto pos102 = Vector3(7949.65625f, 2647.0490722656f, 54.276401519775f);
	auto pos103 = Vector3(7863.0063476563f, 3013.7814941406f, 55.178623199463f);
	wallJumpSpots.emplace(pos102, pos103);
	auto pos104 = Vector3(8698.263671875f, 3783.1169433594f, 57.178703308105f);
	auto pos105 = Vector3(9041.0f, 3975.0f, -63.242683410645f);
	wallJumpSpots.emplace(pos104, pos105);
	auto pos106 = Vector3(9063.0f, 3401.0f, 68.192077636719f);
	auto pos107 = Vector3(9275.0751953125f, 3712.8935546875f, -63.257461547852f);
	wallJumpSpots.emplace(pos106, pos107);
	auto pos108 = Vector3(12064.340820313f, 6424.11328125f, 54.830627441406f);
	auto pos109 = Vector3(12267.9375f, 6742.9453125f, 54.83561706543f);
	wallJumpSpots.emplace(pos108, pos109);
	auto pos110 = Vector3(12797.838867188f, 5814.9653320313f, 58.281986236572f);
	auto pos111 = Vector3(12422.740234375f, 5860.931640625f, 54.815074920654f);
	wallJumpSpots.emplace(pos110, pos111);
	auto pos112 = Vector3(11913.165039063f, 5373.34375f, 54.050819396973f);
	auto pos113 = Vector3(11569.1953125f, 5211.7143554688f, 57.787326812744f);
	wallJumpSpots.emplace(pos112, pos113);
	auto pos114 = Vector3(9237.3603515625f, 2522.8937988281f, 67.796775817871f);
	auto pos115 = Vector3(9344.2041015625f, 2884.958984375f, 65.500213623047f);
	wallJumpSpots.emplace(pos114, pos115);
	auto pos116 = Vector3(7324.2783203125f, 1461.2199707031f, 52.594970703125f);
	auto pos117 = Vector3(7357.3852539063f, 1837.4309082031f, 54.282878875732f);
	wallJumpSpots.emplace(pos116, pos117);
	*/
	//wallJumpSpots.push_back();
}