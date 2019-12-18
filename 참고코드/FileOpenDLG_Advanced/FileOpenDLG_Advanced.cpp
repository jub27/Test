#include<windows.h>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("FileOpenDLG_Advenced");

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

TCHAR buf[1024] = TEXT("abcde");

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_CREATE:

		CreateWindow("button", "Save", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 800, 400, 100, 30, hWnd, (HMENU)100, g_hInst, NULL);
		CreateWindow("button", "Load", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 800, 450, 100, 30, hWnd, (HMENU)101, g_hInst, NULL);

		return 0;


	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			case 100: //SAVE
			{
				OPENFILENAME OFN;
				char str[300];
				char lpstrFile[MAX_PATH] = "";
				char lpstrPath[MAX_PATH] = "";

				memset(&OFN, 0, sizeof(OPENFILENAME));
				OFN.lStructSize = sizeof(OPENFILENAME);
				OFN.hwndOwner = hWnd;
				OFN.lpstrFilter = "Every File(*.*)\0*.*\0Text File\0*.txt;*.doc\0";
				OFN.lpstrFile = lpstrFile;
				OFN.nMaxFile = 256;
				GetCurrentDirectory(MAX_PATH, lpstrPath);
				OFN.lpstrInitialDir = lpstrPath;
				
				if (GetSaveFileName(&OFN) == 0)
				{
					DWORD err = CommDlgExtendedError();
					break;
				}

				HANDLE hFile = CreateFile(OFN.lpstrFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
				DWORD writeB;

				WriteFile(hFile, buf, sizeof(buf), &writeB, NULL);
				
				MessageBox(hWnd, "¼º°ø", "Save", MB_OK);

				CloseHandle(hFile);
				InvalidateRect(hWnd, NULL, false);
			}
			break;
			case 101://load
			{
				HANDLE hFile = CreateFile("save.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
				DWORD readB;
				
				ReadFile(hFile, buf, sizeof(buf), &readB, NULL);

				MessageBox(hWnd, buf, "Load", MB_OK);

				CloseHandle(hFile);
				InvalidateRect(hWnd, NULL, false);
			}
			break;
		}
		return 0;
	}
	case WM_DESTROY:

		PostQuitMessage(0);

		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
