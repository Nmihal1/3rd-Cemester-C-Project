// Framework.cpp : Defines the entry point for the application.


#include "Framework.h"
#include "Winuser.h"
#include <iostream>
#include "time.h"
#include "Rasteriser.h"


// Used to access the message handler properly
Framework* _thisFramework = NULL;

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

INPUT ip;

// The entry point for a Windows application
int APIENTRY wWinMain(_In_	   HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_	   LPWSTR    lpCmdLine,
	_In_	   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (_thisFramework)
	{
		return _thisFramework->Run(hInstance, nCmdShow);
	}
	return -1;
}


Framework::Framework()
{
	_thisFramework = this;
}

Framework::~Framework()
{
}

float timer = 7000.0f;
void KeyInputExample()
{


	if (clock() < 0.7 * timer)
	{
		GetAsyncKeyState(VK_ESCAPE);
		keybd_event(VK_ESCAPE, 0x1B, NULL, NULL);
	}

	else if (clock() > 0.7 * timer && clock() < 0.9 * timer)
	{
		GetAsyncKeyState(VK_SPACE);
		keybd_event(VK_SPACE, 0x20, NULL, NULL);
		GetAsyncKeyState(VK_DIVIDE);
		keybd_event(VK_DIVIDE, 0x6F, NULL, NULL);
	}


	else if (clock() > 0.9 * timer && clock() < 2 * timer)
	{
		GetAsyncKeyState(VK_RIGHT);
		keybd_event(VK_RIGHT, 0x27, NULL, NULL);
	}

	else if (clock() > 2 * timer && clock() < 3 * timer)
	{
		GetAsyncKeyState(VK_OEM_PLUS);
		keybd_event(VK_OEM_PLUS, 0xBB, NULL, NULL);
	}


	else if (clock() > 3 * timer && clock() < 4.2 * timer)
	{
		GetAsyncKeyState(VK_UP);
		keybd_event(VK_UP, 0x26, NULL, NULL);
		GetAsyncKeyState(VK_OEM_MINUS);
		keybd_event(VK_OEM_MINUS, 0x1B, NULL, NULL);

	}

	else if (clock() > 4.2 * timer && clock() < 5 * timer)
	{
		GetAsyncKeyState(VK_DOWN);
		keybd_event(VK_DOWN, 0x28, NULL, NULL);
	}

	else if (clock() > 5 * timer && clock() < 6 * timer)
	{
		GetAsyncKeyState(VK_LEFT);
		keybd_event(VK_LEFT, 0x25, NULL, NULL);
	}




}
void KeyInput() //Simulating the User's Inputs
{

	if (clock() < 1 * timer)
	{
		GetAsyncKeyState(VK_INSERT);
		keybd_event(VK_INSERT, 0x2D, NULL, NULL);
	}


	else if (clock() > 1 * timer && clock() < 1.5 * timer)
	{
		GetAsyncKeyState(VK_LBUTTON);
		keybd_event(VK_LBUTTON, 0x01, NULL, NULL);
	}

	else if (clock() > 1.5 * timer && clock() < 2 * timer)
	{
		GetAsyncKeyState(VK_RBUTTON);
		keybd_event(VK_RBUTTON, 0x02, NULL, NULL);
	}

	else if (clock() > 2 * timer && clock() <= 3 * timer)
	{
		GetAsyncKeyState(VK_DIVIDE);
		keybd_event(VK_DIVIDE, 0x6F, NULL, NULL);
	}
	else if (clock() > 3 && clock() <= 4 * timer)
	{
		GetAsyncKeyState(VK_LMENU);
		keybd_event(VK_LMENU, 0xA4, NULL, NULL);

	}

	else if (clock() > 4 * timer && clock() <= 5 * timer)
	{
		GetAsyncKeyState(VK_CANCEL);
		keybd_event(VK_CANCEL, 0x03, NULL, NULL);
	}

	else if (clock() > 5 * timer && clock() <= 6 * timer)
	{
		GetAsyncKeyState(VK_MBUTTON);
		keybd_event(VK_MBUTTON, 0x04, NULL, NULL);

	}

	else if (clock() > 6 * timer && clock() <= 7 * timer)
	{
		GetAsyncKeyState(VK_BACK);
		keybd_event(VK_BACK, 0x08, NULL, NULL);

	}

	else if (clock() > 7 * timer && clock() <= 8 * timer)
	{
		GetAsyncKeyState(VK_TAB);
		keybd_event(VK_TAB, 0x09, NULL, NULL);

	}

	else if (clock() > 9 * timer && clock() <= 10 * timer)
	{
		GetAsyncKeyState(VK_DOWN);
		keybd_event(VK_DOWN, 0x28, NULL, NULL);

	}

	else if (clock() > 10 * timer && clock() <= 11 * timer)
	{
		GetAsyncKeyState(VK_LEFT);
		keybd_event(VK_LEFT, 0x25, NULL, NULL);

	}

	else if (clock() > 11 * timer && clock() <= 12 * timer)
	{
		GetAsyncKeyState(VK_UP);
		keybd_event(VK_UP, 0x26, NULL, NULL);

	}


	else if (clock() > 12 * timer && clock() <= 13 * timer)
	{
		GetAsyncKeyState(VK_RIGHT);
		keybd_event(VK_RIGHT, 0x27, NULL, NULL);

	}


	else if (clock() > 13 * timer && clock() <= 14 * timer)
	{
		GetAsyncKeyState(VK_CAPITAL);
		keybd_event(VK_CAPITAL, 0x14, NULL, NULL);

	}


	else if (clock() > 15 * timer && clock() <= 16 * timer)
	{
		GetAsyncKeyState(VK_ESCAPE);
		keybd_event(VK_HOME, 0x1B, NULL, NULL);

	}

	else if (clock() > 16 * timer && clock() <= 17 * timer)
	{
		GetAsyncKeyState(VK_OEM_PLUS);
		keybd_event(VK_OEM_PLUS, 0xBB, NULL, NULL);

	}

	else if (clock() > 17 * timer && clock() <= 18 * timer)
	{
		GetAsyncKeyState(VK_OEM_MINUS);
		keybd_event(VK_OEM_MINUS, 0x1B, NULL, NULL);

	}

}

