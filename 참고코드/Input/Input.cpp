#include<windows.h>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Input");

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

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

TCHAR str[256];
int x = 100;
int y = 100;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int len;

	switch (iMessage)
	{
		/*case WM_CHAR:
			len = lstrlen(str);
			str[len] = (TCHAR)wParam;
			str[len + 1] = 0;
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;*/

		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			//TextOut(hdc, 100, 100, str, lstrlen(str));

			TextOut(hdc, x, y, TEXT("A"), 1);
			EndPaint(hWnd, &ps);
			return 0;


		case WM_KEYDOWN:

			switch (wParam)
			{
				case VK_LEFT:
					x -= 10;
					break;
				case VK_RIGHT:
					x += 10;
					break;
				case VK_UP:
					y -= 10;
					break;
				case VK_DOWN:
					y += 10;
					break;
			}

			InvalidateRect(hWnd, NULL, TRUE);
		return 0;
		
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}