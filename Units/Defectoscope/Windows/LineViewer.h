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
	ColorLabel label;
	Chart *chart;
	Cursor *cursor;
public:
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
};

template<class T>struct Border: HBorder
{
	Border(Chart &c): HBorder(c){}
};

template<class Thresh>class LineTresholdsViewer: public LineViewer
{
public:	
	typedef LineViewer Parent;
	typedef typename ChartDraw<Chart, typename TL::MultyListToList<typename TL::MkTlst<
		TL::MkTlst<
		LeftAxes
		, BottomAxes
		, BarSeries
		, Grid	
		>::Result
		, typename TL::ListToWapperList<Thresh, Border>::Result
	>::Result>::Result> TChart;
private:
	TChart chartLoc;
	Cursor cursorLoc;
public:
	LineTresholdsViewer()
		: chartLoc(backScreen)
		, cursorLoc(chartLoc)
	{
		chart = &chartLoc;
		cursor = &cursorLoc;
	}
};