#include <iostream>
#include <windows.h>
#include <process.h>
#include <conio.h>
#include "goto.h"
using namespace std;

#define BUF_SIZE 100
#define NAME_SIZE 20

unsigned WINAPI SendMsg(void* arg);
unsigned WINAPI RecvMsg(void* arg);
void ErrorHandling(const char* msg);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];


#define BOARD_SIZE 15

string board[BOARD_SIZE][BOARD_SIZE];

string blank = "¡à";
string black = "¡Ü";
string white = "¡Û";

int cur_x = 0, cur_y = 0;
int playerNum = -1;


void PrintMap() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++)
			cout << board[i][j];
		cout << endl;
	}
}

void Play() {
	char key;
	while (1) {
		if (_kbhit()) {
			key = _getch();
			switch (key) {
			case 72:
				if (cur_y > 0)
					cur_y--;
				break;
			case 75:
				if (cur_x > 0)
					cur_x--;
				break;
			case 77:
				if (cur_x < BOARD_SIZE - 1)
					cur_x++;
				break;
			case 80:
				if (cur_y < BOARD_SIZE - 1)
					cur_y++;
				break;
			case 13:
				if (board[cur_x][cur_y] == blank) {
					return;
				}
				break;
			}
			gotoxy(cur_x * 2, cur_y);
		}
	}
}

int main()
{
	WSADATA wsaData;
	SOCKET hSock;
	SOCKADDR_IN servAdr;
	HANDLE hSndThread, hRcvThread;

	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
			board[i][j] = blank;

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


	hSndThread = (HANDLE)_beginthreadex(NULL, 0, SendMsg, (void*)&hSock, 0, NULL);
	hRcvThread = (HANDLE)_beginthreadex(NULL, 0, RecvMsg, (void*)&hSock, 0, NULL);

	WaitForSingleObject(hSndThread, INFINITE);
	WaitForSingleObject(hRcvThread, INFINITE);
	closesocket(hSock);
	WSACleanup();
	return 0;
}

unsigned WINAPI SendMsg(void* arg)   // send thread main
{
	SOCKET hSock = *((SOCKET*)arg);
	char msg[3];

	while (1)
	{
		if (playerNum == -1) {
			msg[0] = -1;
			msg[1] = 0;
			msg[2] = 0;
		}
		else {
			PrintMap();
			Play();
			msg[0] = playerNum;
			msg[1] = cur_y;
			msg[2] = cur_x;
			cout << playerNum << endl;
		}
		send(hSock, msg, 3, 0);
	}

	return 0;
}

unsigned WINAPI RecvMsg(void* arg)   // read thread main
{
	int hSock = *((SOCKET*)arg);
	char msg[3];
	int strLen;

	while (1)
	{
		strLen = recv(hSock, msg, 3, 0);

		if (strLen == -1)
			return -1;
		if (msg[0] == -1) {
			playerNum = msg[1];
		}
		else {
			if (msg[0] == 0)
				board[msg[1]][msg[2]] = black;
			else
				board[msg[1]][msg[2]] = white;
		}
	}

	return 0;
}

void ErrorHandling(const char* msg)
{
	cout << msg << "\n";
	exit(1);
}
