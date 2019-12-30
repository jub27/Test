#include <math.h>
#include <stdio.h>
#include "MyBitmap.h"


#define BITMAP_WIDTH 145
#define BITMAP_HEIGHT 235
#define BITMAP_COUNT 10

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LPCTSTR lpszClass = TEXT("BitMapClick");
HINSTANCE g_hInst;
MyBitmap * bitmap[BITMAP_COUNT];
LPCWSTR name[10] = { L"강아지", L"호랑이", L"오리", L"코끼리", L"소", L"말", L"고양이", L"원숭이", L"개구리", L"닭" };
int x, y;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPervlnstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	for (int i = 0; i < BITMAP_COUNT; i++)
		bitmap[i] = new MyBitmap(IDB_BITMAP1 + i, BITMAP_WIDTH * (i % 5), BITMAP_HEIGHT * (i / 5), BITMAP_WIDTH, BITMAP_HEIGHT);

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
	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, MemDC;
	PAINTSTRUCT ps;
	HBITMAP myBitmap, oldBitmap;
	switch (iMessage)
	{
	case WM_LBUTTONDOWN:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		for (int i = 0; i < BITMAP_COUNT; i++)
			if (bitmap[i]->clickCheck(x, y)) {
				MessageBox(hWnd, name[i], L"MessageBox", MB_OK);
				break;
			}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		for (int i = 0; i < BITMAP_COUNT; i++)
			bitmap[i]->print(g_hInst, hdc);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}