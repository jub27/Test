#include <iostream>
#include <stdlib.h>
#include <process.h>
#include <winsock2.h>
#include <windows.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <Ws2tcpip.h> //inet_pton 

/*
edit에서 문자열 가져와서 엔터 누르면 보내기
턴 넘기기
게임 끝내기
클라이언트가 중간에 나갔을때 어떻게 처리할까..
*/

enum INST {
	MAKE_ROOM_REQUEST, MAKE_ROOM_ACCEPT, JOIN_ROOM_REQUEST, JOIN_ROOM_ACCEPT, PLAYER_ID_REQUEST, SET_PLAYER_ID
	, GET_ROOM_LIST, SET_ROOM_LIST, EXIT_ROOM_REQUEST, EXIT_ROOM_ACCEPT, ROOM_INFO_REQUEST, ROOM_INFO_ACCEPT
	, GAME_START_REQUEST, GAME_START_ACCEPT, PEN_DRAW_REQUEST, PEN_DRAW_ACCEPT, ANSWER_FROM_CLIENT, ANSWER_RESULT_FROM_SERVER
	, ERASER_DRAW_REQUEST, ERASER_DRAW_ACCEPT, ERASE_ALL_REQUEST, ERASE_ALL_ACCEPT
};

using namespace std;

#define DATA_SIZE 12
#define ANSWER_SIZE 10
#define READ 3
#define WRITE 5
#define MAX_ROOM_NUM 5
#define MAX_ROOM_PLAYER 4
#define DRAW_COUNT 1
#define PORT_NUM 3800

char answerList[ANSWER_SIZE][128] = { "연필", "핸드폰", "카드", "물티슈", "리모콘", "책", "선풍기", "창문", "베개", "달력" };

typedef struct {
	int inst;
	int playerID;
	int roomNum;
	int data[DATA_SIZE];
	char answer[128];
}Packet;

typedef struct {
	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;
}PER_HANDLE_DATA, * LPPER_HANDLE_DATA;

typedef struct {
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[sizeof(Packet)];
	int rwMode;
}PER_IO_DATA, * LPPER_IO_DATA;

typedef struct {
	bool started;
	char curAnswer[128];
	int curTurnPos;
	vector<int> drawCount;
	vector<int> userList;
}Room;

unsigned int WINAPI EchoThreadMain(LPVOID CompletionPortIO);
void ErrorHandling(const char* message);

vector<SOCKET> socketList;
vector<Room*> roomList;

int player_id = 1;

bool SetNextTurnUser(int roomNum) {
	int j = roomList[roomNum]->curTurnPos;
	for (int i = j + 1; i != j ; i++) {
		if (i >= MAX_ROOM_PLAYER)
			i = 0;
		if (roomList[roomNum]->userList[i] != 0 && roomList[roomNum]->drawCount[i] < DRAW_COUNT) {
			roomList[roomNum]->curTurnPos = i;
			roomList[roomNum]->drawCount[i]++;
			return true;
		}
	}
	return false;
}

