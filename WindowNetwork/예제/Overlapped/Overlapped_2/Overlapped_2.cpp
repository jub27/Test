#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE 512

//소켓 정보 저장을 위한 구조체의 변수
struct SOCKETINFO
{
	WSAOVERLAPPED overlapped;
	SOCKET sock;
	char buf[BUFSIZE + 1];
	int recvbytes;
	int sendbytes;
	WSABUF wsabuf;
};

SOCKET client_sock;
HANDLE hReadEvent, hWriteEvent; // client_sock 변수를 보호하기 위한 이벤트 객체 핸들

//비동기 입출력 시작과 처리함수
DWORD WINAPI WorkerThread(LPVOID arg);
void CALLBACK CompletionRoutine(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags);

//오류 출력 함수
void err_quit(const char* msg);
void err_display(const char* msg);
void err_display(int errcode);

int main()
{
	int retval;

	//윈속 초기화
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	//socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);

	if (listen_sock == INVALID_SOCKET)
		err_quit("socket()");

	//bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	if (retval == SOCKET_ERROR)
		err_quit("bind()");

	//listen()
	retval = listen(listen_sock, SOMAXCONN);

	if (retval == SOCKET_ERROR)
		err_quit("listen()");

	//이벤트 생성
	hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);

	if (hReadEvent == NULL)
		return 1;

	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	if (hWriteEvent == NULL)
		return 1;

	//Thread 생성
	//Thread는 alertable wait 상태가 됨으로써, 비동기 입출력이 완료하면 완료 루틴이 호출되게 한다.
	HANDLE hThread = CreateThread(NULL, 0, WorkerThread, NULL, 0, NULL);

	if (hThread == NULL)
		return 1;

	CloseHandle(hThread);

	while (1)
	{
		//hReadEvent 객체의 신호 상태를 기다린다.
		//초기 상태는 신호 상태이므로 WaitForSingleObject()함수는 곧바로 리턴한다.
		WaitForSingleObject(hReadEvent, INFINITE);

		//accept()
		client_sock = accept(listen_sock, NULL, NULL);

		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}

		//클라이언트가 접속을 할 때마다 hWriteEvent 객체를 신호 상태로 만든다.
		SetEvent(hWriteEvent);
	}

	//윈속 종료
	WSACleanup();
	return 0;
}

//비동기 입출력 처리 함수
DWORD WINAPI WorkerThread(LPVOID arg)
{
	int retval;
	while (1)
	{
		while (1)
		{
			//alertalbe wait 상태인 Thread를 깨운다.
			//alertalbe wait 상태에 진입.
			//함수가 리턴하여 alertable wait 상태에서 벗어나면 리턴 값을 확인.
			DWORD result = WaitForSingleObjectEx(hWriteEvent, INFINITE, TRUE);

			//리턴값이 WAIT_OBJECT_0이면 새로운 클라이언트가 접속한 경우이므로 루프를 벗어나게 된다.
			if (result == WAIT_OBJECT_0)
				break;

			//리턴값이 WAIT_IO_COMPLETION이면 비동기 입출력 작업과 이에 따른 완료 루틴 호출이 끝난 경우이므로
			//다시 alertable wait 상태에 진입한다.
			if (result != WAIT_IO_COMPLETION)
				return 1;
		}

		//접속한 클라이언트 정보 출력
		SOCKADDR_IN clientaddr;

		int addrlen = sizeof(clientaddr);
		getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		//소켓 정보 구조체 할당과 초기화
		SOCKETINFO *ptr = new SOCKETINFO;

		if (ptr == NULL)
		{
			printf("[오류] 메모리가 부족합니다!\n");
			return 1;
		}

		//client_sock 변수 값을 읽어가는 즉시 hReadEvent 이벤트 객체를 신호 상태로 만든다.
		ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
		ptr->sock = client_sock;
		SetEvent(hReadEvent);
		ptr->wsabuf.buf = ptr->buf;
		ptr->wsabuf.len = BUFSIZE;
		ptr->sendbytes = 0;
		ptr->recvbytes = 0;

		//비동기 입출력 시작
		DWORD recvbytes;
		DWORD flags = 0;

		retval = WSARecv(ptr->sock, &ptr->wsabuf, 1, &recvbytes, &flags, &ptr->overlapped, CompletionRoutine);

		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				err_display("WSARecv()");
				return 1;
			}
		}
	}

	return 0;
}

//비동기 입출력 처리 함수
//이전 Overlapped(1)의 입출력 처리 과정은 똑같다.
//Event 체크에서 CompletionRoutin 함수의 호출 여부가 한 루프의 회전을 의미한다.
void CALLBACK CompletionRoutine(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlag)
{
	int retval;

	//클라이언트 정보 얻기
	SOCKETINFO* ptr = (SOCKETINFO*)lpOverlapped;
	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	getpeername(ptr->sock, (SOCKADDR*)&clientaddr, &addrlen);

	//비동기 입출력 결과 확인
	if (dwError != 0 || cbTransferred == 0)
	{
		if (dwError != 0)
			err_display(dwError);

		closesocket(ptr->sock);

		printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		//소켓 정보 제거
		delete ptr;
		
		return;
	}

	//데이터 전송량 갱신
	if (ptr->recvbytes == 0)
	{
		ptr->recvbytes = cbTransferred;
		ptr->sendbytes = 0;
		
		//받은 데이터 출력
		ptr->buf[ptr->recvbytes] = '\0';

		printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), ptr->buf);
	}
	else
	{
		ptr->sendbytes += cbTransferred;
	}

	if (ptr->recvbytes > ptr->sendbytes)
	{
		//데이터 보내기
		ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
		ptr->wsabuf.buf = ptr->buf + ptr->sendbytes;
		ptr->wsabuf.len = ptr->recvbytes - ptr->sendbytes;

		DWORD sendbytes;

		//완료 루틴인 CompletionRoutine가 추가 되었다.
		retval = WSASend(ptr->sock, &ptr->wsabuf, 1, &sendbytes, 0, &ptr->overlapped, CompletionRoutine);

		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				err_display("WSASend()");
				return;
			}
		}
	}
	else
	{
		ptr->recvbytes = 0;

		//데이터 받기
		ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
		ptr->wsabuf.buf = ptr->buf;
		ptr->wsabuf.len = BUFSIZE;

		DWORD recvbytes;
		DWORD flags = 0;

		//완료 루틴인 CompletionRoutine가 추가 되었다.
		retval = WSARecv(ptr->sock, &ptr->wsabuf, 1, &recvbytes, &flags, &ptr->overlapped, CompletionRoutine);

		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				err_display("WSARecv()");
				return;
			}
		}
	}
}

//소켓 함수 오류 출력 후 종료
void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

//소켓 함수 오류 출력
void err_display(const char* msg)
{
	LPVOID lpMsgBuf;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

	//화면에 출력해야 하지만 현재 윈도우로 서버를 만들었기 때문에 Window에 표시되는 함수로 변경해보자!!
	printf("[%s] %s", msg, (const char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

//소켓 함수 오류 출력
void err_display(int errcode)
{
	LPVOID lpMsgBuf;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		errcode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

	//화면에 출력해야 하지만 현재 윈도우로 서버를 만들었기 때문에 Window에 표시되는 함수로 변경해보자!!
	printf("[오류] %s", (const char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}