#include "Character.h"



Character::Character()
{
}

void Character::Init(HWND hWnd, HINSTANCE hInst) {
	this->hWnd = hWnd;
	HDC hdc = GetDC(hWnd);
	MemDC = CreateCompatibleDC(hdc);

	m_pMyBitMap = (HBITMAP)LoadImage(NULL, TEXT("image.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	m_pOldBitMap = (HBITMAP)SelectObject(MemDC, m_pMyBitMap);

	BITMAP Bit_Info;

	GetObject(m_pMyBitMap, sizeof(Bit_Info), &Bit_Info);
	m_size.cx = Bit_Info.bmWidth;
	m_size.cy = Bit_Info.bmHeight;
	x = 0, y = 0;
	jumpY = 0;
}

void Character::Draw(HDC hdc, int motionNum, int dir) {
	int temDir = 0;
	switch (dir) {
	case VK_LEFT:
		temDir = LEFT;
		break;
	case VK_RIGHT:
		temDir = RIGHT;
		break;
	case VK_UP:
		temDir = UP;
		break;
	case VK_DOWN:
		temDir = DOWN;
		break;
	}
	TransparentBlt(hdc, x, y + jumpY, m_size.cx / 4, m_size.cy / 4,
		MemDC, (m_size.cx / 4) * motionNum, (m_size.cy / 4) * temDir, m_size.cx / 4, m_size.cy / 4, RGB(255, 0, 255));
}


void Character::AddX(double x) {
	this->x += x;
}

void Character::AddY(double y) {
	this->y += y;
}

void Character::Jump(int degree) {
	jumpY = sin(degree * 3.14 / 180) * -50;
	InvalidateRect(hWnd, NULL, TRUE);
}

Character::~Character()
{
	DeleteObject(m_pMyBitMap);
	DeleteDC(MemDC);
}
