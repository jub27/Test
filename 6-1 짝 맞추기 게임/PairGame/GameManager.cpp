#include "Gamemanager.h"
#include"resource.h"
Gamemanager* Gamemanager::m_pThis = NULL;

Gamemanager::Gamemanager()
{
}
void Gamemanager::Init(HWND hWnd, HINSTANCE hInst)
{
	HDC hdc = GetDC(hWnd);
	for (int i = 0, r = IDB_BITMAP1; i < BITMAP_NUMS; i += 2, r++) {
		m_bt[i] = new Bitmap;
		m_bt[i+1] = new Bitmap;
		m_bt[i]->Init(hdc, hInst, r);
		m_bt[i + 1]->Init(hdc, hInst, r);
	}
	back = new Bitmap();
	back->Init(hdc, hInst, IDB_BITMAP11);
}
void Gamemanager::Draw(HDC hdc)
{
	for (int i = 0; i < BITMAP_NUMS; i++) {
		if(m_bt[i]->isOpend())
			m_bt[i]->Draw(hdc, BITMAP_WIDTH * (i % 10), BITMAP_HEIGHT * (i / 10));
		else {
			back->Draw(hdc, BITMAP_WIDTH * (i % 10), BITMAP_HEIGHT * (i / 10));
		}
	}
}

int Gamemanager::Click(int x, int y) {
	if (curClick != -1 && lastClick != -1)
		return 0;
	curClick = x / BITMAP_WIDTH + y / BITMAP_HEIGHT * 10;
	if (curClick >= BITMAP_NUMS)
		return 0;
	if (m_bt[curClick]->isOpend()) {
		curClick = -1;
		return 0;
	}
	m_bt[curClick]->Open();
	if (clickFlag) {
		if (m_bt[lastClick]->GetResource() == m_bt[curClick]->GetResource()) {
			clickFlag = false;
			lastClick = -1;
			curClick = -1;
			score++;
			if (score == 10)
				return 4;
			else
				return 2;
		}
		else {
			clickFlag = false;
			return 3;
		}
	}
	else {
		lastClick = curClick;
		curClick = -1;
		clickFlag = true;
		return 1;
	}
}

void Gamemanager::CloseCurLast() {
	m_bt[curClick]->Close();
	m_bt[lastClick]->Close();
	curClick = -1;
	lastClick = -1;
}

void Gamemanager::Shuffle() {
	for (int i = 0; i < BITMAP_NUMS; i++) {
		m_bt[i]->Close();
	}
	for (int i = 0; i < BITMAP_NUMS; i++) {
		Bitmap * tmp;
		tmp = m_bt[i];
		int j = rand() % 20;
		m_bt[i] = m_bt[j];
		m_bt[j] = tmp;
	}
	clickFlag = false;
	lastClick = -1;
	curClick = -1;
	score = 0;
}

void Gamemanager::Release()
{
	delete(m_pThis);
}

Gamemanager::~Gamemanager()
{
	for (int i = 0; i < BITMAP_NUMS; i++)
		delete m_bt[i];
}
