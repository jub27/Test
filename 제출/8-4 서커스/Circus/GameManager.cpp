#include "GameManager.h"
#include <iostream>
#include <conio.h>
using namespace std;

GameManager* GameManager::instance = NULL;

GameManager::GameManager()
{
}

void GameManager::Init(HWND hWnd) {
	m_hWnd = hWnd;

	MemDC = CreateCompatibleDC(GetDC(m_hWnd));
	MyBitMap = CreateCompatibleBitmap(GetDC(m_hWnd), 1024, 533);
	OldBitMap = (HBITMAP)SelectObject(MemDC, MyBitMap);

	for (int i = 0; i < BITMAP_NUMS; i++) {
		bitmap[i] = new Bitmap();
		bitmap[i]->Init(MemDC, source[i]);
	}

	LastTime = GetTickCount();
	CurTime = GetTickCount();
	PlayerX = 0;
	PlayerY = 385;
	JumpX = 0;
	JumpY = 0;
	JumpState = false;
	BackgroundMove = 0;
	FrontMove = 0;

	score = 0;

	FireRingTime = GetTickCount();
	FrontTime = GetTickCount();
	PlayerTime = GetTickCount();
	TitleTime = GetTickCount();

	Enemy.clear();
	Front.clear();
	FireRing.clear();
	FireRing_Gold.clear();
	TakeMoney.clear();

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
			if (BackgroundMove < 10086 - 1024)
				BackgroundMove += 0.7;
			if( PlayerX <= 10000 - 20)
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

int GameManager::Title() {
	Bitmap * stars[3] = { bitmap[STAR], bitmap[STAR1], bitmap[STAR2] };
	static int t = 0;
	static int op = 0;
	bitmap[TITLE]->Draw(MemDC, 0, 0);

	RECT rt = rt = { 400, 100, 500, 150 };
	DrawText(MemDC, L"circus charlie", -1, &rt, DT_CENTER | DT_WORDBREAK);

	rt = { 400, 150, 500, 165 };
	DrawText(MemDC, L"START", -1, &rt, DT_CENTER | DT_WORDBREAK);

	rt = { 400, 180, 500, 195 };
	DrawText(MemDC, L"EXIT", -1, &rt, DT_CENTER | DT_WORDBREAK);

	if (GetTickCount() - TitleTime >= 100) {
		t = (t + 1) % 3;
		TitleTime = GetTickCount();
	}

	if (GetKeyState(VK_UP) & 0x8000) {
		op = 0;
	}
	else if (GetKeyState(VK_DOWN) & 0x8000) {
		op = 1;
	}

	if (op == 0) {
		stars[0]->TransparentDraw(MemDC, 370, 150);
	}
	else if (op == 1) {
		stars[0]->TransparentDraw(MemDC, 370, 180);
	}

	for (int i = 0; i < 10; i++) {
		stars[(i + t + 1) % 3]->TransparentDraw(MemDC, 375 + i * 15, 90);
		stars[(i + t) % 3]->TransparentDraw(MemDC, 375 + i * 15, 115);
	}

	stars[t]->TransparentDraw(MemDC, 375, 102);
	stars[(t + 1) % 3]->TransparentDraw(MemDC, 510, 102);
	if (GetKeyState(VK_RETURN) & 0x8000) {
		if (op == 0) {
			return 0;
		}
		else if (op == 1)
			return 1;
	}
	BitBlt(GetDC(m_hWnd), 0, 0, 1024, 533, MemDC, 0, 0, SRCCOPY);
	return -1;
}

void GameManager::DrawLifeScore() {
	wchar_t buffer[256];
	wsprintfW(buffer, L"%d", score);
	bitmap[UI]->Draw(MemDC, 0, 0);
	RECT rt = { 100, 50, 150, 65 };
	DrawText(MemDC, L"Score : ", -1, &rt, DT_CENTER | DT_WORDBREAK);
	rt = { 150, 50, 200, 65 };
	DrawText(MemDC, buffer, -1, &rt, DT_CENTER | DT_WORDBREAK);
	rt = { 500, 50, 550, 65 };
	DrawText(MemDC, L"Life : ", -1, &rt, DT_CENTER | DT_WORDBREAK);
	for (int i = 0; i < life; i++) {
		bitmap[ICON]->TransparentDraw(MemDC, 550 + 15 * i, 50);
	}
}

void GameManager::DrawBack() {
	int x = 0;
	for (int i = 0; i < 17; i++) {
		if (i == 16)
			bitmap[BACK_DECO]->Draw(MemDC, (-((int)BackgroundMove % 1106) + i * 65 + 1106) % 1106 - 70, 100);
		else 
			bitmap[BACK_NORMAL]->Draw(MemDC, (-((int)BackgroundMove % 1106) + i * 65 + 1106) % 1106 - 70, 100);
		bitmap[BACK]->StretchDraw(MemDC, i * 67, 164, 2);
	}
}

void GameManager::SetFireRing() {
	for (int i = 0; i < FireRing.size();) {
		FireRing[i] -= 0.3;
		if (((GetKeyState(VK_RIGHT) & 0x8000) || JumpState) && (PlayerX - BackgroundMove >= 200) && BackgroundMove < 10086 - 1024)
			FireRing[i] -= 0.7;
		if (FireRing[i] <= -50)
			FireRing.erase(FireRing.begin());
		else
			i++;
	}
	
	for (int i = 0; i < FireRing_Gold.size();) {
		FireRing_Gold[i] -= 0.3;
		if (((GetKeyState(VK_RIGHT) & 0x8000) || JumpState) && (PlayerX - BackgroundMove >= 200) && BackgroundMove < 10086 - 1024)
			FireRing_Gold[i] -= 0.7;
		if (FireRing_Gold[i] <= -50) {
			FireRing_Gold.erase(FireRing_Gold.begin());
			TakeMoney.erase(TakeMoney.begin());
		}
		else
			i++;
	}
	
	if (GetTickCount() - FireRingTime >= 1000 && BackgroundMove < 10086 - 1024) {
		int random = rand() % 100;
		if (random < 40)
			FireRing.push_back(1024.0f);
		else if (random < 60) {
			TakeMoney.push_back(false);
			FireRing_Gold.push_back(1024.0f);
		}
		FireRingTime = GetTickCount();
	}
}

void GameManager::DrawFireRingLeft() {
	for (int i = 0; i < FireRing.size(); i++)
		bitmap[ENEMY_B]->TransparentDraw(MemDC, FireRing[i], 190);
	for (int i = 0; i < FireRing_Gold.size(); i++) {
		if (TakeMoney[i])
			bitmap[ENEMY_I_B_2]->TransparentDraw(MemDC, FireRing_Gold[i], 190);
		else
			bitmap[ENEMY_I_B]->TransparentDraw(MemDC, FireRing_Gold[i], 190);
	}
}

void GameManager::DrawFireRingRight() {
	for (int i = 0; i < FireRing.size(); i++)
		bitmap[ENEMY_F]->TransparentDraw(MemDC, FireRing[i] + 25, 190);
	for (int i = 0; i < FireRing_Gold.size(); i++) {
		if (TakeMoney[i])
			bitmap[ENEMY_I_F_2]->TransparentDraw(MemDC, FireRing_Gold[i] + 26, 190);
		else
			bitmap[ENEMY_I_F]->TransparentDraw(MemDC, FireRing_Gold[i] + 26, 190);
	}
}

void GameManager::DrawMeter() {
	for (int i = 0; i < 11; i++) {
		if (i * 1000 >= BackgroundMove - 86 && i * 1000 <= BackgroundMove + 1024) {
			bitmap[METER]->Draw(MemDC, i * 1000 - BackgroundMove, 460);
			wchar_t buffer[256];
			RECT rt = { i * 1000 - BackgroundMove, 470, i * 1000 + 86 - BackgroundMove , 470 + 20 };
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
	if (GetTickCount() - FrontTime >= 100) {
		if (img == FRONT)
			img = FRONT2;
		else
			img = FRONT;
		FrontTime = GetTickCount();
	}
	for (int i = 0; i < Front.size(); i++) {
		if(Front[i] - BackgroundMove >= -50 && Front[i] - BackgroundMove <= 1041)
			bitmap[img]->TransparentDraw(MemDC, Front[i] - BackgroundMove, 400);
	}
}

void GameManager::DrawPlayer() {
	static Image img = PLAYER0;
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

int GameManager::CheckPlayer() {
	RECT PlayerRect = { PlayerX, PlayerY - JumpY, PlayerX + 30, PlayerY - JumpY + 30};
	int x = 0;
	RECT temp;
	for (int i = 0; i < Front.size(); i++) {
		RECT FrontRect = { Front[i], 300 + 100,   Front[i]+ 30, 300 + 50 + 100 };
		if (IntersectRect(&temp, &PlayerRect, &FrontRect)) {
			Die();
			return 0;
		}
	}

	for (int i = 0; i < FireRing.size(); i++) {
		RECT FireRect1 = { FireRing[i] + BackgroundMove , 190,   FireRing[i] + BackgroundMove + 32, 190 + 15};
		RECT FireRect2 = { FireRing[i] + BackgroundMove , 190 + 110,   FireRing[i] + BackgroundMove + 32, 190 + 110 + 15 };
		if (IntersectRect(&temp, &PlayerRect, &FireRect1) || IntersectRect(&temp, &PlayerRect, &FireRect2)) {
			Die();
			return 0;
		}
	}
	
	for (int i = 0; i < FireRing_Gold.size(); i++) {
		RECT FireRect1 = { FireRing_Gold[i] + BackgroundMove, 190,   FireRing_Gold[i] + BackgroundMove + 32, 190 + 15 };
		RECT GoldRect = { FireRing_Gold[i] + BackgroundMove, 205, FireRing_Gold[i] + BackgroundMove + 32, 274 };
		RECT FireRect2 = { FireRing_Gold[i] + BackgroundMove , 190 + 84,   FireRing_Gold[i] + BackgroundMove + 32, 190 + 84 + 15 };
		if (IntersectRect(&temp, &PlayerRect, &GoldRect)) {
			if (TakeMoney[i] == false) {
				score += 100;
				TakeMoney[i] = true;
			}
		}
		if (IntersectRect(&temp, &PlayerRect, &FireRect1) || IntersectRect(&temp, &PlayerRect, &FireRect2)) {
			Die();
			return 0;
		}
	}
	
	RECT EndRect = { 10010, 400, 10086, 400 + 1};
	if (IntersectRect(&temp, &PlayerRect, &EndRect)) {
		DrawBack();
		DrawFront();
		DrawFireRingLeft();
		DrawEnd();
		bitmap[WIN]->TransparentDraw(MemDC, 1024 - 38 - 33, 400 - 49);
		DrawFireRingRight();
		BitBlt(GetDC(m_hWnd), 0, 0, 1024, 533, MemDC, 0, 0, SRCCOPY);
		return 1;
	}
	return 2;
}

void GameManager::Jump() {
	if (!JumpState)
		return;
	if (GetTickCount() - JumpTime <= 1000) {
		if (PlayerX - BackgroundMove >= 200) {
			if (BackgroundMove < 10086 - 1024)
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

void GameManager::DrawEnd() {
	bitmap[END]->TransparentDraw(MemDC, 10086 - BackgroundMove - 76, 400);
}

int GameManager::GetLife() {
	return life;
}

void GameManager::InitLife() {
	life = 3;
}

void GameManager::Die() {
	life--;
	DrawBack();
	DrawFront();
	DrawFireRingLeft();
	bitmap[DIE]->TransparentDraw(MemDC, PlayerX - BackgroundMove, PlayerY - JumpY );
	DrawFireRingRight();
	BitBlt(GetDC(m_hWnd), 0, 0, 1024, 533, MemDC, 0, 0, SRCCOPY);
}

void GameManager::Release(){
	SelectObject(MemDC, OldBitMap);
	DeleteObject(MyBitMap);
	DeleteDC(MemDC);
}

void GameManager::Render() {
	HDC hdc = GetDC(m_hWnd);
	DrawLifeScore();
	DrawBack();
	OperateInput();
	Jump();
	DrawFront();
	DrawMeter();
	SetFireRing();
	DrawFireRingLeft();
	DrawPlayer();
	DrawFireRingRight();
	DrawEnd();
	BitBlt(hdc, 0, 0, 1024, 533, MemDC, 0, 0, SRCCOPY);
	ReleaseDC(m_hWnd, hdc);
}

GameManager::~GameManager()
{
}