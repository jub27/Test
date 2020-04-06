#include <iostream>
#include <Windows.h>
#include <process.h>
using namespace std;

unsigned int WINAPI ThreadFunction(void* arg)
{
	int i;
	for (i = 0; i < 5; i++)
	{
		Sleep(2000);
		cout << "스레드 실행 중1" << endl;
	}
	return 0;
}

unsigned int WINAPI ThreadFunction2(void* arg)
{
	int i;
	for (i = 0; i < 5; i++)
	{
		Sleep(2000);
		cout << "스레드 실행 중2" << endl;
	}
	return 0;
}

int main()
{
	HANDLE hThread;
	DWORD dwThreadID;
	// thread를 생성.
	hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, NULL, 0, (unsigned int*)&dwThreadID);
	hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction2, NULL, 0, (unsigned int*)&dwThreadID);

	Sleep(10000);
	CloseHandle(hThread);

	return 0;
}