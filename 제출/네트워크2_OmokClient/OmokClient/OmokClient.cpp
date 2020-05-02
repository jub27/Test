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

struct packet {
	int inst;
	int data1;
	int data2;
	int null;
};

#define BOARD_SIZE 15

enum GameState {
	WAITING, START, END
};

string board[BOARD_SIZE][BOARD_SIZE];

string blank = "□";
string black = "●";
string white = "○";

int cur_x = 0, cur_y = 0;
int playerNum = -1;
int turn_Player = 0;
int WinPlayer = -1;
int gameState = WAITING;

bool enterGame = true;

void InitMap() {
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
			board[i][j] = blank;
}

void PrintMap() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++)
			cout << board[i][j];
		cout << endl;
	}
} 

void PrintInfo() {
	gotoxy(0, 15);
	cout << "You : Player" << playerNum << endl;
	if (gameState == WAITING)
		cout << "Waiting...." << endl;
	else if (gameState == START)
		cout << "Player" << turn_Player << "'s Turn" << endl;
	else if (gameState == END) {
		cout << "Player" << WinPlayer << " Win!" << endl;
		cout << "Start : Enter" << endl;
	}
	cout << "Exit : q" << endl;
	gotoxy(cur_x * 2, cur_y);
}

void PrintGame() {
	system("cls");
	PrintMap();
	PrintInfo();
}

bool RangeCheck(int x, int y) {
	if (x < 0 || y < 0 || x >= BOARD_SIZE || y >= BOARD_SIZE)
		return false;
	return true;
}

bool WinPattern(int y, int x, int dy, int dx) {
	string pat = board[y][x];

	for (int i = 0; i < 5; i++, x += dx, y += dy) {
		if (RangeCheck(y, x) == false)
			return false;
		if (pat != board[y][x])
			return false;
	}
	return true;
}

bool WinCheck() {
	for (int y = 0; y < BOARD_SIZE; y++) {
		for (int x = 0; x < BOARD_SIZE; x++) {
			string pat = board[y][x];
			if (pat != blank) {
				if (WinPattern(y, x, 0, 1) || WinPattern(y, x, 0, -1) || WinPattern(y, x, 1, 0) ||
					WinPattern(y, x, 1, 1) || WinPattern(y, x, 1, -1) || WinPattern(y, x, -1, -1) ||
					WinPattern(y, x, -1, 0) || WinPattern(y, x, -1, 1)) {
					if (pat == black)
						WinPlayer = 0;
					else
						WinPlayer = 1;
					return true;
				}
			}
		}
	}
	return false;
}

int Play() {
	char key;
	while (1) {
		if (playerNum == -1)
			return 2;
		if (_kbhit()) {
			key = _getch();
			switch (key) {
			case 'q':
				return 0;
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
				if (gameState == END)
					return 3;
				if (board[cur_y][cur_x] == blank && turn_Player == playerNum && gameState == START) {
					return 1;
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
	packet msg;
	bool numCheck = false;
	while (1)
	{
		if (enterGame == false)
			return 0;
		if (playerNum == -1 && !numCheck) {
			msg.inst = -1;
			msg.data1 = 0;
			msg.data2 = 0;
			numCheck = true;
			send(hSock, (char*)&msg, sizeof(packet), 0);
		}
		else {
			int ret = Play();
			if(ret == 0){
				closesocket(hSock);
				exit(0);
			}
			else if (ret == 1) {
				msg.inst = playerNum;
				msg.data1 = cur_y;
				msg.data2 = cur_x;
				send(hSock, (char*)&msg, sizeof(packet), 0);
			}
			else if (ret == 3) {
				msg.inst = 3;
				send(hSock, (char*)&msg, sizeof(packet), 0);
			}
		}
	}

	return 0;
}

unsigned WINAPI RecvMsg(void* arg)   // read thread main
{
	int hSock = *((SOCKET*)arg);
	packet msg;
	int strLen;
	while (1)
	{
		strLen = recv(hSock, (char*)&msg, sizeof(packet), 0);

		if (strLen == -1)
			return -1;
		if (msg.inst == -2) {
			enterGame = false;
			cout << "다른 플레이어들이 게임중입니다." << endl;
			return 0;
		}
		if (msg.inst == -1) { //플레이어 번호 받기
			playerNum = msg.data1;
		}
		else if (msg.inst == 0) { //플레이어0이 하고나서
			board[msg.data1][msg.data2] = black;
			turn_Player = (turn_Player + 1) % 2;
			if (WinCheck())
				gameState = END;
		}
		else if (msg.inst == 1) { //플레이어1이 하고나서
			board[msg.data1][msg.data2] = white;
			turn_Player = (turn_Player + 1) % 2;
			if (WinCheck())
				gameState = END;
		}
		else if (msg.inst == 2){ //상대 나갔을때
			InitMap();
			gameState = WAITING;
			turn_Player = 0;
			playerNum = 0;
		}
		else if (msg.inst == 3) { //게임 시작
			InitMap();
			turn_Player = 0;
			gameState = START;
		}
		PrintGame();
	}

	return 0;
}

void ErrorHandling(const char* msg)
{
	cout << msg << "\n";
	exit(1);
}