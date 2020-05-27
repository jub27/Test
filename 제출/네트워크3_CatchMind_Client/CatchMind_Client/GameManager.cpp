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
	playerID = -1;
	click = false;
	gameStart = false;
	firstMain = true;
	firstExit = true;
	is_playerID = false;
	curRoomNums = 0;
	for (int i = 0; i < MAX_ROOM_NUM; i++) {
		roomList[i] = 0;
	}
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
	if (!is_playerID) {
		PlayerIDRequest();
		is_playerID = true;
	}
	else {
		if (playerID == -1)
			return;
	}
	if (firstMain) {
		RoomListRequest();
		firstMain = false;
	}
	bitmap[MAIN_BACKGROUND]->Draw(MemDC, 0, 0);
	bitmap[ROOM_LIST]->Draw(MemDC, 100, 100);
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
			firstMain = true;
		}
	}
	ShowRoomList();
	click = false;
	BitBlt(pHDC, 0, 0, GAME_WIDTH, GAME_HEIGHT, MemDC, 0, 0, SRCCOPY);
}

void GameManager::PlayerIDRequest() {
	msg[0] = PLAYER_ID_REQUEST;
	send(sock, msg, BUF_SIZE, 0);
}

void GameManager::FirstMainInit() {
	firstMain = true;
}

void GameManager::MakeRoomRequest() {
	msg[0] = MAKE_ROOM_REQUEST;
	msg[1] = playerID;
	send(sock, msg, BUF_SIZE, 0);
}

void GameManager::SetPlayerID(int playerID) {
	this->playerID = playerID;
}

void GameManager::JoinRoomRequest(int roomNum) {
	msg[0] = JOIN_ROOM_REQUEST;
	msg[1] = roomNum;
	msg[2] = playerID;
	send(sock, msg, BUF_SIZE, 0);
	return;
}

void GameManager::JoinRoom(int roomNum) {
	this->roomNum = roomNum;
	state = ROOM;
	firstMain = true;
	firstExit = true;
}

void GameManager::RoomListRequest() {
	msg[0] = GET_ROOM_LIST;
	send(sock, msg, BUF_SIZE, 0);
}

void GameManager::Room() {

	bitmap[ROOM_BACKGROUND]->Draw(MemDC,0, 0);
	bitmap[EXIT]->Draw(MemDC, 830, 35);
	if (click) {
		if (ClickCehck(830, 35) && !gameStart) {
			state = MAIN_MENU;
			if (firstExit) {
				ExitRoomRequest();
				firstExit = false;
			}
			firstMain = true;
			click = false;
		}
	}
	click = false;
	BitBlt(pHDC, 0, 0, GAME_WIDTH, GAME_HEIGHT, MemDC, 0, 0, SRCCOPY);
}

void GameManager::ExitRoomRequest() {
	msg[0] = EXIT_ROOM_REQUEST;
	msg[1] = playerID;
	msg[2] = roomNum;
	send(sock, msg, BUF_SIZE, 0);
}

void GameManager::SetRoomList(char * msg) {
	int j = 0;
	curRoomNums = msg[1];
	for (int i = 0; i < MAX_ROOM_NUM; i++)
		roomList[i] = 0;
	for (int i = 0; i < (int)msg[1]; i++)
		roomList[i] = (int)msg[2+i];
}

bool GameManager::ClickCehck(int x, int y) {
	RECT in_Rect = {x, y, x + BUTTON_WIDTH, y + BUTTON_HEIGHT};
	return PtInRect(&in_Rect, mouse);
}

void GameManager::ExitRoom() {
	roomNum = -1;
	state = MAIN_MENU;
}

void GameManager::ShowRoomList() {
	int x = 120, y = 150;
	for (int i = 0; i < curRoomNums; i++) {
		bitmap[ROOM_INFO]->Draw(MemDC, x, y);
		RECT rt = { x+10, y+10, x + 50, y + 30 };
		char buf[256];
		sprintf_s(buf, "%d/%d", roomList[i], MAX_ROOM_PLAYER);
		DrawText(MemDC, buf, -1, &rt, DT_CENTER | DT_WORDBREAK);
		if (click) {
			if (ClickCehck(x, y)) {
				JoinRoomRequest(i + 1);
			}
		}
		y += 50;
	}
	
}

int GameManager::GetPlayerID() {
	return playerID;
}

GameManager::~GameManager() {
}