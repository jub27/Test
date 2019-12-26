#include "BitmapPrinter.h"

BitmapPrinter* BitmapPrinter::bitmapPrinter = NULL;

BitmapPrinter::BitmapPrinter()
{
}

void BitmapPrinter::SetHDC(HDC hdc) {
	MemDC = CreateCompatibleDC(hdc);
	this->hdc = hdc;
}

void BitmapPrinter::SetInstance(HINSTANCE hInstance) {
	this->hInstance = hInstance;
}

void BitmapPrinter::Print(int resource) {
	myBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(resource));
	oldBitmap = (HBITMAP)SelectObject(MemDC, myBitmap);
	BitBlt(hdc, BITMAP_X, BITMAP_Y, BITMAP_WIDTH, BITMAP_HEIGHT, MemDC, 0, 0, SRCCOPY);
	SelectObject(MemDC, oldBitmap);
	DeleteObject(myBitmap);
	DeleteDC(MemDC);
}

BitmapPrinter::~BitmapPrinter()
{
}
