#include<Windows.h>
#include "resource.h"
#include "GameManager.h"
#include <ctime>
#include <cstdlib>

BOOL CALLBACK DifficultyDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("FindMine");

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, GAME_WIDTH, GAME_HEIGHT, NULL,
		(HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	
	srand((unsigned int)time(NULL));
	GameManager::GetInstance()->Init(hWnd);


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
	POINT pt;
	static int time = 0;
	char buffer[20];
	RECT rt = { 160, 480, 180, 500 };
	LPCSTR p;
	int gameResult = 0;

	switch (iMessage)
	{
	case WM_CREATE:
		GameManager::GetInstance()->NewGame();
		SetTimer(hWnd, 1, 1000, NULL);
		SendMessage(hWnd, WM_TIMER, 1, 0); 
		time = 0;

		_itoa_s(time, buffer, 20, 10);
		p = buffer;
		DrawText(GetDC(hWnd), p, -1, &rt, DT_CENTER | DT_WORDBREAK);

		return 0;
	case WM_TIMER:
		if (GameManager::GetInstance()->is_GameStart()) {
			time++;
			_itoa_s(time, buffer, 20, 10);
			p = buffer;
			DrawText(GetDC(hWnd), p, -1, &rt, DT_CENTER | DT_WORDBREAK);
		}
		return 0;
	case WM_LBUTTONDOWN:
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		gameResult = GameManager::GetInstance()->LeftClick(pt.x, pt.y);
		if (gameResult == GAME_OVER) {
			InvalidateRect(hWnd, NULL, FALSE);
			if (MessageBox(hWnd, TEXT("다시 시작하겠습니까?"), TEXT("Game Over"), MB_YESNO) == IDYES) {
				GameManager::GetInstance()->NewGame();
				time = 0;
			}
			else {
				KillTimer(hWnd, 1);
				PostQuitMessage(0);
				return 0;
			}
		}
		else if (gameResult == GAME_WIN) {
			InvalidateRect(hWnd, NULL, FALSE);
			if (MessageBox(hWnd, TEXT("다시 시작하겠습니까?"), TEXT("Game Win"), MB_YESNO) == IDYES) {
				GameManager::GetInstance()->NewGame();
				time = 0;
			}
			else {
				KillTimer(hWnd, 1);
				PostQuitMessage(0);
				return 0;
			}
		}
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_RBUTTONDOWN:
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		GameManager::GetInstance()->RightClick(pt.x, pt.y);
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_40001:
			GameManager::GetInstance()->NewGame();
			time = 0;
			InvalidateRect(hWnd, NULL, FALSE);
			return 0;
		case ID_40002:
			if (DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DifficultyDlgProc) == 1) {
				GameManager::GetInstance()->NewGame();
				time = 0;
				InvalidateRect(hWnd, NULL, FALSE);
			}
			return 0;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GameManager::GetInstance()->Draw();
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

BOOL CALLBACK DifficultyDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HWND hRadio;
	static int difficulty = 0;

	switch (iMessage)
	{
	case WM_INITDIALOG:
		CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
		difficulty = 0;
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_RADIO1:
			difficulty = 0;
			return TRUE;
		case IDC_RADIO2:
			difficulty = 1;
			return TRUE;
		case IDC_RADIO3:
			difficulty = 2;
			return TRUE;
		case IDOK:
			GameManager::GetInstance()->SetDifficulty(difficulty);
			EndDialog(hDlg, 1);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;}