#include <iostream>
#include <stdlib.h>
#include <process.h>
#include <winsock2.h>
#include <windows.h>
#include <vector>
#include <Ws2tcpip.h> //inet_pton 

// vs warning and winsock error 


using namespace std;


#define BUF_SIZE 12
#define MAX_ROOM_NUM 5
#define MAX_ROOM_PLAYER 4
#define PORT_NUM 3800
#define READ 3
#define WRITE 5

int player_id = 1;

enum INST {
    N, MAKE_ROOM_REQUEST, MAKE_ROOM_ACCEPT, JOIN_ROOM_REQUEST, JOIN_ROOM_ACCEPT, PLAYER_ID_REQUEST, SET_PLAYER_ID
    , GET_ROOM_LIST, SET_ROOM_LIST, EXIT_ROOM_REQUEST, EXIT_ROOM_ACCEPT
};

typedef struct    // socket info
{
    SOCKET hClntSock;
    SOCKADDR_IN clntAdr;
    CHAR ip[22];
} PER_HANDLE_DATA, * LPPER_HANDLE_DATA;

typedef struct    // buffer info
{
    OVERLAPPED overlapped;
    WSABUF wsaBuf;
    char buffer[BUF_SIZE];
    INT rwMode;    // READ or WRITE
} PER_IO_DATA, * LPPER_IO_DATA;

typedef struct {
    int roomNumber;
    vector<int> userList;
}Room;

// Ŭ���̾�Ʈ ����� ���� vector container
std::vector<LPPER_HANDLE_DATA> UserList;
vector<Room*> roomList;

void ErrorHandling(const char* message);
unsigned __stdcall ThreadMain(void* CompletionPortIO);

int main(int argc, char* argv[])
{
    WSADATA    wsaData;
    HANDLE hComPort;
    SYSTEM_INFO sysInfo;
    LPPER_IO_DATA ioInfo;
    LPPER_HANDLE_DATA handleInfo;

    SOCKET hServSock;
    SOCKADDR_IN servAdr;
    DWORD recvBytes, flags = 0;
    INT i;

    // winsock start
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup Error");

    hComPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    GetSystemInfo(&sysInfo);

    // main thread�� ����� thread ����
    for (i = 0; i < sysInfo.dwNumberOfProcessors; i++)
        _beginthreadex(NULL, 0, ThreadMain, (LPVOID)hComPort, 0, NULL);

    // socket ����
    hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAdr.sin_port = htons(PORT_NUM);

    // bind and listen q
    bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr));
    listen(hServSock, 5);

    while (1)
    {
        SOCKET hClntSock;
        SOCKADDR_IN clntAdr;
        int addrLen = sizeof(clntAdr);

        hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &addrLen);

        handleInfo = (LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));        // LPPER_HANDLE_DATA �ʱ�ȭ
        inet_ntop(AF_INET, &clntAdr.sin_addr, handleInfo->ip, INET_ADDRSTRLEN);    // get new client ip
        handleInfo->hClntSock = hClntSock;                                        // Ŭ���̾�Ʈ�� ������ ����ü�� ��� ���´�.
        printf("New Client Access : %s\n", handleInfo->ip);
        memcpy(&(handleInfo->clntAdr), &clntAdr, addrLen);

        // ���� ����� ��Ʈ�� accept�� ���ؼ� return �� Ŭ���̾�Ʈ ������ ���´�.
        CreateIoCompletionPort((HANDLE)hClntSock, hComPort, (DWORD)handleInfo, 0);

        // Ŭ���̾�Ʈ user data �ʱ�ȭ
        UserList.push_back(handleInfo);

        // Ŭ���̾�Ʈ�� ������ �� data �ʱ�ȭ
        ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
        memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
        memset(ioInfo->buffer, 0x00, BUF_SIZE);
        ioInfo->wsaBuf.len = BUF_SIZE;
        ioInfo->wsaBuf.buf = ioInfo->buffer;
        ioInfo->rwMode = READ;

        // �񵿱� ����� ����
        WSARecv(handleInfo->hClntSock, &(ioInfo->wsaBuf), 1, &recvBytes, &flags, &(ioInfo->overlapped), NULL);
    }
    return 0;
}

