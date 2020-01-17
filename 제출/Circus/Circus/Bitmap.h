#pragma once
#include <Windows.h>
class Bitmap
{
private:
	HDC		MemDC;
	HBITMAP	MyBitMap;
	HBITMAP	OldBitMap;
	SIZE	size;
public:
	Bitmap();
	void Init(HDC hdc, LPCWSTR source);
	void TransparentDraw(HDC hdc, int x, int y);
	void Draw(HDC hdc, int x, int y);
	void StretchDraw(HDC hdc, int x, int y);
	~Bitmap();
};

