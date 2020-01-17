#pragma once
#include <Windows.h>
#include "Bitmap.h"

#define BITMAP_NUMS 28

class GameManager
{
private:
	HWND m_hWnd;
	HDC		MemDC;
	HBITMAP	MyBitMap;
	HBITMAP	OldBitMap;
	static GameManager * instance;
	LPCWSTR source[BITMAP_NUMS] = { L"res\\back.bmp",L"res\\back_deco.bmp", L"res\\back_normal.bmp", L"res\\back_normal2.bmp", L"res\\cash.bmp", 
		L"res\\die.bmp", L"res\\end.bmp", L"res\\enemy.bmp", L"res\\enemy_1b.bmp", L"res\\enemy_1f.bmp",L"res\\enemy_b.bmp",L"res\\enemy_f.bmp",L"res\\enemy_l.bmp", 
		L"res\\enemy_l_b.bmp",L"res\\enemy_l_f.bmp", L"res\\enemy1.bmp", L"res\\front.bmp", L"res\\front2.bmp", L"res\\icon.bmp", L"res\\miter.bmp", L"res\\player0.bmp", 
		L"res\\plater1.bmp", L"res\\player2.bmp", L"res\\star.bmp", L"res\\star1.bmp", L"res\\star2.bmp", L"res\\win.bmp", L"res\\win2.bmp" };
	Bitmap * bitmap[BITMAP_NUMS];
public:
	static GameManager* GetInstance() {
		if (instance == NULL)
			instance = new GameManager();
		return instance;
	}
	GameManager();
	void DrawBack();
	void Init(HWND hWnd);
	void Render();
	~GameManager();
};

