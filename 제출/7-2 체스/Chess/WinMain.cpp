#include "GameManager.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Chess");
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
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1100, 1080, NULL,
		(HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	delete GameManager::GetInstance();
	return (int)Message.wParam;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	POINT in_Pt;
	int ClickResult;
	switch (iMessage)
	{
	case WM_CREATE:
		GameManager::GetInstance()->Init(hWnd);
		GameManager::GetInstance()->ResetBoard();
		return 0;
	case WM_LBUTTONDOWN:
		in_Pt.x = LOWORD(lParam);
		in_Pt.y = HIWORD(lParam);
		ClickResult = GameManager::GetInstance()->Click(in_Pt.x, in_Pt.y);
		InvalidateRect(hWnd, NULL, TRUE);
		if (ClickResult == 1) {
			if (MessageBox(hWnd, L"다시 시작", L"WHITE WIN", MB_YESNO) == IDYES) {
				GameManager::GetInstance()->ResetBoard();
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else {
				PostQuitMessage(0);
				return 0;
			}
		}
		else if (ClickResult == 2) {
			if (MessageBox(hWnd, L"다시 시작", L"BLACK WIN", MB_YESNO) == IDYES) {
				GameManager::GetInstance()->ResetBoard();
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else {
				PostQuitMessage(0);
				return 0;
			}
		}
		return 0;
	case WM_RBUTTONDOWN:
		GameManager::GetInstance()->UnClick();
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GameManager::GetInstance()->CheckSelectable();
		GameManager::GetInstance()->DrawBoard(hdc);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}