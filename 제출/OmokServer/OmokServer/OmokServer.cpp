#include <windows.h>
#include <iostream>
#include <process.h> 
using namespace std;

#define BUF_SIZE 100
#define MAX_CLNT 2

unsigned WINAPI HandleClnt(void* arg);
void SendMsg(char* msg, int len, SOCKET socket);
void ErrorHandling(const char* msg);

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
		if (clntCnt == 2)
			continue;
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
	char rMsg[3];
	char sMsg[3];

	while (recv(hClntSock, rMsg, 3, 0) != -1)
	{
		if (rMsg[0] == -1) {//번호 요청 받았을 때
			sMsg[0] = -1;
			for (i = 0; i < clntCnt; i++)
			{
				if (hClntSock == clntSocks[i])
				{
					sMsg[1] = i;
					break;
				}
			}
			sMsg[2] = 0;
			SendMsg(sMsg, 3, hClntSock);
			if (clntCnt == 2) { //2명되면 게임시작
				sMsg[0] = 3;
				SendMsg(sMsg, 3, clntSocks[0]);
				SendMsg(sMsg, 3, clntSocks[1]);
			}
		}
		else if( rMsg[0] == 0 || rMsg[0] == 1 ){ //플레이어가 플레이 하고나서
			SendMsg(rMsg, 3, clntSocks[0]);
			SendMsg(rMsg, 3, clntSocks[1]);
		}
		else if (rMsg[0] == 3) {
			sMsg[0] = 3;
			SendMsg(sMsg, 3, clntSocks[0]);
			SendMsg(sMsg, 3, clntSocks[1]);
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
		sMsg[0] = 2;
		SendMsg(sMsg, 3, clntSocks[(index + 1) % 2]);
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
	send(clntSock, msg, len, 0);
	ReleaseMutex(hMutex);
}


void ErrorHandling(const char* msg)
{
	cout << msg << "\n";
	exit(1);
}