#pragma once
#include <GdiPlus.h>
#include "message.h"
class IOportsViewer
{
	HANDLE hTimer;
public:
	HWND &hWnd;
	Gdiplus::Bitmap *&backScreen;
public:
	IOportsViewer(HWND &, Gdiplus::Bitmap *&);
	void Size(Gdiplus::Graphics &, int, int);
	void Start();
	void Stop();
	void MouseLDown(TLButtonDown &);
};