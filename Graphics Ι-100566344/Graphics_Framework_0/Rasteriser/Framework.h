#pragma once

#include <windows.h>
#include "Resource.h"
#include "Model.h"
#include "Matrix_4D.h"

class Framework
{
public:
	Framework();
	virtual ~Framework();

	int Run(HINSTANCE hInstance, int nCmdShow);

	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual void InitializeScene(HWND _hWnd);
	virtual void RenderScene(HDC hdc);

	virtual void Update(HWND _hWnd);

	virtual void Values(int action);

	void DrawString(HDC hdc, int xPos, int yPos, int fSize, COLORREF textColor, LPCTSTR text);
	void DrawTextFrame(HDC hdc);
	void DrawViewportFrame(HDC hdc); // draw the viewport fram


private:
	HINSTANCE	_hInstance;
	HWND		_hWnd;

	bool InitialiseMainWindow(int nCmdShow);
	int MainLoop();

};