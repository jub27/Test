#pragma once
#include "Card.h"
#include "define.h"

class CardManager
{
private:
	Card  m_CardList[CARD_MAX];
	Card* m_preOpenCard;
public:
	CardManager();
	~CardManager();
	bool CompareCard(int index1, int index2);
	void init();
	void randSetImage();
	void setPosAll();
	int selectCard(POINT mouse);
	void CardClose(int index);
	void drawAll(HDC hdc);
	bool winCheck(HWND hWnd);
	inline Card* getPreOpenCard()
	{
		return m_preOpenCard;
	}
	inline RECT GetCardRect(int index)
	{
		return m_CardList[index].getCardRect();
	}
};