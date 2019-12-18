#include "defines.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWnd;
LPCTSTR lpszClass = TEXT("DoubleBuffering");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
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
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;
}

//그림은 두장이지만 두장의 그림을 계속 갱신해서 그려줄 MemDC가 하나더 필요하다.
//메모리상의 DC가 확보되었지만 도화지역활을 해줄 공간을 위해 아무것도 없는 공간에 
//하얀색 비트맵을 그려준다 - Bitmap & OldBitmap이 하나씩 더 필요하다.
HDC		g_MemDC[3];
HBITMAP	g_hBitmap[3];
HBITMAP g_hOld[3];

//시작좌표
int		x = 500;
int		y = 300;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage)
	{
	case WM_CREATE:

		SetTimer(hWnd, 1, 10, NULL);

		hdc = GetDC(hWnd);

		//숨겨 그릴 장소(도화지)를 준비한다.
		g_MemDC[0] = CreateCompatibleDC(hdc);
		g_hBitmap[0] = CreateCompatibleBitmap(hdc, 1024, 768);
		g_hOld[0] = (HBITMAP)SelectObject(g_MemDC[0], g_hBitmap[0]);

		//숨겨 그릴 장소(도화지)에 배경을 그릴 준비를 한다.
		g_MemDC[1] = CreateCompatibleDC(g_MemDC[0]);
		g_hBitmap[1] = (HBITMAP)LoadImage(NULL, "back.bmp", IMAGE_BITMAP, 0, 0
			, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
		g_hOld[1] = (HBITMAP)SelectObject(g_MemDC[1], g_hBitmap[1]);

		//숨겨 그릴 장소(도화지)에 캐릭터를 그릴 준비를 한다.
		g_MemDC[2] = CreateCompatibleDC(g_MemDC[0]);
		g_hBitmap[2] = (HBITMAP)LoadImage(NULL, "char.bmp", IMAGE_BITMAP, 0, 0
			, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
		g_hOld[2] = (HBITMAP)SelectObject(g_MemDC[2], g_hBitmap[2]);

		ReleaseDC(hWnd, hdc);
			return 0;

	case WM_PAINT:

		hdc = BeginPaint(hWnd, &ps);

		//그림이 계속 지워졌다가 다시 그려지는 곳은 g_MemDC[0]이기 때문에 깜빡임이 보이지 않는다.

		//숨겨 그릴 장소(도화지)에 배경을 그린다.
		BitBlt(g_MemDC[0], 0, 0, 1024, 768, g_MemDC[1], 0, 0, SRCCOPY);

		//숨겨 그릴 장소(도화지)에 캐릭터를 그린다.
		TransparentBlt(g_MemDC[0], x, y, 240, 192, g_MemDC[2], 0, 0, 240, 192, RGB(255, 0, 255));

		//숨겨 그린 것을 원래 보여야할 hdc에 그린다.
		//단지 hdc에 그림을 덮어 그리는 행위만 하게된다.
		//그리는 행위는 하지만 뒷배경이 공백이 아니고 무언가를 계속 그리기 때문에 그림이 존재해서 그리는 동안 해당 그림을 보여주고 있는 것이라고 생각하면 된다.
		BitBlt(hdc, 0, 0, 1024, 768, g_MemDC[0], 0, 0, SRCCOPY);

		EndPaint(hWnd, &ps);

		return 0;

	case WM_TIMER:

		if (GetKeyState(VK_LEFT) & 0x8000)
			x -= 10;

		if (GetKeyState(VK_RIGHT) & 0x8000)
			x += 10;

		if (GetKeyState(VK_UP) & 0x8000)
			y -= 10;

		if (GetKeyState(VK_DOWN) & 0x8000)
			y += 10;

		InvalidateRect(hWnd, NULL, false);
		
		return 0;
	
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}