#pragma once
#include "message.h"
#include "ColorLabel.h"
#include "Chart.h"
#include "Chart.hpp"
#include "BarSeries.h"
#include "FixedGridSeries.h"
#include "GridChart.h"
#include "Common.h"
#include "USPCData.h"

class ThicknessViewer
{
public:
	typedef ChartDraw<Chart, TL::MkTlst<
		NoOffsetLeftAxes
		, BottomAxesGrid
		, BarSeriesDouble
		, FixedGrid		
	>::Result> TChart;
	TChart chart;
	bool mouseMove;
	Gdiplus::Bitmap *backScreen;
	ColorLabel label;
	Cursor cursor;
	bool painting;
public:
	HWND hWnd;
	TMouseMove storedMouseMove;
	bool openDetailedWindow;
	ItemData<Thickness> &viewerData;
	ThicknessViewer();
	unsigned operator()(TCreate &);
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TMouseMove &);
	void operator()(TLButtonDbClk &);
	void operator()(TMouseWell &);
	void operator()(TLButtonDown &l);
	void operator()(TRButtonDown &);
	void operator()(TDestroy &);

	bool Draw(TMouseMove &, VGraphics &);
	bool GetColorBar(int , double &, unsigned &, double &, unsigned &);
};

