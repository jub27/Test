#include "Gamemanager.h"


Gamemanager* Gamemanager::m_pThis = NULL;

Gamemanager::Gamemanager()
{
}
void Gamemanager::Init(HWND hWnd, HINSTANCE hInst)
{
	HDC hdc = GetDC(hWnd);
	m_bt = new Bitmap;
	m_bt->Init(hdc, hInst);
}
void Gamemanager::Draw(HDC hdc, int x, int y)
{
	m_bt->Draw(hdc, x, y);
}

void Gamemanager::Release()
{
	delete(m_pThis);
}

Gamemanager::~Gamemanager()
{
	delete(m_bt);
}
