#include <winsock2.h>
#include <stdlib.h>
#include <process.h>
#include <iostream>
using namespace std;

#define SERVERPORT 9000
#define BUFSIZE    512

enum PACKET_INDEX
{
	PACKET_INDEX_CHAT,
};

//가장작은 바이트 단위로 차례로 정렬을 해주겠다는 의미
#pragma pack(push, 1)
struct PACKET_HEADER
{
	WORD index;
	WORD size;
};
#pragma pack(pop)

void Recv(SOCKET sock)
{
	char buf[BUFSIZE + 1];
	SOCKADDR_IN clientaddr;
	int addrlen;
	int retval;

	addrlen = sizeof(clientaddr);

	//클라이언트 정보를 알고 싶다면.
	getpeername(sock, (SOCKADDR*)&clientaddr, &addrlen);

	while (true)
	{
		retval = recv(sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR)
		{
			cout << "err on recv" << endl;
			break;
		}
		else if (retval == 0)
			break;

		buf[retval] = '\0';
		cout << "[TCP " << inet_ntoa(clientaddr.sin_addr) << ":" << ntohs(clientaddr.sin_port) << "]" << &buf[sizeof(PACKET_HEADER)];

		retval = send(sock, buf, retval, 0);
		if (retval == SOCKET_ERROR)
		{
			cout << "err on send" << endl;
			break;
		}
	}

	closesocket(sock);
	cout << "[TCP 서버] 클라이언트 종료 ip = " << inet_ntoa(clientaddr.sin_addr) << ",포트 = " << ntohs(clientaddr.sin_port) << endl;
}

unsigned int WINAPI ThreadRecv(void* arg)
{
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
	{
		cout << "error on listen_sock!!" << endl;
		return -1;
	}

	//bind()
	SOCKADDR_IN serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9001);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	int retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		cout << "error on bind!!" << endl;
		return -1;
	}

	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		cout << "error on listen!!" << endl;
		return -1;
	}


	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread;
	DWORD ThreadId;

	while (true)
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			cout << "error on accept!!";
			continue;
		}

		cout << "[TCP 서버] 클라이언트 접속 ip = " << inet_ntoa(clientaddr.sin_addr) << ",포트 = " << ntohs(clientaddr.sin_port) << endl;

		Recv(client_sock);
	}

	closesocket(listen_sock);
}

int main()
{
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	HANDLE hThread;
	DWORD dwThreadID;
	hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadRecv, NULL, 0, (unsigned*)&dwThreadID);

	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);

	WSACleanup();
	return 0;
}