#include <Windows.h>
#include <vector>
using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCSTR szClassName = "≈ ≈©∏ ≈¯";

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
	WndClass.lpszClassName = szClassName;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	RegisterClass(&WndClass);

	int			nWidth, nHeight;//¿©µµøÏ ≈©±‚ 
	nWidth = 1024 + GetSystemMetrics(SM_CXFRAME) * 2;
	nHeight = 768 + GetSystemMetrics(SM_CYFRAME) * 2 +
		GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYMENU);

	hWnd = CreateWindow(szClassName, szClassName, WS_OVERLAPPEDWINDOW, 0, 0,
		nWidth, nHeight, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;
}

#define WIDTH 33
#define HEIGHT 25

int g_map[13][13];
int cur_select = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage)
	{
	case WM_CREATE:
		CreateWindow("button", "1", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 800, 0 , 100, 30 , hWnd, (HMENU)0, g_hInst, NULL);
		CreateWindow("button", "2", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 800, 40, 100, 30, hWnd, (HMENU)1, g_hInst, NULL);
		CreateWindow("button", "3", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 800, 80, 100, 30, hWnd, (HMENU)2, g_hInst, NULL);

		CreateWindow("button", "Save", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 800, 400, 100, 30, hWnd, (HMENU)100, g_hInst, NULL);
		CreateWindow("button", "Load", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 800, 450, 100, 30, hWnd, (HMENU)101, g_hInst, NULL);
		CreateWindow("button", "LoadJson", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 800, 300, 100, 30, hWnd, (HMENU)102, g_hInst, NULL);

		memset(g_map, 0, sizeof(int) * 13 * 13);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		for (int i = 0; i < 13; i++)
		{
			for (int j = 0; j < 13; j++)
			{
				if (g_map[i][j] == 0)
					Rectangle(hdc, j * WIDTH, i * HEIGHT, (j + 1) * WIDTH, (i + 1) * HEIGHT);
				else if(g_map[i][j] == 1)
					Ellipse(hdc, j * WIDTH, i * HEIGHT, (j + 1) * WIDTH, (i + 1) * HEIGHT);
				else if (g_map[i][j] == 2)
				{
					HBRUSH hBlack = (HBRUSH)GetStockObject(BLACK_BRUSH);
					HBRUSH old = (HBRUSH)SelectObject(hdc, hBlack);
					Ellipse(hdc, j * WIDTH, i * HEIGHT, (j + 1) * WIDTH, (i + 1) * HEIGHT);
					SelectObject(hdc, old);
				}
					
			}
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case 0:
		case 1:
		case 2:
			cur_select = LOWORD(wParam);
			break;
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
				for (int i = 0; i < 13; i++)
				{
					for (int j = 0; j < 13; j++)
					{
						DWORD writeB;
						WriteFile(hFile, &g_map[i][j], sizeof(int), &writeB, NULL);
					}
				}
				CloseHandle(hFile);
				InvalidateRect(hWnd, NULL, false);
			}
			break;
		case 101://load
			{
				HANDLE hFile = CreateFile("save.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
				for (int i = 0; i < 13; i++)
				{
					for (int j = 0; j < 13; j++)
					{
						DWORD readB;
						ReadFile(hFile, &g_map[i][j], sizeof(int), &readB, NULL);
					}
				}
				CloseHandle(hFile);
				InvalidateRect(hWnd, NULL, false);
			}
			break;
		}
		return 0;
	}
	case WM_LBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			if ((pt.x < WIDTH * 13 && pt.x > 0) && (pt.y < HEIGHT * 13 && pt.y > 0))
			{
				g_map[pt.y / HEIGHT][pt.x / WIDTH] = cur_select;
				InvalidateRect(hWnd, NULL, false);
			}
		}
		return 0;
	case WM_RBUTTONDOWN:
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
		}
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
