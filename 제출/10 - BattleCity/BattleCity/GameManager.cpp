#include "GameManager.h"

GameManager* GameManager::instance = NULL;

GameManager::GameManager() {
}

void GameManager::InitGame(HWND hWnd) {
	m_hWnd = hWnd;
	pHDC = GetDC(m_hWnd);
	MemDC = CreateCompatibleDC(pHDC);
	MyBitMap = CreateCompatibleBitmap(pHDC, 600, 377);
	OldBitMap = (HBITMAP)SelectObject(MemDC, MyBitMap);
	for (int i = 0; i < BITMAP_NUMS; i++) {
		bitmap[i] = new Bitmap();
		bitmap[i]->Init(MemDC, source[i]);
	}
	GameState = TITLE_MENU;	

}

void GameManager::Title() {
	static int choice = GAME_START;
	if (GetKeyState(VK_UP) & 0x8000)
		choice = GAME_START;
	else if (GetKeyState(VK_DOWN) & 0x8000)
		choice = GAME_EXIT;
	bitmap[TITLE]->Draw(MemDC, 0, 0);
	if (choice == GAME_START)
		bitmap[MISSILE_RIGHT]->TransparentDraw(MemDC, 200, 180);
	else
		bitmap[MISSILE_RIGHT]->TransparentDraw(MemDC, 200, 240);

	BitBlt(pHDC, 0, 0, 600, 377, MemDC, 0, 0, SRCCOPY);
	life = 3;
	curStage = 1;
	Player = NULL;
	if (GetKeyState(VK_RETURN) & 0x8000)
		GameState = choice;
	else
		GameState = TITLE_MENU;
}

void GameManager::InitStage() {
	Map::GetInstance()->LoadMap(curStage);
	centerLive = true;
	if (Player != NULL)
		delete Player;
	for (int i = 0; i < enemyVector.size(); i++)
		delete enemyVector[i];
	for (int i = 0; i < missileVector.size(); i++)
		delete missileVector[i];
	missileVector.clear();
	enemyVector.clear();
	Player = new Tank(PLAYER_START_X, PLAYER_START_Y, UP, true);
	Player->Live();
	for (int i = 0; i < ENEMY_NUMS; i++) {
		int x = ENEMY_START_X_0;
		if (i % 3 == 0)
			x = ENEMY_START_X_0;
		else if (i % 3 == 1)
			x = ENEMY_START_X_1;
		else
			x = ENEMY_START_X_2;
		enemyVector.push_back(new Tank(x, ENEMY_START_Y, DOWN, false));
	}
	enemyCreateTime = GetTickCount();
	curEnemyNums = 0;
	curEnemyIndex = 0;
	gameOverTick = 0;
	stageClearTick = 0;
}


void GameManager::StartStage() {
	static bool initStage = false;
	if (!initStage) {
		InitStage();
		initStage = true;
	}

	if (GetTickCount() >= gameOverTick && gameOverTick != 0) {
		GameState = TITLE_MENU;
		initStage = false;
		return;
	}

	if (GetTickCount() >= stageClearTick && stageClearTick != 0) {
		initStage = false;
		curStage++;
		if (curStage == 6)
			GameState = TITLE_MENU;
		return;
	}

	if (MissileCenterCollision() && centerLive) {
		centerLive = false;
		gameOverTick = GetTickCount() + 3000;
	}
	TankMissileCollision();
	MissileMissileCollision();
	ShieldCheck();
	MakeTank();
	ShotMissile();
	MoveTank();
	MoveMissile();

	Render();
}

void GameManager::Print() {
	PrintMapNoBush();
	PrintUI();
	PrintTank();
	PrintBush();
	PrintMissile();
	PrintCommandCenter();
}

void GameManager::PrintUI() {
	bitmap[UI]->Draw(MemDC, MAP_SIZE * BLOCK_WIDTH, 0);
	for (int i = 0; i < ENEMY_NUMS - curEnemyIndex; i++) {
		bitmap[ENEMY_ICON]->TransparentDraw(MemDC, MAP_SIZE * BLOCK_WIDTH + 20 + 20 * (i % 2), 20 * (i / 2));
	}
	bitmap[PLAYER_ICON]->TransparentDraw(MemDC, MAP_SIZE * BLOCK_WIDTH + 20, 250);
	char buffer[20];
	_itoa_s(life, buffer, 20, 10);
	LPCSTR p = buffer;
	RECT rt1 = { MAP_SIZE * BLOCK_WIDTH + 30, 250, MAP_SIZE * BLOCK_WIDTH + 20 + 50, 300 };
	DrawText(MemDC, p, -1, &rt1, DT_CENTER | DT_WORDBREAK);
	bitmap[STAGE_ICON]->TransparentDraw(MemDC, MAP_SIZE * BLOCK_WIDTH + 20, 300);
	_itoa_s(curStage, buffer, 20, 10);
	p = buffer;
	RECT rt2 = { MAP_SIZE * BLOCK_WIDTH + 30, 300, MAP_SIZE * BLOCK_WIDTH + 20 + 50, 320 };
	DrawText(MemDC, p, -1, &rt2, DT_CENTER | DT_WORDBREAK);
}

