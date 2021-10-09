#pragma once

class screenClicker
{
public:
	void init();

	void click(int x, int y, int button);
	void pressKey(unsigned short key);
	void holdKey(unsigned short key);
	void releaseKey(unsigned short key);
};

extern screenClicker g_clicker;