#pragma once
#include <windows.h>
enum Dir{
	DOWN, UP, LEFT, RIGHT
};
class Character
{
private:
	HDC		MemDC;
	HBITMAP	m_pMyBitMap;
	HBITMAP	m_pOldBitMap;
	SIZE	m_size;
	int x, y;
	Dir prevDir;
	Dir curDir;
	int m;
public:
	Character();
	void Init(HWND hWnd, HINSTANCE hInst);
	void Draw(HDC hdc);
	void Move(int direction);
	~Character();
};

