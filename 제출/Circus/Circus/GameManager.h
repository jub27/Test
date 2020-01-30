#pragma once
#include <Windows.h>
#include <math.h>
#include "Bitmap.h"
#include <vector>

using namespace std;

#define PI			3.141592f
#define BITMAP_NUMS 28

#define  STAGE_LENGTH 10000;

enum Image {
	BACK, BACK_DECO, BACK_NORMAL, BACK_NORMAL2, CASH, DIE, END, ENEMY, ENEMY_1B, ENEMY_1F, ENEMY_B, ENEMY_F, ENEMY_I,
	ENEMY_I_B, ENEMY_I_F, ENEMY1, FRONT, FRONT2, ICON, METER, PLAYER0, PLAYER1, PLAYER2, STAR, STAR1, STAR2, WIN, WIN2
};

class GameManager
{
private:
	HWND m_hWnd;
	HDC		MemDC;
	HBITMAP	MyBitMap;
	HBITMAP	OldBitMap;
	HDC		BackDC;
	HBITMAP	Back_MyBitMap;
	HBITMAP	Back_OldBitMap;
	static GameManager * instance;
	LPCWSTR source[BITMAP_NUMS] = { L"res\\back.bmp",L"res\\back_deco.bmp", L"res\\back_normal.bmp", L"res\\back_normal2.bmp", L"res\\cash.bmp", 
		L"res\\die.bmp", L"res\\end.bmp", L"res\\enemy.bmp", L"res\\enemy_1b.bmp", L"res\\enemy_1f.bmp",L"res\\enemy_b.bmp",L"res\\enemy_f.bmp",L"res\\enemy_l.bmp", 
		L"res\\enemy_l_b.bmp",L"res\\enemy_l_f.bmp", L"res\\enemy1.bmp", L"res\\front.bmp", L"res\\front2.bmp", L"res\\icon.bmp", L"res\\miter.bmp", L"res\\player0.bmp", 
		L"res\\player1.bmp", L"res\\player2.bmp", L"res\\star.bmp", L"res\\star1.bmp", L"res\\star2.bmp", L"res\\win.bmp", L"res\\win2.bmp" };
	Bitmap * bitmap[BITMAP_NUMS];
	
	DWORD CurTime, LastTime;

	vector<pair<int, int>> Enemy;
	vector<int> Front;
	vector<double> FireRing;
	vector<double> FireRing_Gold;


	double PlayerX, PlayerY;
	double JumpX, JumpY;
	bool JumpState;
	DWORD JumpTime;
	double BackgroundMove;
	double FrontMove;
public:
	static GameManager* GetInstance() {
		if (instance == NULL)
			instance = new GameManager();
		return instance;
	}
	GameManager();
	void OperateInput();
	void DrawBack();
	void SetFront();
	void Update();
	void DrawFront();
	void DrawPlayer();
	void DrawMeter();
	void SetFireRing();
	void DrawFireRingLeft();
	void DrawFireRingRight();
	void Jump();
	bool CheckDie();
	void Init(HWND hWnd);
	void Render();
	~GameManager();
};