#pragma once
#include "message.h"
#include "Chart.h"
#include "Chart.hpp"
#include "ColorLabel.h"
#include "BarSeries.h"
#include "uspc7100_exports.h"

class LineViewer
{
	Gdiplus::Bitmap *backScreen;
public:
	HWND hWnd;
	typedef ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxes
		//, LineSeries		
		, BarSeries
		, Grid	
	>::Result> TChart;
	TChart chart;
	ColorLabel label;
	Cursor cursor;
	int count;
	double *data;
	USPC7100_ASCANDATAHEADER *scan;
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
	void SetData(double *, USPC7100_ASCANDATAHEADER *, int);
};