#include "Window.h"


Window::Window(void)
{
	wClsName = "app";
//	wWndName = "Teren - projekt";
	wWndName = "Terrain - project";
	wWndClassName = "firstwindow";
	wHinst = NULL;
	wHwnd = NULL;
}


Window::~Window(void)
{
}

HINSTANCE Window::wGetInstance(void)
{
	return wHinst;
}

void Window::wSetInstance(HINSTANCE hinst)
{
	wHinst = hinst;
}

HWND Window::wGetHWND(void)
{
	return wHwnd;
}

bool Window::InitializeWindow(void)
{
	WNDCLASSEX wndclassex;
	wndclassex.cbSize = sizeof(WNDCLASSEX);
	wndclassex.style = CS_HREDRAW | CS_VREDRAW;
	wndclassex.lpfnWndProc = WndProc;
	wndclassex.cbWndExtra = 0;
	wndclassex.cbClsExtra = 0;
	wndclassex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclassex.hCursor = LoadCursor(NULL, IDC_CROSS);
	wndclassex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndclassex.lpszMenuName = NULL;
	wndclassex.lpszClassName = wClsName;
	wndclassex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclassex.hInstance = wHinst;
	wndclassex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wndclassex))
	{
		DWORD dw = GetLastError();
		CHAR szBuf[80];
		sprintf(szBuf, "failed: GetLastError returned %u\n", dw);
		MessageBox(NULL, szBuf, "Error", MB_OK);
		ExitProcess(dw);
	}

	
	wHwnd = CreateWindowEx(
		NULL,
		wClsName,
		wWndName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wind_width,
		wind_height,
		(HWND)NULL,
		(HMENU)NULL,
		wHinst,
		(LPVOID)NULL);

	if(!wHwnd)
	{
		DWORD dw = GetLastError();
		CHAR szBuf[80];
		sprintf(szBuf, "hwnd: GetLastError returned %u\n", dw);
		MessageBox(NULL, szBuf, "Error", MB_OK);
		ExitProcess(dw);
	}

	ShowWindow(wHwnd, SW_SHOW);
	SetForegroundWindow(wHwnd);
	SetFocus(wHwnd);

	return true;
}