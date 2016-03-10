#include "stdafx.h"
#include "ResultViewer.h"
#include <stdio.h>
#include "EmptyWindow.h"
#include "LabelMessage.h"
#include "DebugMess.h"
//------------------------------------------------------------------------------------------------------
using namespace Gdiplus;

bool ResultViewer::Draw(TMouseMove &l, VGraphics &g)
{
	/*
	char buf[512];
	double dX = (double)(chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight) / (chart.maxAxesX - chart.minAxesX);
	double x = -0.5 * dX + chart.rect.left + chart.offsetAxesLeft;
	int leftOffs = int((double(l.x) - x) / dX);
    dprint("leftOffs %d ", leftOffs);
	int status =  Singleton<ThicknessViewerData>::Instance().status[leftOffs];
	if(status)
	{		
		char *txt = StatusLabel(status).text;
		if(status == PrimaryData::Nominal || status == PrimaryData::Defect || status == PrimaryData::Treshold2Class)// || status == PrimaryData::DefectMinMax || status == PrimaryData::DefectDifferentWall)
		{
			double yMin = Singleton<ThicknessViewerData>::Instance().zonesMin[leftOffs];////////////////////////////////////////////////////
			double yMax = Singleton<ThicknessViewerData>::Instance().zonesMax[leftOffs];
			double delta = yMax - yMin;
			sprintf(buf, "<ff>Зона <0xff0000>%d <ff>Смещение <ff0000>%.2f <ff>м толщ.мин. <0xff0000>%0.2f <ff>толщ.макс. <0xff0000>%0.2f %s <ff>Толщ.класс 2 %.2f  <ff>Толщ.брак %.2f%"
				, 1 + leftOffs
				, 0.001 * zone_length * (1 + leftOffs)
				, yMin
				, yMax
				, txt
				, Singleton<ThresholdsTable>::Instance().items.get<Border2Class>().value
				, Singleton<ThresholdsTable>::Instance().items.get<BorderDefect>().value
				);
		}
		else
		{
			sprintf(buf, "<ff>Зона <ff0000>%d  %s  "
				, 1 + leftOffs
				, txt
				);
		}
		RECT r;
		GetClientRect(l.hwnd, &r);
		label.left = 40;
		label = buf;
		label.Draw(g());
		return true;
	}
	
	return false;
	*/
//////////////////////////////////test
	int x, y;
	chart.CoordCell(l.x, l.y, x, y);	
	wsprintf(label.buffer, L"<ff>���� %d         ", 1 + x);
	label.Draw(g());

	return x < viewerData.currentOffset;
	//////////////////////////////////////////test
}

bool ResultViewer::GetColorBar(int zone, double &data, unsigned &color)
{
	data = 0.9;
	ColorBar(
		data
		, color
		, viewerData.commonStatus[zone - 1]
	    , data
		);

	return zone <= viewerData.currentOffset;
}
//-----------------------------------------------------------------------------
ResultViewer::ResultViewer()
	: backScreen(NULL)
	, chart(backScreen)
	, cursor(chart)
	, openDetailedWindow(false)
	, painting(true)
	, mouseMove(true)
	, viewerData(Singleton<ResultViewerData>::Instance())
{
	storedMouseMove.x = 0;
	storedMouseMove.y = 0;
	chart.rect.top = 17;
	
	chart.minAxesX = 0;
	chart.maxAxesX = App::zonesCount;
	chart.minAxesY = 0;

	label.fontHeight = 12;
	label.top = 0;	

	cursor.SetMouseMoveHandler(this, &ResultViewer::Draw);
	chart.items.get<BarSeries>().SetColorBarHandler(this, &ResultViewer::GetColorBar);

	chart.items.get<BottomAxesMeters__>().minBorder = 0;
	chart.items.get<BottomAxesMeters__>().maxBorder = 0.001 * App::zonesCount * App::zone_length;
}
//----------------------------------------------------------------------------------------------------
#pragma warning(disable : 4996)
void ResultViewer::operator()(TSize &l)
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
	g.FillRectangle(&solidBrush, 0, 29, 10, l.Height);   
	g.FillRectangle(&solidBrush, 0, 0, l.Width, 29);  

//	chart.minAxesY = Singleton<BorderCredibilityTable>::Instance().items.get<MinimumThicknessPipeWall>().value;
//	chart.maxAxesY = Singleton<BorderCredibilityTable>::Instance().items.get<MaximumThicknessPipeWall>().value;
	chart.rect.right = l.Width;
	chart.rect.bottom = l.Height;
//	label.Draw(g);
	chart.Draw(g);
	//storedMouseMove.hwnd = l.hwnd;
}
//----------------------------------------------------------------------------------------------------
void ResultViewer::operator()(TPaint &l)
{
	if(NULL == backScreen) return;
	PAINTSTRUCT p;
	HDC hdc = BeginPaint(l.hwnd, &p);
	{		
		Graphics g(hdc);		
		g.DrawCachedBitmap(&CachedBitmap(backScreen, &g), 0, 0);
		cursor.VerticalCursor(storedMouseMove, PaintGraphics(g));
	}
	EndPaint(l.hwnd, &p);
}
//-----------------------------------------------------------------------------------------------------
void ResultViewer::operator()(TMouseMove &l)
{
	if(mouseMove)
	{
		if(cursor.VerticalCursor(l, HDCGraphics(l.hwnd, backScreen)))
		{
			storedMouseMove = l;
		}
	}
}
//----------------------------------------------------------------------------------------------------
void ResultViewer::operator()(TLButtonDown &l)
{
	mouseMove = false;
}
//--------------------------------------------------------------------------
void ResultViewer::operator()(TLButtonDbClk &l)
{
	mouseMove = true;
	if(cursor.VerticalCursor(*(TMouseMove *)&l, HDCGraphics(l.hwnd, backScreen)))
	{
		storedMouseMove.x = l.x;
	}
}
void ResultViewer::operator()(TMouseWell &l)
{
		mouseMove = false;

		chart.OffsetToPixelHorizontal(storedMouseMove.x, l.delta / 120);
		cursor.VerticalCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));
}
//------------------------------------------------------------------------------------------------------
unsigned ResultViewer::operator()(TCreate &l)
{
	storedMouseMove.hwnd = l.hwnd;
	storedMouseMove.x = 0;	
	storedMouseMove.y = WORD(chart.rect.top + 1);
	return 0;
}
void ResultViewer::operator()(TDestroy &m)
{
	delete backScreen;
    backScreen = NULL;
	SetWindowLongPtr(m.hwnd, GWLP_USERDATA, NULL);
}
