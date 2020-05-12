#include "GameManager.h"
#include <iostream>
#include <windows.h>
#include <process.h> 

using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Chess");
HWND hWnd;
#define BUF_SIZE 5

unsigned WINAPI SendMsg(void* arg);
unsigned WINAPI RecvMsg(void* arg);
void ErrorHandling(const char* msg);

SOCKET hSock;
bool turn = false;
bool numCheck = false;
int playerNumber = -1;
bool saero = false;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPervlnstance, LPSTR lpszCmdParam, int nCmdShow)
{
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
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1020, 1040, NULL,
		(HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	WSADATA wsaData;
	SOCKADDR_IN servAdr;
	HANDLE hRcvThread;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error!");
	}

	hSock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAdr.sin_port = htons(9000);

	if (connect(hSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
	{
		ErrorHandling("connect() error");
	}

	hRcvThread = (HANDLE)_beginthreadex(NULL, 0, RecvMsg, (void*)&hSock, 0, NULL);

	char msg[BUF_SIZE];
	msg[0] = REQUEST_PLAYER_NUMBER;
	send(hSock, msg, BUF_SIZE, 0);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	delete GameManager::GetInstance();
	
	closesocket(hSock);
	WSACleanup();

	return (int)Message.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	POINT in_Pt;
	string ClickResult;
	char msg[BUF_SIZE];
	if (turn && !saero) {
		InvalidateRect(hWnd, NULL, TRUE);
		saero = true;
	}
	switch (iMessage)
	{
	case WM_CREATE:
		GameManager::GetInstance()->Init(hWnd);
		GameManager::GetInstance()->ResetBoard();
		return 0;
	case WM_LBUTTONDOWN:
		if (!turn)
			return 0;
		in_Pt.x = LOWORD(lParam);
		in_Pt.y = HIWORD(lParam);
		ClickResult = GameManager::GetInstance()->Click(in_Pt);
		InvalidateRect(hWnd, NULL, TRUE);
		if (ClickResult[0] == WHITE_WIN) {
			msg[0] = ClickResult[0];
			msg[1] = ClickResult[1];
			msg[2] = ClickResult[2];
			msg[3] = ClickResult[3];
			msg[4] = ClickResult[4];
			send(hSock, msg, BUF_SIZE, 0);
			turn = false;
		}
		else if (ClickResult[0] == BLACK_WIN) {
			msg[0] = ClickResult[0];
			msg[1] = ClickResult[1];
			msg[2] = ClickResult[2];
			msg[3] = ClickResult[3];
			msg[4] = ClickResult[4];
			send(hSock, msg, BUF_SIZE, 0);
			turn = false;
		}
		else if (ClickResult[0] == PIECE_MOVE) {
			msg[0] = ClickResult[0];
			msg[1] = ClickResult[1];
			msg[2] = ClickResult[2];
			msg[3] = ClickResult[3];
			msg[4] = ClickResult[4];
			send(hSock, msg, BUF_SIZE, 0);
			turn = false;
		}
		return 0;
	case WM_RBUTTONDOWN:
		if (!turn)
			return 0;
		GameManager::GetInstance()->UnClick();
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GameManager::GetInstance()->CheckSelectable();
		GameManager::GetInstance()->DrawBoard(hdc, turn);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

unsigned WINAPI RecvMsg(void* arg)   // read thread main
{
	int hSock = *((SOCKET*)arg);
	char msg[BUF_SIZE];
	int strLen;
	string moveInst = "    ";
	while (1)
	{
		strLen = recv(hSock, msg, BUF_SIZE, 0);

		if (strLen == -1)
			return -1;

		switch (msg[0]) {
		case ASSIGN_PLAYER_NUMBER:
			playerNumber = msg[1];
			if (playerNumber == 2) {
				msg[0] = REQUEST_GAME_START;
				send(hSock, msg, BUF_SIZE, 0);
			}
			if (msg[1] == 1)
				GameManager::GetInstance()->SetTeam(BLACK);
			else
				GameManager::GetInstance()->SetTeam(WHITE);
			break;
		case GAME_START:
			if (playerNumber == 1)
				turn = true;
			GameManager::GetInstance()->Init(hWnd);
			GameManager::GetInstance()->ResetBoard();
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case PIECE_MOVE:
			moveInst[0] = msg[1];
			moveInst[1] = msg[2];
			moveInst[2] = msg[3];
			moveInst[3] = msg[4];
			GameManager::GetInstance()->MovePiece(moveInst);
			InvalidateRect(hWnd, NULL, TRUE);
			turn = true;
			break;
		case BLACK_WIN:
			if (GameManager::GetInstance()->GetTeam() == WHITE) {
				moveInst[0] = msg[1];
				moveInst[1] = msg[2];
				moveInst[2] = msg[3];
				moveInst[3] = msg[4];
				GameManager::GetInstance()->MovePiece(moveInst);
			}
			InvalidateRect(hWnd, NULL, TRUE);
			if (MessageBox(hWnd, L"다시 시작", L"BLACK WIN", MB_YESNO) == IDYES) {
				msg[0] = REQUEST_GAME_START;
				GameManager::GetInstance()->Init(hWnd);
				GameManager::GetInstance()->ResetBoard();
				InvalidateRect(hWnd, NULL, TRUE);
				send(hSock, msg, BUF_SIZE, 0);
			}
			else {
				exit(1);
			}
			break;
		case WHITE_WIN:
			if (GameManager::GetInstance()->GetTeam() == BLACK) {
				moveInst[0] = msg[1];
				moveInst[1] = msg[2];
				moveInst[2] = msg[3];
				moveInst[3] = msg[4];
				GameManager::GetInstance()->MovePiece(moveInst);
			}
			InvalidateRect(hWnd, NULL, TRUE);
			if (MessageBox(hWnd, L"다시 시작", L"WHITE WIN", MB_YESNO) == IDYES) {
				msg[0] = REQUEST_GAME_START;
				GameManager::GetInstance()->Init(hWnd);
				GameManager::GetInstance()->ResetBoard();
				InvalidateRect(hWnd, NULL, TRUE);
				send(hSock, msg, BUF_SIZE, 0);
			}
			else {
				exit(1);
			}
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
