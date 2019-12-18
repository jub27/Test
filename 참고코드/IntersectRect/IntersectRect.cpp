#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("IntersectRect");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
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

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768,
		NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

RECT rcTemp = { 0 };
RECT rcRectangle1 = { 100, 100, 200, 200 };
RECT rcRectangle2 = { 400, 100, 500, 200 };

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage)
	{
	case WM_CREATE:

		SetTimer(hWnd, 1, 100, NULL);
		SendMessage(hWnd, WM_TIMER, 1, 0);
		
		return 0;

	case WM_TIMER:

		if (!IntersectRect(&rcTemp, &rcRectangle1, &rcRectangle2))
		{
			rcRectangle1.left += 10;
			rcRectangle1.right += 10;
		}

		InvalidateRect(hWnd, NULL, TRUE);

		return 0;

	case WM_PAINT:

		hdc = BeginPaint(hWnd, &ps);

		Rectangle(hdc, rcRectangle1.left, rcRectangle1.top, rcRectangle1.right, rcRectangle1.bottom);
		Rectangle(hdc, rcRectangle2.left, rcRectangle2.top, rcRectangle2.right, rcRectangle2.bottom);

		EndPaint(hWnd, &ps);

		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}