unsigned __stdcall ThreadMain(void* pComPort)
{
    HANDLE hComPort = (HANDLE)pComPort;
    bool roomMessage = false;
    SOCKET sock;
    DWORD bytesTrans;
    LPPER_HANDLE_DATA handleInfo;
    LPPER_IO_DATA ioInfo;
    int flags = 0;
    CHAR message[BUF_SIZE];

    while (1)
    {
        // �񵿱� ����� �ϷḦ ��ٸ���.
        GetQueuedCompletionStatus(hComPort, &bytesTrans, (LPDWORD)&handleInfo, (LPOVERLAPPED*)&ioInfo, INFINITE);
        // Ŭ���̾�Ʈ�� socket�� �����´�.
        sock = handleInfo->hClntSock;

        // ù ������ �б� ����
        if (ioInfo->rwMode == READ)
        {
            // critical section
            // EOF ����, client exit
            if (bytesTrans == 0)
            {
                std::vector<LPPER_HANDLE_DATA>::iterator iter;
                for (iter = UserList.begin(); iter != UserList.end(); ++iter) {
                    if ((*iter)->hClntSock == sock) {
                        UserList.erase(iter);
                        break;
                    }
                }
                closesocket(sock);
                free(handleInfo);
                free(ioInfo);
                continue;
            }
            // critical section end

            // Ŭ���̾�Ʈ�� ���� ������ ����ü�� ������ �ٲ۴�.
            // ���� ������ ���⸦ ����

            /*  (�߰� ����)
                handling�� ����Ͽ�
                ���� Ŭ���̾�Ʈ���� ���� */
            std::vector<LPPER_HANDLE_DATA>::iterator iter;
            memcpy(message, ioInfo->wsaBuf.buf, BUF_SIZE);
            switch (message[0]) {
            case MAKE_ROOM_REQUEST:
                if (roomList.size() < MAX_ROOM_NUM) {
                    message[0] = MAKE_ROOM_ACCEPT;
                    roomList.push_back(new Room());
                    message[2] = roomList.size();
                    roomMessage = true;
                }
                else
                    continue;
                break;
            case JOIN_ROOM_REQUEST:
                if (roomList[(int)message[1]-1]->userList.size() < MAX_ROOM_PLAYER) {
                    message[0] = JOIN_ROOM_ACCEPT;
                    roomList[(int)message[1]-1]->userList.push_back((int)message[2]);
                }
                roomMessage = true;
                break;
            case PLAYER_ID_REQUEST:
                message[0] = SET_PLAYER_ID;
                message[1] = player_id++;
                roomMessage = false;
                break;
            case GET_ROOM_LIST:
                message[0] = SET_ROOM_LIST;
                cout << roomList.size() << endl;
                message[1] = 4;
                for (int i = 0; i < roomList.size(); i++) {
                    message[2 + i] = roomList[i]->userList.size();
                }
                roomMessage = false;
                break;
            case EXIT_ROOM_REQUEST:
                message[0] = EXIT_ROOM_ACCEPT;
                for (vector<int>::iterator iter = roomList[(int)message[2] - 1]->userList.begin(); iter != roomList[(int)message[2] - 1]->userList.end(); iter++) {
                    if ((*iter) == (int)message[1]) {
                        roomList[(int)message[2] - 1]->userList.erase(iter);
                        break;
                    }
                }
                roomMessage = true;
                break;
            }
            message[BUF_SIZE - 1] = '\0';
            free(ioInfo);

            if (roomMessage) {
                for (iter = UserList.begin(); iter != UserList.end(); ++iter) {

                    ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
                    memset(&(ioInfo->overlapped), 0x00, sizeof(OVERLAPPED));
                    int len = strlen(message);
                    ioInfo->wsaBuf.len = len;
                    strcpy(ioInfo->buffer, message);
                    ioInfo->wsaBuf.buf = ioInfo->buffer;
                    ioInfo->rwMode = WRITE;
                    if (WSASend((*iter)->hClntSock, &(ioInfo->wsaBuf), 1, &bytesTrans, 0, &(ioInfo->overlapped), NULL) == SOCKET_ERROR)
                    {
                        if (WSAGetLastError() != WSA_IO_PENDING)
                            ErrorHandling("WSASend() error");
                    }
                }
            }
            else {
                ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
                memset(&(ioInfo->overlapped), 0x00, sizeof(OVERLAPPED));
                int len = strlen(message);
                ioInfo->wsaBuf.len = len;
                strcpy(ioInfo->buffer, message);
                ioInfo->wsaBuf.buf = ioInfo->buffer;
                ioInfo->rwMode = WRITE;
                if (WSASend(sock, &(ioInfo->wsaBuf), 1, &bytesTrans, 0, &(ioInfo->overlapped), NULL) == SOCKET_ERROR)
                {
                    if (WSAGetLastError() != WSA_IO_PENDING)
                        ErrorHandling("WSASend() error");
                }
            }
            // ������ ����ü �ʱ�ȭ, ���� -> �б� ���� ����
            ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
            memset(&(ioInfo->overlapped), 0x00, sizeof(OVERLAPPED));
            ioInfo->wsaBuf.len = BUF_SIZE;
            ioInfo->wsaBuf.buf = ioInfo->buffer;
            ioInfo->rwMode = READ;

            // �б� ����
            if (WSARecv(sock, &(ioInfo->wsaBuf), 1, &bytesTrans, (LPDWORD)&flags, &(ioInfo->overlapped), NULL) == SOCKET_ERROR)
            {
                if (WSAGetLastError() != WSA_IO_PENDING)
                    ErrorHandling("WSASend() error");
            }
        }
        // ���� ����
        else {
            free(ioInfo);
        }
    }
    return 0;
}

void ErrorHandling(const char* message)
{
    perror(message);
    exit(1);
}
