#include "GameManager.h"

GameManager* GameManager::instance = NULL;

GameManager::GameManager() {
}

void GameManager::Init(HWND hWnd, SOCKET sock) {
	m_hWnd = hWnd;
	pHDC = GetDC(m_hWnd);
	MemDC = CreateCompatibleDC(pHDC);
	MyBitMap = CreateCompatibleBitmap(pHDC, GAME_WIDTH, GAME_HEIGHT);
	OldBitMap = (HBITMAP)SelectObject(MemDC, MyBitMap);
	for (int i = 0; i < BITMAP_NUMS; i++) {
		bitmap[i] = new Bitmap();
		bitmap[i]->Init(MemDC, source[i]);
	}
	this->sock = sock;
	state = MAIN_MENU;
	mouse.x = -1;
	mouse.y = -1;
	click = false;
}

STATE GameManager::GetState() {
	return state;
}

void GameManager::SetMousePoint(int x, int y) {
	mouse.x = x;
	mouse.y = y;
}

void GameManager::SetMouseClick(bool c) {
	click = c;
}

void GameManager::MainMenu() {
	bitmap[MAIN_BACKGROUND]->Draw(MemDC, 0, 0);
	bitmap[ROOM_LIST]->Draw(MemDC, 100, 100);
	if (click) {
		if (ClickCehck(100, 100)) {

		}
	}
	bitmap[MAKE_ROOM]->Draw(MemDC, 900, 200);
	if (click) {
		if (ClickCehck(900, 200)) {
			MakeRoomRequest();
		}
	}
	bitmap[EXIT]->Draw(MemDC, 900, 250);
	if (click) {
		if (ClickCehck(900, 250)) {
			state = GAME_EXIT;
		}
	}
	BitBlt(pHDC, 0, 0, GAME_WIDTH, GAME_HEIGHT, MemDC, 0, 0, SRCCOPY);
}

void GameManager::MakeRoomRequest() {
	msg[0] = MAKE_ROOM_REQUEST;
	send(sock, msg, BUF_SIZE, 0);
}

void GameManager::UpdateRoomList() {

}

void GameManager::SetPlayerNum(int playerNum) {
	this->playerNum = playerNum;
}

void GameManager::JoinRoomRequest(int roomNum) {
	msg[0] = JOIN_ROOM_REQUEST;
	msg[1] = roomNum;
	send(sock, msg, BUF_SIZE, 0);
	return;
}

void GameManager::JoinRoom(int roomNum) {
	this->roomNum = roomNum;
	state = ROOM;
}

void GameManager::UpdateRoom() {
	bitmap[ROOM_BACKGROUND]->Draw(MemDC,0, 0);
	BitBlt(pHDC, 0, 0, GAME_WIDTH, GAME_HEIGHT, MemDC, 0, 0, SRCCOPY);
}

bool GameManager::ClickCehck(int x, int y) {
	RECT in_Rect = {x, y, x + BUTTON_WIDTH, y + BUTTON_HEIGHT};
	return PtInRect(&in_Rect, mouse);
}

GameManager::~GameManager() {
}