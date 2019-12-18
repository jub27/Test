#include "BitMapManager.h"

BitMapManager* BitMapManager::m_hThis = NULL;

BitMapManager::BitMapManager()
{
	m_parrBitMap = new BitMap[IMAGE_END];
}

void BitMapManager::Init(HWND hWnd)
{
	TCHAR buf[256];
	HDC hdc = GetDC(hWnd);
	for (int i = IMAGE_START; i < IMAGE_CARDEND; i++)
	{
		wsprintf(buf, L"RES//0%d.bmp", i);
		m_parrBitMap[i].Init(hdc,buf);
	}

	ReleaseDC(hWnd, hdc);
}

BitMapManager::~BitMapManager()
{
	delete[] m_parrBitMap;
}
