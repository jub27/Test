#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE 512

//소켓 정보 저장을 위한 구조체와 변수
struct SOCKETINFO
{
	WSAOVERLAPPED overlapped; // Overlapped 구조체
	SOCKET sock;
	char buf[BUFSIZE + 1];
	int recvbytes;
	int sendbytes;
	WSABUF wsabuf; // WSABUF 구조체
};

//소켓 정보 관리를 위한 변수
int nTotalSockets = 0; // 관리하고 있는 소켓의 개수
SOCKETINFO* SocketInfoArray[WSA_MAXIMUM_WAIT_EVENTS]; // 소캣 정보 배열
WSAEVENT EventArray[WSA_MAXIMUM_WAIT_EVENTS]; // 이벤트 객체 핸들 배열
CRITICAL_SECTION cs; //임계영역 Thread 동기화를 하면서 소캣 정보와 이벤트 객체 핸들을 보호하기 위한 조치

//비동기 입출력 처리 함수
DWORD WINAPI WorkerThread(LPVOID arg);

//소켓 관리 함수
BOOL AddSocketInfo(SOCKET sock);
void RemoveSocketInfo(int nIndex);

//오류 출력 함수
void err_quit(const char* msg);
void err_display(const char* msg);

int main()
{
	int retval;

	//임계영역 초기화
	InitializeCriticalSection(&cs);

	//윈속 초기화
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	//sock()
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

	//listen
	retval = listen(listen_sock, SOMAXCONN);

	if (retval == SOCKET_ERROR)
		err_quit("listen()");

	//더미(dummy) 이벤트 객체 생성
	WSAEVENT hEvent = WSACreateEvent();

	if (hEvent == WSA_INVALID_EVENT)
		err_quit("WSACreateEvent()");

	//핸들을 저장
	//이벤트 객테는 특정 소켓과 짝짓지 않고 특별 용도로 사용하기 때문이다.
	EventArray[nTotalSockets++] = hEvent;

	//Thread 생성
	HANDLE hThread = CreateThread(NULL, 0, WorkerThread, NULL, 0, NULL);

	if (hThread == NULL)
		return 1;

	CloseHandle(hThread);

	//데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	DWORD recvbytes, flags;

	while (1)
	{
		//accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);

		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}

		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		//소켓 정보 추가
		//소켓 정보를 SocketInfoArray[]에 저장
		//대응 하는 이벤트 객체를 생성하여 EventArray[]배열에 저장.
		if (AddSocketInfo(client_sock) == FALSE)
		{
			closesocket(client_sock);

			printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
				inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

			continue;
		}

		//비동기 입출력 시작
		//새로 생성된 소켓에 대해 WSARecv()함수를 호출하여 비동기 입출력을 시작한다.
		SOCKETINFO *ptr = SocketInfoArray[nTotalSockets - 1];
		flags = 0;
		retval = WSARecv(ptr->sock, &ptr->wsabuf, 1, &recvbytes, &flags, &ptr->overlapped, NULL);

		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				err_display("WSARecv()");
				RemoveSocketInfo(nTotalSockets - 1);
				continue;
			}
		}

		//소켓의 개수(nTotalSockets)변화를 알림.
		//더미 이벤트 객체를 신호상태로 만든다.
		//WSAWaiteForMulitipleEvents()함수가 대기 상태에서 리턴되게 만든다.
		//이벤트가 들어올때까지 대기
		WSASetEvent(EventArray[0]);
	}

	//윈속 종료
	WSACleanup();

	//임계 영역 삭제
	DeleteCriticalSection(&cs);

	return 0;
}

