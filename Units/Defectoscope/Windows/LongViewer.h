#pragma once
#include "message.h"
#include "Chart.h"
#include "Chart.hpp"
#include "GridChart.h"
#include "ColorLabel.h"
#include "BarSeries.h"
#include "FixedGridSeries.h"
class LongViewerData;
class LongViewer
{
public:
	HWND hWnd;
private:	
	typedef ChartDraw<Chart, TL::MkTlst<
		FixedLeftAxes
		, BottomAxesGrid
		, FixedGridSeries
		, FixedGrid		
	>::Result> TChart;
	struct CursorLabel
	{
		friend class LongViewer;
		LongViewer &owner;
		ColorLabel &label;
		Cursor &cursor;
		TChart &chart;
		CursorLabel(LongViewer &);
		bool Draw(TMouseMove &l, VGraphics &g);
		bool GetColorBar(unsigned sensor, int zone, double &data, unsigned &color);
	};
	Gdiplus::Bitmap *backScreen;
	TChart chart;
	bool mouseMove;
public:
	TMouseMove storedMouseMove;
private:
	ColorLabel label;
	Cursor cursor;
	LongViewerData &viewerData;
	CursorLabel cursorLabel;
public:
	LongViewer();
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TMouseMove &);
	void operator()(TLButtonDbClk &);
	void operator()(TMouseWell &);
	void operator()(TLButtonDown &);
	void Update();
};