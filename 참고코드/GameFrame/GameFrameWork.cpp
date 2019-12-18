#include "GameFrameWork.h"
#include <math.h>

GameFrameWork::GameFrameWork()
{
	Player_x = 200.0f;
	Player_y = 500.0f;

	m_bJump = false;

	m_dwLastTime = GetTickCount();
	m_dwCurTime = GetTickCount();
}


GameFrameWork::~GameFrameWork()
{
}

void GameFrameWork::Init(HWND hWnd)
{
	m_hWnd = hWnd;
	HDC hdc = GetDC(hWnd);
	
	m_hMemDC[0] = CreateCompatibleDC(hdc);
	m_hBitmap[0] = CreateCompatibleBitmap(hdc, 1024, 768);
	m_hOld[0] = (HBITMAP)SelectObject(m_hMemDC[0], m_hBitmap[0]);

	m_hMemDC[1] = CreateCompatibleDC(m_hMemDC[0]);
	m_hBitmap[1] = (HBITMAP)LoadImage(NULL, "back.bmp", IMAGE_BITMAP, 0, 0
		, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	m_hOld[1] = (HBITMAP)SelectObject(m_hMemDC[1], m_hBitmap[1]);

	m_hMemDC[2] = CreateCompatibleDC(m_hMemDC[0]);
	m_hBitmap[2] = (HBITMAP)LoadImage(NULL, "char.bmp", IMAGE_BITMAP, 0, 0
		, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	m_hOld[2] = (HBITMAP)SelectObject(m_hMemDC[2], m_hBitmap[2]);
	ReleaseDC(hWnd, hdc);
}


void GameFrameWork::Release()
{
	for (int i = 0; i < 3; i++)
	{
		SelectObject(m_hMemDC[i], m_hOld[i]);
		DeleteObject(m_hBitmap[i]);
		DeleteDC(m_hMemDC[i]);
	}
}

void GameFrameWork::Update()
{
	//좌클릭
	if (GetKeyState(VK_LBUTTON) & 0x8000)
	{
		POINT pt;
		
		//마우스 포인트의 좌표를 가져온다.
		GetCursorPos(&pt);
		
		//스크린상의 좌표에서 Client상의 좌표로 바꿔 준다.
		ScreenToClient(m_hWnd, &pt);
	}

	//매 프레임마다 찍히는 TickCount를 받는다. -  현제의 틱카운드를 받아온다.
	m_dwCurTime = GetTickCount();

	//현재 시간을 저장한다. 루틴이 돌아간 후 이전 루틴에서의 틱 카운트간의 차이를 체크하기 위해서 이다.
	//1000 이상의 차이가 나면 1초이므로  / 1000해서 초로 환산.
	m_fDeltaTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;

	//현재 타임을 받는다.
	//결론적으로 이전 타임의 시간을 받는것.
	m_dwLastTime = m_dwCurTime;

	//전, 후, 좌, 우, 점프를 체크 한다.
	OperateInput();

	//점프는 점프해서 움직이고 있는 시간이 있기때문에 점프 지속시간동안 동작이 없어야함.
	if (m_eState == JUMP)
	{
		m_fCurJumpTime += m_fDeltaTime;

		//원그리는 것 이것을 적절하게 조합하면 점프가 된다.
		//점프 지속시간을 체크해서 위치나 상태를 초기화 해준다.
		Player_x = m_fJumpX - cosf(m_fCurJumpTime * PI ) * 80;
		Player_y = m_fJumpY - sinf(m_fCurJumpTime * PI ) * 80;

		//점프 지속시간 체크용 초기화.
		if (m_fCurJumpTime > 1.0f)
		{
			m_fCurJumpTime = 0.0f;

			//정확한 점프를 위해 플레이어의 위치를 조정할 필요가 있거나 
			//플레이어의 위치를 자신이 원하는 룰에 의해 조정해주는 루틴을 짜면된다.

			//상태 초기화
			m_eState = IDLE;
		}
	}

	//그려준다.
	Render();
}

//Degree(각도)값을 받아 Radian 값 변환 함수
float DegreeToRadian(float degree)
{
	return (PI / 180) * degree;
}

void GameFrameWork::OperateInput()
{
	//좌, 우, 위, 아래
	if (GetKeyState(VK_LEFT) & 0x8000)
		Player_x -= 500 * m_fDeltaTime;
	if (GetKeyState(VK_RIGHT) & 0x8000)
		Player_x += 500 * m_fDeltaTime;
	if (GetKeyState(VK_UP) & 0x8000)
		Player_y -= 500 * m_fDeltaTime;
	if (GetKeyState(VK_DOWN) & 0x8000)
		Player_y += 500 * m_fDeltaTime;

	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		if (m_eState == IDLE)
		{
			// 키 다운
			m_eState = JUMP;
			
			//다음 점프를 계산하기 위해 현재의 현재위치를 저장해준다.
			m_fJumpY = Player_y;
			m_fJumpX = Player_x;
		}
	}
}

//그리기
void GameFrameWork::Render()
{
	HDC hdc = GetDC(m_hWnd);

	BitBlt(m_hMemDC[0], 0, 0, 1024, 768, m_hMemDC[1], 0, 0, SRCCOPY);
	TransparentBlt(m_hMemDC[0], (int)Player_x, (int)Player_y, 240, 192, 
		m_hMemDC[2], 0, 0, 240, 192, RGB(255, 0, 255));

	BitBlt(hdc, 0, 0, 1024, 768, m_hMemDC[0], 0, 0, SRCCOPY);

	ReleaseDC(m_hWnd, hdc);
}
