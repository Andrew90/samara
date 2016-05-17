#pragma once
#include "message.h"
#include "Chart.h"
#include "Chart.hpp"
#include "ColorLabel.h"
#include "DataViewer.h"

template<class T>class ScanWindow: public T
{
public:
	TMouseMove storedMouseMove;
	ColorLabel label;
	Cursor cursor;
	ScanWindow()
	{
		chart.maxAxesY = 0;
		chart.maxAxesY = 1024;
		chart.minAxesX = 0;
		chart.maxAxesX = 1024;
	}
	void operator()(TSize &l)
	{
		if(l.resizing == SIZE_MINIMIZED || 0 == l.Width || 0 == l.Height) return;	

		if(NULL != backScreen)
		{
			if(backScreen->GetWidth() < l.Width || backScreen->GetHeight() < l.Height)
			{
				delete backScreen;
				backScreen = new Bitmap(l.Width, l.Height);
			}
		}
		else if(l.Width > 0 && l.Height > 0)
		{
			backScreen = new Bitmap(l.Width, l.Height);
		}
		else
		{
			return;
		}
		Graphics g(backScreen);
		SolidBrush solidBrush(Color(0xffaaaaaa));
		g.FillRectangle(&solidBrush, 0, 0, 10, l.Height);   
		g.FillRectangle(&solidBrush, 0, 0, l.Width, 29);  


		chart.rect.right = l.Width;
		chart.rect.bottom = l.Height;
		chart.Draw(g);
	}
	void operator()(TPaint &)
	{
		if(NULL == backScreen) return;
		PAINTSTRUCT p;
		HDC hdc = BeginPaint(l.hwnd, &p);
		{		
			Graphics g(hdc);		
			g.DrawCachedBitmap(&CachedBitmap(backScreen, &g), 0, 0);
			cursor.CrossCursor(storedMouseMove, PaintGraphics(g));
		}
		EndPaint(l.hwnd, &p);
	}
	void operator()(TCommand &l)
	{
		EventDo(l);
	}
	void operator()(TGetMinMaxInfo &l)
	{
		if(NULL != l.pMinMaxInfo)
	{
		l.pMinMaxInfo->ptMinTrackSize.x = 300;
		l.pMinMaxInfo->ptMinTrackSize.y = 100;
	}	
	}
	unsigned operator()(TCreate &l)
	{
	}
	void operator()(TMouseWell &l)
	{
	}
};

class CrossData
{
public:
	typedef ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxes
		, LineSeries
		, Grid
	>::Result>	TChart;
	DefectData data;
	TChart chart;
	Gdiplus::Bitmap *backScreen;	
	CrossData()
		: chart(backScreen)
	{}
};