int Framework::Run(HINSTANCE hInstance, int nCmdShow)
{
	_hInstance = hInstance;
	if (!InitialiseMainWindow(nCmdShow))
	{
		return -1;
	}
	return MainLoop();

}

/*void Framework::Update()
{
	float timer = 7000.0f;
	if (clock() < 0.7 * timer)
	{
		Values(1);
		//_facecull = true;

	}

	else if (clock() > 0.7 * timer && clock() < 0.9 * timer)
	{
		Values(0);
		//_facecull = false;
	}


	else if (clock() > 0.9 * timer && clock() < 2 * timer)
	{
		Values(11);
		//_depthsort = true;
	}

	else if (clock() > 2 * timer && clock() < 3 * timer)
	{
		Values(17);
	}


	else if (clock() > 3 * timer && clock() < 4.2 * timer)
	{
		GetAsyncKeyState(VK_UP);
		keybd_event(VK_UP, 0x26, NULL, NULL);
		GetAsyncKeyState(VK_OEM_MINUS);
		keybd_event(VK_OEM_MINUS, 0x1B, NULL, NULL);

	}

	else if (clock() > 4.2 * timer && clock() < 5 * timer)
	{
		GetAsyncKeyState(VK_DOWN);
		keybd_event(VK_DOWN, 0x28, NULL, NULL);
	}

	else if (clock() > 5 * timer && clock() < 6 * timer)
	{
		GetAsyncKeyState(VK_LEFT);
		keybd_event(VK_LEFT, 0x25, NULL, NULL);
	}

}*/

int Framework::MainLoop()
{
	MSG msg;

	HACCEL hAccelTable = LoadAccelerators(_hInstance, MAKEINTRESOURCE(IDC_RASTERISER));

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	}
	return (int)msg.wParam;

}

