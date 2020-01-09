#include "GameManager.h"

GameManager* GameManager::instance = NULL;

GameManager::GameManager()
{
}

void GameManager::Init(HWND hWnd) {
	Team team;
	for (int i = 0; i < BITMAPNUMS; i++) {
		block[i] = new Piece();
		if (i < 6)
			team = BLACK;
		else if (i < 12)
			team = WHITE;
		else
			team = TILE;
		block[i]->Init(GetDC(hWnd), source[i], team);
	}
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++) {
				board[i][j] = NULL;
		}
	
}

void GameManager::ResetBoard() {


	for(int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++) {
				board[i][j] = NULL;
				selectable[i][j] = false;
		}

	for (int i = 0; i < BOARD_SIZE; i++) {
		board[6][i] = block[W_PAWN];
		board[1][i] = block[B_PAWN];
	}
	board[0][0] = board[0][7] = block[B_ROOK];
	board[0][1] = board[0][6] = block[B_KNIGHT];
	board[0][2] = board[0][5] = block[B_BISHOP];
	board[0][3] = block[B_QUEEN];
	board[0][4] = block[B_KING];

	board[7][0] = board[7][7] = block[W_ROOK];
	board[7][1] = board[7][6] = block[W_KNIGHT];
	board[7][2] = board[7][5] = block[W_BISHOP];
	board[7][3] = block[W_QUEEN];
	board[7][4] = block[W_KING];
	selectedX = -1;
	selectedY = -1;
	curTeam = WHITE;
}

void GameManager::DrawBoard(HDC hdc) {
	int x = 0, y = 0;
	for (int i = 0; i < BOARD_SIZE; i++) {
		x = 0;
		for (int j = 0; j < BOARD_SIZE; j++) {
			if ((i + j) % 2 == 0)
				block[TILE0]->Draw(hdc, x, y);
			else
				block[TILE1]->Draw(hdc, x, y);
			if (board[i][j] != NULL)
				board[i][j]->Draw(hdc, x, y);
			if (selectable[i][j])
				block[SELECTABLE]->Draw(hdc, x, y);
			x += 125;
		}
		y += 125;
	}
}

void GameManager::ShowSelectable() {
	ClearSelectable();
	if (selectedX == -1 && selectedY == -1) {
		if (curTeam == WHITE) {
			for (int i = 0; i < BOARD_SIZE; i++)
				for (int j = 0; j < BOARD_SIZE; j++) {
					if(board[i][j] != NULL)
						if (board[i][j]->GetTeam() == WHITE)
							selectable[i][j] = true;
				}
		}
		else {
			for (int i = 0; i < BOARD_SIZE; i++)
				for (int j = 0; j < BOARD_SIZE; j++) {
					if (board[i][j] != NULL)
						if (board[i][j]->GetTeam() == BLACK)
							selectable[i][j] = true;
				}
		}
	}
	else {
		if (board[selectedY][selectedX]->GetTeam() == WHITE) {
			if (board[selectedY][selectedX] == block[W_PAWN]) {
				if (selectedY > 0)
					selectable[selectedY - 1][selectedX] = true;
			}
		}
		else {

		}
	}
}

void GameManager::Click(int x, int y) {
	x = x / 125;
	y = y / 125;
	if (selectable[y][x]) {
		if (selectedX == -1 && selectedY == -1) {
			selectedX = x;
			selectedY = y;
		}
		else {
			if (curTeam == WHITE)
				curTeam = BLACK;
			else
				curTeam = WHITE;
		}
	}
	else 
		return;
}

void GameManager::ClearSelectable() {
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
			selectable[i][j] = false;
}

GameManager::~GameManager()
{
}
