#pragma once
#include "Piece.h"

#define BITMAPNUMS 15
#define BOARD_SIZE 8
enum BlockName {
	B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING, W_PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING, TILE0, TILE1, SELECTABLE
};
class GameManager
{
private:
	LPCWSTR source[BITMAPNUMS] = { L"res\\block_b_00.bmp", L"res\\block_b_01.bmp", L"res\\block_b_02.bmp", L"res\\block_b_03.bmp", L"res\\block_b_04.bmp", L"res\\block_b_05.bmp"
	, L"res\\block_w_00.bmp", L"res\\block_w_01.bmp", L"res\\block_w_02.bmp", L"res\\block_w_03.bmp", L"res\\block_w_04.bmp", L"res\\block_w_05.bmp", L"res\\block00.bmp", L"res\\block01.bmp", L"res\\block02.bmp"};
	static GameManager* instance;
	Piece * block[BITMAPNUMS];
	Piece * board[BOARD_SIZE][BOARD_SIZE];
	bool selectable[BOARD_SIZE][BOARD_SIZE];
	int selectedX, selectedY;
	Team curTeam;
public:
	static GameManager* GetInstance() {
		if (instance == NULL)
			instance = new GameManager();
		return instance;
	}
	GameManager();
	void Init(HWND hWnd);
	void CheckSelectable();
	void ResetBoard();
	void DrawBoard(HDC hdc);
	int Click(POINT point);
	void UnClick();
	void ClearSelectable();
	~GameManager();
};

