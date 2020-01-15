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

void GameManager::CheckSelectable() {
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
				if (selectedY > 0) {
					if (selectedY == BOARD_SIZE - 2) {
						if (board[selectedY - 2][selectedX] == NULL)
							selectable[selectedY - 2][selectedX] = true;
					}
					if (board[selectedY - 1][selectedX] == NULL)
						selectable[selectedY - 1][selectedX] = true;
					if (selectedX == 0) {
						if (board[selectedY - 1][selectedX + 1] != NULL && board[selectedY - 1][selectedX + 1]->GetTeam() == BLACK)
							selectable[selectedY - 1][selectedX + 1] = true;
					}
					else if (selectedX < BOARD_SIZE - 1) {
						if (board[selectedY - 1][selectedX + 1] != NULL && board[selectedY - 1][selectedX + 1]->GetTeam() == BLACK)
							selectable[selectedY - 1][selectedX + 1] = true;
						if (board[selectedY - 1][selectedX - 1] != NULL && board[selectedY - 1][selectedX - 1]->GetTeam() == BLACK)
							selectable[selectedY - 1][selectedX - 1] = true;
					}
					else {
						if (board[selectedY - 1][selectedX - 1] != NULL && board[selectedY - 1][selectedX - 1]->GetTeam() == BLACK)
							selectable[selectedY - 1][selectedX - 1] = true;
					}
				}
			}
			else if (board[selectedY][selectedX] == block[W_KNIGHT]) {
				if(selectedY + 2 < BOARD_SIZE && selectedX + 1 < BOARD_SIZE)
					if (board[selectedY + 2][selectedX + 1] == NULL || board[selectedY + 2][selectedX + 1]->GetTeam() == BLACK)
						selectable[selectedY + 2][selectedX + 1] = true;

				if (selectedY + 2 < BOARD_SIZE && selectedX - 1 >= 0 )
					if (board[selectedY + 2][selectedX - 1] == NULL || board[selectedY + 2][selectedX - 1]->GetTeam() == BLACK)
						selectable[selectedY + 2][selectedX - 1] = true;

				if (selectedY - 2 >= 0 && selectedX + 1 < BOARD_SIZE)
					if (board[selectedY - 2][selectedX + 1] == NULL || board[selectedY - 2][selectedX + 1]->GetTeam() == BLACK)
						selectable[selectedY - 2][selectedX + 1] = true;

				if (selectedY - 2 >= 0 && selectedX - 1 >= 0) 
					if (board[selectedY - 2][selectedX - 1] == NULL || board[selectedY - 2][selectedX - 1]->GetTeam() == BLACK)
						selectable[selectedY - 2][selectedX - 1] = true;

				if (selectedY + 1 < BOARD_SIZE && selectedX + 2 < BOARD_SIZE)
					if (board[selectedY + 1][selectedX + 2] == NULL || board[selectedY + 1][selectedX + 2]->GetTeam() == BLACK)
						selectable[selectedY + 1][selectedX + 2] = true;

				if (selectedY + 1 < BOARD_SIZE && selectedX - 2 >= 0)
					if (board[selectedY + 1][selectedX - 2] == NULL || board[selectedY + 1][selectedX - 2]->GetTeam() == BLACK)
						selectable[selectedY + 1][selectedX - 2] = true;

				if (selectedY - 1 >= 0 && selectedX + 2 < BOARD_SIZE)
					if (board[selectedY - 1][selectedX + 2] == NULL || board[selectedY - 1][selectedX + 2]->GetTeam() == BLACK)
						selectable[selectedY - 1][selectedX + 2] = true;

				if (selectedY -1 >= 0 && selectedX - 2 >= 0)
					if (board[selectedY - 1][selectedX - 2] == NULL || board[selectedY -1][selectedX - 2]->GetTeam() == BLACK)
						selectable[selectedY - 1][selectedX - 2] = true;

			}
			else if (board[selectedY][selectedX] == block[W_BISHOP]) {
				int y = selectedY + 1, x = selectedX + 1;
				while (x < BOARD_SIZE && y < BOARD_SIZE) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == BLACK) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					x++;
					y++;
				}
				y = selectedY + 1;
				x = selectedX - 1;
				while (x >= 0 && y < BOARD_SIZE) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == BLACK) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					x--;
					y++;
				}
				y = selectedY - 1;
				x = selectedX - 1;
				while (x >= 0 && y >= 0) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == BLACK) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					x--;
					y--;
				}
				y = selectedY - 1;
				x = selectedX + 1;
				while (x < BOARD_SIZE && y >= 0) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == BLACK) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					x++;
					y--;
				}
			}
			else if (board[selectedY][selectedX] == block[W_ROOK]) {
				int x = selectedX + 1;
				int y = selectedY;
				while (x < BOARD_SIZE) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == BLACK) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					x++;
				}
				x = selectedX - 1;
				y = selectedY;
				while (x >= 0) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == BLACK) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					x--;
				}
				x = selectedX;
				y = selectedY - 1;
				while (y >= 0) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == BLACK) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					y--;
				}
				x = selectedX;
				y = selectedY + 1;
				while (y < BOARD_SIZE) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == BLACK) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					y++;
				}
			}
			else if (board[selectedY][selectedX] == block[W_QUEEN]) {
				int x, y;
				x = selectedX;
				y = selectedY - 1;
				while (y >= 0) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == BLACK) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					y--;
				}
				x = selectedX + 1;
				y = selectedY - 1;
				while (y >= 0 && x < BOARD_SIZE) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == BLACK) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					x++;
					y--;
				}
				x = selectedX + 1;
				y = selectedY;
				while (x < BOARD_SIZE) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == BLACK) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					x++;
				}
				x = selectedX + 1;
				y = selectedY + 1;
				while (x < BOARD_SIZE && y < BOARD_SIZE) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == BLACK) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					x++;
					y++;
				}
				x = selectedX;
				y = selectedY + 1;
				while (y < BOARD_SIZE) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == BLACK) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					y++;
				}
				x = selectedX - 1;
				y = selectedY + 1;
				while (y < BOARD_SIZE && x >= 0) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == BLACK) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					y++;
					x--;
				}
				x = selectedX - 1;
				y = selectedY;
				while (x >= 0) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == BLACK) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					x--;
				}
				x = selectedX - 1;
				y = selectedY - 1;
				while (y >= 0 && x >= 0) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == BLACK) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					y--;
					x--;
				}
			}
			else if (board[selectedY][selectedX] == block[W_KING]) {
				int x = selectedX;
				int y = selectedY - 1;
				if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
					if (board[y][x] == NULL || board[y][x]->GetTeam() == BLACK)
						selectable[y][x] = true;
				}
				x++;
				if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
					if (board[y][x] == NULL || board[y][x]->GetTeam() == BLACK)
						selectable[y][x] = true;
				}
				y++;
				if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
					if (board[y][x] == NULL || board[y][x]->GetTeam() == BLACK)
						selectable[y][x] = true;
				}
				y++;
				if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
					if (board[y][x] == NULL || board[y][x]->GetTeam() == BLACK)
						selectable[y][x] = true;
				}
				x--;
				if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
					if (board[y][x] == NULL || board[y][x]->GetTeam() == BLACK)
						selectable[y][x] = true;
				}
				x--;
				if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
					if (board[y][x] == NULL || board[y][x]->GetTeam() == BLACK)
						selectable[y][x] = true;
				}
				y--;
				if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
					if (board[y][x] == NULL || board[y][x]->GetTeam() == BLACK)
						selectable[y][x] = true;
				}
				y--;
				if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
					if (board[y][x] == NULL || board[y][x]->GetTeam() == BLACK)
						selectable[y][x] = true;
				}
			}
		}
		else {
			if (board[selectedY][selectedX] == block[B_PAWN]) {
				if (selectedY < BOARD_SIZE-1) {
					if (selectedY == 1) {
						if (board[selectedY + 2][selectedX] == NULL)
							selectable[selectedY + 2][selectedX] = true;
					}
					if(board[selectedY+1][selectedX] == NULL)
						selectable[selectedY + 1][selectedX] = true;
					if (selectedX == 0) {
						if (board[selectedY + 1][selectedX + 1] != NULL && board[selectedY + 1][selectedX + 1]->GetTeam() == WHITE)
							selectable[selectedY + 1][selectedX + 1] = true;
					}
					else if (selectedX < BOARD_SIZE - 1) {
						if (board[selectedY + 1][selectedX + 1] != NULL && board[selectedY + 1][selectedX + 1]->GetTeam() == WHITE)
							selectable[selectedY + 1][selectedX + 1] = true;
						if (board[selectedY + 1][selectedX - 1] != NULL && board[selectedY + 1][selectedX - 1]->GetTeam() == WHITE)
							selectable[selectedY + 1][selectedX - 1] = true;
					}
					else {
						if (board[selectedY + 1][selectedX - 1] != NULL && board[selectedY + 1][selectedX - 1]->GetTeam() == WHITE)
							selectable[selectedY+ 1][selectedX - 1] = true;
					}
				}
			}
			else if (board[selectedY][selectedX] == block[B_KNIGHT]) {
				if (selectedY + 2 < BOARD_SIZE && selectedX + 1 < BOARD_SIZE)
					if (board[selectedY + 2][selectedX + 1] == NULL || board[selectedY + 2][selectedX + 1]->GetTeam() == WHITE)
						selectable[selectedY + 2][selectedX + 1] = true;

				if (selectedY + 2 < BOARD_SIZE && selectedX - 1 >= 0)
					if (board[selectedY + 2][selectedX - 1] == NULL || board[selectedY + 2][selectedX - 1]->GetTeam() == WHITE)
						selectable[selectedY + 2][selectedX - 1] = true;

				if (selectedY - 2 >= 0 && selectedX + 1 < BOARD_SIZE)
					if (board[selectedY - 2][selectedX + 1] == NULL || board[selectedY - 2][selectedX + 1]->GetTeam() == WHITE)
						selectable[selectedY - 2][selectedX + 1] = true;

				if (selectedY - 2 >= 0 && selectedX - 1 >= 0)
					if (board[selectedY - 2][selectedX - 1] == NULL || board[selectedY - 2][selectedX - 1]->GetTeam() == WHITE)
						selectable[selectedY - 2][selectedX - 1] = true;

				if (selectedY + 1 < BOARD_SIZE && selectedX + 2 < BOARD_SIZE)
					if (board[selectedY + 1][selectedX + 2] == NULL || board[selectedY + 1][selectedX + 2]->GetTeam() == WHITE)
						selectable[selectedY + 1][selectedX + 2] = true;

				if (selectedY + 1 < BOARD_SIZE && selectedX - 2 >= 0)
					if (board[selectedY + 1][selectedX - 2] == NULL || board[selectedY + 1][selectedX - 2]->GetTeam() == WHITE)
						selectable[selectedY + 1][selectedX - 2] = true;

				if (selectedY - 1 >= 0 && selectedX + 2 < BOARD_SIZE)
					if (board[selectedY - 1][selectedX + 2] == NULL || board[selectedY - 1][selectedX + 2]->GetTeam() == WHITE)
						selectable[selectedY - 1][selectedX + 2] = true;

				if (selectedY - 1 >= 0 && selectedX - 2 >= 0)
					if (board[selectedY - 1][selectedX - 2] == NULL || board[selectedY - 1][selectedX - 2]->GetTeam() == WHITE)
						selectable[selectedY - 1][selectedX - 2] = true;

			}
			else if (board[selectedY][selectedX] == block[B_BISHOP]) {
				int y = selectedY + 1, x = selectedX + 1;
				while (x < BOARD_SIZE && y < BOARD_SIZE) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == WHITE) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					x++;
					y++;
				}
				y = selectedY + 1;
				x = selectedX - 1;
				while (x >= 0 && y < BOARD_SIZE) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == WHITE) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					x--;
					y++;
				}
				y = selectedY - 1;
				x = selectedX - 1;
				while (x >= 0 && y >= 0) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == WHITE) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					x--;
					y--;
				}
				y = selectedY - 1;
				x = selectedX + 1;
				while (x < BOARD_SIZE && y >= 0) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == WHITE) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					x++;
					y--;
				}
			}
			else if (board[selectedY][selectedX] == block[B_ROOK]) {
				int x = selectedX + 1;
				int y = selectedY;
				while (x < BOARD_SIZE) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == WHITE) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					x++;
				}
				x = selectedX - 1;
				y = selectedY;
				while (x >= 0) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == WHITE) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					x--;
				}
				x = selectedX;
				y = selectedY - 1;
				while (y >= 0) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == WHITE) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					y--;
				}
				x = selectedX;
				y = selectedY + 1;
				while (y < BOARD_SIZE) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == WHITE) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					y++;
				}
			}
			else if (board[selectedY][selectedX] == block[B_QUEEN]) {
				int x, y;
				x = selectedX;
				y = selectedY - 1;
				while (y >= 0) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == WHITE) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					y--;
				}
				x = selectedX + 1;
				y = selectedY - 1;
				while (y >= 0 && x < BOARD_SIZE) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == WHITE) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					x++;
					y--;
				}
				x = selectedX + 1;
				y = selectedY;
				while (x < BOARD_SIZE) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == WHITE) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					x++;
				}
				x = selectedX + 1;
				y = selectedY + 1;
				while (x < BOARD_SIZE && y < BOARD_SIZE) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == WHITE) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					x++;
					y++;
				}
				x = selectedX;
				y = selectedY + 1;
				while (y < BOARD_SIZE) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == WHITE) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					y++;
				}
				x = selectedX - 1;
				y = selectedY + 1;
				while (y < BOARD_SIZE && x >= 0) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == WHITE) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					y++;
					x--;
				}
				x = selectedX - 1;
				y = selectedY;
				while (x >= 0) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == WHITE) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					x--;
				}
				x = selectedX - 1;
				y = selectedY - 1;
				while (y >= 0 && x >= 0) {
					if (board[y][x] == NULL)
						selectable[y][x] = true;
					else if (board[y][x]->GetTeam() == WHITE) {
						selectable[y][x] = true;
						break;
					}
					else
						break;
					y--;
					x--;
				}
			}
			else if (board[selectedY][selectedX] == block[B_KING]) {
				int x = selectedX;
				int y = selectedY - 1;
				if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
					if (board[y][x] == NULL || board[y][x]->GetTeam() == WHITE)
						selectable[y][x] = true;
				}
				x++;
				if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
					if (board[y][x] == NULL || board[y][x]->GetTeam() == WHITE)
						selectable[y][x] = true;
				}
				y++;
				if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
					if (board[y][x] == NULL || board[y][x]->GetTeam() == WHITE)
						selectable[y][x] = true;
				}
				y++;
				if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
					if (board[y][x] == NULL || board[y][x]->GetTeam() == WHITE)
						selectable[y][x] = true;
				}
				x--;
				if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
					if (board[y][x] == NULL || board[y][x]->GetTeam() == WHITE)
						selectable[y][x] = true;
				}
				x--;
				if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
					if (board[y][x] == NULL || board[y][x]->GetTeam() == WHITE)
						selectable[y][x] = true;
				}
				y--;
				if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
					if (board[y][x] == NULL || board[y][x]->GetTeam() == WHITE)
						selectable[y][x] = true;
				}
				y--;
				if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
					if (board[y][x] == NULL || board[y][x]->GetTeam() == WHITE)
						selectable[y][x] = true;
				}
			}
		}
	}
}

int GameManager::Click(POINT point) {
	int ret = 0;
	RECT inRect = { 0, 0, 125 * BOARD_SIZE, 125 * BOARD_SIZE };
	if (!PtInRect(&inRect, point))
		return ret;
	int x = point.x / 125;
	int y = point.y / 125;
	if (selectable[y][x]) {
		if (selectedX == -1 && selectedY == -1) {
			selectedX = x;
			selectedY = y;
			return ret;
		}
		else {
			if (board[y][x] == block[B_KING])
				ret = 1;
			else if (board[y][x] == block[W_KING])
				ret = 2;
			board[y][x] = board[selectedY][selectedX];
			board[selectedY][selectedX] = NULL;

			selectedX = -1;
			selectedY = -1;

			if (curTeam == WHITE)
				curTeam = BLACK;
			else
				curTeam = WHITE;
		}
		return ret;
	}
	else
		return ret;
}

void GameManager::UnClick() {
	selectedX = -1;
	selectedY = -1;
}

void GameManager::ClearSelectable() {
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
			selectable[i][j] = false;
}

GameManager::~GameManager()
{
	for (int i = 0; i < BITMAPNUMS; i++)
		delete block[i];
}