#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
char g_szClassName[256] = "Checkbox";

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
	WndClass.lpszClassName = g_szClassName;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(g_szClassName, g_szClassName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
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
	static HWND c1, c2;
	static BOOL bEllipse = FALSE;

	switch (iMessage)
	{
		case WM_CREATE:

			c1 = CreateWindow(TEXT("button"), TEXT("Draw Ellipse"), WS_CHILD | WS_VISIBLE |
				BS_CHECKBOX, 20, 20, 160, 25, hWnd, (HMENU)0, g_hInst, NULL);
			c2 = CreateWindow(TEXT("button"), TEXT("GoodBye Message?"), WS_CHILD | WS_VISIBLE |
				BS_AUTOCHECKBOX, 20, 50, 160, 25, hWnd, (HMENU)1, g_hInst, NULL);
			
			return 0;
		case WM_COMMAND:
			
			case 0:
					if(SendMessage(c1, BM_GETCHECK, 0, 0) == BST_UNCHECKED)
					{
						SendMessage(c1, BM_SETCHECK, BST_CHECKED, 0);
						bEllipse = TRUE;
					}
					else
					{
						SendMessage(c1, BM_SETCHECK, BST_UNCHECKED, 0);
						bEllipse = FALSE;
					}

					InvalidateRect(hWnd, NULL, TRUE);
				break;
			
			return 0;
		
		case WM_PAINT:
			
			hdc = BeginPaint(hWnd, &ps);

			if(bEllipse == TRUE)
			{
				Ellipse(hdc, 200, 100, 400, 200);
			}
			else
			{
				Rectangle(hdc, 200, 100, 400, 200);
			}

			EndPaint(hWnd, &ps);

			return 0;
		
		case WM_DESTROY:

			if(SendMessage(c2, BM_GETCHECK, 0, 0) == BST_CHECKED)
			{
				MessageBox(hWnd, TEXT("Good bye"), TEXT("Check"), MB_OK);
			}

			PostQuitMessage(0);
			return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
