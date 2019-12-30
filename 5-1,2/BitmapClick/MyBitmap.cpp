#include "MyBitmap.h"



MyBitmap::MyBitmap(int res, int x, int y, int width, int height)
{
	this->res = res;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;

}

void MyBitmap::print(HINSTANCE hInst, HDC hdc) {
	HDC MemDC = CreateCompatibleDC(hdc);
	HBITMAP myBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(res));
	HBITMAP oldBitmap = (HBITMAP)SelectObject(MemDC, myBitmap);
	BitBlt(hdc, x, y, width, height, MemDC, 0, 0, SRCCOPY);
	SelectObject(MemDC, oldBitmap);
	DeleteObject(myBitmap);
	DeleteDC(MemDC);
}

bool MyBitmap::clickCheck(int click_x, int click_y) {
	if (click_x >= x && click_x < x + width && click_y >= y && click_y < y + height)
		return true;
	return false;
}

MyBitmap::~MyBitmap()
{
}
