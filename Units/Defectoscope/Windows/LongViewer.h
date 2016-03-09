#pragma once
#include "message.h"
#include "Chart.h"
#include "Chart.hpp"
#include "GridChart.h"
#include "ColorLabel.h"
#include "BarSeries.h"
#include "FixedGridSeries.h"
#include "USPCData.h"

class LongViewer
{
public:
	HWND hWnd;
public:
	typedef ChartDraw<Chart, TL::MkTlst<
		FixedLeftAxes
		, BottomAxesGrid
		, FixedGridSeries
		, FixedGrid		
	>::Result> TChart;
	Gdiplus::Bitmap *backScreen;
	TChart chart;
	bool mouseMove;
public:
	TMouseMove storedMouseMove;
	ColorLabel label;
public:
	Cursor cursor;
	ItemData<Long> &viewerData;
public:
	LongViewer();
	unsigned operator()(TCreate &);
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TMouseMove &);
	void operator()(TLButtonDbClk &);
	void operator()(TMouseWell &);
	void operator()(TLButtonDown &);
	void operator()(TRButtonDown &);
	//void operator()(TDestroy &);

	bool LongViewer::Draw(TMouseMove &, VGraphics &);
	bool LongViewer::GetColorBar(unsigned , int , double &, unsigned &);
};