#include "farmSequence.h"

#include "screenReader.h"
#include "screenClicker.h"
#include <random>
#include <stdlib.h>

farmSequence g_sequence;

int g_seq;

void farmSequence::init()
{
	g_seq = 0;
	system("Color 07");
}

void sleepRandomBig()
{
	Sleep(rand() % 3000 + 100);
}

void sleepRandomTiny()
{
	Sleep(rand() % 30 + 1);
}

void clickRandom3(int x, int y, int button)
{
	int randX = (rand() % 5) - 2;
	randX += x;
	int randY = (rand() % 5) - 2;
	randY += y;

	if (randX < g_screen.pReadback->windowPosX)
	{
		randX = g_screen.pReadback->windowPosX;
	}
	if (randX > g_screen.pReadback->windowPosX + 1279)
	{
		randX = g_screen.pReadback->windowPosX + 1279;
	}
	if (randY < g_screen.pReadback->windowPosY)
	{
		randY = g_screen.pReadback->windowPosY;
	}
	if (randY > g_screen.pReadback->windowPosY + 719)
	{
		randY = g_screen.pReadback->windowPosY + 719;
	}


	g_clicker.click(randX, randY, button);
}

int seqPickRetry = 0;

void farmSequence::tick()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 8);
	if (g_seq == 0)
	{
		Sleep(16);
		seqPickRetry = 0;
		// click play
		if (g_screen.pReadback && g_screen.pReadback->playButtonFound)
		{
			static int firstLoop = 1;
			if (firstLoop)
			{
				firstLoop = 0;
				printf("Starting up in 3 sec\n");
				Sleep(3 * 1000);
			}
			g_seq = 1;
			printf("\nclicking PLAY at %d, %d",
				g_screen.pReadback->windowPosX + g_screen.pReadback->playButtonX,
				g_screen.pReadback->windowPosY + g_screen.pReadback->playButtonY);


			clickRandom3(
				g_screen.pReadback->windowPosX + g_screen.pReadback->playButtonX,
				g_screen.pReadback->windowPosY + g_screen.pReadback->playButtonY, 0);

			printf("...click\n");
			sleepRandomBig();
		}
		else if (g_screen.pReadback)
		{
			
			static int firstLoop1 = 1;
			if (firstLoop1)
			{
				firstLoop1 = 0;
				printf("Tip: game window must be 1280x720 and show character selection.\n");
				printf("Trying to locate Diablo 2 window...");
			}
			Sleep(3 * 1000);
			printf(".");
		}
			
	}

	if (g_seq == 1)
	{
		if (g_screen.pReadback && g_screen.pReadback->hellButtonFound)
		{
			printf("clicking HELL at %d, %d",
				g_screen.pReadback->windowPosX + g_screen.pReadback->hellButtonX,
				g_screen.pReadback->windowPosY + g_screen.pReadback->hellButtonY);
			for (int i = 0; i < 1; i++)
			{
				clickRandom3(
					g_screen.pReadback->windowPosX + g_screen.pReadback->hellButtonX,
					g_screen.pReadback->windowPosY + g_screen.pReadback->hellButtonY, 0);
			}

			printf("...click\n");
			sleepRandomBig();
			printf("waiting 4 secs for load\n");
			Sleep(4000);
			printf("...4 more on my old PC\n");
			Sleep(4000);
			g_seq = 20; //walker sequence next
		}
	}

	if (g_seq == 2)
	{
		// sorc teleport sequence
		int clickX, clickY;
		clickX = 410 + g_screen.pReadback->windowPosX;
		clickY = 620 + g_screen.pReadback->windowPosY;
		printf("1 clicking at %d, %d (down the stairs)\n", clickX, clickY);
		clickRandom3(clickX, clickY, 0);
		Sleep(1800);
		sleepRandomTiny();
		g_seq = 10;

		printf("using Telekinesis\n");
		g_clicker.pressKey(119);
		sleepRandomTiny();
		clickX = 717 + g_screen.pReadback->windowPosX;
		clickY = 631 + g_screen.pReadback->windowPosY;
		printf("2 Telekinesis stash at %d, %d \n", clickX, clickY);
		clickRandom3(clickX, clickY, -1);
		Sleep(30);
		sleepRandomTiny();
		clickRandom3(clickX, clickY, 1);
		Sleep(100);
		sleepRandomTiny();
		
		printf("3 Stashing last 3 columns\n");
		g_clicker.holdKey(17); //ctrl
		sleepRandomTiny();
		
		for (int cx = 0; cx < 3; cx++)
		{
			for (int cy = 0; cy < 4; cy++)
			{
				clickX = cx * 36 + 1088 + g_screen.pReadback->windowPosX;
				clickY = cy * 36 + 385 + g_screen.pReadback->windowPosY;
				clickRandom3(clickX, clickY, -1);
				Sleep(100);
				sleepRandomTiny();
				clickRandom3(clickX, clickY, 0);
				Sleep(100);
				sleepRandomTiny();
			}
		}
		
		Sleep(30);
		Sleep(30);
		g_clicker.releaseKey(17);
		Sleep(30);
		Sleep(30);
		sleepRandomTiny();

		printf("pressing ESC\n");
		g_clicker.pressKey(27);
		sleepRandomTiny();

		clickX = 510 + g_screen.pReadback->windowPosX;
		clickY = 420 + g_screen.pReadback->windowPosY;
		printf("4 clicking at %d, %d (down the stairs again)\n", clickX, clickY);
		clickRandom3(clickX, clickY, 0);
		Sleep(1800);
		sleepRandomTiny();

		clickX = 522 + g_screen.pReadback->windowPosX;
		clickY = 480 + g_screen.pReadback->windowPosY;
		printf("5 clicking at %d, %d (next to waypoint)\n", clickX, clickY);
		clickRandom3(clickX, clickY, 0);
		Sleep(1800);


		clickX = 400 + g_screen.pReadback->windowPosX;
		clickY = 640 + g_screen.pReadback->windowPosY;
		clickRandom3(clickX, clickY, -1);
		sleepRandomTiny();

		printf("6 clicking at %d, %d (next to building)\n", clickX, clickY);
		clickRandom3(clickX, clickY, 0);
		Sleep(1800);
		sleepRandomTiny();


		clickX = 410 + g_screen.pReadback->windowPosX;
		clickY = 640 + g_screen.pReadback->windowPosY;
		printf("7 clicking at %d, %d (next to building)\n", clickX, clickY);
		clickRandom3(clickX, clickY, 0);
		Sleep(1800);
		sleepRandomTiny();

		clickX = 480 + 50 + g_screen.pReadback->windowPosX;
		clickY = 630 + g_screen.pReadback->windowPosY;
		printf("8 clicking at %d, %d (south of building)\n", clickX, clickY);
		clickRandom3(clickX, clickY, 0);
		Sleep(1800);
		sleepRandomTiny();

		clickX = 240 - 50 + g_screen.pReadback->windowPosX;
		clickY = 76 + g_screen.pReadback->windowPosY;
		printf("9 clicking at %d, %d (red portal)\n", clickX, clickY);
		clickRandom3(clickX, clickY, -1);
		Sleep(30);
		sleepRandomTiny();
		clickRandom3(clickX, clickY, 0);
		Sleep(1800);
		sleepRandomTiny();

		printf("using Teleport\n");
		g_clicker.pressKey(117);
		sleepRandomTiny();

		clickX = 940 + g_screen.pReadback->windowPosX;
		clickY = 15 + g_screen.pReadback->windowPosY;
		printf("10 teleport to %d, %d\n", clickX, clickY);
		clickRandom3(clickX, clickY, -1);
		Sleep(30);
		sleepRandomTiny();
		clickRandom3(clickX, clickY, 1);
		Sleep(400);
		sleepRandomTiny();

		clickX = 1155 + g_screen.pReadback->windowPosX;
		clickY = 23 + g_screen.pReadback->windowPosY;
		printf("11 teleport to %d, %d\n", clickX, clickY);
		clickRandom3(clickX, clickY, -1);
		Sleep(30);
		sleepRandomTiny();
		clickRandom3(clickX, clickY, 1);
		Sleep(400);
		sleepRandomTiny();

		clickX = 836 + g_screen.pReadback->windowPosX;
		clickY = 191 + g_screen.pReadback->windowPosY;
		printf("12 teleport to %d, %d\n", clickX, clickY);
		clickRandom3(clickX, clickY, -1);
		Sleep(30);
		sleepRandomTiny();
		clickRandom3(clickX, clickY, 1);
		Sleep(400);
		sleepRandomTiny();

		clickX = 640 + 45 + g_screen.pReadback->windowPosX;
		clickY = 360 - 35 + g_screen.pReadback->windowPosY;
		printf("12b teleport to %d, %d\n", clickX, clickY);
		clickRandom3(clickX, clickY, -1);
		Sleep(30);
		sleepRandomTiny();
		clickRandom3(clickX, clickY, 1);
		Sleep(400);
		sleepRandomTiny();


		clickX = 1090 + g_screen.pReadback->windowPosX;
		clickY = 174 + g_screen.pReadback->windowPosY;
		printf("13 murder at to %d, %d\n", clickX, clickY);
		clickRandom3(clickX, clickY, -1);
		Sleep(30);
		sleepRandomTiny();
		for (int i = 0; i < 5; i++)
		{
			clickX = 1090 + g_screen.pReadback->windowPosX;
			clickY = 174 + g_screen.pReadback->windowPosY;
			clickX += (rand() % 100) - 50;
			clickY += (rand() % 100) - 50;
			g_clicker.pressKey(118);
			sleepRandomTiny();
			clickRandom3(clickX, clickY, 1);
			sleepRandomTiny();
			Sleep(1250);
			g_clicker.pressKey(116);
			sleepRandomTiny();
			clickX += (rand() % 100) - 50;
			clickY += (rand() % 100) - 50;
			clickRandom3(clickX, clickY, 1);
			Sleep(290);
			sleepRandomTiny();
			clickX += (rand() % 100) - 50;
			clickY += (rand() % 100) - 50;
			clickRandom3(clickX, clickY, 1);
			Sleep(290);
			sleepRandomTiny();
			clickX += (rand() % 100) - 50;
			clickY += (rand() % 100) - 50;
			clickRandom3(clickX, clickY, 1);
			Sleep(290);
			sleepRandomTiny();
		}

		printf("14 show items\n");
		g_clicker.pressKey(18);
		sleepRandomTiny();
		Sleep(16);
		return;
	}

	if (g_seq == 10)
	{
		printf("watching items...\n");
		if (seqPickRetry < 3 && g_screen.pReadback && g_screen.pReadback->itemFound)
		{
			seqPickRetry += 1;
			g_seq = 10;
			if (g_screen.pReadback->itemFound == 0x00fc00)
			{
				SetConsoleTextAttribute(hConsole, 10);
			}
			if (g_screen.pReadback->itemFound == 0xffa800)
			{
				SetConsoleTextAttribute(hConsole, 12);
			}
			if (g_screen.pReadback->itemFound == 0xc7b377)
			{
				SetConsoleTextAttribute(hConsole, 14);
			}
			printf("clicking ITEM color %#08x at %d, %d\n", g_screen.pReadback->itemFound,
				g_screen.pReadback->windowPosX + g_screen.pReadback->itemX,
				g_screen.pReadback->windowPosY + g_screen.pReadback->itemY);
			SetConsoleTextAttribute(hConsole, 8);
			clickRandom3(
					g_screen.pReadback->windowPosX + g_screen.pReadback->itemX,
					g_screen.pReadback->windowPosY + g_screen.pReadback->itemY, -1);
			sleepRandomTiny();
			clickRandom3(
				g_screen.pReadback->windowPosX + g_screen.pReadback->itemX,
				g_screen.pReadback->windowPosY + g_screen.pReadback->itemY, 0);
			Sleep(4000);
			sleepRandomTiny();
			return;
		}
		else
		{
			printf("no item found\n");
			sleepRandomTiny();
			Sleep(500);
			g_seq = 100;
		}
	}

	if (g_seq == 100)
	{
		printf("pressing ESC\n");
		g_clicker.pressKey(27);
		sleepRandomTiny();
		g_seq = 101;
	}

	if (g_seq == 101)
	{
		if (g_screen.pReadback && g_screen.pReadback->saveAndQuitButtonFound)
		{
			printf("clicking SAVE AND QUIT at %d, %d",
				g_screen.pReadback->windowPosX + g_screen.pReadback->saveAndQuitButtonX,
				g_screen.pReadback->windowPosY + g_screen.pReadback->saveAndQuitButtonY);
			clickRandom3(
				g_screen.pReadback->windowPosX + g_screen.pReadback->saveAndQuitButtonX,
				g_screen.pReadback->windowPosY + g_screen.pReadback->saveAndQuitButtonY, 0);
			printf("...click\n");
			sleepRandomBig();
			Sleep(4 * 1000);
			g_seq = 0;
		}
	}



	// NON-SORCS
	//walker sequence
	if (g_seq == 20)
	{
	}
}