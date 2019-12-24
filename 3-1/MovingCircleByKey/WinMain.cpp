#include <Windows.h>
#include <math.h>



LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("MovingCircleByKey");



struct Circle {
	int left, top, right, bottom;
	Circle(int left, int top, int right, int bottom) {
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
	}
	void Up() {
		top--;
		bottom--;
	}
	void Down() {
		top++;
		bottom++;
	}
	void Left(){
		left--;
		right--;
	}
	void Right() {
		left++;
		right++;
	}
	void Draw(HDC hdc) {
		Ellipse(hdc, left, top, right, bottom);
	}
};

Circle circle = Circle(0,0, 100, 100);

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
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
		(HMENU)NULL, hInstance, NULL);
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
	SYSTEMTIME st;
	switch (iMessage)
	{
	case WM_CREATE:
		return 0;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_UP:
			circle.Up();
			break;
		case VK_DOWN:
			circle.Down();
			break;
		case VK_LEFT:
			circle.Left();
			break;
		case VK_RIGHT:
			circle.Right();
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		circle.Draw(hdc);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}