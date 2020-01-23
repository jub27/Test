#include "GameManager.h"

GameManager* GameManager::instance = NULL;

GameManager::GameManager()
{
}

void GameManager::Init(HWND hWnd) {
	m_hWnd = hWnd;

	MemDC = CreateCompatibleDC(GetDC(m_hWnd));
	MyBitMap = CreateCompatibleBitmap(GetDC(m_hWnd), 10100, 433);
	OldBitMap = (HBITMAP)SelectObject(MemDC, MyBitMap);

	BackDC = CreateCompatibleDC(MemDC);
	Back_MyBitMap = CreateCompatibleBitmap(MemDC, 10100, 433);
	Back_OldBitMap = (HBITMAP)SelectObject(BackDC, Back_MyBitMap);

	for (int i = 0; i < BITMAP_NUMS; i++) {
		bitmap[i] = new Bitmap();
		bitmap[i]->Init(MemDC, source[i]);
	}

	LastTime = GetTickCount();
	CurTime = GetTickCount();
	PlayerX = 0;
	PlayerY = 285;
	JumpX = 0;
	JumpY = 0;
	JumpState = false;
	BackgroundMove = 0;
	FrontMove = 0;
	SetBack();
	//SetFront();
}

void GameManager::OperateInput() {
	if (JumpState)
		return;
	if (GetKeyState(VK_LEFT) & 0x8000) {
		if(PlayerX >= BackgroundMove)
			PlayerX -= 0.3;
	}
	if (GetKeyState(VK_RIGHT) & 0x8000) {
		if (PlayerX - BackgroundMove >= 200) {
			if (BackgroundMove < 10100 - 1024)
				BackgroundMove += 1;
			PlayerX += 1;
		}
		else
			PlayerX += 1;
	}
	if (GetKeyState(VK_SPACE) & 0x8000) {
			JumpState = true;
			JumpTime = GetTickCount();
	}
	
}

void GameManager::SetBack() {
	int x = 0;
	for (int i = 0; i < 200; i++) {
		if (i % 10 == 0) {
			bitmap[BACK_DECO]->Draw(BackDC, x, 0);
			x += 66;
		}
		else {
			bitmap[BACK_NORMAL]->Draw(BackDC, x, 0);
			x += 65;
		}
		bitmap[BACK]->StretchDraw(BackDC, i * 67, 64);
	}
	for (int i = 0; i < 11; i++) {
		bitmap[METER]->Draw(BackDC, i * 1000, 360);
		wchar_t buffer[256];
		RECT rt = { i * 1000, 370, i * 1000 + 86 , 360 + 30 };
		wsprintfW(buffer, L"%d", (100 - 10*i));
		DrawText(BackDC, buffer, -1, &rt, DT_CENTER | DT_WORDBREAK);
	}
	bitmap[END]->TransparentDraw(BackDC, 10000, 300);
}

void GameManager::SetFront() {
	int x = 1000;
	while (x <= 8500) {
		x += rand() % 900 + 100;
		Front.push_back(x);
	}
}

void GameManager::UpdateFront() {
	static Image img = FRONT;
	static DWORD FrontTime = GetTickCount();
	if (GetTickCount() - FrontTime >= 100) {
		if (img == FRONT)
			img = FRONT2;
		else
			img = FRONT;
		FrontTime = GetTickCount();
	}
	for (int i = 0; i < Front.size(); i++)
		bitmap[img]->TransparentDraw(MemDC, Front[i] , 300);
}

void GameManager::DrawPlayer() {
	static Image img = PLAYER0;
	
	static DWORD PlayerTime = GetTickCount();
	if (GetTickCount() - PlayerTime >= 100) {
		if (img == PLAYER0)
			img = PLAYER1;
		else if (img == PLAYER1)
			img = PLAYER2;
		else
			img = PLAYER0;
		PlayerTime = GetTickCount();
	}
	bitmap[img]->TransparentDraw(MemDC, PlayerX, PlayerY - JumpY);
}

bool GameManager::CheckDie() {
	RECT PlayerRect = { PlayerX, PlayerY - JumpY, PlayerX + 30, PlayerY - JumpY + 30 };
	int x = 0;
	for (int i = 0; i < Front.size(); i++) {
		RECT FrontRect = { Front[i], 300,   Front[i] + 30, 300 + 50 };
		RECT temp;
		if (IntersectRect(&temp, &PlayerRect, &FrontRect)) {
			bitmap[DIE]->TransparentDraw(MemDC, PlayerX, PlayerY - JumpY);
			BitBlt(GetDC(m_hWnd), 0, 0, 1024, 768, MemDC, BackgroundMove, 0, SRCCOPY);
			return true;
		}
	}
	return false;
}

void GameManager::Jump() {
	if (!JumpState)
		return;
	if (GetTickCount() - JumpTime <= 1000) {
		if (PlayerX - BackgroundMove >= 200) {
			if (BackgroundMove < 10100 - 1024)
				BackgroundMove += 1;
			PlayerX += 1;
		}
		else
			PlayerX += 1;
		JumpY = sin((GetTickCount() - JumpTime) / 1000.0f * PI) * 150;
	}
	else 
		JumpState = false;
}

void GameManager::Render() {
	BitBlt(MemDC, 0, 0, 1024 + BackgroundMove, 433, BackDC, BackgroundMove, 0, SRCCOPY);
	OperateInput();
	Jump();
	UpdateFront();
	DrawPlayer();
	BitBlt(GetDC(m_hWnd), 0, 0, 1024, 433, MemDC, BackgroundMove, 0, SRCCOPY);/////////////////////////////
}

GameManager::~GameManager()
{
}