void GameManager::PrintMapNoBush() {
	for (int i = 0; i < MAP_SIZE; i++)
		for (int j = 0; j < MAP_SIZE; j++) {
			switch (Map::GetInstance()->GetBlockInfo(i, j)) {
			case 'B':
				bitmap[BLANK]->Draw(MemDC, BLOCK_WIDTH * j, BLOCK_HEIGHT * i);
				break;
			case 'n':
				bitmap[NORMAL_BLOCK]->Draw(MemDC, BLOCK_WIDTH * j, BLOCK_HEIGHT * i);
				break;
			case 'w':
				bitmap[WHITE_BLOCK]->Draw(MemDC, BLOCK_WIDTH * j, BLOCK_HEIGHT * i);
				break;
			case 'W':
				bitmap[WATER_BLOCK]->Draw(MemDC, BLOCK_WIDTH * j, BLOCK_HEIGHT * i);
				break;
			}
		}
}

void GameManager::PrintBush() {
	for (int i = 0; i < MAP_SIZE; i++)
		for (int j = 0; j < MAP_SIZE; j++)
			if (Map::GetInstance()->GetBlockInfo(i, j) == 'b')
				bitmap[BUSH_BLOCK]->TransparentDraw(MemDC, BLOCK_WIDTH * j, BLOCK_HEIGHT * i);
}

void GameManager::PrintTank() {
	static int shieldImg = 0;
	for (int i = 0; i < enemyVector.size(); i++) {
		if (enemyVector[i]->GetState() == LIVE) {
			bitmap[ETANK_DOWN_0 + enemyVector[i]->GetDir() * 2 + enemyVector[i]->GetMotion()]->TransparentDraw(MemDC, enemyVector[i]->GetX(), enemyVector[i]->GetY());
			if (enemyVector[i]->is_Shield()) {
				if (shieldImg == 0) {
					bitmap[SHIELD_0]->TransparentDraw(MemDC, enemyVector[i]->GetX(), enemyVector[i]->GetY());
					shieldImg = 1;
				}
				else {
					bitmap[SHIELD_1]->TransparentDraw(MemDC, enemyVector[i]->GetX(), enemyVector[i]->GetY());
					shieldImg = 0;
				}
			}
		}
		else if (enemyVector[i]->GetState() == EXPLOSION) {
			if (GetTickCount() - enemyVector[i]->GetExplisionTick() <= 125)
				bitmap[EXPLOSION_0]->TransparentDraw(MemDC, enemyVector[i]->GetX(), enemyVector[i]->GetY());
			else if (GetTickCount() - enemyVector[i]->GetExplisionTick() <= 250)
				bitmap[EXPLOSION_1]->TransparentDraw(MemDC, enemyVector[i]->GetX(), enemyVector[i]->GetY());
			else if (GetTickCount() - enemyVector[i]->GetExplisionTick() <= 375)
				bitmap[EXPLOSION_2]->TransparentDraw(MemDC, enemyVector[i]->GetX(), enemyVector[i]->GetY());
			else if (GetTickCount() - enemyVector[i]->GetExplisionTick() <= 500)
				bitmap[EXPLOSION_3]->TransparentDraw(MemDC, enemyVector[i]->GetX() - BLOCK_WIDTH / 2, enemyVector[i]->GetY() - BLOCK_HEIGHT / 2);
			else if (GetTickCount() - enemyVector[i]->GetExplisionTick() <= 625)
				bitmap[EXPLOSION_4]->TransparentDraw(MemDC, enemyVector[i]->GetX() - BLOCK_WIDTH / 2, enemyVector[i]->GetY() - BLOCK_HEIGHT / 2);
			else if (GetTickCount() - enemyVector[i]->GetExplisionTick() <= 750)
				enemyVector[i]->Die();
		}
	}
	if (Player->GetState() == LIVE) {
		bitmap[PTANK_DOWN_0 + Player->GetDir() * 2 + Player->GetMotion()]->TransparentDraw(MemDC, Player->GetX(), Player->GetY());
		if (Player->is_Shield()) {
			if (shieldImg == 0) {
				bitmap[SHIELD_0]->TransparentDraw(MemDC, Player->GetX(), Player->GetY());
				shieldImg = 1;
			}
			else {
				bitmap[SHIELD_1]->TransparentDraw(MemDC, Player->GetX(), Player->GetY());
				shieldImg = 0;
			}
		}
	}
	else if (Player->GetState() == EXPLOSION) {
		if (GetTickCount() - Player->GetExplisionTick() <= 125)
			bitmap[EXPLOSION_0]->TransparentDraw(MemDC, Player->GetX(), Player->GetY());
		else if (GetTickCount() - Player->GetExplisionTick() <= 250)
			bitmap[EXPLOSION_1]->TransparentDraw(MemDC, Player->GetX(), Player->GetY());
		else if (GetTickCount() - Player->GetExplisionTick() <= 375)
			bitmap[EXPLOSION_2]->TransparentDraw(MemDC, Player->GetX(), Player->GetY());
		else if (GetTickCount() - Player->GetExplisionTick() <= 500)
			bitmap[EXPLOSION_3]->TransparentDraw(MemDC, Player->GetX() - BLOCK_WIDTH / 2, Player->GetY() - BLOCK_HEIGHT / 2);
		else if (GetTickCount() - Player->GetExplisionTick() <= 625)
			bitmap[EXPLOSION_4]->TransparentDraw(MemDC, Player->GetX() - BLOCK_WIDTH / 2, Player->GetY() - BLOCK_HEIGHT / 2);
		else if (GetTickCount() - Player->GetExplisionTick() <= 750)
			Player->Die();
	}
}

