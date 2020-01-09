#include "Piece.h"

using namespace std;

Piece::Piece()
{
}

void Piece::Init(HDC hdc, LPCWSTR source, Team team) {

	MemDC = CreateCompatibleDC(hdc);

	m_pMyBitMap = (HBITMAP)LoadImage(NULL, source,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	m_pOldBitMap = (HBITMAP)SelectObject(MemDC, m_pMyBitMap);

	BITMAP Bit_Info;

	GetObject(m_pMyBitMap, sizeof(Bit_Info), &Bit_Info);
	m_size.cx = Bit_Info.bmWidth;
	m_size.cy = Bit_Info.bmHeight;
	this->team = team;

}
void Piece::Draw(HDC hdc, int x, int y)
{

	TransparentBlt(hdc, x, y, m_size.cx, m_size.cy, MemDC, 0, 0,
		m_size.cx, m_size.cy, RGB(255, 0, 255));
}

Team Piece::GetTeam() {
	return team;
}

Piece::~Piece()
{
	SelectObject(MemDC, m_pOldBitMap);
	DeleteObject(m_pMyBitMap);
	DeleteDC(MemDC);
}