bool Framework::InitialiseMainWindow(int nCmdShow)
{
#define MAX_LOADSTRING 100

	WCHAR windowTitle[MAX_LOADSTRING];
	WCHAR windowClass[MAX_LOADSTRING];

	LoadStringW(_hInstance, IDS_APP_TITLE, windowTitle, MAX_LOADSTRING);
	LoadStringW(_hInstance, IDC_RASTERISER, windowClass, MAX_LOADSTRING);

	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = _hInstance;
	wcex.hIcon = LoadIcon(_hInstance, MAKEINTRESOURCE(IDI_RASTERISER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = windowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	if (!RegisterClassExW(&wcex))
	{
		return false;
	}

	_hWnd = CreateWindowW(windowClass,
		windowTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
		nullptr, nullptr, _hInstance, nullptr);
	if (!_hWnd)
	{
		return false;
	}

	//Add code on Window Initialization

	InitializeScene(_hWnd);

	//Show Window

	ShowWindow(_hWnd, nCmdShow);
	UpdateWindow(_hWnd);

	return true;
}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (_thisFramework != NULL)
	{
		// If framework is started, then we can call our own message proc
		return _thisFramework->MsgProc(hWnd, message, wParam, lParam);
	}
	else
	{
		// otherwise, we just pass control to the default message proc
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

LRESULT Framework::MsgProc(HWND hWnd, UINT message, WPARAM value, LPARAM lParam)
{


	const int TIMER = 1;
	switch (message)
	{
	case WM_CREATE:
		UINT u;
		u = SetTimer(hWnd, TIMER, 60, NULL);
		break;


	case WM_ERASEBKGND:
		return 1;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		RenderScene(hdc);


		//KeyInput();
		//KeyInputExample();

		EndPaint(hWnd, &ps);


	}
	break;

	case WM_TIMER:
		RECT Client;
		Update(hWnd);
		InvalidateRect(hWnd, NULL, FALSE);
		break;

		/*case WM_SIZE:
			Update();
			InvalidateRect(hWnd, NULL, FALSE);
			break;*/
			// On mouse keys pressing
			/*case WM_LBUTTONDOWN:
				Values(2);
				InvalidateRect(hWnd, NULL, TRUE);
				break;
			case WM_RBUTTONDOWN:
				Values(3);
				InvalidateRect(hWnd, NULL, TRUE);
				break;

			case WM_LBUTTONUP:
				//OnLButtonUp(hWnd, lParam);
				break;

			case WM_MOUSEMOVE:
				//OnMouseMove(hWnd, lParam);
				break;*/


				// On keyboard keys pressing
	case WM_KEYDOWN:
	{
		switch (value)
		{
		case VK_SPACE:
			Values(0);
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case VK_ESCAPE:
			Values(1);
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case VK_LBUTTON:
			Values(2);
			InvalidateRect(hWnd, NULL, TRUE);
			break;

			/*case VK_RBUTTON:
				Values(3);
				InvalidateRect(hWnd, NULL, TRUE);
				break;*/

		case '/':
			Values(4);
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case ',':
			Values(5);
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case VK_CANCEL:
			Values(6);
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case VK_MBUTTON:
			Values(7);
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case VK_BACK:
			Values(8);
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case VK_TAB:
			Values(9);
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case VK_HOME:
			Values(10);
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case VK_DOWN:
			Values(11);
			InvalidateRect(hWnd, NULL, TRUE);
			break;


		case VK_UP:
			Values(12);
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case VK_RIGHT:
			Values(13);
			InvalidateRect(hWnd, NULL, TRUE);
			break;


		case VK_LEFT:
			Values(14);
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case VK_CAPITAL:
			Values(15);
			InvalidateRect(hWnd, NULL, TRUE);
			break;

			/*case VK_ESCAPE:
				Values(16);
				InvalidateRect(hWnd, NULL, TRUE);
				break;*/


		case VK_OEM_PLUS:
			Values(17);
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case VK_OEM_MINUS:
			Values(18);
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case '.':
			Values(19);
			InvalidateRect(hWnd, NULL, TRUE);
			break;

			//default:
				//break;

		//break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, message, value, lParam);
		}
		return 0;
	}
	}
}



void Framework::InitializeScene(HWND _hWnd)
{

}

void Framework::RenderScene(HDC hdc)
{
}

void Framework::Values(int action)
{
}
void Framework::Update(HWND hWnd)
{
}


