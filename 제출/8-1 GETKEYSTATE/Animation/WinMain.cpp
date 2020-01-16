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
	static int countX = 0;
	static int countY = 0;
	static bool xFlag = true;
	static bool yFlag = true;
	static bool jumpFlag = true;
	bool motionFlag = false;
	switch (iMessage)
	{
	case WM_KEYDOWN:
		if (GetKeyState(VK_LEFT) & 0x8000)
			if (xFlag) {
				xFlag = false;
				character->SetPrevDir(character->GetCurDir());
				character->SetCurDir(LEFT);
				if (character->GetCurDir() == character->GetPrevDir())
					character->NextMotion();
				else
					character->InitMotion();
				SetTimer(hWnd, 2, 10, NULL);
				motionFlag = true;
			}
		if (GetKeyState(VK_RIGHT) & 0x8000)
			if (xFlag) {
				xFlag = false;
				character->SetPrevDir(character->GetCurDir());
				character->SetCurDir(RIGHT);
				if (character->GetCurDir() == character->GetPrevDir())
					character->NextMotion();
				else
					character->InitMotion();
				SetTimer(hWnd, 3, 10, NULL);
				motionFlag = true;
			}
		if (GetKeyState(VK_UP) & 0x8000)
			if (yFlag) {		
				yFlag = false;
				if (!motionFlag) {
					character->SetPrevDir(character->GetCurDir());
					character->SetCurDir(UP);
					if (character->GetCurDir() == character->GetPrevDir())
						character->NextMotion();
					else
						character->InitMotion();
				}
				SetTimer(hWnd, 4, 10, NULL);
			}
		if (GetKeyState(VK_DOWN) & 0x8000)
			if (yFlag) {
				yFlag = false;
				if (!motionFlag) {
					character->SetPrevDir(character->GetCurDir());
					character->SetCurDir(DOWN);
					if (character->GetCurDir() == character->GetPrevDir())
						character->NextMotion();
					else
						character->InitMotion();
				}
				SetTimer(hWnd, 5, 10, NULL);
			}
		if (GetKeyState(VK_SPACE) & 0x8000)
			if (jumpFlag) {
				jumpFlag = false;
				SetTimer(hWnd, 1, 10, NULL);
				SendMessage(hWnd, WM_TIMER, 1, 0);
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
			character->MoveX(-1);
			countX += 1;
			if (countX == 5) {
				KillTimer(hWnd, 2);
				countX = 0;
				xFlag = true;
			}
			break;
		case 3:
			character->MoveX(1);
			countX += 1;
			if (countX == 5) {
				KillTimer(hWnd, 3);
				countX = 0;
				xFlag = true;
			}
			break;
		case 4:
			character->MoveY(-1);
			countY += 1;
			if (countY == 5) {
				KillTimer(hWnd, 4);
				countY = 0;
				yFlag = true;
			}
			break;
		case 5:
			character->MoveY(1);
			countY += 1;
			if (countY == 5) {
				KillTimer(hWnd, 5);
				countY = 0;
				yFlag = true;
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