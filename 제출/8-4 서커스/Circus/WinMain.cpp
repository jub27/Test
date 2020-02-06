#include<Windows.h>
#include<time.h>
#include "GameManager.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Circus");
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPervlnstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 533,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	HDC hdc;
	hdc = GetDC(hWnd);
	srand(time(NULL));
	GameManager::GetInstance()->Init(hWnd);
	GameManager::GetInstance()->InitLife();;
	int GameState = -1;
	while (true) {
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
			if (Message.message == WM_QUIT)
				break;
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else {
			if(GameState == -1)
				GameState = GameManager::GetInstance()->Title();
			if (GameState == 0) {
				GameManager::GetInstance()->Render();
				int result = GameManager::GetInstance()->CheckPlayer();
				if (result == 0) {
					Sleep(1000);
					int life = GameManager::GetInstance()->GetLife();
					if (life == 0) {
						GameManager::GetInstance()->InitLife();
						GameState = -1;
					}
					GameManager::GetInstance()->Init(hWnd);
				}
				else if (result == 1) {
					Sleep(1000);
					GameManager::GetInstance()->Init(hWnd);
					GameManager::GetInstance()->InitLife();
					GameState = -1;
				}
			}
			else if (GameState == 1)
				break;
		}

	}
	GameManager::GetInstance()->Release();
	return (int)Message.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}