#include "GameManager.h"

GameManager::GameManager()
{
	m_startTextRect = { 230,230,350,320 };
	m_selectPreCardNum = CARD_NONE;
	m_bStopTimer = false;
	m_bStartFlag = false;
}

GameManager::~GameManager()
{
}

void GameManager::init(HWND hWnd)
{
	BitMapManager::GetInstance()->Init(hWnd);
	m_cardManager.init();
}

void GameManager::draw(HDC hdc)
{
	if (m_bStartFlag == false)
	{
		Rectangle(hdc, m_startTextRect.left, m_startTextRect.top, m_startTextRect.right, m_startTextRect.bottom);
		SetTextAlign(hdc, TA_CENTER);
		TextOut(hdc, (m_startTextRect.left + m_startTextRect.right) / 2, (m_startTextRect.top + m_startTextRect.bottom) / 2, L"게임 시작", lstrlen(L"게임 시작"));
	}

	if (m_bStartFlag == true)
	{
		//m_backGround->Draw(hdc, 0, 0, 1.5, 1);
		m_cardManager.drawAll(hdc);
	}
}

void GameManager::playGame(HWND hWnd, POINT Point)
{
	if (m_bStartFlag == false)	// 시작 메뉴
	{
		InvalidateRect(hWnd, NULL, true);

		if (PtInRect(&m_startTextRect, Point))
			m_bStartFlag = true;
	}
	else if (m_bStopTimer == false)	// 게임 시작
	{
		m_selectCurCardNum = m_cardManager.selectCard(Point);

		if(m_selectCurCardNum != CARD_NONE)
		{
			InvalidateRect(hWnd, &m_cardManager.GetCardRect(m_selectCurCardNum), true);
			if(m_selectPreCardNum == CARD_NONE)
				m_selectPreCardNum = m_selectCurCardNum;
			else
			{
				if(m_cardManager.CompareCard(m_selectPreCardNum, m_selectCurCardNum))
				{
					m_selectPreCardNum = CARD_NONE;
					m_selectCurCardNum = CARD_NONE;
					if (m_cardManager.winCheck(hWnd))
					{
						if (MessageBox(hWnd, L"게임 승리!", L"승리", MB_OK) == IDOK)
							SendMessage(hWnd, WM_DESTROY, 0, 0);
					}
				}
				else
				{
					m_bStopTimer = true;
					SetTimer(hWnd, 1, 1000, NULL);
				}
			}
		}
	}
}

void GameManager::CloseCard(HWND hWnd)
{
	m_cardManager.CardClose(m_selectPreCardNum);
	m_cardManager.CardClose(m_selectCurCardNum);
	m_bStopTimer = false;
	InvalidateRect(hWnd, &m_cardManager.GetCardRect(m_selectCurCardNum), true);
	InvalidateRect(hWnd, &m_cardManager.GetCardRect(m_selectPreCardNum), true);
	m_selectPreCardNum = CARD_NONE;
	m_selectCurCardNum = CARD_NONE;
}
