#pragma once
#include "define.h"

class BitMap
{
private:
	HDC     MemDC;
	HBITMAP m_BitMap;
	HBITMAP m_OldBitMap;
	SIZE    m_size;
public:
	void Init(HDC hdc, LPCWSTR FileName);
	void Draw(HDC hdc, int x, int y, float spX, float spY);
	inline SIZE GetSize()
	{
		return m_size;
	}
	BitMap();
	~BitMap();
};

