#pragma once
#include <Windows.h>
#include <vector>
#include "Bitmap.h"

using namespace std;

#define BITMAP_NUMS 7
#define BUF_SIZE 12
#define MAX_ROOM_NUM 5
#define MAX_ROOM_PLAYER 4

#define GAME_WIDTH 1024
#define GAME_HEIGHT 768

#define BUTTON_WIDTH 80
#define BUTTON_HEIGHT 30

enum INST {
	N ,MAKE_ROOM_REQUEST, MAKE_ROOM_ACCEPT, JOIN_ROOM_REQUEST, JOIN_ROOM_ACCEPT, PLAYER_ID_REQUEST, SET_PLAYER_ID
	, GET_ROOM_LIST, SET_ROOM_LIST, EXIT_ROOM_REQUEST, EXIT_ROOM_ACCEPT
};

enum bitmaps {
	MAIN_BACKGROUND, MAKE_ROOM, EXIT, ROOM_LIST, ROOM_BACKGROUND, ROOM_INFO, CHARACTER
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
	LPCSTR source[BITMAP_NUMS] = {"res\\MAIN_BACKGROUND.bmp","res\\MAKE_ROOM.bmp","res\\EXIT.bmp", "res\\ROOM_LIST.bmp", "res\\ROOM_BACKGROUND.bmp"
	, "res\\ROOM_INFO.bmp", "res\\CHARACTER.bmp"};
	Bitmap* bitmap[BITMAP_NUMS];
	STATE state;
	POINT mouse;
	SOCKET sock;
	bool click;
	bool gameStart;

	int roomNum;
	int playerID;
	int roomList[MAX_ROOM_NUM];
	int curRoomNums;
	char msg[BUF_SIZE];
	bool firstMain;
	bool firstExit;
	bool is_playerID;
public:
	GameManager();
	static GameManager* GetInstance() {
		if (instance == NULL)
			instance = new GameManager();
		return instance;
	}
	void MainMenu();
	void Room();
	void ShowRoomList();
	void SetRoomList(char * msg);
	void RoomListRequest();
	void MakeRoomRequest();
	void ExitRoomRequest();
	void ExitRoom();
	void JoinRoomRequest(int roomNum);
	void JoinRoom(int roomNum);
	void Init(HWND hWnd, SOCKET sock);
	STATE GetState();
	void SetMousePoint(int x, int y);
	void SetMouseClick(bool c);
	void PlayerIDRequest();
	void SetPlayerID(int playerID);
	int GetPlayerID();
	void FirstMainInit();
	bool ClickCehck(int x, int y);
	~GameManager();
};