#pragma once
#include <Windows.h>

#define FPS			60

#define PI			3.141592f

//컨트롤의 상태 - 플레이어가 움직이고 있는 상태를 체크하는 것이다.
enum STATE
{
	IDLE,
	MOVE,
	JUMP
};

class GameFrameWork
{
private:
	HWND		m_hWnd;

	//타임체크용
	DWORD		m_dwLastTime;
	DWORD		m_dwCurTime;
	float		m_fDeltaTime;

	HDC			m_hMemDC[3];
	HBITMAP		m_hBitmap[3];	//0 Back 1 BackGround 2 char
	HBITMAP		m_hOld[3];

	bool		m_bJump;
	//수학적으로
	STATE		m_eState;
	
	float		m_fCurJumpTime;

	//물리적
	float		m_fVectorX;
	float		m_fVectorY;
public:
	GameFrameWork();
	~GameFrameWork();

	float		Player_x;
	float		Player_y;

	float		m_fJumpX;
	float		m_fJumpY;

	void Init(HWND hWnd);
	void Release();
	void Update();
	void OperateInput();
	void Render();
	
};

