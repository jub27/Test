#include <iostream>
#include <stdlib.h>
#include <process.h>
#include <winsock2.h>
#include <windows.h>
#include <vector>
#include <Ws2tcpip.h> //inet_pton 

/*
edit에서 문자열 가져와서 엔터 누르면 보내기
정답 데이터 저장해놓기
턴 넘기기
게임 끝내기
클라이언트가 중간에 나갔을때 어떻게 처리할까..
*/

// vs warning and winsock error 
enum INST {
	MAKE_ROOM_REQUEST, MAKE_ROOM_ACCEPT, JOIN_ROOM_REQUEST, JOIN_ROOM_ACCEPT, PLAYER_ID_REQUEST, SET_PLAYER_ID
	, GET_ROOM_LIST, SET_ROOM_LIST, EXIT_ROOM_REQUEST, EXIT_ROOM_ACCEPT, ROOM_INFO_REQUEST, ROOM_INFO_ACCEPT
	, GAME_START_REQUEST, GAME_START_ACCEPT, DRAW_REQUEST, DRAW_ACCEPT
};

using namespace std;

#define BUF_SIZE 12
#define READ 3
#define WRITE 5
#define MAX_ROOM_NUM 5
#define MAX_ROOM_PLAYER 4
#define DRAW_COUNT 1

#define PORT_NUM 3800

typedef struct {
	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;
}PER_HANDLE_DATA, * LPPER_HANDLE_DATA;

typedef struct {
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[BUF_SIZE * sizeof(int)];
	int rwMode;
}PER_IO_DATA, * LPPER_IO_DATA;

typedef struct {
	bool started;
	int curTurnPos;
	vector<int> drawCount;
	vector<int> userList;
}Room;

unsigned int WINAPI EchoThreadMain(LPVOID CompletionPortIO);
void ErrorHandling(const char* message);

vector<SOCKET> socketList;
vector<Room*> roomList;

int player_id = 1;

void SetNextTurnUser(int roomNum) {
	for (int i = roomList[roomNum]->curTurnPos + 1; ; i++) {
		if (i >= MAX_ROOM_PLAYER)
			i = 0;
		if (roomList[roomNum]->userList[i] != 0) {
			roomList[roomNum]->curTurnPos = i;
			return;
		}
	}
	return;
}

void SetFirstTurnUser(int roomNum) {
	for (int i = 0; i < MAX_ROOM_PLAYER; i++) {
		if (roomList[roomNum]->userList[i] != 0) {
			roomList[roomNum]->curTurnPos = i;
			break;
		}
	}
}

int playerNumsOfRoom(int roomNum) {
	int ret = 0;
	for (int i = 0; i < MAX_ROOM_PLAYER; i++) {
		if (roomList[roomNum]->userList[i] != 0)
			ret++;
	}
	return ret;
}

int main() {
	WSADATA wsaData;
	HANDLE hComPort;
	SYSTEM_INFO sysInfo;
	LPPER_IO_DATA ioInfo;
	LPPER_HANDLE_DATA handleInfo;

	SOCKET hServSock;
	SOCKADDR_IN servAdr;
	DWORD recvBytes, i, flags = 0;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	hComPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	GetSystemInfo(&sysInfo);
	for (i = 0; i < sysInfo.dwNumberOfProcessors; i++)
		_beginthreadex(NULL, 0, EchoThreadMain, (LPVOID)hComPort, 0, NULL);

	hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(PORT_NUM);

	bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr));
	listen(hServSock, 5);

	while (1) {
		SOCKET hClntSock;
		SOCKADDR_IN clntAdr;
		int addrLen = sizeof(clntAdr);

		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &addrLen);
		handleInfo = (LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));
		handleInfo->hClntSock = hClntSock;
		memcpy(&(handleInfo->clntAdr), &clntAdr, addrLen);
		socketList.push_back(hClntSock);
		CreateIoCompletionPort((HANDLE)hClntSock, hComPort, (DWORD)handleInfo, 0);

		ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
		ioInfo->wsaBuf.len = BUF_SIZE * sizeof(int);
		ioInfo->wsaBuf.buf = ioInfo->buffer;
		ioInfo->rwMode = READ;
		WSARecv(handleInfo->hClntSock, &(ioInfo->wsaBuf), 1, &recvBytes, &flags, &(ioInfo->overlapped), NULL);
	}
	return 0;
}

