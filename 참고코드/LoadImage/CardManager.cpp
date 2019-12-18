#include "CardManager.h"

CardManager::CardManager()
{
	m_preOpenCard = NULL;	// 이전에 열었던 카드
}

CardManager::~CardManager()
{
}

void CardManager::init()
{
	randSetImage();
	setPosAll();
}

void CardManager::randSetImage()
{
	int randCard[20];
	int arrNum = 0;
	int randSize = 20;

	while (arrNum < randSize)					// 중복이 아닌 난수 20개가 생성될 때까지의 반복문
	{
		bool doubleCheck = false;
		int randNum = rand() % randSize;		// 1부터 NUM 갯수만큼 난수 생성

		for (int i = 0; i <= arrNum; i++)		// 이전에 생성된 값과 중복 확인을 위한 for문
		{
			if (randCard[i] == randNum)			// 중복이 있다면?
			{
				doubleCheck = true;				// YES로, for 반복문을 빠져 나옴
				break;
			}
		}
		if (doubleCheck == false)				// 전부 확인 결과 중복이 없다면?
		{
			randCard[arrNum] = randNum;		    // 현재 배열의 원소에 Lotto 값 입력
			arrNum++;							// 배열의 다음 주소로 이동
		}
	}

	for (int i = 0; i < 20; i++)	
	{	
		if (randCard[i] >= 10)
			randCard[i] = randCard[i] - 10;	// 이미지는0~9개 뿐이므로 10~19는 중복카드 한개 더 생성
		m_CardList[i].setImage((IMAGE)randCard[i]);
	}
}

void CardManager::setPosAll() // 카드리스트 배치
{
	for (int i = 0; i < 5; i++)		
		m_CardList[i].setPos(50 + (100 * i), 10);
	for (int i = 0; i < 5; i++)
		m_CardList[i + 5].setPos(50 + (100 * i), 140);
	for (int i = 0; i < 5; i++)
		m_CardList[i + 10].setPos(50 + (100 * i), 270);
	for (int i = 0; i < 5; i++)
		m_CardList[i + 15].setPos(50 + (100 * i), 400);
}

int CardManager::selectCard(POINT mouse)
{
	bool check = false;

	for (int i = 0; i < 20; i++)
	{
		check = m_CardList[i].cardOpen(mouse);	// 현재 마우스 위치의 카드가 닫혀 있으면 열음

		if (check == true)	// 현재 마우스 위치의 카드를 열었다면?
		{
			if (m_preOpenCard == NULL)	// 만약 이전에 열은 카드가 없으면
				m_preOpenCard = &m_CardList[i];
			return i;	// 선택한 카드의 index 반환
		}
	}
	return CARD_NONE;	// 마우스 포인터가 카드 범위안에 없음을 반환
}

bool CardManager::CompareCard(int index1, int index2)
{
	if(index1 == index2)
		return false;
	if(m_CardList[index1].getImageIndex() == m_CardList[index2].getImageIndex())
		return true;
	else
		return false;
}

void CardManager::CardClose(int index)
{
	m_CardList[index].cardClose();
}

void CardManager::drawAll(HDC hdc)
{
	for (int i = 0; i < 20; i++)
		m_CardList[i].draw(hdc);
}

bool CardManager::winCheck(HWND hWnd)
{
	for (int i = 0; i < 20; i++)
	{
		if (m_CardList[i].getCardState() == CARD_CLOSE)
			return false;
	}
	return true;
}