void GameManager::PrintMissile() {
	for (int i = 0; i < missileVector.size(); i++)
		bitmap[MISSILE_DOWN + missileVector[i]->GetDir()]->TransparentDraw(MemDC, missileVector[i]->GetX(), missileVector[i]->GetY());
}

void GameManager::MakeTank() {
	if (GetTickCount() - enemyCreateTime > 3000 && curEnemyNums < 3 && curEnemyIndex < ENEMY_NUMS) {
		enemyVector[curEnemyIndex]->Live();
		curEnemyNums++;
		curEnemyIndex++;
		enemyCreateTime = GetTickCount();
	}
	if (Player->GetState() == DEAD) {
		delete Player;
		if (life > 0) {
			Player = new Tank(PLAYER_START_X, PLAYER_START_Y, UP, true);
			Player->Live();
			life--;
		}
		else {
			gameOverTick = GetTickCount() + 3000;
		}
	}
}

void GameManager::PrintCommandCenter() {
	if (centerLive)
		bitmap[COMMAND_CENTER]->TransparentDraw(MemDC, (MAP_SIZE / 2 - 1) * BLOCK_WIDTH, (MAP_SIZE - 2) * BLOCK_HEIGHT);
	else
		bitmap[LOSE]->TransparentDraw(MemDC, (MAP_SIZE / 2 - 1) * BLOCK_WIDTH, (MAP_SIZE - 2) * BLOCK_HEIGHT);
}

void GameManager::ShieldCheck() {
	for (int i = 0; i < enemyVector.size(); i++) {
		if (GetTickCount() - enemyVector[i]->GetShieldTick() >= 1500)
			enemyVector[i]->ShieldOff();
	}

	if (GetTickCount() - Player->GetShieldTick() >= 1500)
		Player->ShieldOff();
}

void GameManager::MoveTank() {
	if (Player->GetState() == LIVE) {
		if (GetKeyState(VK_DOWN) & 0x8000) {
			Player->Move(DOWN);
		}
		else if (GetKeyState(VK_LEFT) & 0x8000) {
			Player->Move(LEFT);
		}
		else if (GetKeyState(VK_RIGHT) & 0x8000) {
			Player->Move(RIGHT);
		}
		else if (GetKeyState(VK_UP) & 0x8000) {
			Player->Move(UP);
		}
	}
	for (int i = 0; i < enemyVector.size(); i++) {
		if (enemyVector[i]->GetState() == LIVE) {
			direction dir = direction(rand() % 4);
			enemyVector[i]->Move(dir);
		}
	}
}

