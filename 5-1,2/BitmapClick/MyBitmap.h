#pragma once
#include "resource1.h"
#include <Windows.h>

class MyBitmap
{
private:
	int res, x, y, width, height;
public:
	MyBitmap(int res,int x, int y, int width, int height);
	void print(HINSTANCE hInst, HDC hdc);
	bool clickCheck(int click_x, int click_y);
	~MyBitmap();
};

