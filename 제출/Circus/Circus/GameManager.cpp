#include "GameManager.h"

GameManager* GameManager::instance = NULL;

GameManager::GameManager()
{
}

void GameManager::Init(HWND hWnd) {
	m_hWnd = hWnd;

	MemDC = CreateCompatibleDC(GetDC(m_hWnd));
	MyBitMap = CreateCompatibleBitmap(GetDC(m_hWnd), 1024, 433);
	OldBitMap = (HBITMAP)SelectObject(MemDC, MyBitMap);

	BackDC = CreateCompatibleDC(MemDC);
	Back_MyBitMap = CreateCompatibleBitmap(MemDC, 1024, 433);
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
	SetFront();
}

void GameManager::OperateInput() {
	if (JumpState)
		return;
	if (GetKeyState(VK_LEFT) & 0x8000) {
		if(PlayerX >= BackgroundMove)
			PlayerX -= 0.7;
	}
	else if (GetKeyState(VK_RIGHT) & 0x8000) {
		if (PlayerX - BackgroundMove >= 200) {
			BackgroundMove += 0.7;
			PlayerX += 0.7;
		}
		else
			PlayerX += 0.7;
	}
	if (GetKeyState(VK_SPACE) & 0x8000) {
			JumpState = true;
			JumpTime = GetTickCount();
	}
	
}

void GameManager::DrawBack() {
	int x = 0;
	for (int i = 0; i < 17; i++) {
		if (i == 16)
			bitmap[BACK_DECO]->Draw(MemDC, (-((int)BackgroundMove % 1106) + i * 65 + 1106) % 1106 - 70, 0);
		else 
			bitmap[BACK_NORMAL]->Draw(MemDC, (-((int)BackgroundMove % 1106) + i * 65 + 1106) % 1106 - 70, 0);
		bitmap[BACK]->StretchDraw(MemDC, i * 67, 64);
	}
}

void GameManager::SetFireRing() {
	static DWORD FireRingTime = GetTickCount();
	for (int i = 0; i < FireRing.size();) {
		FireRing[i] -= 0.3;
		if (((GetKeyState(VK_RIGHT) & 0x8000) || JumpState) && (PlayerX - BackgroundMove >= 200))
			FireRing[i] -= 0.7;
		if (FireRing[i] <= -50)
			FireRing.erase(FireRing.begin());
		else
			i++;
	}
	for (int i = 0; i < FireRing_Gold.size(); i++) {
		FireRing_Gold[i] -= 0.3;
		if (((GetKeyState(VK_RIGHT) & 0x8000) || JumpState) && (PlayerX - BackgroundMove >= 200))
			FireRing_Gold[i] -= 0.7;
		if (FireRing_Gold[i] <= -50)
			FireRing_Gold.erase(FireRing_Gold.begin());
		else
			i++;
	}
	if (GetTickCount() - FireRingTime >= 1000) {
		int random = rand() % 100;
		if (random < 80)
			FireRing.push_back(1024.0f);
		else if (random < 0)
			FireRing_Gold.push_back(1024.0f);
		FireRingTime = GetTickCount();
	}
}

void GameManager::DrawFireRingLeft() {
	for (int i = 0; i < FireRing.size(); i++)
		bitmap[ENEMY_B]->TransparentDraw(MemDC, FireRing[i], 90);
	for (int i = 0; i < FireRing_Gold.size(); i++)
		bitmap[ENEMY_I_B]->TransparentDraw(MemDC, FireRing_Gold[i], 90);
}

void GameManager::DrawFireRingRight() {
	for (int i = 0; i < FireRing.size(); i++)
		bitmap[ENEMY_F]->TransparentDraw(MemDC, FireRing[i] + 25, 90);
	for (int i = 0; i < FireRing_Gold.size(); i++)
		bitmap[ENEMY_I_F]->TransparentDraw(MemDC, FireRing_Gold[i] + 26, 90);
}

void GameManager::DrawMeter() {
	for (int i = 0; i < 11; i++) {
		if (i * 1000 >= BackgroundMove - 86 && i * 1000 <= BackgroundMove + 1024) {
			bitmap[METER]->Draw(MemDC, i * 1000 - BackgroundMove, 360);
			wchar_t buffer[256];
			RECT rt = { i * 1000 - BackgroundMove, 370, i * 1000 + 86 - BackgroundMove , 360 + 30 };
			wsprintfW(buffer, L"%d", (100 - 10 * i));
			DrawText(MemDC, buffer, -1, &rt, DT_CENTER | DT_WORDBREAK);
		}
	}
}

