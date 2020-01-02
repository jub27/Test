#pragma once
#include<Windows.h>
class Bitmap
{
private:
	HDC		MemDC;
	HBITMAP	m_pMyBitMap;
	HBITMAP	m_pOldBitMap;
	SIZE	m_size;
	bool opened;
	int resource;
public:
	Bitmap();

	void Init(HDC hdc, HINSTANCE hInst, int resource);
	void Draw(HDC hdc, int x, int y, int spX = 1, int spY = 1);
	void Open();
	void Close();
	bool isOpend();
	int GetResource();
	~Bitmap();
};

