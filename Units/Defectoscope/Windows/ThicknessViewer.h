#pragma once
#include "message.h"
#include "ColorLabel.h"
#include "Chart.h"
//#include "ThicknessData.h"
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
	struct CursorLabel
	{
		ThicknessViewer &owner;
		ColorLabel &label;
		Cursor &cursor;
		TChart &chart;
		CursorLabel(ThicknessViewer &);
		bool Draw(TMouseMove &, VGraphics &);
		bool GetColorBar(int , double &, unsigned &, double &, unsigned &);
	};
	TChart chart;
	bool mouseMove;
	Gdiplus::Bitmap *backScreen;
	ColorLabel label;
	Cursor cursor;
	CursorLabel cursorLabel;	
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
	void operator()(TKeyDown &l);
	void operator()(TLButtonDown &l);
	void operator()(TRButtonDown &);
	void Repaint();
};