void GameManager::SetFront() {
	int x = 1000;
	while (x <= 8500) {
		x += rand() % 900 + 100;
		Front.push_back(x);
	}
}

void GameManager::DrawFront() {
	static Image img = FRONT;
	static DWORD FrontTime = GetTickCount();
	if (GetTickCount() - FrontTime >= 100) {
		if (img == FRONT)
			img = FRONT2;
		else
			img = FRONT;
		FrontTime = GetTickCount();
	}
	for (int i = 0; i < Front.size(); i++) {
		if(Front[i] - BackgroundMove >= -50 && Front[i] - BackgroundMove <= 1041)
			bitmap[img]->TransparentDraw(MemDC, Front[i] - BackgroundMove, 300);
	}
}

void GameManager::DrawPlayer() {
	static Image img = PLAYER0;
	
	static DWORD PlayerTime = GetTickCount();
	if (GetTickCount() - PlayerTime >= 100) {
		if (JumpState)
			img = PLAYER2;
		else if (! (GetKeyState(VK_RIGHT) & 0x8000) && !(GetKeyState(VK_LEFT) & 0x8000))
			img = PLAYER0;
		else if (img == PLAYER0)
			img = PLAYER1;
		else if (img == PLAYER1)
			img = PLAYER2;
		else
			img = PLAYER0;
		PlayerTime = GetTickCount();
	}
	bitmap[img]->TransparentDraw(MemDC, PlayerX - BackgroundMove, PlayerY - JumpY);
}

bool GameManager::CheckDie() {
	RECT PlayerRect = { PlayerX - BackgroundMove, PlayerY - JumpY, PlayerX - BackgroundMove + 30, PlayerY - JumpY + 30 };
	int x = 0;
	for (int i = 0; i < Front.size(); i++) {
		RECT FrontRect = { Front[i] - BackgroundMove, 300,   Front[i] - BackgroundMove + 30, 300 + 50 };
		RECT temp;
		if (IntersectRect(&temp, &PlayerRect, &FrontRect)) {
			DrawBack();
			DrawFront();
			bitmap[DIE]->TransparentDraw(MemDC, PlayerX - BackgroundMove, PlayerY - JumpY);
			BitBlt(GetDC(m_hWnd), 0, 0, 1041, 768, MemDC, 0, 0, SRCCOPY);
			return true;
		}
	}
	/* //여기 할 차례 FireRing 충돌 판정
	for (int i = 0; i < FireRing.size(); i++) {
		RECT FrontRect = { FireRing[i], 300,   Front[i] - BackgroundMove + 30, 300 + 50 };
		RECT temp;
		if (IntersectRect(&temp, &PlayerRect, &FrontRect)) {
			DrawBack();
			DrawFront();
			bitmap[DIE]->TransparentDraw(MemDC, PlayerX - BackgroundMove, PlayerY - JumpY);
			BitBlt(GetDC(m_hWnd), 0, 0, 1041, 768, MemDC, 0, 0, SRCCOPY);
			return true;
		}
	}
	for (int i = 0; i < FireRing_Gold.size(); i++) {
		RECT FrontRect = { Front[i] - BackgroundMove, 300,   Front[i] - BackgroundMove + 30, 300 + 50 };
		RECT temp;
		if (IntersectRect(&temp, &PlayerRect, &FrontRect)) {
			DrawBack();
			DrawFront();
			bitmap[DIE]->TransparentDraw(MemDC, PlayerX - BackgroundMove, PlayerY - JumpY);
			BitBlt(GetDC(m_hWnd), 0, 0, 1041, 768, MemDC, 0, 0, SRCCOPY);
			return true;
		}
	}
	*/
	return false;
}

void GameManager::Jump() {
	if (!JumpState)
		return;
	if (GetTickCount() - JumpTime <= 1000) {
		if (PlayerX - BackgroundMove >= 200) {
			BackgroundMove += 0.7;
			PlayerX += 0.7;
		}
		else
			PlayerX += 0.7;
		JumpY = sin((GetTickCount() - JumpTime) / 1000.0f * PI) * 180;
	}
	else 
		JumpState = false;
}

void GameManager::Update() {

}

void GameManager::Render() {
	HDC hdc = GetDC(m_hWnd);
	DrawBack();
	OperateInput();
	Jump();
	DrawFront();
	DrawMeter();
	SetFireRing();
	DrawFireRingLeft();
	DrawPlayer();
	DrawFireRingRight();
	BitBlt(hdc, 0, 0, 1041, 433, MemDC, 0, 0, SRCCOPY);
	ReleaseDC(m_hWnd, hdc);
}

GameManager::~GameManager()
{
}