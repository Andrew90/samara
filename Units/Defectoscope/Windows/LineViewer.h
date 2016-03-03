#pragma once
#include "message.h"
#include "Chart.h"
#include "Chart.hpp"
#include "ColorLabel.h"
#include "BarSeries.h"
#include "Borders.h"
#include "App.h"

class LineViewer
{
public:
	Gdiplus::Bitmap *backScreen;
public:
	int offsetX;
public:
	HWND hWnd;
	struct Border2Class: HBorder{Border2Class(Chart &c): HBorder(c){}};
	struct BorderDefect: HBorder{BorderDefect(Chart &c): HBorder(c){}};
	typedef ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxes
		, BarSeries
		, Grid	
		, Border2Class
		, BorderDefect
	>::Result> TChart;
	TChart chart;
	ColorLabel label;
	Cursor cursor;
	TMouseMove storedMouseMove;
	bool mouseMove;
	LineViewer();
	bool CursorDraw(TMouseMove &l, VGraphics &g);
	void operator()(TSize &);
	void operator()(TPaint &);
	unsigned operator()(TCreate &);
	void operator()(TMouseWell &);
	void operator()(TMouseMove &);
	void operator()(TLButtonDown &);
	void operator()(TLButtonDbClk &);
	void operator()(TDestroy &);
};