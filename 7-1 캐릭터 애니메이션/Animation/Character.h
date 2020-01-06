#pragma once
#include <windows.h>
#include <ctime>
#include <cmath>
enum Dir {
	DOWN, UP, LEFT, RIGHT
};
class Character
{
private:
	HWND hWnd;
	HDC		MemDC;
	HBITMAP	m_pMyBitMap;
	HBITMAP	m_pOldBitMap;
	SIZE	m_size;
	double x, y;
	double jumpY;
public:
	Character();
	void Init(HWND hWnd, HINSTANCE hInst);
	void Draw(HDC hdc, int motionNum, int dir);
	void Jump(int degree);
	void AddX(double x);
	void AddY(double y);
	~Character();
};

