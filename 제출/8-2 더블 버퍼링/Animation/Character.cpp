#include "Character.h"



Character::Character()
{
}

void Character::Init(HWND hWnd, HINSTANCE hInst) {
	HDC hdc = GetDC(hWnd);
	MemDC[0] = CreateCompatibleDC(hdc);
	MemDC[1] = CreateCompatibleDC(MemDC[0]);
	MemDC[2] = CreateCompatibleDC(MemDC[0]);

	m_pMyBitMap[0] = CreateCompatibleBitmap(hdc, 1024, 768);
	m_pMyBitMap[1] = (HBITMAP)LoadImage(NULL, TEXT("image.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	m_pMyBitMap[2] = (HBITMAP)LoadImage(NULL, TEXT("back.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

	m_pOldBitMap[0] = (HBITMAP)SelectObject(MemDC[0], m_pMyBitMap[0]);
	m_pOldBitMap[1] = (HBITMAP)SelectObject(MemDC[1], m_pMyBitMap[1]);
	m_pOldBitMap[2] = (HBITMAP)SelectObject(MemDC[2], m_pMyBitMap[2]);
	BITMAP Bit_Info;

	GetObject(m_pMyBitMap[1], sizeof(Bit_Info), &Bit_Info);
	m_size.cx = Bit_Info.bmWidth;
	m_size.cy = Bit_Info.bmHeight;
	x = 0, y = 0;
	jumpY = 0;
	curDir = DOWN;
	prevDir = UP;
	motion = 0;
	speed = 5;
}

void Character::Draw(HDC hdc) {
	BitBlt(MemDC[0], 0, 0, 1024, 768, MemDC[2], 0, 0, SRCCOPY);
	TransparentBlt(MemDC[0], x, y + jumpY, m_size.cx / 4, m_size.cy / 4,
		MemDC[1], (m_size.cx / 4) * motion, (m_size.cy / 4) * curDir, m_size.cx / 4, m_size.cy / 4, RGB(255, 0, 255));

	BitBlt(hdc, 0, 0, 1024, 768, MemDC[0], 0, 0, SRCCOPY);
}


void Character::MoveX(int dir) {
	this->x += speed * dir;
}

void Character::MoveY(int dir) {
	this->y += speed * dir;
}

void Character::Jump(int degree) {
	jumpY = sin(degree * 3.14 / 180) * -50;
}

Dir Character::GetCurDir() {
	return curDir;
}

Dir Character::GetPrevDir() {
	return prevDir;
}


void Character::SetPrevDir(Dir dir) {
	prevDir = dir;
}
void Character::SetCurDir(Dir dir) {
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
	DeleteObject(m_pMyBitMap[0]);
	DeleteObject(m_pMyBitMap[1]);
	DeleteObject(m_pOldBitMap[0]);
	DeleteObject(m_pOldBitMap[1]);
	DeleteDC(MemDC[0]);
	DeleteDC(MemDC[1]);
}