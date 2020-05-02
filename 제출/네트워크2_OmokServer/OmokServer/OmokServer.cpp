#include <windows.h>
#include <iostream>
#include <process.h> 
using namespace std;

#define BUF_SIZE 100
#define MAX_CLNT 2

unsigned WINAPI HandleClnt(void* arg);
void SendMsg(char* msg, int len, SOCKET socket);
void ErrorHandling(const char* msg);

struct packet {
	int inst;
	int data1;
	int data2;
	int null;
};

int clntCnt = 0;
SOCKET clntSocks[MAX_CLNT];
HANDLE hMutex;

int main()
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAdr, clntAdr;
	int clntAdrSz;
	HANDLE  hThread[2];

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error!");
	}

	hMutex = CreateMutex(NULL, FALSE, NULL);

	hServSock = socket(AF_INET, SOCK_STREAM, 0);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(9000);

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
	{
		ErrorHandling("bind() error");
	}

	if (listen(hServSock, 2) == SOCKET_ERROR)
	{
		ErrorHandling("listen() error");
	}

	while (1)
	{
		clntAdrSz = sizeof(clntAdr);
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSz);
		
		if (clntCnt == 2) {
			packet msg;
			msg.inst = -2;
			send(hClntSock, (char*)&msg ,sizeof(msg), 0);
			closesocket(hClntSock);
			continue;
		}
		WaitForSingleObject(hMutex, INFINITE);

		clntSocks[clntCnt++] = hClntSock;

		ReleaseMutex(hMutex);

		hThread[clntCnt-1] = (HANDLE)_beginthreadex(NULL, 0, HandleClnt, (void*)&hClntSock, 0, NULL);

		cout << "Connected client IP: " << inet_ntoa(clntAdr.sin_addr) << "\n";
	}

	closesocket(hServSock);
	WSACleanup();
	return 0;
}


unsigned WINAPI HandleClnt(void* arg)
{
	SOCKET hClntSock = *((SOCKET*)arg);
	int strLen = 0, i;

	packet rMsg;
	packet sMsg;

	while (recv(hClntSock, (char *)&rMsg, sizeof(packet), 0) != -1)
	{
		if (rMsg.inst == -1) {//번호 요청 받았을 때
			sMsg.inst = -1;
			for (i = 0; i < clntCnt; i++)
			{
				if (hClntSock == clntSocks[i])
				{
					sMsg.data1 = i;
					break;
				}
			}
			sMsg.data2 = 0;
			SendMsg((char*)&sMsg, sizeof(sMsg), hClntSock);
			if (clntCnt == 2) { //2명되면 게임시작
				sMsg.inst = 3;
				SendMsg((char*)&sMsg, 3, clntSocks[0]);
				SendMsg((char*)&sMsg, 3, clntSocks[1]);
			}
		}
		else if( rMsg.inst == 0 || rMsg.inst == 1 ){ //플레이어가 플레이 하고나서
			SendMsg((char*)&rMsg, 3, clntSocks[0]);
			SendMsg((char*)&rMsg, 3, clntSocks[1]);
		}
		else if (rMsg.inst == 3) {
			sMsg.inst = 3;
			SendMsg((char*)&sMsg, 3, clntSocks[0]);
			SendMsg((char*)&sMsg, 3, clntSocks[1]);
		}

	}
	///나갔을때 나머지 플레이어한테 알림
	if (clntCnt == 2) {
		int index = -1;
		for (i = 0; i < clntCnt; i++)
		{
			if (hClntSock == clntSocks[i])
			{
				index = i;
				break;
			}
		}
		sMsg.inst = 2;
		SendMsg((char*)&sMsg, 3, clntSocks[(index + 1) % 2]);
	}

	WaitForSingleObject(hMutex, INFINITE);

	for (i = 0; i < clntCnt; i++)   // remove disconnected client
	{
		if (hClntSock == clntSocks[i])
		{
			while (i < clntCnt - 1)
			{
				clntSocks[i] = clntSocks[i + 1];
				i++;
			}
			break;
		}
	}
	clntCnt--;
	ReleaseMutex(hMutex);
	closesocket(hClntSock);
	return 0;
}

void SendMsg(char* msg, int len, SOCKET clntSock)
{
	WaitForSingleObject(hMutex, INFINITE);
	send(clntSock, msg, sizeof(packet), 0);
	ReleaseMutex(hMutex);
}


void ErrorHandling(const char* msg)
{
	cout << msg << "\n";
	exit(1);
}