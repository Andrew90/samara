#pragma once
#include "message.h"
#include "Chart.h"
#include "Chart.hpp"
#include "GridChart.h"
#include "ColorLabel.h"
#include "BarSeries.h"
#include "FixedGridSeries.h"
#include "USPCData.h"
class CrossViewer
{
public:
	HWND hWnd;
	typedef ChartDraw<Chart, TL::MkTlst<
		FixedLeftAxes
		, BottomAxesGrid
		, FixedGridSeries
		, FixedGrid
	>::Result>	TChart;
public:
	TChart chart;
	Gdiplus::Bitmap *backScreen;	
	bool mouseMove;
	TMouseMove storedMouseMove;
	ColorLabel label;
	Cursor cursor;
public:
	ItemData<Cross> &viewerData;
	CrossViewer();
	unsigned operator()(TCreate &);
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TMouseMove &);
	void operator()(TLButtonDbClk &);
	void operator()(TMouseWell &);
	void operator()(TLButtonDown &);
	void operator()(TRButtonDown &);

	bool Draw(TMouseMove &, VGraphics &);
	bool GetColorBar(unsigned , int , double &, unsigned &);
};