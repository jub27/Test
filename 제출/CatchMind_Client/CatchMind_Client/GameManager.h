#pragma once
#include <Windows.h>
#include <vector>
#include "Bitmap.h"

using namespace std;

#define BITMAP_NUMS 5
#define BUF_SIZE 4

#define GAME_WIDTH 1024
#define GAME_HEIGHT 768

#define BUTTON_WIDTH 80
#define BUTTON_HEIGHT 30

enum INST {
	MAKE_ROOM_REQUEST, MAKE_ROOM_ACCEPT, JOIN_ROOM_REQUEST, JOIN_ROOM_ACCEPT, PLAYER_ID_REQUEST, SET_PLAYER_ID
};

enum bitmaps {
	MAIN_BACKGROUND, MAKE_ROOM, EXIT, ROOM_LIST, ROOM_BACKGROUND
};

enum STATE {
	MAIN_MENU, ROOM, GAME_EXIT
};

class GameManager
{
private:
	HWND m_hWnd;
	HDC pHDC;
	HDC		MemDC;
	HBITMAP	MyBitMap;
	HBITMAP	OldBitMap;
	static GameManager* instance;
	LPCSTR source[BITMAP_NUMS] = {"res\\MAIN_BACKGROUND.bmp","res\\MAKE_ROOM.bmp","res\\EXIT.bmp", "res\\ROOM_LIST.bmp", "res\\ROOM_BACKGROUND.bmp"};
	Bitmap* bitmap[BITMAP_NUMS];
	STATE state;
	POINT mouse;
	SOCKET sock;
	bool click;
	int roomNum;
	int playerNum;
	char msg[BUF_SIZE];
public:
	GameManager();
	static GameManager* GetInstance() {
		if (instance == NULL)
			instance = new GameManager();
		return instance;
	}
	void MainMenu();
	void UpdateRoom();
	void UpdateRoomList();
	void MakeRoomRequest();
	void JoinRoomRequest(int roomNum);
	void JoinRoom(int roomNum);
	void Init(HWND hWnd, SOCKET sock);
	STATE GetState();
	void SetMousePoint(int x, int y);
	void SetMouseClick(bool c);
	void SetPlayerNum(int playerNum);
	bool ClickCehck(int x, int y);
	~GameManager();
};

