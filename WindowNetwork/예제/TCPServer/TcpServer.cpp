#include <iostream>
#include <winsock2.h>
using namespace std;

#define BUF_SIZE 1024
void ErrorHandling(const char *message);

int main()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	SOCKET hServSock;
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	int strLen;

	SOCKADDR_IN servAdr;
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(9000);
	strLen = bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr));

	if (strLen == SOCKET_ERROR)
		ErrorHandling("bind() error");

	strLen = listen(hServSock, 5);

	if (strLen == SOCKET_ERROR)
		ErrorHandling("listen() error");

	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;
	int clntAdrSize;
	char message[BUF_SIZE];

	while(true)
	{
		clntAdrSize = sizeof(clntAdr);
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSize);
		if (hClntSock == INVALID_SOCKET)
			ErrorHandling("accept() error");
		else
		{
			cout << "[TCPServer]Client Connected... ip = " << inet_ntoa(clntAdr.sin_addr) << ", PORT = " << ntohs(clntAdr.sin_port) << "\n";
		}

		while (true)
		{
			strLen = recv(hClntSock, message, BUF_SIZE, 0);

			if (strLen == 0)
			{
				cout << "Error on recv" << "\n";
				break;
			}

			strLen = send(hClntSock, message, strLen, 0);

			if (strLen == SOCKET_ERROR)
			{
				cout << "Error on send" << "\n";
				break;
			}
		}
		closesocket(hClntSock);
		cout << "[TCPServer]Client Disconnected... ip = " << inet_ntoa(clntAdr.sin_addr) << ", PORT = " << ntohs(clntAdr.sin_port) << "\n";
	}

	closesocket(hServSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(const char *message)
{
	cout << "Error" << message << "/n";
	exit(1);
}