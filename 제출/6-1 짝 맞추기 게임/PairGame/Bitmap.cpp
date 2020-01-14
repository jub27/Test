#include "Bitmap.h"

Bitmap::Bitmap()
{
}

void Bitmap::Init(HDC hdc, HINSTANCE hInst, int resource)
{
	MemDC = CreateCompatibleDC(hdc);
	m_pMyBitMap = LoadBitmap(hInst, MAKEINTRESOURCE(resource));
	m_pOldBitMap = (HBITMAP)SelectObject(MemDC, m_pMyBitMap);

	BITMAP Bit_Info;

	GetObject(m_pMyBitMap, sizeof(Bit_Info), &Bit_Info);
	m_size.cx = Bit_Info.bmWidth;
	m_size.cy = Bit_Info.bmHeight;
	this->resource = resource;
	opened = false;
}
void Bitmap::Draw(HDC hdc, int x, int y, int spX, int spY)
{
	BitBlt(hdc, x, y, m_size.cx, m_size.cy, MemDC, 0, 0, SRCCOPY);
}

bool Bitmap::isOpend() {
	return opened;
}

void Bitmap::Close() {
	opened = false;
}

void Bitmap::Open() {
	opened = true;
}

int Bitmap::GetResource() {
	return resource;
}

Bitmap::~Bitmap()
{
	SelectObject(MemDC, m_pOldBitMap);
	DeleteObject(m_pMyBitMap);
	DeleteDC(MemDC);
}
