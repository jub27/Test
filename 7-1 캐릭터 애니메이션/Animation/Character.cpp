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
	curDir = VK_DOWN;
	prevDir = -1;
	motion = 0;
	speed = 5;
}

void Character::Draw(HDC hdc) {
	int temDir = 0;
	switch (curDir) {
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
		MemDC, (m_size.cx / 4) * motion, (m_size.cy / 4) * temDir, m_size.cx / 4, m_size.cy / 4, RGB(255, 0, 255));
}


void Character::MoveX(int dir) {
	this->x += speed * dir;
}

void Character::MoveY(int dir) {
	this->y += speed * dir;
}

void Character::Jump(int degree) {
	jumpY = sin(degree * 3.14 / 180) * -50;
	InvalidateRect(hWnd, NULL, TRUE);
}

int Character::GetCurDir() {
	return curDir;
}

int Character::GetPrevDir() {
	return prevDir;
}


void Character::SetPrevDir(int dir) {
	prevDir = dir;
}
void Character::SetCurDir(int dir) {
	curDir = dir;
}


void Character::InitMotion() {
	motion = 0;
}
void Character::NextMotion() {
	motion = (motion + 1) % 4;
}

Character::~Character()
{
	DeleteObject(m_pMyBitMap);
	DeleteDC(MemDC);
}