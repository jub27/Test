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

	SOCKET hSocket;
	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	SOCKADDR_IN servAdr;
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAdr.sin_port = htons(9000);

	if (connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");
	else
		cout << "Connected..........." << "\n";

	char message[BUF_SIZE];
	int strLen;
	while (1)
	{
		cout << "Input message(Q to quit): ";
		cin >> message;

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		send(hSocket, message, strlen(message), 0);
		strLen = recv(hSocket, message, BUF_SIZE - 1, 0);
		message[strLen] = 0;
		cout << "Message from server: " << message << "\n";
	}

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(const char *message)
{
	cout << "Error" << message << "\n";
	exit(1);
}