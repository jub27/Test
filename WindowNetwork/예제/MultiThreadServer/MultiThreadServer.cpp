#include <winsock2.h>
#include <stdlib.h>
#include <process.h>
#include <iostream>
using namespace std;

#define SERVERPORT 9000
#define BUFSIZE    512
void ErrorHandling(const char *message);

// Ŭ���̾�Ʈ�� ������ ���(unsigned int = DWORD)
unsigned int WINAPI ProcessClient(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);

	while (1) 
	{
		// ������ �ޱ�
		retval = recv(client_sock, buf, BUFSIZE, 0);
		
		if (retval == SOCKET_ERROR) 
		{
			cout << "recv() error" << "\n";
			break;
		}
		else if (retval == 0)
		{
			cout << "recv() error" << "\n";
			break;
		}

		// ���� ������ ���
		buf[retval] = '\0';
		cout << "[TCP " << inet_ntoa(clientaddr.sin_addr) << ":" << ntohs(clientaddr.sin_port) << "] " << buf << "\n";

		// ������ ������
		retval = send(client_sock, buf, retval, 0);
		if (retval == SOCKET_ERROR) 
		{
			cout << "send() error" << "\n";
			break;
		}
	}

	// closesocket()
	closesocket(client_sock);
	cout << "[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ� : " << inet_ntoa(clientaddr.sin_addr) << ", PORT : " << ntohs(clientaddr.sin_port) << "\n";

	return 0;
}

int main(int argc, char *argv[])
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);

	if (listen_sock == INVALID_SOCKET)
	{
		ErrorHandling("send() error");
	}

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		ErrorHandling("send() error");
	}

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		ErrorHandling("send() error");
	}

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread;

	while (1) 
	{
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) 
		{
			ErrorHandling("send() error");
			break;
		}

		// ������ Ŭ���̾�Ʈ ���� ���
		cout << "[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ� : " << inet_ntoa(clientaddr.sin_addr) << "PORT : " << ntohs(clientaddr.sin_port) << "\n";

		// ������ ����
		hThread = (HANDLE)_beginthreadex(NULL, 0, ProcessClient,(LPVOID)client_sock, 0, NULL);

		if (hThread == NULL) 
		{ 
			closesocket(client_sock); 
		}
		else 
		{ 
			CloseHandle(hThread); 
		}
	}

	// closesocket()
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}

void ErrorHandling(const char *message)
{
	cout << "Error" << message << "\n";
	exit(1);
}
