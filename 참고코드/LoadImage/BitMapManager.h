#pragma once
#include"define.h"
#include"BitMap.h"

class BitMapManager
{
private:
	BitMap* m_parrBitMap;
	static BitMapManager* m_hThis;
public:
	static BitMapManager* GetInstance()
	{
		if (m_hThis == NULL)
			m_hThis = new BitMapManager;
		return m_hThis;
	}
	BitMap* GetImage(IMAGE index)
	{
		return &m_parrBitMap[index];
	}
	BitMapManager();
	void Init(HWND hWnd);
	~BitMapManager();
};

