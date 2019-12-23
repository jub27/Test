#include <Windows.h>
#include <math.h>
#include <stdio.h>

#define CLOCK_CENTER_X 110
#define CLOCK_CENTER_Y 100
#define CLOCK_RADIUS 100

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("AnalogueClock");

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
	int clockHour = 3;
	char buf[80];
	switch (iMessage)
	{
	case WM_CREATE:
		SetTimer(hWnd, 1000, 10, NULL);
		SendMessage(hWnd, WM_TIMER, 1, 0);
		return 0;
	case WM_TIMER:
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_PAINT:
		GetLocalTime(&st);
		hdc = BeginPaint(hWnd, &ps);
		SetTextAlign(hdc, TA_CENTER);
		for (int i = 0; i <= 360; i += 30) {
			sprintf(buf, "%d", clockHour);
			TextOut(hdc, CLOCK_CENTER_X + cos( i * (3.14 / 180)) * CLOCK_RADIUS, CLOCK_CENTER_Y - sin(i * (3.14 / 180)) * CLOCK_RADIUS, buf, strlen(buf));
			clockHour--;
			if (clockHour == 0)
				clockHour = 12;
		}
		sprintf(buf, "%d:%d:%d", st.wHour, st.wMinute , st.wSecond);
		TextOut(hdc, 200, 200, buf,strlen(buf)) ;
		MoveToEx(hdc, CLOCK_CENTER_X, CLOCK_CENTER_Y, NULL);
		LineTo(hdc, CLOCK_CENTER_X + cos((360 - (st.wSecond / 60.0 * 360 - 90) )* (3.14 / 180)) * CLOCK_RADIUS * 0.7, CLOCK_CENTER_Y - sin((360 - (st.wSecond / 60.0 * 360 - 90)) * (3.14 / 180)) * CLOCK_RADIUS * 0.7);
		MoveToEx(hdc, CLOCK_CENTER_X, CLOCK_CENTER_Y, NULL);
		LineTo(hdc, CLOCK_CENTER_X + cos((360 - (st.wMinute / 60.0 * 360 - 90))* (3.14 / 180)) * CLOCK_RADIUS * 0.5, CLOCK_CENTER_Y - sin((360 - (st.wMinute / 60.0 * 360 - 90)) * (3.14 / 180)) * CLOCK_RADIUS * 0.5);
		MoveToEx(hdc, CLOCK_CENTER_X, CLOCK_CENTER_Y, NULL);
		LineTo(hdc, CLOCK_CENTER_X + cos((360 - ((st.wHour%12) / 12.0 * 360 + st.wMinute / 60.0 * 30 - 90))* (3.14 / 180)) * CLOCK_RADIUS * 0.3, CLOCK_CENTER_Y - sin((360 - ((st.wHour % 12) / 12.0 * 360 + st.wMinute / 60.0 * 30 - 90)) * (3.14 / 180)) * CLOCK_RADIUS * 0.3);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}