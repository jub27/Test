#pragma once
#include <Windows.h>
#include <vector>
#include "Tank.h"
#include "Bitmap.h"
#include "Map.h"

using namespace std;

#define BITMAP_NUMS 39

#define PLAYER_START_X 128
#define PLAYER_START_Y 312

#define ENEMY_START_X_0 0
#define ENEMY_START_X_1 BLOCK_WIDTH * (MAP_SIZE / 2 - 1)
#define ENEMY_START_X_2 BLOCK_WIDTH * (MAP_SIZE - 2)
#define ENEMY_START_Y 0

#define ENEMY_NUMS 20

#define TITLE_MENU 0
#define GAME_START 1
#define GAME_EXIT 2

enum bitmaps {
	BLANK, NORMAL_BLOCK, WHITE_BLOCK, WATER_BLOCK, BUSH_BLOCK, COMMAND_CENTER, LOSE, PTANK_DOWN_0, PTANK_DOWN_1, PTANK_LEFT_0, PTANK_LEFT_1, PTANK_RIGHT_0,
	PTANK_RIGHT_1, PTANK_UP_0, PTANK_UP_1, ETANK_DOWN_0, ETANK_DOWN_1, ETANK_LEFT_0, ETANK_LEFT_1, ETANK_RIGHT_0, ETANK_RIGHT_1, ETANK_UP_0, ETANK_UP_1,
	ENEMY_ICON, EXPLOSION_0, EXPLOSION_1, EXPLOSION_2, EXPLOSION_3, EXPLOSION_4, MISSILE_DOWN, MISSILE_LEFT, MISSILE_RIGHT, MISSILE_UP, PLAYER_ICON,
	SHIELD_0, SHIELD_1, STAGE_ICON, TITLE, UI
};

class GameManager
{
private:
	HWND m_hWnd;
	HDC pHDC;
	HDC		MemDC;
	HBITMAP	MyBitMap;
	HBITMAP	OldBitMap;
	static GameManager* instance;
	LPCSTR source[BITMAP_NUMS] = { "res\\blank.bmp", "res\\nblock.bmp", "res\\wblock.bmp","res\\water.bmp", "res\\bush.bmp", "res\\block13.bmp", "res\\block14.bmp"
	, "res\\tank_down_00.bmp","res\\tank_down_01.bmp","res\\tank_left_00.bmp", "res\\tank_left_01.bmp", "res\\tank_right_00.bmp", "res\\tank_right_01.bmp"
	, "res\\tank_up_00.bmp", "res\\tank_up_01.bmp", "res\\e_down_00.bmp", "res\\e_down_01.bmp", "res\\e_left_00.bmp", "res\\e_left_01.bmp", "res\\e_right_00.bmp","res\\e_right_01.bmp"
	, "res\\e_up_00.bmp", "res\\e_up_01.bmp", "res\\enemy_icon.bmp", "res\\explosion_00.bmp","res\\explosion_01.bmp" ,"res\\explosion_02.bmp" ,"res\\explosion_03.bmp"
	, "res\\explosion_04.bmp","res\\missile_0.bmp", "res\\missile_1.bmp", "res\\missile_2.bmp", "res\\missile_3.bmp", "res\\player_icon.bmp"
	, "res\\shield_00.bmp", "res\\shield_01.bmp","res\\stage_icon.bmp", "res\\Title.bmp", "res\\ui.bmp" };
	Bitmap * bitmap[BITMAP_NUMS];

	int curStage;

	vector<Missile*> missileVector;
	vector<Tank*> enemyVector;
	Tank* Player;
	int GameState;
	int life;

	DWORD enemyCreateTime;
	int curEnemyNums;
	int curEnemyIndex;

	bool centerLive;
	DWORD gameOverTick;
	DWORD stageClearTick;

public:
	GameManager();
	static GameManager* GetInstance() {
		if (instance == NULL)
			instance = new GameManager();
		return instance;
	}
	void InitGame(HWND hWnd);
	void Title();
	void InitStage();
	void StartStage();
	void Print();
	void PrintMapNoBush();
	void PrintBush();
	void PrintTank();
	void PrintCommandCenter();
	void MakeTank();
	void MoveTank();
	void MoveMissile();
	void ShotMissile();
	void PrintMissile();
	void ShieldCheck();
	void PrintUI();
	void Render();
	void TankMissileCollision();
	void MissileMissileCollision();
	bool MissileCenterCollision();
	bool GameOver();
	bool StageClear();
	int GetGameState();
	~GameManager();
};