//비동기 입출력 처리 함수
DWORD WINAPI WorkerThread(LPVOID arg)
{
	int retval;

	while (1)
	{
		//이벤트 객체 관찰
		//이벤트 객체가 신호상태가 되기를 기다린다.
		DWORD index = WSAWaitForMultipleEvents(nTotalSockets, EventArray, FALSE, WSA_INFINITE, FALSE);

		if (index == WSA_WAIT_FAILED) 
			continue;

		//함수가 리턴하면 이벤트 객체를 비신호 상태로 만든다 
		index -= WSA_WAIT_EVENT_0;

		//배열 인덱스를 체크한다.
		WSAResetEvent(EventArray[index]);

		//배열 인덱스가 0이면 EventArray[0]가 신호 상태가 된것이다.
		//클라이언트가 새로 접속하여 새로운 소켓 정보가 추가되었다는 뜻.
		//소켓의 총개수가 변하면서 다기 이벤트 대기상태로 돌아간다.
		if (index == 0)
			continue;

		//클라이언트 정보 얻기
		SOCKETINFO *ptr = SocketInfoArray[index];
		SOCKADDR_IN clientaddr;

		int addrlen = sizeof(clientaddr);
		getpeername(ptr->sock, (SOCKADDR*)&clientaddr, &addrlen);

		//비동기 입출력 결과 확인
		DWORD cbTransferred;
		DWORD flags;

		//함수를 호출하여 비동기 입출력 결과를 확인한다.
		retval = WSAGetOverlappedResult(ptr->sock, &ptr->overlapped, &cbTransferred, FALSE, &flags);

		if (retval == FALSE || cbTransferred == 0)
		{
			//오류가 발생했거나 클라이언트가 정상 종료한 경우면 소켓 정보를 제거한다.
			RemoveSocketInfo(index);

			printf("[TCP 서버] 클라이언트 종료 : IP 주소 = %s, 포트 번호 = %d\n",
				inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

			continue;
		}

		//데이터 전송량 갱신
		if (ptr->recvbytes == 0)
		{
			//받은 데이터 수 확인
			ptr->recvbytes = cbTransferred;
			ptr->sendbytes = 0;

			//받은 데이터 출력
			ptr->buf[ptr->recvbytes] = '\0';

			printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), ptr->buf);
		}
		else
		{
			//보내지 못한 데이터를 보냈기때문에 보낸 데이터량을 갱신한다.
			ptr->sendbytes += cbTransferred;
		}

		//보낸 데이터가 받은 데이터보다 적으면, 아직 보내지 못한 데이터를 마저 보낸다.
		if (ptr->recvbytes > ptr->sendbytes)
		{
			//데이터 보내기
			ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
			ptr->overlapped.hEvent = EventArray[index];
			ptr->wsabuf.buf = ptr->buf + ptr->sendbytes;
			ptr->wsabuf.len = ptr->recvbytes - ptr->sendbytes;

			DWORD sendbytes;
			//WSASend()는 비동기적으로 동작하기때문에, 다음 루프를 돌때 확인할수 있다.
			retval = WSASend(ptr->sock, &ptr->wsabuf, 1, &sendbytes, 0, &ptr->overlapped, NULL);

			if (retval == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
					err_display("WSASend()");
				}

				continue;
			}
		}
		else
		{
			//받은 데이터 수 초기화
			ptr->recvbytes = 0;

			//데이터 받기
			ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
			ptr->overlapped.hEvent = EventArray[index];
			ptr->wsabuf.buf = ptr->buf;
			ptr->wsabuf.len = BUFSIZE;

			DWORD recvbytes;
			flags = 0;
			//WSARecv()는 비동기적으로 동작하기때문에, 다음 루프를 돌때 확인할수 있다.
			retval = WSARecv(ptr->sock, &ptr->wsabuf, 1, &recvbytes, &flags, &ptr->overlapped, NULL);

			if (retval == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
					err_display("WSARecv()");
				}

				continue;
			}
		}
	}
}

//소켓 정보 추가
BOOL AddSocketInfo(SOCKET sock)
{
	EnterCriticalSection(&cs);

	if (nTotalSockets >= WSA_MAXIMUM_WAIT_EVENTS)
		return FALSE;

	SOCKETINFO* ptr = new SOCKETINFO;

	if (ptr == NULL)
		return FALSE;

	//이벤트 생성
	WSAEVENT hEvent = WSACreateEvent();

	if (hEvent == WSA_INVALID_EVENT)
		return FALSE;

	ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
	ptr->overlapped.hEvent = hEvent;
	ptr->sock = sock;
	ptr->recvbytes = ptr->sendbytes = 0;
	ptr->wsabuf.buf = ptr->buf;
	ptr->wsabuf.len = BUFSIZE;
	SocketInfoArray[nTotalSockets] = ptr;
	EventArray[nTotalSockets] = hEvent;
	++nTotalSockets;

	LeaveCriticalSection(&cs);

	return TRUE;
}

//소켓 정보 삭제
void RemoveSocketInfo(int nIndex)
{
	EnterCriticalSection(&cs);

	SOCKETINFO* ptr = SocketInfoArray[nIndex];
	closesocket(ptr->sock);
	delete ptr;
	WSACloseEvent(EventArray[nIndex]);

	if (nIndex != (nTotalSockets - 1))
	{
		SocketInfoArray[nIndex] = SocketInfoArray[nTotalSockets - 1];
		EventArray[nIndex] = EventArray[nTotalSockets - 1];
	}

	--nTotalSockets;

	LeaveCriticalSection(&cs);
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