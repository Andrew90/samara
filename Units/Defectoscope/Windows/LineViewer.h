#pragma once
#include "message.h"
#include "Chart.h"
#include "Chart.hpp"

class LineViewer
{
	Gdiplus::Bitmap *backScreen;
public:
	HWND hWnd;
	LineViewer();
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TCommand &);
	unsigned operator()(TCreate &);
	void operator()(TMouseWell &);
};