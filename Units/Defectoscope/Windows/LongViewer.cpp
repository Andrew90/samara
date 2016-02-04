#include "stdafx.h"
#include "LongViewer.h"
#include "EmptyWindow.h"
#include "LongViewer.h"
#include "LongData.h"
#include "DebugMess.h"
#include "ConstData.h"

using namespace Gdiplus;
//-----------------------------------------------------------------
LongViewer::CursorLabel::CursorLabel(LongViewer &o)
	: owner(o)
	, label(o.label)
	, cursor(o.cursor)
	, chart(o.chart)
{
	label.fontHeight = 10;
	label.top = 2;	
}

bool LongViewer::CursorLabel::Draw(TMouseMove &l, VGraphics &g)
{
	int x, y;
	chart.CoordCell(l.x, l.y, x, y);	
	wsprintf(label.buffer, L"<ff>зона %d  датчик %d        ", 1 + x, 1 + y);
	label.Draw(g());

	return x < owner.viewerData.currentOffset;
}

bool LongViewer::CursorLabel::GetColorBar(unsigned sensor, int zone, double &data, unsigned &color)
{
	data = owner.viewerData.buffer[sensor][zone];
	color = ConstData::ZoneColor(owner.viewerData.status[sensor][zone]);
	return zone < owner.viewerData.currentOffset;
}
//------------------------------------------------------------------
LongViewer::LongViewer()
	: backScreen(NULL)
	, chart(backScreen)
	, cursor(chart)
	, viewerData(Singleton<LongViewerData>::Instance())
	, cursorLabel(*this)
{
	
	chart.items.get<FixedGridSeries>().sensorCount = App::maxSensorCrossCount;
	chart.minAxesY = 1;
	chart.maxAxesY = 1 + App::maxSensorCrossCount;
	chart.minAxesX = 0;
	chart.maxAxesX = App::zonesCount;
	cursor.SetMouseMoveHandler(&cursorLabel, &LongViewer::CursorLabel::Draw);
	label.fontHeight = 12;
	label.top = 0;
	chart.rect.top = 17;
	mouseMove = true;
	chart.items.get<FixedGridSeries>().SetColorCellHandler(&cursorLabel, &LongViewer::CursorLabel::GetColorBar);
}
//--------------------------------------------------------------------------
void LongViewer::operator()(TSize &l)
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
void LongViewer::operator()(TPaint &l)
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
void LongViewer::operator()(TMouseMove &l)
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
void LongViewer::operator()(TLButtonDbClk &l)
{
	 mouseMove = true;
	if(cursor.CrossCursor(*(TMouseMove *)&l, HDCGraphics(l.hwnd, backScreen)))
	{
		storedMouseMove.x = l.x;
	}
}
//--------------------------------------------------------------------------------
void LongViewer::operator()(TMouseWell &l)
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
	}
}
//--------------------------------------------------------------------------------------
void LongViewer::operator()(TLButtonDown &l)
{
	mouseMove = false;
}
//-----------------------------------------------------------------------------------------
void LongViewer::Update()
{
	RepaintWindow(hWnd);
}
//------------------------------------------------------------------------------------------------
unsigned LongViewer::operator()(TCreate &l)
{
	storedMouseMove.hwnd = l.hwnd;
	storedMouseMove.x = 0;	
	storedMouseMove.y = WORD(chart.rect.top + 1);
	return 0;
}