unsigned int WINAPI EchoThreadMain(LPVOID pComPort) {
	HANDLE hComPort = (HANDLE)pComPort;
	SOCKET sock;
	DWORD bytesTrans;
	LPPER_HANDLE_DATA handleInfo;
	LPPER_IO_DATA ioInfo;
	DWORD flags = 0;
	bool allMessage = false;
	int msg[BUF_SIZE];
	int count = 0;
	bool roomMaster = true;
	while (1) {
		GetQueuedCompletionStatus(hComPort, &bytesTrans, (LPDWORD)&handleInfo, (LPOVERLAPPED*)&ioInfo, INFINITE);
		sock = handleInfo->hClntSock;

		if (ioInfo->rwMode == READ) {
			if (bytesTrans == 0) {
				closesocket(sock);
				free(handleInfo);
				free(ioInfo);
				continue;
			}
			memcpy(msg, ioInfo->wsaBuf.buf, BUF_SIZE*(sizeof(int)));
			cout << msg[0] <<"!!"<< endl;
			switch (msg[0]) {
			case MAKE_ROOM_REQUEST:
				if (roomList.size() < MAX_ROOM_NUM) {
					msg[0] = MAKE_ROOM_ACCEPT;
					roomList.push_back(new Room());
					roomList[roomList.size() - 1]->started = false;
					for (int i = 0; i < MAX_ROOM_PLAYER; i++) {
						roomList[roomList.size() - 1]->userList.push_back(0);
						roomList[roomList.size() - 1]->drawCount.push_back(0);
					}
					msg[2] = roomList.size()-1;
					allMessage = true;
				}
				else
					continue;
				break;
			case JOIN_ROOM_REQUEST:
				if (roomList[(int)msg[2]]->started == false) {
					for (int i = 0; i < MAX_ROOM_PLAYER; i++) {
						if (roomList[(int)msg[2]]->userList[i] == 0) {
							msg[0] = JOIN_ROOM_ACCEPT;
							roomList[(int)msg[2]]->userList[i] = (int)msg[1];
							msg[3] = i;
							break;
						}
					}
				}
				allMessage = true;
				break;
			case PLAYER_ID_REQUEST:
				msg[0] = SET_PLAYER_ID;
				msg[1] = player_id++;
				allMessage = false;
				break;
			case GET_ROOM_LIST:
				msg[0] = SET_ROOM_LIST;
				msg[2] = roomList.size();
				for (int i = 0; i < roomList.size(); i++) {
					msg[3 + i] = playerNumsOfRoom(i);
				}
				allMessage = false;
				break;
			case ROOM_INFO_REQUEST:
				msg[0] = ROOM_INFO_ACCEPT;
				for (int i = 0; i < MAX_ROOM_PLAYER; i++) {
					msg[3 + i] = roomList[msg[2]]->userList[i];
				}
				allMessage = false;
				break;
			case GAME_START_REQUEST:
				msg[0] = GAME_START_ACCEPT;
				roomList[msg[2]]->started = true;
				SetFirstTurnUser(msg[2]);
				msg[3] = roomList[msg[2]]->userList[roomList[msg[2]]->curTurnPos];
				allMessage = true;
				break;
			case DRAW_REQUEST:
				msg[0] = DRAW_ACCEPT;
				allMessage = true;
				break;
			case EXIT_ROOM_REQUEST:
				msg[0] = EXIT_ROOM_ACCEPT;
				for (vector<int>::iterator iter = roomList[(int)msg[2]]->userList.begin(); iter != roomList[(int)msg[2]]->userList.end(); iter++) {
					if ((*iter) == (int)msg[1]) {
						(*iter) = 0;
						break;
					}
				}
				allMessage = true;
				break;
			}
			cout << (int)msg[0] << endl;
			if (allMessage) {
				for (int i = 0; i < socketList.size(); i++) {
					ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
					memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
					memcpy(ioInfo->buffer, msg, BUF_SIZE * sizeof(int));
					ioInfo->wsaBuf.len = BUF_SIZE * sizeof(int);
					ioInfo->wsaBuf.buf = ioInfo->buffer;
					ioInfo->rwMode = WRITE;
					WSASend(socketList[i], &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);
				}
			}
			else {
				ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
				memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
				memcpy(ioInfo->buffer, msg, BUF_SIZE * sizeof(int));
				ioInfo->wsaBuf.len = BUF_SIZE * sizeof(int);
				ioInfo->wsaBuf.buf = ioInfo->buffer;
				ioInfo->rwMode = WRITE;
				WSASend(sock, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);
			}

			ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = BUF_SIZE * sizeof(int);
			ioInfo->wsaBuf.buf = ioInfo->buffer;
			ioInfo->rwMode = READ;
			WSARecv(sock, &(ioInfo->wsaBuf), 1, NULL, &flags, &(ioInfo->overlapped), NULL);
		}
		else {
			free(ioInfo);
		}
	}
	return 0;
}

void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}