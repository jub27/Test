#pragma once
#include "resource.h"
#include <iostream>
#include <Windows.h>

#define BITMAP_X 100
#define BITMAP_Y 100
#define BITMAP_WIDTH 145
#define BITMAP_HEIGHT 235

class BitmapPrinter
{
private:
	HINSTANCE hInstance;
	HDC MemDC;
	HDC hdc;
	HBITMAP myBitmap, oldBitmap;
	static BitmapPrinter * bitmapPrinter;
public:
	static BitmapPrinter* GetInstance() {
		if (bitmapPrinter == NULL)
			bitmapPrinter = new BitmapPrinter();
		return bitmapPrinter;
	}
	BitmapPrinter();
	void SetHDC(HDC hdc);
	void SetInstance(HINSTANCE hInstance);
	void Print(int resource);
	~BitmapPrinter();
};

