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
	int curDir;
	int prevDir;
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
	int GetCurDir();
	int GetPrevDir();
	void SetPrevDir(int dir);
	void SetCurDir(int dir);
	void NextMotion();
	void InitMotion();
	~Character();
};