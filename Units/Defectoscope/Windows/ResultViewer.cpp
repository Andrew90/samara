#include "stdafx.h"
#include "ResultViewer.h"
#include <math.h>  
#include <stdio.h>
#include "EmptyWindow.h"
#include "LabelMessage.h"
#include "DebugMess.h"
//------------------------------------------------------------------------------------------------------
using namespace Gdiplus;

bool ResultViewer::Draw(TMouseMove &l, VGraphics &g)
{
	int left = chart.rect.left + chart.offsetAxesLeft;
	int right = chart.rect.right - chart.offsetAxesRight;
	int t = right - left;
	if(0 == t) return false;
	double dX = (double)t / App::count_zones;
	double dM = (double)t / (0.001 * App::count_zones * App::zone_length);
	int x = int((l.x - left) / dX);
	double m = (double)(l.x - left) / dM;
	m /= 0.05;
	m= ceil(m);
	m *= 0.05;
	bool drawZones =  x < viewerData.currentOffset;
	if(drawZones)
	{
	int color;
	bool b;
	char *s = StatusText()(viewerData.commonStatus[x], color, b);
	
	wsprintf(label.buffer, L"<ff>Результат смещение %s м. зона %d <%6x>%S"
		, Wchar_from<double, 3>(m)()
		, 1 + x
		, color
		, s
		);
	label.Draw(g());
	}
	return drawZones;
}

bool ResultViewer::GetColorBar(int zone, double &data, unsigned &color)
{
	data = 0.9;
	ColorBar()(
		data
		, color
		, viewerData.commonStatus[zone]
	    , data
		);

	return zone < viewerData.currentOffset;
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
	chart.maxAxesX = App::count_zones;
	chart.minAxesY = 0;

	label.fontHeight = 12;
	label.top = 0;	

	cursor.SetMouseMoveHandler(this, &ResultViewer::Draw);
	chart.items.get<BarSeries>().SetColorBarHandler(this, &ResultViewer::GetColorBar);

	chart.items.get<BottomAxesMeters__>().minBorder = 0;
	chart.items.get<BottomAxesMeters__>().maxBorder = 0.001 * App::count_zones * App::zone_length;
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
