#pragma once
#include<Windows.h>
#include <string>
enum Team {
	WHITE, BLACK, TILE
};

class Piece
{
private:
	HDC		MemDC;
	HBITMAP	m_pMyBitMap;
	HBITMAP	m_pOldBitMap;
	SIZE	m_size;
	int resource;
	Team team;
public:
	Piece();
	void Init(HDC hdc, LPCWSTR source, Team team);
	void Draw(HDC hdc, int x, int y);
	Team GetTeam();
	~Piece();
};