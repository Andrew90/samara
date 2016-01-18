#pragma once
#include "message.h"
#include "Chart.h"
#include "Chart.hpp"
#include "GridChart.h"
#include "ColorLabel.h"
#include "BarSeries.h"
#include "FixedGridSeries.h"
class CrossViewerData;
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
	struct CursorLabel
	{
		CrossViewer &owner;
		ColorLabel &label;
		Cursor &cursor;
		TChart &chart;
		CursorLabel(CrossViewer &);
		bool Draw(TMouseMove &l, VGraphics &g);
		bool GetColorBar(unsigned sensor, unsigned zone, double &data, unsigned &color);
	};
	TChart chart;
	Gdiplus::Bitmap *backScreen;	
	bool mouseMove;
	TMouseMove storedMouseMove;
	ColorLabel label;
	Cursor cursor;
	CursorLabel cursorLabel;
public:
	CrossViewerData &viewerData;
	CrossViewer();
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TMouseMove &);
	void operator()(TLButtonDbClk &);
	void operator()(TMouseWell &);
	void operator()(TLButtonDown &);
	void Repaint();
};