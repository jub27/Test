#pragma once
#include<Windows.h>
#include"Bitmap.h"

#define BITMAP_NUMS 20
#define BITMAP_WIDTH 145
#define BITMAP_HEIGHT 235
class Gamemanager
{
private:
	static Gamemanager* m_pThis;
	Bitmap* m_bt[BITMAP_NUMS];
	Bitmap* back;
	int score;
	bool clickFlag;
	int lastClick;
	int curClick;

public:
	static Gamemanager* GetInstans()
	{
		if (m_pThis == NULL)
		{
			m_pThis = new Gamemanager;
		}
		return m_pThis;
	}
	Gamemanager();

	void Init(HWND hWnd, HINSTANCE hInst);
	void Draw(HDC hdc);
	int Click(int x, int y);
	void CloseCurLast();
	void Shuffle();
	void Release();

	~Gamemanager();
};

