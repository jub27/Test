#pragma once
#include "define.h"
#include "CardManager.h"

class GameManager
{
private:
	CardManager m_cardManager;
	BitMap*		m_backGround;
	int			m_selectCurCardNum;
	int			m_selectPreCardNum;
	RECT		m_selectCurCardRect;
	RECT		m_selectPreCardRect;
	RECT        m_startTextRect;
	bool		m_bStopTimer;
	bool		m_bStartFlag;
public:
	GameManager();
	~GameManager();
	void CloseCard(HWND hWnd);
	void init(HWND hWnd);
	void draw(HDC hdc);
	void playGame(HWND hWnd, POINT Point);
};

