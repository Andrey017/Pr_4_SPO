#include <Windows.h>
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM);

#define TIMER1 1000
#define TIMER2 2000

HWND hwnd1;
HWND hwnd2;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	WNDCLASS wnd;
	memset(&wnd, 0, sizeof(wnd));

	wnd.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wnd.hCursor = LoadCursor(NULL, IDC_CROSS);
	wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wnd.hInstance = hInstance;
	wnd.lpfnWndProc = WndProc;
	wnd.lpszClassName = "MainWndClass";
	wnd.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

	if (!RegisterClass(&wnd))
		return 0;

	hwnd1 = CreateWindow("MainWndClass", "Pr 4 SPO", WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPCHILDREN, 50, 50, 500, 500, NULL, NULL, hInstance, NULL);

	if (!hwnd1)
		return 0;
	ShowWindow(hwnd1, SW_NORMAL);
	UpdateWindow(hwnd1);


	WNDCLASS wnd1;
	memset(&wnd1, 0, sizeof(wnd1));

	wnd1.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wnd1.hCursor = LoadCursor(NULL, IDC_CROSS);
	wnd1.hIcon = LoadIcon(NULL, IDI_ERROR);
	wnd1.hInstance = hInstance;
	wnd1.lpfnWndProc = ChildWndProc;
	wnd1.lpszClassName = "childClass";
	wnd1.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

	if (!RegisterClass(&wnd1))
		return 0;

	hwnd2 = CreateWindowEx(WS_EX_CLIENTEDGE, "childClass", "Child Window", WS_CHILD | WS_VISIBLE | WS_CAPTION | WS_MAXIMIZE | WS_THICKFRAME, 100, 100, 320, 240, hwnd1, NULL, hInstance, NULL);

	ShowWindow(hwnd2, SW_HIDE);
	UpdateWindow(hwnd2);


	SetTimer(hwnd1, TIMER1, 5000, NULL);
	SetTimer(hwnd1, TIMER2, 1000, NULL);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	DestroyWindow(hwnd1);
	UnregisterClass("MainWndClass", hInstance);

	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	long style;
	RECT wndRect;
	switch (msg)
	{
	case WM_TIMER:
		switch (wParam)
		{
		case TIMER1:
			style = GetWindowLong(hwnd, GWL_STYLE);
			style ^= WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPCHILDREN;
			style |= WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX;
			SetWindowLong(hwnd, GWL_STYLE, style);
			ShowWindow(hwnd, SW_HIDE);
			ShowWindow(hwnd, SW_SHOWNORMAL);
			break;
		case TIMER2:
			DeleteObject((HBRUSH)SetClassLong(hwnd, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256))));
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
			GetWindowRect(hwnd, &wndRect);
			MoveWindow(hwnd, wndRect.left, wndRect.top - 20, wndRect.right - wndRect.left, wndRect.bottom - wndRect.top, true);
			break;
		case VK_DOWN:
			GetWindowRect(hwnd, &wndRect);
			MoveWindow(hwnd, wndRect.left, wndRect.top + 20, wndRect.right - wndRect.left, wndRect.bottom - wndRect.top, true);
			break;
		case VK_LEFT:
			GetWindowRect(hwnd, &wndRect);
			MoveWindow(hwnd, wndRect.left - 20, wndRect.top, wndRect.right - wndRect.left, wndRect.bottom - wndRect.top, true);
			break;
		case VK_RIGHT:
			GetWindowRect(hwnd, &wndRect);
			MoveWindow(hwnd, wndRect.left + 20, wndRect.top, wndRect.right - wndRect.left, wndRect.bottom - wndRect.top, true);
			break;
		}
		break;

	case WM_LBUTTONDOWN:
	{
		GetWindowRect(hwnd, &wndRect);
		SetWindowPos(hwnd, NULL, NULL, NULL, wndRect.right - wndRect.left + 10, wndRect.bottom - wndRect.top + 10, SWP_NOZORDER | SWP_NOMOVE);
	}
	break;
	case WM_RBUTTONDBLCLK:
		ShowWindow(hwnd2, SW_SHOWNORMAL);
		break;
	case WM_LBUTTONDBLCLK:
		ShowWindow(hwnd2, SW_SHOWNORMAL);
		break;
	case WM_DESTROY:
		DeleteObject((HBRUSH)SetClassLong(hwnd, GCL_HBRBACKGROUND, (LONG)GetStockObject(WHITE_BRUSH)));
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	RECT wndRect;
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		GetWindowRect(hwnd, &wndRect);
		SetWindowPos(hwnd, NULL, NULL, NULL, wndRect.right - wndRect.left + 10, wndRect.bottom - wndRect.top + 10, SWP_NOZORDER | SWP_NOMOVE);
		break;
	case WM_RBUTTONDOWN:
		GetWindowRect(hwnd, &wndRect);
		SetWindowPos(hwnd, NULL, NULL, NULL, wndRect.right - wndRect.left - 10, wndRect.bottom - wndRect.top - 10, SWP_NOZORDER | SWP_NOMOVE);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
