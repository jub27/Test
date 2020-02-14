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
	mine = mineNumByDifficulty[difficulty];
	curMine = mine;
	gameStart = false;
	while (mine > 0) {
		mine_x = rand() % width[difficulty];
		mine_y = rand() % height[difficulty];
		if (!map[mine_y][mine_x].mine) {
			map[mine_y][mine_x].mine = true;
			mine--;
		}
		else 
			continue;
		AddNearMine(mine_x - 1, mine_y + 1);
		AddNearMine(mine_x, mine_y + 1);
		AddNearMine(mine_x + 1, mine_y + 1);
		AddNearMine(mine_x + 1, mine_y);
		AddNearMine(mine_x + 1, mine_y - 1);
		AddNearMine(mine_x, mine_y - 1);
		AddNearMine(mine_x - 1, mine_y - 1);
		AddNearMine(mine_x - 1, mine_y);
	}
}

void GameManager::AddNearMine(int x, int y) {
	if (x < 0 || x >= width[difficulty] || y < 0 || y >= height[difficulty])
		return;
	map[y][x].nearMine++;
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
	DrawCurMineNume();
}

void GameManager::DrawBack() {
	bitmap[BACK]->Draw(MemDC, 0, 0);
}

void GameManager::DrawBlocks() {
	for (int i = 0; i < height[difficulty]; i++)
		for (int j = 0; j < width[difficulty]; j++) {
			if (!map[i][j].push) {
				if (map[i][j].flag)
					bitmap[FLAG]->Draw(MemDC, startX[difficulty] + 26 * j, startY[difficulty] + 26 * i);
				else
					bitmap[BLOCK]->Draw(MemDC, startX[difficulty] + 26 * j, startY[difficulty] + 26 * i);
			}
			else if (!map[i][j].mine) {
				if (map[i][j].nearMine > 0)
					bitmap[map[i][j].nearMine + 2]->Draw(MemDC, startX[difficulty] + 26 * j, startY[difficulty] + 26 * i);
				else
					bitmap[BLOCK0]->Draw(MemDC, startX[difficulty] + 26 * j, startY[difficulty] + 26 * i);
			}
			else
				bitmap[MINE]->Draw(MemDC, startX[difficulty] + 26 * j, startY[difficulty] + 26 * i);
		}
}

int GameManager::LeftClick(int clickX, int clickY) {
	int x = (clickX - startX[difficulty]) / 26;
	int y = (clickY - startY[difficulty]) / 26;
	if (x < 0 || x >= width[difficulty] || y < 0 || y >= height[difficulty])
		return RANGE_OUT;
	if (!map[y][x].push && !map[y][x].flag) {
		gameStart = true;
		map[y][x].push = true;
		if (map[y][x].mine) {
			Draw();
			return GAME_OVER;
		}
		if (map[y][x].nearMine == 0) {
			LeftClick(clickX + 26, clickY + 26);
			LeftClick(clickX + 26, clickY - 26);
			LeftClick(clickX + 26, clickY);
			LeftClick(clickX - 26, clickY + 26);
			LeftClick(clickX - 26, clickY - 26);
			LeftClick(clickX - 26, clickY);
			LeftClick(clickX, clickY + 26);
			LeftClick(clickX, clickY - 26);
		}
	}
	return CLICK_OK;
}

void GameManager::RightClick(int clickX, int clickY) {
	int x = (clickX - startX[difficulty]) / 26;
	int y = (clickY - startY[difficulty]) / 26;
	if (x < 0 || x >= width[difficulty] || y < 0 || y >= height[difficulty])
		return;
	if (map[y][x].flag)
		curMine++;
	else
		curMine--;
	map[y][x].flag = !map[y][x].flag;
}

void GameManager::DrawCurMineNume() {
	char buffer[20];
	RECT rt = { 660, 480, 680, 500 };
	_itoa_s(curMine, buffer, 20, 10);
	LPCSTR p = buffer;
	DrawText(GetDC(m_hWnd), p, -1, &rt, DT_CENTER | DT_WORDBREAK);
}

void GameManager::SetDifficulty(int dif) {
	difficulty = dif;
	NewGame();
}

bool GameManager::is_GameStart() {
	return gameStart;
}

GameManager::~GameManager()
{
}
