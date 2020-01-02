#include<Windows.h>
#include <time.h>
#include "GameManager.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("HelloWorld");
	int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPervlnstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	srand(time(NULL));

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
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
		(HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	Gamemanager::GetInstans()->Init(hWnd, g_hInst);
	Gamemanager::GetInstans()->Shuffle();
	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int x, y;
	int result;
	switch (iMessage)
	{
	case WM_LBUTTONDOWN:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		result = Gamemanager::GetInstans()->Click(x, y);
		if (result == 3) {
			SetTimer(hWnd, 1, 1000, NULL);
		}
		else if (result == 4) {
			InvalidateRect(hWnd, NULL, TRUE);
			if (MessageBox(hWnd, TEXT("다시하기"), TEXT("GameClear"), MB_YESNO) == IDYES) {
				Gamemanager::GetInstans()->Shuffle();
			}
			else {
				Gamemanager::GetInstans()->Release();
				PostQuitMessage(0);
				return 0;
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_TIMER:
		Gamemanager::GetInstans()->CloseCurLast();
		InvalidateRect(hWnd, NULL, TRUE);
		KillTimer(hWnd, 1);
		return 0;
	case WM_DESTROY:
		Gamemanager::GetInstans()->Release();
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		Gamemanager::GetInstans()->Draw(hdc);
		EndPaint(hWnd, &ps);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}