#include "GameManager.h"

GameManager * GameManager::gameManager = NULL;

GameManager::GameManager()
{
	
}

void GameManager::Init(HWND hWnd) {
	m_hWnd = hWnd;

	MemDC = CreateCompatibleDC(GetDC(m_hWnd));
	MyBitmap = CreateCompatibleBitmap(GetDC(m_hWnd), 1024, 533);
	OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);

	for (int i = 0; i < BITMAP_NUMS; i++) {
		bitmap[i] = new Bitmap();
		bitmap[i]->Init(MemDC, resource[i]);
	}
	difficulty = 0;

	startX[0] = START_X_0;
	startX[1] = START_X_1;
	startX[2] = START_X_2;

	startY[0] = START_Y_0;
	startY[1] = START_Y_1;
	startY[2] = START_Y_2;

}

void GameManager::NewGame() {
	int mine = 0;
	int mine_x = -1, mine_y = -1;
	InitMap();
	switch (difficulty) {
	case 0:
		mine = 10;
		curMine = mine;
		while (mine > 0) {
			mine_x = rand() % 9;
			mine_y = rand() % 9;
			if (!map[mine_y][mine_x].mine) {
				map[mine_y][mine_x].mine = true;
				mine--;
			}
			if(mine_x == 0)
				
		}
		break;
	case 1:
		mine = 40;
		curMine = mine;
		while (mine > 0) {
			mine_x = rand() % 16;
			mine_y = rand() % 16;
			if (!map[mine_y][mine_x].mine) {
				map[mine_y][mine_x].mine = true;
				mine--;
			}
		}
		break;
	case 2:
		mine = 99;
		curMine = mine;
		while (mine > 0) {
			mine_x = rand() % 30;
			mine_y = rand() % 16;
			if (!map[mine_y][mine_x].mine) {
				map[mine_y][mine_x].mine = true;
				mine--;
			}
		}
		break;
	}
}

void GameManager::AddNearMine(int x, int y) {
	if( x < 0;)
}

void GameManager::InitMap() {
	for (int i = 0; i < 16; i++)
		for (int j = 0; j < 30; j++) {
			map[i][j].mine = false;
			map[i][j].push = false;
			map[i][j].flag = false;
			map[i][j].nearMine = 0;
		}
}

void GameManager::Draw() {
	DrawBack();
	DrawBlocks();
	BitBlt(GetDC(m_hWnd), 0, 0, 870, 512, MemDC, 0, 0, SRCCOPY);
}

void GameManager::DrawBack() {
	bitmap[BACK]->Draw(MemDC, 0, 0);
}

void GameManager::DrawBlocks() {
	switch (difficulty) {
	case 0:
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++) {
				if (!map[i][j].push) {
					if(map[i][j].flag)
						bitmap[FLAG]->Draw(MemDC, START_X_0 + 26 * j, START_Y_0 + 26 * i);
					else
						bitmap[BLOCK]->Draw(MemDC, START_X_0 + 26 * j, START_Y_0 + 26 * i);
				}
				else if(!map[i][j].mine)
					bitmap[BLOCK0]->Draw(MemDC, START_X_0 + 26 * j, START_Y_0 + 26 * i);
				else
					bitmap[MINE]->Draw(MemDC, START_X_0 + 26 * j, START_Y_0 + 26 * i);
			}
		break;
	case 1:
		for (int i = 0; i < 16; i++)
			for (int j = 0; j < 16; j++) {
				if (!map[i][j].push) {
					if (map[i][j].flag)
						bitmap[FLAG]->Draw(MemDC, START_X_1 + 26 * j, START_Y_1 + 26 * i);
					else
						bitmap[BLOCK]->Draw(MemDC, START_X_1 + 26 * j, START_Y_1 + 26 * i);
				}
				else if (!map[i][j].mine)
					bitmap[BLOCK0]->Draw(MemDC, START_X_1 + 26 * j, START_Y_1 + 26 * i);
				else
					bitmap[MINE]->Draw(MemDC, START_X_1 + 26 * j, START_Y_1 + 26 * i);
			}
		break;
	case 2:
		for (int i = 0; i < 16; i++)
			for (int j = 0; j < 30; j++) {
				if (!map[i][j].push) {
					if (map[i][j].flag)
						bitmap[FLAG]->Draw(MemDC, START_X_2 + 26 * j, START_Y_2 + 26 * i);
					else
						bitmap[BLOCK]->Draw(MemDC, START_X_2 + 26 * j, START_Y_2 + 26 * i);
				}
				else if (!map[i][j].mine)
					bitmap[BLOCK0]->Draw(MemDC, START_X_2 + 26 * j, START_Y_2 + 26 * i);
				else
					bitmap[MINE]->Draw(MemDC, START_X_2 + 26 * j, START_Y_2 + 26 * i);
			}
		break;
	}
}

void GameManager::LeftClick(int clickX, int clickY) {
	int x = (clickX - startX[difficulty]) / 26;
	int y = (clickY - startY[difficulty]) / 26;
	if (x < 0 || y < 0)
		return;
	if (difficulty == 0) {
		if (x >= 9 || y >= 9)
			return;
	}
	else if (difficulty == 1) {
		if (x >= 16 || y >= 16)
			return;
	}
	else {
		if (x >= 30 || y >= 16)
			return;
	}
	if (!map[y][x].push)
		map[y][x].push = true;
}

void GameManager::RightClick(int clickX, int clickY) {
	int x = (clickX - startX[difficulty]) / 26;
	int y = (clickY - startY[difficulty]) / 26;
	if (x < 0 || y < 0)
		return;
	if (difficulty == 0) {
		if (x >= 9 || y >= 9)
			return;
	}
	else if (difficulty == 1) {
		if (x >= 16 || y >= 16)
			return;
	}
	else {
		if (x >= 30 || y >= 16)
			return;
	}
	map[y][x].flag = !map[y][x].flag;
}

void GameManager::SetDifficulty(int dif) {
	difficulty = dif;
	NewGame();
}

GameManager::~GameManager()
{
}
