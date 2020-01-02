#include "Character.h"



Character::Character()
{
}

void Character::Init(HWND hWnd, HINSTANCE hInst) {
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
	curDir = DOWN;
	prevDir = DOWN;
	m = 0;
}

void Character::Draw(HDC hdc) {
	TransparentBlt(hdc, x, y, m_size.cx / 4, m_size.cy / 4,
		MemDC, (m_size.cx / 4) * m, (m_size.cy / 4) * curDir, m_size.cx / 4, m_size.cy / 4, RGB(255, 0, 255));
}

void Character::Move(int direction) {
	prevDir = curDir;
	switch (direction) {
	case VK_LEFT:
		x -= 10;
		curDir = LEFT;
		break;
	case VK_RIGHT:
		x += 10;
		curDir = RIGHT;
		break;
	case VK_UP:
		y -= 10;
		curDir = UP;
		break;
	case VK_DOWN:
		y += 10;
		curDir = DOWN;
		break;
	}
	if (curDir == prevDir)
		m = (m + 1) % 4;
	else
		m = 0;

}

Character::~Character()
{
}
