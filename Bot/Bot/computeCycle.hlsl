

Texture2D<float4> ScreenTex : register(t0);
RWByteAddressBuffer BufferOut : register(u0);

float fuzzyMatchGroup4(float4 color1, float4 color2, float4 color3, float4 color4, unsigned int HTMLMatch1, unsigned int HTMLMatch2, unsigned int HTMLMatch3, unsigned int HTMLMatch4)
{
	float4 matchColor1 = float4(float(HTMLMatch1 / 65536) / 255.0, float((HTMLMatch1 / 256) % 256) / 255.0, float(HTMLMatch1 % 256) / 255.0, 0.0f);
	float4 matchColor2 = float4(float(HTMLMatch2 / 65536) / 255.0, float((HTMLMatch2 / 256) % 256) / 255.0, float(HTMLMatch2 % 256) / 255.0, 0.0f);
	float4 matchColor3 = float4(float(HTMLMatch3 / 65536) / 255.0, float((HTMLMatch3 / 256) % 256) / 255.0, float(HTMLMatch3 % 256) / 255.0, 0.0f);
	float4 matchColor4 = float4(float(HTMLMatch4 / 65536) / 255.0, float((HTMLMatch4 / 256) % 256) / 255.0, float(HTMLMatch4 % 256) / 255.0, 0.0f);
	
	//no div by zero
	color1 += 0.001f;
	color2 += 0.001f;
	color3 += 0.001f;
	color4 += 0.001f;
	matchColor1 += 0.001f;
	matchColor2 += 0.001f;
	matchColor3 += 0.001f;
	matchColor4 += 0.001f;

	float totalHueDiff = 0;
	totalHueDiff += abs(color1.r / color1.g - matchColor1.r / matchColor1.g);
	totalHueDiff += abs(color1.r / color1.b - matchColor1.r / matchColor1.b);
	totalHueDiff += abs(color1.g / color1.b - matchColor1.g / matchColor1.b);

	totalHueDiff += abs(color2.r / color2.g - matchColor2.r / matchColor2.g);
	totalHueDiff += abs(color2.r / color2.b - matchColor2.r / matchColor2.b);
	totalHueDiff += abs(color2.g / color2.b - matchColor2.g / matchColor2.b);

	totalHueDiff += abs(color3.r / color3.g - matchColor3.r / matchColor3.g);
	totalHueDiff += abs(color3.r / color3.b - matchColor3.r / matchColor3.b);
	totalHueDiff += abs(color3.g / color3.b - matchColor3.g / matchColor3.b);

	totalHueDiff += abs(color4.r / color4.g - matchColor4.r / matchColor4.g);
	totalHueDiff += abs(color4.r / color4.b - matchColor4.r / matchColor4.b);
	totalHueDiff += abs(color4.g / color4.b - matchColor4.g / matchColor4.b);

	float ratioDiff = 0;
	float ratio1 = length(color1) / length(matchColor1);
	float ratio2 = length(color2) / length(matchColor2);
	float ratio3 = length(color3) / length(matchColor3);
	float ratio4 = length(color4) / length(matchColor4);

	ratioDiff += abs(ratio2 - ratio1);
	ratioDiff += abs(ratio3 - ratio1);
	ratioDiff += abs(ratio4 - ratio1);

	return totalHueDiff + ratioDiff*0;
}