void SetFirstTurnUser(int roomNum) {
	for (int i = 0; i < MAX_ROOM_PLAYER; i++) {
		if (roomList[roomNum]->userList[i] != 0) {
			roomList[roomNum]->curTurnPos = i;
			roomList[roomNum]->drawCount[i]++;
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

void InitRoom(int roomNum) {
	for (int i = 0; i < MAX_ROOM_PLAYER; i++) {
		roomList[roomNum]->drawCount[i] = 0;
		roomList[roomNum]->started = false;
	}
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
		ioInfo->wsaBuf.len = sizeof(Packet);
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
	Packet * packet;
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
			packet = new Packet();
			memcpy(packet, ioInfo->wsaBuf.buf, sizeof(Packet));
			cout << packet->inst <<"!!"<< endl;
			switch (packet->inst) {
			case MAKE_ROOM_REQUEST:
				if (roomList.size() < MAX_ROOM_NUM) {
					packet->inst = MAKE_ROOM_ACCEPT;
					roomList.push_back(new Room());
					roomList[roomList.size() - 1]->started = false;
					for (int i = 0; i < MAX_ROOM_PLAYER; i++) {
						roomList[roomList.size() - 1]->userList.push_back(0);
						roomList[roomList.size() - 1]->drawCount.push_back(0);
					}
					packet->data[0] = roomList.size()-1;
					allMessage = true;
				}
				else
					continue;
				break;
			case JOIN_ROOM_REQUEST:
				if (roomList[packet->data[0]]->started == false) {
					for (int i = 0; i < MAX_ROOM_PLAYER; i++) {
						if (roomList[packet->data[0]]->userList[i] == 0) {
							packet->inst = JOIN_ROOM_ACCEPT;
							roomList[packet->data[0]]->userList[i] = packet->playerID;
							packet->data[1] = i;
							break;
						}
					}
				}
				allMessage = true;
				break;
			case PLAYER_ID_REQUEST:
				packet->inst = SET_PLAYER_ID;
				packet->data[0] = player_id++;
				allMessage = false;
				break;
			case GET_ROOM_LIST:
				packet->inst = SET_ROOM_LIST;
				packet->data[0] = roomList.size();
				for (int i = 0; i < roomList.size(); i++) {
					packet->data[1 + i] = playerNumsOfRoom(i);
				}
				allMessage = false;
				break;
			case ROOM_INFO_REQUEST:
				packet->inst = ROOM_INFO_ACCEPT;
				for (int i = 0; i < MAX_ROOM_PLAYER; i++) {
					packet->data[i] = roomList[packet->roomNum]->userList[i];
				}
				allMessage = false;
				break;
			case GAME_START_REQUEST:
				packet->inst = GAME_START_ACCEPT;
				roomList[packet->roomNum]->started = true;
				SetFirstTurnUser(packet->roomNum);
				packet->data[0] = roomList[packet->roomNum]->userList[roomList[packet->roomNum]->curTurnPos];
				strcpy_s(packet->answer, answerList[rand() % ANSWER_SIZE]);
				strcpy_s(roomList[packet->roomNum]->curAnswer, packet->answer);
				allMessage = true;
				break;
			case PEN_DRAW_REQUEST:
				packet->inst = PEN_DRAW_ACCEPT;
				allMessage = true;
				break;
			case ERASER_DRAW_REQUEST:
				packet->inst = ERASER_DRAW_ACCEPT;
				allMessage = true;
				break;
			case ERASE_ALL_REQUEST:
				packet->inst = ERASE_ALL_ACCEPT;
				allMessage = true;
				break;
			case ANSWER_FROM_CLIENT:
				packet->inst = ANSWER_RESULT_FROM_SERVER;
				if (strcmp(roomList[packet->roomNum]->curAnswer, packet->answer) == 0) {
					packet->data[0] = 1;
					if (SetNextTurnUser(packet->roomNum)) {
						packet->data[1] = roomList[packet->roomNum]->userList[roomList[packet->roomNum]->curTurnPos];
						strcpy_s(packet->answer, answerList[rand() % ANSWER_SIZE]);
						strcpy_s(roomList[packet->roomNum]->curAnswer, packet->answer);
					}
					else {
						packet->data[1] = -1;
						InitRoom(packet->roomNum);
					}
				}
				else
					packet->data[0] = 0;
				allMessage = true;
				break;
			case EXIT_ROOM_REQUEST:
				packet->inst = EXIT_ROOM_ACCEPT;
				if (packet->roomNum != -1) {
					for (int i = 0; i < roomList[packet->roomNum]->userList.size(); i++) {
						if (roomList[packet->roomNum]->userList[i] == packet->playerID) {
							roomList[packet->roomNum]->userList[i] = 0;
							break;
						}
					}
					if (roomList[packet->roomNum]->started) {
						InitRoom(packet->roomNum);
						packet->data[0] = -1;
					}
					else
						packet->data[0] = 0;
				}
				allMessage = true;
				break;
			}
			cout << packet->inst << endl;
			if (allMessage) {
				for (int i = 0; i < socketList.size(); i++) {
					ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
					memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
					memcpy(ioInfo->buffer, packet, sizeof(Packet));
					ioInfo->wsaBuf.len = sizeof(Packet);
					ioInfo->wsaBuf.buf = ioInfo->buffer;
					ioInfo->rwMode = WRITE;
					WSASend(socketList[i], &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);
				}
			}
			else {
				ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
				memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
				memcpy(ioInfo->buffer, packet, sizeof(Packet));
				ioInfo->wsaBuf.len = sizeof(Packet);
				ioInfo->wsaBuf.buf = ioInfo->buffer;
				ioInfo->rwMode = WRITE;
				WSASend(sock, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);
			}
			free(packet);
			ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = sizeof(Packet);
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