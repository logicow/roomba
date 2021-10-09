#include "screenClicker.h"
#include "screenReader.h"
#include "windows.h"

screenClicker g_clicker;

void screenClicker::init()
{
	bool bSuccess = SetProcessDPIAware();
	return;
}

void screenClicker::click(int x, int y, int button)
{
	INPUT ip;
	ZeroMemory(&ip, sizeof(INPUT));
	ip.type = INPUT_MOUSE;
	ip.mi.dx = x * 65536 / (g_screen.desktopCoordinates.right - g_screen.desktopCoordinates.left);
	ip.mi.dy = y * 65536 / (g_screen.desktopCoordinates.bottom - g_screen.desktopCoordinates.top);
	ip.mi.mouseData = 0; // use for wheel
	ip.mi.dwFlags = (MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN);
	if (button == 1)
	{
		ip.mi.dwFlags = (MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN);
	}
	if (button == -1)
	{
		ip.mi.dwFlags = (MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE);
	}
	ip.mi.time = 0;
	ip.mi.dwExtraInfo = 0;
	unsigned int res;
	res = SendInput(1, &ip, sizeof(INPUT));

	if (button == 1)
	{
		Sleep(16);
	}

	ip.type = INPUT_MOUSE;
	ip.mi.dx = x * 65536 / (g_screen.desktopCoordinates.right - g_screen.desktopCoordinates.left);
	ip.mi.dy = y * 65536 / (g_screen.desktopCoordinates.bottom - g_screen.desktopCoordinates.top);
	ip.mi.mouseData = 0; // use for wheel
	ip.mi.dwFlags = (MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP);
	if (button == 1)
	{
		ip.mi.dwFlags = (MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTUP);
	}
	if (button == -1)
	{
		ip.mi.dwFlags = (MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE);
	}
	ip.mi.time = 0;
	ip.mi.dwExtraInfo = 0;
	res = SendInput(1, &ip, sizeof(INPUT));
	return;
}

void screenClicker::pressKey(unsigned short key)
{
	INPUT ip;
	ZeroMemory(&ip, sizeof(INPUT));
	ip.type = INPUT_KEYBOARD;
	ip.ki.wVk = key;
	ip.ki.wScan = 0;
	ip.ki.dwFlags = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	SendInput(1, &ip, sizeof(INPUT));

	ip.ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput(1, &ip, sizeof(INPUT));
}


void screenClicker::holdKey(unsigned short key)
{
	INPUT ip;
	ZeroMemory(&ip, sizeof(INPUT));
	ip.type = INPUT_KEYBOARD;
	ip.ki.wVk = key;
	ip.ki.wScan = 0;
	ip.ki.dwFlags = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	SendInput(1, &ip, sizeof(INPUT));
}


void screenClicker::releaseKey(unsigned short key)
{
	INPUT ip;
	ZeroMemory(&ip, sizeof(INPUT));
	ip.type = INPUT_KEYBOARD;
	ip.ki.wVk = key;
	ip.ki.wScan = 0;
	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;
	SendInput(1, &ip, sizeof(INPUT));
}