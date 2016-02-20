#pragma once

#include "message.h"
#include "Chart.h"
#include "Chart.hpp"
#include "GridChart.h"
#include "ColorLabel.h"
#include "BarSeries.h"
#include "FixedGridSeries.h"
#include "StepSeries.h"
#include "App.h"

class TresholdWindow
{
public:
	HWND hWnd;
	typedef ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxesGrid
		//, FixedGridSeries
		, StepSeries
		, FixedGrid
	>::Result>	TChart;
	TChart chart;
	Gdiplus::Bitmap *backScreen;	
	bool mouseMove;
	TMouseMove storedMouseMove;
	ColorLabel label;
	Cursor cursor;
	double border[3][App::zonesCount];
public:
	TresholdWindow();
	unsigned operator()(TCreate &);
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TMouseMove &);
	void operator()(TLButtonDbClk &);
	void operator()(TMouseWell &);
	void operator()(TLButtonDown &);
	void operator()(TKeyDown &);

	bool Draw(TMouseMove &, VGraphics &);
	static wchar_t *Title();
};