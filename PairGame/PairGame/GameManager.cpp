#include "Gamemanager.h"


Gamemanager* Gamemanager::m_pThis = NULL;

Gamemanager::Gamemanager()
{
}
void Gamemanager::Init(HWND hWnd, HINSTANCE hInst)
{
	HDC hdc = GetDC(hWnd);
	for (int i = 0; i < BITMAP_NUMS; i++) {
		m_bt[i] = new Bitmap;
		m_bt[i]->Init(hdc, hInst);
	}
	back = new Bitmap(); //µÞ¸éÀ» µû·Î ¸¸µé¾îµÐ´Ù
}
void Gamemanager::Draw(HDC hdc, int x, int y)
{
	for (int i = 0; i < BITMAP_NUMS; i++) {
		m_bt[i]->Draw(hdc, x, y);
	}
}

void Gamemanager::Release()
{
	delete(m_pThis);
}

Gamemanager::~Gamemanager()
{
	delete(m_bt);
}
