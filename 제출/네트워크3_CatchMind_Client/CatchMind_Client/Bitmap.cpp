#include "Bitmap.h"



Bitmap::Bitmap()
{
}

void Bitmap::Init(HDC hdc, LPCSTR source) {
	m_HDC = hdc;
	this->source = source;
	MemDC = CreateCompatibleDC(hdc);

	MyBitMap = (HBITMAP)LoadImage(NULL, source,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

	OldBitMap = (HBITMAP)SelectObject(MemDC, MyBitMap);

	BITMAP Bit_Info;

	GetObject(MyBitMap, sizeof(Bit_Info), &Bit_Info);
	size.cx = Bit_Info.bmWidth;
	size.cy = Bit_Info.bmHeight;

}
void Bitmap::TransparentDraw(HDC hdc, int x, int y)
{
	TransparentBlt(hdc, x, y, size.cx, size.cy, MemDC, 0, 0,
		size.cx, size.cy, RGB(255, 0, 255));
}

void Bitmap::Draw(HDC hdc, int x, int y)
{
	BitBlt(hdc, x, y, size.cx, size.cy, MemDC, 0, 0, SRCCOPY);
}

void Bitmap::StretchDraw(HDC hdc, int x, int y, double i) {
	StretchBlt(hdc, x, y, size.cx, size.cy * i, MemDC, 0, 0, size.cx, size.cy, SRCCOPY);
}

Bitmap::~Bitmap()
{
}