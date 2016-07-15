#pragma once
#include "message.h"
#include "Chart.h"
#include "Chart.hpp"
#include "ColorLabel.h"

class ScanWindow
{
public:
	typedef ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxes
		, LineSeries
		, Grid
	>::Result>	TChart;
	HWND hWnd;
	TChart chart;
	Gdiplus::Bitmap *backScreen;	
	int offset;
	int maxX, maxY;
	double data[512];
	TMouseMove storedMouseMove;
	ColorLabel label;
	Cursor cursor;
	bool mouseMove;
	ScanWindow();
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TMouseWell &);
	void operator()(TMouseMove &);
	void operator()(TLButtonDown &);
	void operator()(TLButtonDbClk &);

	void Open(int zone, int sensor, int offset_, wchar_t *mess, unsigned char(&)[512], int countSamples, int maxY);
	bool CursorDraw(TMouseMove &, VGraphics &);
};
