#include "Card.h"

Card::Card()
{
	m_eCardState = CARD_CLOSE;
}

void Card::setImage(IMAGE Index)
{
	m_pBitMap[CARD_OPEN] = BitMapManager::GetInstance()->GetImage(Index);
	m_pBitMap[CARD_CLOSE] = BitMapManager::GetInstance()->GetImage(IMAGE_BLACK);
	m_eImageIndex = Index;
}

void Card::setPos(int x, int y, float spX, float spY)
{
	m_ix = x;
	m_iy = y;
	m_fSpX = spX;
	m_fSpY = spY;
	m_BitMapRect.left = x;
	m_BitMapRect.top = y;
	m_BitMapRect.right = m_BitMapRect.left + m_pBitMap[CARD_OPEN]->GetSize().cx * m_fSpX;
	m_BitMapRect.bottom = m_BitMapRect.top + m_pBitMap[CARD_OPEN]->GetSize().cy * m_fSpY;
}

void Card::draw(HDC hdc)
{
	m_pBitMap[m_eCardState]->Draw(hdc, m_ix, m_iy, m_fSpX, m_fSpY);
}

bool Card::cardOpen(POINT point)
{
	if(PtInRect(&m_BitMapRect, point))
	{
		if(m_eCardState == CARD_CLOSE)
		{
			m_eCardState = CARD_OPEN;
			return true;
		}
	}
	return false;
}

void Card::cardClose()
{
	m_eCardState = CARD_CLOSE;
}

Card::~Card()
{
}
