#pragma once
#include "message.h"
#include "Chart.h"
#include "Chart.hpp"
#include "ColorLabel.h"
#include "ScanWindowToolBar.h"
#include "USPCData.h"

class ScanWindow
{
	void (*ptrScan)(int, int, int, void(*)());
public:
	typedef ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxes
		, LineSeries
		, Grid
	>::Result>	TChart;
	HWND hWnd;
	TChart chart;
	Gdiplus::Bitmap *backScreen;	
	ScanWindowToolBar toolBar;
	int offset, zone, sensor, offsetInZone;
	int maxX, maxY;
	int g1Tof;
    int g1Amp;
	double data[512];
	TMouseMove storedMouseMove;
	ColorLabel label;
//	Cursor cursor;
	bool mouseMove;
	ScanWindow();
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	//void operator()(TMouseWell &);
	//void operator()(TMouseMove &);
	void operator()(TLButtonDown &);
	//void operator()(TLButtonDbClk &);

	void Open(int zone, int sensor, int offset_, wchar_t *mess, USPC7100_ASCANDATAHEADER *data/*, unsigned char(&)[512], int countSamples, int maxY*/, void(*)());
	bool CursorDraw(TMouseMove &, VGraphics &);

	void SensPlus();
	void SensMinus();
	void OffsPlus();
	void OffsMinus();
};
