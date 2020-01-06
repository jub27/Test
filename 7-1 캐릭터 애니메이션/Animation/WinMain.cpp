#include "Character.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Animation");

Character * character;

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

	character = new Character();
	character->Init(hWnd, hInstance);

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
	static int degree = 0;
	static int count = 0;
	static bool moveFlag = true;
	static bool jumpFlag = true;
	switch (iMessage)
	{
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
		case VK_RIGHT:
		case VK_UP:
		case VK_DOWN:
			if (moveFlag) {
				moveFlag = false;
				character->SetPrevDir(character->GetCurDir());
				character->SetCurDir(wParam);
				if (character->GetCurDir() == character->GetPrevDir())
					character->NextMotion();
				else
					character->InitMotion();
				SetTimer(hWnd, 2, 16, NULL);
			}
			break;
		case VK_SPACE:
			if (jumpFlag) {
				jumpFlag = false;
				SetTimer(hWnd, 1, 10, NULL);
				SendMessage(hWnd, WM_TIMER, 1, 0);
				break;
			}
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		character->Draw(hdc);

		EndPaint(hWnd, &ps);
		return 0;
	case WM_TIMER:
		switch (wParam) {
		case 1:
			character->Jump(degree);
			degree += 10;
			if (degree == 180) {
				jumpFlag = true;
				KillTimer(hWnd, 1);
				degree = 0;
			}
			break;
		case 2:
			switch (character->GetCurDir()) {
			case VK_LEFT:
				character->MoveX(-1);
				break;
			case VK_RIGHT:
				character->MoveX(1);
				break;
			case VK_UP:
				character->MoveY(-1);
				break;
			case VK_DOWN:
				character->MoveY(1);
				break;
			}
			count += 1;
			if (count == 5) {
				KillTimer(hWnd, 2);
				count = 0;
				moveFlag = true;
			}
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_DESTROY:
		delete character;
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}