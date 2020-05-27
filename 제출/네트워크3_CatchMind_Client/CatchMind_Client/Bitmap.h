#pragma once
#include <Windows.h>
#include <iostream>
using namespace std;
class Bitmap
{
private:
	HDC m_HDC;
	LPCSTR source;
	HDC		MemDC;
	HBITMAP	MyBitMap;
	HBITMAP	OldBitMap;
	SIZE	size;
public:
	Bitmap();
	void Init(HDC hdc, LPCSTR source);
	void TransparentDraw(HDC hdc, int x, int y);
	void Draw(HDC hdc, int x, int y);
	void StretchDraw(HDC hdc, int x, int y, double i);
	~Bitmap();
};