bool GameManager::MissileCenterCollision() {
	for (int i = 0; i < missileVector.size(); i++) {
		RECT missileRect = { missileVector[i]->GetX(), missileVector[i]->GetY(),
			missileVector[i]->GetX() + MISSILE_WIDTH, missileVector[i]->GetY() + MISSILE_HEIGHT };
		RECT commandRect = { (MAP_SIZE / 2 - 1) * BLOCK_WIDTH, (MAP_SIZE - 2) * BLOCK_HEIGHT ,
		(MAP_SIZE / 2 + 1)* BLOCK_WIDTH, MAP_SIZE * BLOCK_HEIGHT };
		RECT rcTemp;
		if (IntersectRect(&rcTemp, &missileRect, &commandRect)) {
			missileVector.erase(missileVector.begin() + i);
			return true;
		}
	}
	return false;
}

void GameManager::Render() {
	Print();
	BitBlt(pHDC, 0, 0, 600, 377, MemDC, 0, 0, SRCCOPY);
}

void GameManager::ShotMissile() {
	Missile* m;
	if (Player->GetState() == LIVE) {
		if (GetKeyState(VK_SPACE) & 0x8000) {
			m = Player->Shot();
			if (m != NULL)
				missileVector.push_back(m);
		}
	}
	for (int i = 0; i < enemyVector.size(); i++) {
		if (enemyVector[i]->GetState() == LIVE) {
			m = enemyVector[i]->Shot();
			if (m != NULL)
				missileVector.push_back(m);
		}
	}
}

void GameManager::MoveMissile() {
	for (int i = 0; i < missileVector.size();) {
		if (!missileVector[i]->Move())
			missileVector.erase(missileVector.begin() + i);
		else
			i++;
	}
}

void GameManager::MissileMissileCollision() {
	vector<int> index;
	for (int i = missileVector.size() - 1; i >= 0; i--) {
		RECT rect1 = { missileVector[i]->GetX(), missileVector[i]->GetY(),
			missileVector[i]->GetX() + MISSILE_WIDTH, missileVector[i]->GetY() + MISSILE_HEIGHT };
		for (int j = 0; j < missileVector.size(); j++) {
			if (i == j)
				continue;
			RECT rect2 = { missileVector[j]->GetX(), missileVector[j]->GetY(),
			missileVector[j]->GetX() + MISSILE_WIDTH, missileVector[j]->GetY() + MISSILE_HEIGHT };
			RECT rcTemp;
			if (IntersectRect(&rcTemp, &rect1, &rect2)) {
				index.push_back(i);
				break;
			}
		}
	}
	for (int i = 0; i < index.size(); i++) {
		missileVector.erase(missileVector.begin() + index[i]);
	}
}

void GameManager::TankMissileCollision() {
	for (int i = 0; i < missileVector.size();) {
		RECT missileRect = { missileVector[i]->GetX(), missileVector[i]->GetY(),
			missileVector[i]->GetX() + MISSILE_WIDTH, missileVector[i]->GetY() + MISSILE_HEIGHT };
		if (Player->GetState() == LIVE) {
			RECT playerRect = { Player->GetX(), Player->GetY(),
				Player->GetX() + TANK_WIDTH, Player->GetY() + TANK_HEIGHT };
			RECT rcTemp;
			if (IntersectRect(&rcTemp, &playerRect, &missileRect)) {
				if (!Player->is_Shield() && !missileVector[i]->is_PlayerMissile()) {
					Player->Explosion();
				}
				missileVector.erase(missileVector.begin() + i);
				continue;
			}
		}
		bool flag = false;
		for (int j = 0; j < enemyVector.size(); j++) {
			if (enemyVector[j]->GetState() == LIVE) {
				RECT enemyRect = { enemyVector[j]->GetX(), enemyVector[j]->GetY(),
					enemyVector[j]->GetX() + TANK_WIDTH, enemyVector[j]->GetY() + TANK_HEIGHT };
				RECT rcTemp;
				if (IntersectRect(&rcTemp, &enemyRect, &missileRect)) {
					if (!enemyVector[j]->is_Shield() && missileVector[i]->is_PlayerMissile()) {
						enemyVector[j]->Explosion();
						curEnemyNums--;
						if (curEnemyNums == 0 && curEnemyIndex == ENEMY_NUMS) {
							stageClearTick = GetTickCount() + 3000;
						}
					}
					missileVector.erase(missileVector.begin() + i);
					flag = true;
					break;
				}
			}
		}
		if (flag)
			continue;
		else
			i++;
	}
}

int GameManager::GetGameState() {
	return GameState;
}

bool GameManager::GameOver() {
	return false;
}

bool GameManager::StageClear() {
	return false;
}

GameManager::~GameManager() {
}