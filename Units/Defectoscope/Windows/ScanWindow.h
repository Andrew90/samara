#pragma once
#include "message.h"
#include "Chart.h"
#include "Chart.hpp"
#include "ColorLabel.h"
#include "ScanWindowToolBar.h"
#include "USPCData.h"

class RedLineSeries
{
	LineSeries line;
public:
	double buf[512];
	RedLineSeries(Chart &);
	void Draw();
};

class ScanWindow
{
	void *owner;
	void (*ptrScan)(int, int, int, void *, void(*)());
public:
	typedef ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxes
		, LineSeries
		, RedLineSeries
		, Grid
	>::Result>	TChart;
	HWND hWnd;
	TChart chart;
	Gdiplus::Bitmap *backScreen;	
	ScanWindowToolBar toolBar;
	int offset, zone, sensor, offsetInZone;
	int maxX, minY, maxY;
	int g1Tof;
    int g1Amp;
	double data[512];
	TMouseMove storedMouseMove;
	int currentX, currentY;
	Cursor cursor;
	int lengthMess;
	ColorLabel label;	
	bool mouseMove;
	ScanWindow();
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TLButtonDown &);

	void operator()(TMouseMove &);
	void operator()(TLButtonDbClk &);
	void operator()(TMouseWell &);

	void Open(int zone, int sensor, int offset_, wchar_t *mess, wchar_t *mess1, USPC7100_ASCANDATAHEADER *data, void *, void(*)());
	bool CursorDraw(TMouseMove &, VGraphics &);

	void SensPlus();
	void SensMinus();
	void OffsPlus();
	void OffsMinus();
};
