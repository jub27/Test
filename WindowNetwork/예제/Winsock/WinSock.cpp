#include <winsock2.h>

int main()
{
	//���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}

	MessageBox(NULL, L"���� �ʱ�ȭ ����", L"�˸�", MB_OK);

	//Socket ����
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		return 0;

	MessageBox(NULL, L"TCP������ ���ὺƮ�� ���ϻ���", L"�˸�", MB_OK);

	//�ּ�ü�� ���� ����
	SOCKADDR_IN sockaddr_in;
	sockaddr_in.sin_family = AF_INET;
	sockaddr_in.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockaddr_in.sin_addr.s_addr = htonl(INADDR_ANY);
	sockaddr_in.sin_port = htons(9000);

	//CloseSocket
	closesocket(sock);

	//���� ����
	WSACleanup();
	return 0;
}