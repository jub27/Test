#include<windows.h>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("File In-Out");

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
	HANDLE hFile;
	DWORD dwRead;

	switch (iMessage)
	{
	case WM_CREATE:
		return 0;
	
	case WM_LBUTTONDOWN:

		hFile = CreateFile(TEXT("Test.txt"), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile != INVALID_HANDLE_VALUE)
		{
			ReadFile(hFile, buf, 1024, &dwRead, NULL);
			CloseHandle(hFile);
			
			MessageBox(hWnd, buf, TEXT("알림"), MB_OK);
		}

		return 0;

	case WM_RBUTTONDOWN:

		hFile = CreateFile(TEXT("Test.txt"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		
		WriteFile(hFile, buf, lstrlen(buf), &dwRead, NULL);
		CloseHandle(hFile);
		
		MessageBox(hWnd, TEXT("Test.txt를 생성 했습니다."), TEXT("알림"), MB_OK);

		return 0;
	case WM_DESTROY:

		PostQuitMessage(0);

		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
