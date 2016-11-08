#pragma once
#include "message.h"
#include "Chart.h"
#include "Chart.hpp"
#include "ColorLabel.h"
#include "ScanWindowToolBar.h"
#include "USPCData.h"
#include "Gate.h"

class ScanWindow
{
	void *owner;
	void (*ptrScan)(int, int, int, void *, void(*)());
public:
	class GateIF: public Gate
	{
	public:
		bool visible;
		GateIF(Chart &);
		void Draw();
	};
	class Gate1: public Gate
	{
	public:
		bool visible;
		Gate1(Chart &);
		void Draw();
	};
public:
	typedef ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxes
		, LineSeries
		, Grid
		, GateIF
		, Gate1
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
	bool mouseMove;
	ScanWindow();
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TLButtonDown &);

	void Open(int zone, int sensor, int offset_, wchar_t *mess, wchar_t *mess1, USPC7100_ASCANDATAHEADER *data, void *, void(*)());
	bool CursorDraw(TMouseMove &, VGraphics &);

	void SensPlus();
	void SensMinus();
	void OffsPlus();
	void OffsMinus();
};
