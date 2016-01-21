#pragma once

#include <windows.h>
#include "message.h"
#include "IOportsViewer.h"
class IOportsWindow
{
public:
	static const int width = 400;
	static const int height = 200;
	Gdiplus::Bitmap *backScreen;
	IOportsViewer viewer;
public:
	HWND hWnd;
	IOportsWindow();
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TNotify &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
	unsigned operator()(TMessage &);
	void operator()(TPaint &);
	void operator()(TLButtonDown &);
	static IOportsWindow &Instance();
};

