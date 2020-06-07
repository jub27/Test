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
	firstMain = true;
	firstExit = true;
	firstRoom = true;
	is_playerID = false;
	draw = false;
	curRoomNums = 0;
	point = 0;
	turn = false;
	pos = -1;
	edit = NULL;
	for (int i = 0; i < MAX_ROOM_NUM; i++) {
		roomList[i] = 0;
	}
}

STATE GameManager::GetState() {
	return state;
}

void GameManager::SetDrawFalse() {
	draw = false;
}

void GameManager::SetDrawTrue() {
	draw = true;
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
	if (playerID != -1) {
		if (firstMain) {
			RoomListRequest();
			firstMain = false;
		}
	}
	else {
		return;
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
	packet = new Packet();
	packet->inst = PLAYER_ID_REQUEST;
	packet->playerID = playerID;
	packet->roomNum = roomNum;
	send(sock,(char *)packet, sizeof(Packet), 0);
	free(packet);
}

void GameManager::FirstMainInit() {
	firstMain = true;
}

void GameManager::FirstRoomInit() {
	firstRoom = true;
}

void GameManager::MakeRoomRequest() {
	packet = new Packet();
	packet->inst = MAKE_ROOM_REQUEST;
	packet->playerID = playerID;
	packet->roomNum = roomNum;
	send(sock, (char*)packet, sizeof(Packet), 0);
	free(packet);
}

void GameManager::SetPlayerID(int playerID) {
	this->playerID = playerID;
}

void GameManager::JoinRoomRequest(int roomNum) {
	packet = new Packet();
	packet->inst = JOIN_ROOM_REQUEST;
	packet->playerID = playerID;
	packet->data[0] = roomNum;
	send(sock, (char*)packet, sizeof(Packet), 0);
	free(packet);
}

void GameManager::JoinRoom(int roomNum, int pos) {
	this->roomNum = roomNum;
	this->pos = pos;
	state = ROOM;
	firstMain = true;
	firstExit = true;
}

void GameManager::RoomListRequest() {
	packet = new Packet();
	packet->inst = GET_ROOM_LIST;
	packet->playerID = playerID;
	packet->roomNum = roomNum;
	send(sock, (char*)packet, sizeof(Packet), 0);
	free(packet);
}

void GameManager::RoomInfoRequest() {
	packet = new Packet();
	packet->inst = ROOM_INFO_REQUEST;
	packet->playerID = playerID;
	packet->roomNum = roomNum;
	send(sock, (char*)packet, sizeof(Packet), 0);
	free(packet);
}

void GameManager::SetRoomInfo(int * msg) {
	for (int i = 0; i < MAX_ROOM_PLAYER; i++)
		roomInfo[i] = msg[i];
}

void GameManager::Room() {
	if (firstRoom) {
		RoomInfoRequest();
		firstRoom = false;
	}
	if(edit == NULL)
		edit = CreateWindow("EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, EDIT_BOX_X, EDIT_BOX_Y, EDIT_BOX_X_SIZE, EDIT_BOX_Y_SIZE, m_hWnd, (HMENU)EDIT_ID, NULL, NULL);
	bitmap[ROOM_BACKGROUND]->Draw(MemDC,0, 0);
	bitmap[EXIT]->Draw(MemDC, 830, 35);
	if (click) {
		if (ClickCehck(830, 35) && state != GAME_START) {
			state = MAIN_MENU;
			if (firstExit) {
				ExitRoomRequest();
				firstExit = false;
			}
			firstMain = true;
			click = false;
		}
	}
	int x = 150; int y = 180;
	int count = 0;
	for (int i = 0; i < MAX_ROOM_PLAYER; i++, y += 103) {
		if (roomInfo[i] != 0) {
			count++;
			char buf[256];
			RECT rt = { x + 10, y + 10, x + 50, y + 30 };
			sprintf_s(buf, "%d", roomInfo[i]);
			DrawText(MemDC, buf, -1, &rt, DT_CENTER | DT_WORDBREAK);
		}
	}
	if (count >= 2 && state == ROOM) {
		bitmap[START]->Draw(MemDC, 450, 300);
		if (click) {
			if (ClickCehck(450, 300)) {
				GameStartRequest();
				click = false;
			}
		}
	}
	if (state == GAME_START) {
		for (int i = 0; i < lineList.size(); i++) {
			MoveToEx(MemDC, lineList[i].x1, lineList[i].y1, NULL);
			LineTo(MemDC, lineList[i].x2, lineList[i].y2);
		}
		if (turn) {
			RECT rt = {500,100,550,150 };
			char buf[256];
			sprintf_s(buf, "%s", answer);
			DrawText(MemDC, buf, -1, &rt, DT_CENTER | DT_WORDBREAK);
		}
	}
	click = false;

	BitBlt(pHDC, 0, 0, GAME_WIDTH, EDIT_BOX_Y, MemDC, 0, 0, SRCCOPY);
	BitBlt(pHDC, 0, EDIT_BOX_Y + EDIT_BOX_Y_SIZE, GAME_WIDTH, GAME_HEIGHT - (EDIT_BOX_Y + EDIT_BOX_Y_SIZE), MemDC, 0, EDIT_BOX_Y + EDIT_BOX_Y_SIZE, SRCCOPY);
	BitBlt(pHDC, 0, EDIT_BOX_Y,EDIT_BOX_X, EDIT_BOX_Y_SIZE, MemDC, 0, EDIT_BOX_Y, SRCCOPY);
	BitBlt(pHDC, EDIT_BOX_X + EDIT_BOX_X_SIZE, EDIT_BOX_Y, GAME_WIDTH - (EDIT_BOX_X+EDIT_BOX_X_SIZE), GAME_HEIGHT - EDIT_BOX_Y, MemDC, EDIT_BOX_X + EDIT_BOX_X_SIZE, EDIT_BOX_Y, SRCCOPY);
}

void GameManager::InitRoom() {
	lineList.clear();
	state = ROOM;
	turn = false;
	strcpy_s(answer, "");
}

void GameManager::GameStartRequest() {
	packet = new Packet();
	packet->inst = GAME_START_REQUEST;
	packet->playerID = playerID;
	packet->roomNum = roomNum;
	send(sock, (char*)packet, sizeof(Packet), 0);
	free(packet);
}

void GameManager::SendAnswer(char * str) {
	packet = new Packet();
	packet->inst = ANSWER_FROM_CLIENT;
	packet->playerID = playerID;
	packet->roomNum = roomNum;
	strcpy_s(packet->answer, str);
	send(sock, (char*)packet, sizeof(Packet), 0);
	free(packet);
}

void GameManager::ExitRoomRequest() {
	packet = new Packet();
	packet->inst = EXIT_ROOM_REQUEST;
	packet->playerID = playerID;
	packet->roomNum = roomNum;
	send(sock, (char*)packet, sizeof(Packet), 0);
	free(packet);
}

void GameManager::SetRoomList(int * msg) {
	int j = 0;
	curRoomNums = msg[0];
	for (int i = 0; i < MAX_ROOM_NUM; i++)
		roomList[i] = 0;
	for (int i = 0; i < curRoomNums; i++)
		roomList[i] = msg[1+i];
}

bool GameManager::ClickCehck(int x, int y) {
	RECT in_Rect = {x, y, x + BUTTON_WIDTH, y + BUTTON_HEIGHT};
	return PtInRect(&in_Rect, mouse);
}

void GameManager::GameStart() {
	state = GAME_START;
}

void GameManager::ExitRoom() {
	roomNum = -1;
	state = MAIN_MENU;
}

bool GameManager::RangeCheck(int x, int y) {
	if (x < BOARD_X_START || x > BOARD_X_END ||
		y < BOARD_Y_START || y > BOARD_Y_END) {
		return false;
	}
	return true;
}

void GameManager::DrawRequest(int newX, int newY) {
	if (!RangeCheck(mouse.x, mouse.y) || !RangeCheck(newX, newY))
		return;
	packet = new Packet();
	packet->inst = DRAW_REQUEST;
	packet->playerID = playerID;
	packet->roomNum = roomNum;
	packet->data[0] = mouse.x;
	packet->data[1] = mouse.y;
	packet->data[2] = newX;
	packet->data[3] = newY;
	send(sock, (char *)packet, sizeof(Packet), 0);
	free(packet);
}

void GameManager::SetTurnTrue(char* answer) {
	strcpy_s(this->answer, answer);
	turn = true;
}

void GameManager::AddLine(int x1, int y1, int x2, int y2) {
	Line a = { x1, y1, x2, y2 };
	lineList.push_back(a);
}

void GameManager::ShowRoomList() {
	int x = ROOM_START_X, y = ROOM_START_Y;
	for (int i = 0; i < curRoomNums; i++) {
		bitmap[ROOM_INFO]->Draw(MemDC, x, y);
		RECT rt = { x+10, y+10, x + 50, y + 30 };
		char buf[256];
		sprintf_s(buf, "%d/%d", roomList[i], MAX_ROOM_PLAYER);
		DrawText(MemDC, buf, -1, &rt, DT_CENTER | DT_WORDBREAK);
		if (click) {
			if (ClickCehck(x, y)) {
				JoinRoomRequest(i);
			}
		}
		y += 50;
	}
	
}

void GameManager::SetTurnFalse() {
	turn = false;
}

bool GameManager::GetTurn() {
	return turn;
}

int GameManager::GetPlayerID() {
	return playerID;
}

int GameManager::GetRoomNum() {
	return roomNum;
}

bool GameManager::GetDraw() {
	return draw;
}

HWND GameManager::GetEdit() {
	return edit;
}

GameManager::~GameManager() {
}