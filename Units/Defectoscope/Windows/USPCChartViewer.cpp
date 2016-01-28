#include "stdafx.h"
#include "USPCChartViewer.h"
#include "EmptyWindow.h"
#include "DebugMess.h"
#include "ConstData.h"
#include "App.h"

using namespace Gdiplus;
//-----------------------------------------------------------------
USPCChartViewer::CursorLabel::CursorLabel(USPCChartViewer &o)
	: owner(o)
	, label(o.label)
	, cursor(o.cursor)
	, chart(o.chart)
{
	label.fontHeight = 10;
	label.top = 2;	
}

bool USPCChartViewer::CursorLabel::Draw(TMouseMove &l, VGraphics &g)
{
	//int x, y;
	//chart.CoordCell(l.x, l.y, x, y);	
	//wsprintf(label.buffer, L"<ff>зона %d  датчик %d        ", 1 + x, 1 + y);
	//label.Draw(g());
	//
	//return x < owner.viewerData.currentOffset;
	return true;
}

bool USPCChartViewer::CursorLabel::GetColorBar(unsigned sensor, int zone, double &data, unsigned &color)
{
	//data = owner.viewerData.buffer[sensor][zone];
	//color = ConstData::ZoneColor(owner.viewerData.status[sensor][zone]);
	//return zone < owner.viewerData.currentOffset;
	return true;
}
//------------------------------------------------------------------
USPCChartViewer::USPCChartViewer()
	: backScreen(NULL)
	, chart(backScreen)
	, cursor(chart)
	, cursorLabel(*this)
{
	
	chart.items.get<FixedGridSeries>().sensorCount = App::maxSensorCrossCount;
	chart.minAxesY = 1;
	chart.maxAxesY = 1 + App::maxSensorCrossCount;
	chart.minAxesX = 0;
	chart.maxAxesX = App::zonesCount;
//	cursor.SetMouseMoveHandler(&cursorLabel, &LongViewer::CursorLabel::Draw);
	label.fontHeight = 12;
	label.top = 0;
	chart.rect.top = 17;
	mouseMove = true;
//	chart.items.get<FixedGridSeries>().SetColorCellHandler(&cursorLabel, &LongViewer::CursorLabel::GetColorBar);
}
//--------------------------------------------------------------------------
void USPCChartViewer::operator()(TSize &l)
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
//	label.Draw(g);
	chart.Draw(g);

}
//-----------------------------------------------------------------------
void USPCChartViewer::operator()(TPaint &l)
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
//------------------------------------------------------------------------
void USPCChartViewer::operator()(TMouseMove &l)
{
	if(mouseMove)
	{
		if(cursor.CrossCursor(l, HDCGraphics(l.hwnd, backScreen)))
		{
			storedMouseMove = l;
		}
	}
}
//------------------------------------------------------------------------------
void USPCChartViewer::operator()(TLButtonDbClk &l)
{
	 mouseMove = true;
	if(cursor.CrossCursor(*(TMouseMove *)&l, HDCGraphics(l.hwnd, backScreen)))
	{
		storedMouseMove.x = l.x;
	}
}
//--------------------------------------------------------------------------------
void USPCChartViewer::operator()(TMouseWell &l)
{
	RECT r;
	GetWindowRect(l.hwnd, &r);
	if(InRect(l.x, l.y, r))
	{
		mouseMove = false;
	
		chart.items.get<FixedGridSeries>().OffsetToPixel(
			storedMouseMove.x
			, storedMouseMove.y
			, l.delta / 120
			, 0 == l.flags.lButton 
			);
		cursor.CrossCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));
		//zprint("~~~~~~~~x %d y %d %d\n", l.x, l.y, l.delta / 120);
	}
}
//--------------------------------------------------------------------------------------
void USPCChartViewer::operator()(TLButtonDown &l)
{
	mouseMove = false;
}
//-----------------------------------------------------------------------------------------
void USPCChartViewer::Update()
{
	RepaintWindow(hWnd);
}
//------------------------------------------------------------------------------------------------
unsigned USPCChartViewer::operator()(TCreate &l)
{
	storedMouseMove.hwnd = l.hwnd;
	storedMouseMove.x = 0;	
	storedMouseMove.y = WORD(chart.rect.top + 1);
	return 0;
}
