#include<Windows.h>
#include<time.h>
#include "GameManager.h"
#include <process.h>
#include <iostream>
using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCSTR lpszClass = "CatchMind";

#define NAME_SIZE 20
#define PORT_NUM 3800

unsigned WINAPI RecvMsg(void* arg);
void ErrorHandling(const char* msg);

SOCKET hSock;
char msg[BUF_SIZE];

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPervlnstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, GAME_WIDTH, GAME_HEIGHT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	WSADATA wsaData;
	SOCKADDR_IN servAdr;
	HANDLE hSndThread, hRcvThread;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error!");
	}

	hSock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAdr.sin_port = htons(PORT_NUM);

	if (connect(hSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
	{
		ErrorHandling("connect() error");
	}
	GameManager::GetInstance()->Init(hWnd, hSock);
	//hSndThread = (HANDLE)_beginthreadex(NULL, 0, SendMsg, (void*)&hSock, 0, NULL);
	hRcvThread = (HANDLE)_beginthreadex(NULL, 0, RecvMsg, (void*)&hSock, 0, NULL);

	while (true) {
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
			if (Message.message == WM_QUIT)
				break;
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else {
			int GameState = GameManager::GetInstance()->GetState();
			switch (GameState) {
			case MAIN_MENU:
				GameManager::GetInstance()->MainMenu();
				break;
			case GAME_START:
			case ROOM:
				GameManager::GetInstance()->Room();
				break;
			case GAME_EXIT:
				break;
			}
			if (GameState == GAME_EXIT)
				break;
		}

	}

	closesocket(hSock);
	WSACleanup();

	return (int)Message.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){
	switch (iMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_MOUSEMOVE:
		if (GameManager::GetInstance()->GetTurn() && GameManager::GetInstance()->GetDraw()) {
			GameManager::GetInstance()->DrawRequest(LOWORD(lParam), HIWORD(lParam));
		}
		GameManager::GetInstance()->SetMousePoint(LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_LBUTTONDOWN:
		GameManager::GetInstance()->SetMouseClick(false);
		if (GameManager::GetInstance()->GetTurn()) {
			GameManager::GetInstance()->SetDrawTrue();
		}
		return 0;
	case WM_LBUTTONUP:
		GameManager::GetInstance()->SetMouseClick(true);
		if (GameManager::GetInstance()->GetTurn()) {
			GameManager::GetInstance()->SetDrawFalse();
		}
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

unsigned WINAPI RecvMsg(void* arg)   // read thread main
{
	int hSock = *((SOCKET*)arg);
	int strLen;

	while (1)
	{
		strLen = recv(hSock, msg, BUF_SIZE, 0);

		if (strLen == -1)
			return -1;
		switch (msg[0]) {
		case MAKE_ROOM_ACCEPT:
			if ((int)msg[1] == GameManager::GetInstance()->GetPlayerID()) {
				GameManager::GetInstance()->JoinRoomRequest((int)msg[2]);
			}
			GameManager::GetInstance()->FirstMainInit();
			break;
		case JOIN_ROOM_ACCEPT:
			if((int)msg[1] == GameManager::GetInstance()->GetPlayerID())
				GameManager::GetInstance()->JoinRoom((int)msg[2], (int)msg[3]);
			if ((int)msg[2] == GameManager::GetInstance()->GetRoomNum())
				GameManager::GetInstance()->FirstRoomInit();
			GameManager::GetInstance()->FirstMainInit();
			break;
		case SET_PLAYER_ID:
			GameManager::GetInstance()->SetPlayerID((int)msg[1]);
			break;
		case SET_ROOM_LIST:
			if ((int)msg[1] == GameManager::GetInstance()->GetPlayerID())
				GameManager::GetInstance()->SetRoomList(msg);
			break;
		case ROOM_INFO_ACCEPT:
			if ((int)msg[1] == GameManager::GetInstance()->GetPlayerID()) {
				GameManager::GetInstance()->SetRoomInfo(msg);
			}
			break;
		case GAME_START_ACCEPT:
			if ((int)msg[2] == GameManager::GetInstance()->GetRoomNum()) {
				GameManager::GetInstance()->GameStart();
			}
			break;
		case DRAW_ACCEPT:
			if ((int)msg[2] == GameManager::GetInstance()->GetRoomNum()) {
				GameManager::GetInstance()->DrawPen((int)msg[3], (int)msg[4], (int)msg[5], (int)msg[6]);
			}
			break;
		case EXIT_ROOM_ACCEPT:
			if ((int)msg[1] == GameManager::GetInstance()->GetPlayerID())
				GameManager::GetInstance()->ExitRoom();
			if ((int)msg[2] == GameManager::GetInstance()->GetRoomNum())
				GameManager::GetInstance()->FirstRoomInit();
			GameManager::GetInstance()->FirstMainInit();
			break;
		}
	}

	return 0;
}

void ErrorHandling(const char* msg)
{
	cout << msg << "\n";
	exit(1);
}
