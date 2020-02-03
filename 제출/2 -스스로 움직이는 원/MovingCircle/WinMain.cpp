
#include <Windows.h>
#include <math.h>
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("MovingCircle");

struct Circle {
	int center_x, center_y, radius;
};

Circle circle = { 50, 50, 50 };
int xLimit = 458, yLimit = 396;
int xSpeed = 1, ySpeed = 1;

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
	HDC hdc;
	PAINTSTRUCT ps;
	SYSTEMTIME st;
	switch (iMessage)
	{
	case WM_CREATE:
		SetTimer(hWnd, 1, 5, NULL);
		SendMessage(hWnd, WM_TIMER, 1, 0);
		return 0;
	case WM_TIMER:
		GetLocalTime(&st);
		circle.center_x += xSpeed;
		circle.center_y += ySpeed;
		if (circle.center_x + circle.radius >= xLimit && xSpeed == 1)
			xSpeed *= -1;
		if (circle.center_x - circle.radius <= 0 && xSpeed == -1)
			xSpeed *= -1;
		if (circle.center_y + circle.radius >= yLimit && ySpeed == 1)
			ySpeed *= -1;
		if (circle.center_y - circle.radius <=0 && ySpeed == -1)
			ySpeed *= -1;
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd,&ps);
		Rectangle(hdc, 0, 0, xLimit, yLimit);
		for (int i = 0; i < 360; i++)
			SetPixel(hdc, circle.center_x + cos(i* (3.14 / 180)) * circle.radius, circle.center_y + sin(i * (3.14 / 180)) * circle.radius, RGB(0, 0, 0));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