[numthreads(32, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	// struct order in C++
	int windowPosX = BufferOut.Load(0);
	int windowPosY = BufferOut.Load(4);
	int playButtonFound = 0; // BufferOut.Load(8);
	int playButtonX = BufferOut.Load(12);
	int playButtonY = BufferOut.Load(16);
	int hellButtonFound = 0; // BufferOut.Load(20);
	int hellButtonX = BufferOut.Load(24);
	int hellButtonY = BufferOut.Load(28);
	int townWaypointFound = BufferOut.Load(32);
	int townWaypointX = BufferOut.Load(36);
	int townWaypointY = BufferOut.Load(40);
	int lostCityButtonFound = BufferOut.Load(44);
	int lostCityButtonX = BufferOut.Load(48);
	int lostCityButtonY = BufferOut.Load(52);
	int tunnelsEntranceFound = 0; // BufferOut.Load(56);
	int tunnelsEntranceX = BufferOut.Load(60);
	int tunnelsEntranceY = BufferOut.Load(64);
	int mercFound = BufferOut.Load(68);
	int mercX = BufferOut.Load(72);
	int mercY = BufferOut.Load(76);
	int plagueBearerFound = BufferOut.Load(80);
	int plagueBearerX = BufferOut.Load(84);
	int plagueBearerY = BufferOut.Load(88);
	int embalmedFound = BufferOut.Load(92);
	int embalmedX = BufferOut.Load(96);
	int embalmedY = BufferOut.Load(100);
	int invaderFound = BufferOut.Load(104);
	int invaderX = BufferOut.Load(108);
	int invaderY = BufferOut.Load(112);
	int horrorMageFound = BufferOut.Load(116);
	int horrorMageX = BufferOut.Load(120);
	int horrorMageY = BufferOut.Load(124);
	int itemFound = 0; // BufferOut.Load(128);
	int itemX = BufferOut.Load(142);
	int itemY = BufferOut.Load(136);
	int chestFound = BufferOut.Load(140);
	int chestX = BufferOut.Load(144);
	int chestY = BufferOut.Load(148);
	int mercHealth = BufferOut.Load(152);
	int playerHealth = BufferOut.Load(156);
	int playerMP = BufferOut.Load(160);
	int potionSlot1Type = BufferOut.Load(164);
	int potionSlot2Type = BufferOut.Load(168);
	int potionSlot3Type = BufferOut.Load(172);
	int potionSlot4Type = BufferOut.Load(176);
	int lostCityEdgeLeft = BufferOut.Load(180);
	int lostCityEdgeRight = BufferOut.Load(184);
	int lostCityEdgeTop = BufferOut.Load(188);
	int lostCityEdgeBottom = BufferOut.Load(192);
	int tunnelsEdgeLeft = BufferOut.Load(196);
	int tunnelsEdgeRight = BufferOut.Load(200);
	int tunnelsEdgeTop = BufferOut.Load(204);
	int tunnelsEdgeBottom = BufferOut.Load(208);
	int saveAndQuitButtonFound = 0; // BufferOut.Load(212);
	int saveAndQuitButtonX = BufferOut.Load(216);
	int saveAndQuitButtonY = BufferOut.Load(220);


	if (windowPosX == 0)
	{
		//gotta find window pos
		for (int curLine = 0; curLine < 2160; curLine++)
		{
			for (int curPixBlock = 0; curPixBlock < 3840; curPixBlock += 32)
			{
				int curPix = curPixBlock + DTid;
				float4 curColor = ScreenTex.Load(int3(curPix, curLine, 0));
				unsigned int curHTML = uint(curColor.r * 255) * 65536 + uint(curColor.g * 255) * 256 + uint(curColor.b * 255);

				//BufferOut.Store(0, curHTML);
				//BufferOut.Store(4, 123);
				//if (curHTML != 0)
				if (curHTML == 0x292a2a) // 1
				//if (curHTML == 0x499FF3)
				{
					float4 nextColor = ScreenTex.Load(int3(curPix + 1, curLine, 0));
					unsigned int nextHTML = uint(nextColor.r * 255) * 65536 + uint(nextColor.g * 255) * 256 + uint(nextColor.b * 255);

					//BufferOut.Store(0, 456);
					//BufferOut.Store(4, 456);

					if (nextHTML == 0x303232) // 2
					{
						nextColor = ScreenTex.Load(int3(curPix + 2, curLine, 0));
						nextHTML = uint(nextColor.r * 255) * 65536 + uint(nextColor.g * 255) * 256 + uint(nextColor.b * 255);

						//BufferOut.Store(0, 789);
						//BufferOut.Store(4, 789);

						if (nextHTML == 0x2f2f30) // 3
						{
							nextColor = ScreenTex.Load(int3(curPix + 3, curLine, 0));
							nextHTML = uint(nextColor.r * 255) * 65536 + uint(nextColor.g * 255) * 256 + uint(nextColor.b * 255);
							if (nextHTML == 0x2f3031) // 4
							{
								windowPosX = curPix;
								windowPosY = curLine;
								BufferOut.Store(0, windowPosX);
								BufferOut.Store(4, windowPosY);
							}
						}
					}
				}
			}
		}
	}
	
	// generic window only tests
	// 
	// single pixel tests
	if (DTid.x == 0 && windowPosX != 0) // big performance oof
	{
		//test for play button
		int curPix = 517;
		int curLine = 641;
		float4 curColor = ScreenTex.Load(int3(curPix + windowPosX, curLine + windowPosY, 0));
		unsigned int curHTML = uint(curColor.r * 255) * 65536 + uint(curColor.g * 255) * 256 + uint(curColor.b * 255);

		float4 prevColor = ScreenTex.Load(int3(curPix + windowPosX - 1, curLine + windowPosY, 0));
		unsigned int prevHTML = uint(prevColor.r * 255) * 65536 + uint(prevColor.g * 255) * 256 + uint(prevColor.b * 255);

		//BufferOut.Store(12, curHTML);
		//BufferOut.Store(16, prevHTML);

		if (curHTML == 0xfff6e3)
		{
			if (prevHTML == 0x6b645d || prevHTML == 0xfff6e3)
			{
				playButtonX = curPix;
				playButtonY = curLine;
				playButtonFound = 1;
				BufferOut.Store(8, playButtonFound);
				BufferOut.Store(12, playButtonX);
				BufferOut.Store(16, playButtonY);
			}
		}



		//test for hell button
		curPix = 626;
		curLine = 390;
		curColor = ScreenTex.Load(int3(curPix + windowPosX, curLine + windowPosY, 0));
		curHTML = uint(curColor.r * 255) * 65536 + uint(curColor.g * 255) * 256 + uint(curColor.b * 255);

		prevColor = ScreenTex.Load(int3(curPix + windowPosX - 1, curLine + windowPosY, 0));
		prevHTML = uint(prevColor.r * 255) * 65536 + uint(prevColor.g * 255) * 256 + uint(prevColor.b * 255);

		//BufferOut.Store(24, curHTML);
		//BufferOut.Store(28, prevHTML);

		if (curHTML == 0xfff6e3)
		{
			if (prevHTML == 0x7d786f || prevHTML == 0x7c786f)
			{
				hellButtonX = curPix;
				hellButtonY = curLine;
				hellButtonFound = 1;
				BufferOut.Store(24, hellButtonX);
				BufferOut.Store(28, hellButtonY);
			}
		}
		BufferOut.Store(20, hellButtonFound);

		//test for save and quit
		curPix = 594;
		curLine = 312;
		curColor = ScreenTex.Load(int3(curPix + windowPosX, curLine + windowPosY, 0));
		curHTML = uint(curColor.r * 255) * 65536 + uint(curColor.g * 255) * 256 + uint(curColor.b * 255);

		prevColor = ScreenTex.Load(int3(curPix + windowPosX - 1, curLine + windowPosY, 0));
		prevHTML = uint(prevColor.r * 255) * 65536 + uint(prevColor.g * 255) * 256 + uint(prevColor.b * 255);

		//BufferOut.Store(216, curHTML);
		//BufferOut.Store(220, prevHTML);

		if (curHTML == 0xeae1d0 || curHTML == 0x00b2ab9e)
		{
			if (prevHTML == 0x68615b || prevHTML == 0x0069635c)
			{
				saveAndQuitButtonX = curPix;
				saveAndQuitButtonY = curLine;
				saveAndQuitButtonFound = 1;
				BufferOut.Store(216, saveAndQuitButtonX);
				BufferOut.Store(220, saveAndQuitButtonY);
			}
		}
		BufferOut.Store(212, saveAndQuitButtonFound);

		//reset to zero for tunnel entrance
		BufferOut.Store(56, 0);
		// reset to zero item found
		BufferOut.Store(128, 0);
	}

	// screen scan for lost city ancient tunnels entrance
	/*
	if (windowPosX != 0)
	{
		float bestDiff = 30000.0f;

		for (int curScreenLine = 0; curScreenLine < 720; curScreenLine++)
		{
			int curLine = curScreenLine + windowPosY;

			for (int curPixBlock = 0; curPixBlock < 1280; curPixBlock += 32)
			{
				int curPix = curPixBlock + DTid + windowPosX;
				float4 curColor1 = ScreenTex.Load(int3(curPix, curLine, 0));
				float4 curColor2 = ScreenTex.Load(int3(curPix + 61, curLine, 0));
				float4 curColor3 = ScreenTex.Load(int3(curPix + 66, curLine, 0));
				float4 curColor4 = ScreenTex.Load(int3(curPix, curLine + 28, 0));
				
				float curDiff = fuzzyMatchGroup4(curColor1, curColor2, curColor3, curColor4, 0x302a20, 0x18150e, 0x7a684e, 0x5d523f);

				if (curDiff < bestDiff)
				{
					bestDiff = curDiff;
					tunnelsEntranceFound = bestDiff * 100000;
					tunnelsEntranceX = curPix - windowPosX;
					tunnelsEntranceY = curLine - windowPosY;
					BufferOut.Store(56, tunnelsEntranceFound);
					BufferOut.Store(60, tunnelsEntranceX);
					BufferOut.Store(64, tunnelsEntranceY);
				}
			}
		}
	}
	*/

	// screen scan for items
	if (windowPosX != 0)
	{
		//only do top 654, full rejuvs art triggers unique color
		for (int curScreenLine = 0; curScreenLine < 654; curScreenLine++)
		{
			int curLine = curScreenLine + windowPosY;

			// skip last 100, clock triggers unique color
			for (int curPixBlock = 0; curPixBlock < 1280; curPixBlock += 32)
			{
				int curPix = curPixBlock + DTid + windowPosX;
				float4 curColor = ScreenTex.Load(int3(curPix, curLine, 0));
				uint curHTML = uint(curColor.r * 255) * 65536 + uint(curColor.g * 255) * 256 + uint(curColor.b * 255);

				float4 nextColor = ScreenTex.Load(int3(curPix + 1, curLine, 0));
				unsigned int nextHTML = uint(nextColor.r * 255) * 65536 + uint(nextColor.g * 255) * 256 + uint(nextColor.b * 255);

				if ((curHTML == 0x00fc00 || curHTML == 0xffa800 || curHTML == 0xc7b377) && (nextHTML == curHTML))
				{
					itemX = curPix - windowPosX;
					itemY = curLine - windowPosY;
					itemFound = curHTML;
					BufferOut.Store(128, itemFound);
					BufferOut.Store(132, itemX);
					BufferOut.Store(136, itemY);
				}
			}
		}
	}
}