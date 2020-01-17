#include "GameManager.h"


GameManager* GameManager::instance = NULL;

GameManager::GameManager()
{
}

void GameManager::Init(HWND hWnd) {
	m_hWnd = hWnd;
	MemDC = CreateCompatibleDC(GetDC(m_hWnd));
	MyBitMap = CreateCompatibleBitmap(GetDC(m_hWnd), 1024, 768);
	OldBitMap = (HBITMAP)SelectObject(MemDC, MyBitMap);
	for (int i = 0; i < BITMAP_NUMS; i++) {
		bitmap[i] = new Bitmap();
		bitmap[i]->Init(MemDC, source[i]);
	}
}

void GameManager::DrawBack() {
	for (int i = 0; i < 10; i++) {
		bitmap[2]->Draw(MemDC, i * 65, 0);
		bitmap[0]->StretchDraw(MemDC, i * 67, 64);
	}
}

void GameManager::Render() {
	DrawBack();
	BitBlt(GetDC(m_hWnd), 0, 0, 1024, 768, MemDC, 0, 0, SRCCOPY);
}

GameManager::~GameManager()
{
}