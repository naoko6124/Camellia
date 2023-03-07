#pragma once

#include <Windows.h>

namespace Camellia
{
	class Window
	{
	public:
		void Init()
		{
			WNDCLASSW wc = { 0 };

			wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hInstance = NULL;
			wc.lpszClassName = (LPCWSTR)"WcCamellia";
			wc.lpfnWndProc = WndProc;
			wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

			if (!RegisterClassW(&wc))
				return;

			RECT rect;
			rect.top = 100;
			rect.bottom = 700;
			rect.left = 100;
			rect.right = 900;

			AdjustWindowRect(&rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE, FALSE);

			hWnd = CreateWindowW(
				(LPCWSTR)"WcCamellia",
				(LPCWSTR)"Camellia",
				WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE,
				CW_USEDEFAULT, CW_USEDEFAULT,
				rect.right - rect.left, rect.bottom - rect.top,
				NULL, NULL, NULL, NULL
			);
		}

		void GetEvents()
		{
			MSG msg;
			if (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					isRunning = false;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			switch (uMsg)
			{
			case WM_DESTROY:
				PostQuitMessage(0);
				break;
			}
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	public:
		bool isRunning = true;
		HWND hWnd;
	};
}