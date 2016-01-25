#pragma once
#include "message.h"
#include "ColorLabel.h"
#include "Chart.h"
#include "ResultData.h"
#include "Chart.hpp"
#include "BarSeries.h"
#include "FixedGridSeries.h"
#include "EmptyLeftAxes.h"

class ResultViewer
{
	class BottomAxesMeters__: public BottomAxesMeters
	{
	public:
		BottomAxesMeters__(Chart &c): BottomAxesMeters(c, App::zone_length){}
	};
public:
	typedef ChartDraw<Chart, TL::MkTlst<
		EmptyLeftAxes
		, BottomAxesMeters__
		, BarSeries
		, FixedGrid		
	>::Result> TChart;
	struct CursorLabel
	{
		ResultViewer &owner;
		ColorLabel &label;
		Cursor &cursor;
		TChart &chart;
		CursorLabel(ResultViewer &);
		bool Draw(TMouseMove &, VGraphics &);
		bool GetColorBar(int , double &, unsigned &);
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
	ResultViewerData &viewerData;
	ResultViewer();
	unsigned operator()(TCreate &);
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TMouseMove &);
	void operator()(TLButtonDbClk &);
	void operator()(TMouseWell &);
	void operator()(TKeyDown &l);
	void operator()(TLButtonDown &l);
	void Repaint();
};

