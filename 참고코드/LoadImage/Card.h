#pragma once
#include"BitMap.h"
#include"BitMapManager.h"

class Card
{
private:
	CARD    m_eCardState;
	IMAGE   m_eImageIndex;
	BitMap* m_pBitMap[CARD_END];
	int     m_ix;
	int     m_iy;
	float   m_fSpX;
	float   m_fSpY;
	RECT    m_BitMapRect;
public:
	Card();
	void setImage(IMAGE Index);
	void setPos(int x, int y, float spX = 0.5, float = 0.5);
	void draw(HDC hdc);
	bool cardOpen(POINT point);
	void cardClose();
	inline IMAGE getImageIndex()
	{
		return m_eImageIndex;
	}
	inline CARD getCardState()
	{
		return m_eCardState;
	}
	inline RECT getCardRect()
	{
		return  m_BitMapRect;
	}
	~Card();
};

