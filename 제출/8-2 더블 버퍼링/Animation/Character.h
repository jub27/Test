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
	HDC		MemDC[3];
	HBITMAP	m_pMyBitMap[3];
	HBITMAP	m_pOldBitMap[3];
	SIZE	m_size;
	Dir curDir;
	Dir prevDir;
	int motion;
	int speed;
	double x, y;
	double jumpY;
public:
	Character();
	void Init(HWND hWnd, HINSTANCE hInst);
	void Draw(HDC hdc);
	void Jump(int degree);
	void MoveX(int dir);
	void MoveY(int dir);
	Dir GetCurDir();
	Dir GetPrevDir();
	void SetPrevDir(Dir dir);
	void SetCurDir(Dir dir);
	void NextMotion();
	void InitMotion();
	~Character();
};