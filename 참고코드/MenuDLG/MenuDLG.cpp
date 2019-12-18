#include <windows.h>
#include "resource.h"

BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("MenuDLG");

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

	//메뉴 리소스를 작성해서 윈도우 클래스에 배치
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_COMMAND:

		switch (LOWORD(wParam))
		{
		case ID_FILE_OPEN:

			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, AboutDlgProc);

			break;
		}

		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HWND hRadio;
	switch (iMessage)
	{
		case WM_INITDIALOG:

			hRadio = GetDlgItem(hDlg, IDC_RADIO1);
			SendMessage(hRadio, BM_SETCHECK, BST_CHECKED, 0);

			return TRUE;

		case WM_COMMAND:

			switch (wParam)
			{
				case IDOK:
					
					if (IsDlgButtonChecked(hDlg, IDC_RADIO1) == BST_CHECKED)
					{
						MessageBox(hDlg, "Beginner", "초급자", MB_OK);
					}
					else if (IsDlgButtonChecked(hDlg, IDC_RADIO2) == BST_CHECKED)
					{
						MessageBox(hDlg, "Intermediate", "중급자", MB_OK);
					}
					else if (IsDlgButtonChecked(hDlg, IDC_RADIO3) == BST_CHECKED)
					{
						MessageBox(hDlg, "Advanced ", "고급자", MB_OK);
					}

					EndDialog(hDlg, 0);

					return TRUE;

				case IDCANCEL:
					EndDialog(hDlg, 0);
					return TRUE;
			}

			break;
	}

	return FALSE;
}
