#include <winsock2.h>

int main()
{
	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}

	MessageBox(NULL, L"윈속 초기화 성공", L"알림", MB_OK);

	//Socket 생성
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		return 0;

	MessageBox(NULL, L"TCP형식의 연결스트림 소켓생성", L"알림", MB_OK);

	//주소체계 정보 구성
	SOCKADDR_IN sockaddr_in;
	sockaddr_in.sin_family = AF_INET;
	sockaddr_in.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockaddr_in.sin_addr.s_addr = htonl(INADDR_ANY);
	sockaddr_in.sin_port = htons(9000);

	//CloseSocket
	closesocket(sock);

	//윈속 종료
	WSACleanup();
	return 0;
}