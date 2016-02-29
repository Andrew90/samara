#include "stdafx.h"
#include "LongViewer.h"
#include "EmptyWindow.h"
#include "USPCData.h"
#include "DebugMess.h"
#include "ConstData.h"
#include "MenuApi.h"
#include "ViewersMenu.hpp"

using namespace Gdiplus;
//-----------------------------------------------------------------
bool LongViewer::Draw(TMouseMove &l, VGraphics &g)
{
	int x, y;
	chart.CoordCell(l.x, l.y, x, y);	
	wsprintf(label.buffer, L"<ff>зона %d  датчик %d  %x      ", 1 + x, 1 + y, viewerData.status[y][x]);
	label.Draw(g());

	return x < viewerData.currentOffsetZones;
}

bool LongViewer::GetColorBar(unsigned sensor, int zone, double &data, unsigned &color)
{
	--sensor;
	data = viewerData.buffer[sensor][zone];
	color = ConstData::ZoneColor(viewerData.status[sensor][zone]);
	return zone < viewerData.currentOffsetZones;
}
//------------------------------------------------------------------
LongViewer::LongViewer()
	: backScreen(NULL)
	, chart(backScreen)
	, cursor(chart)
	, viewerData(Singleton<ItemData<Long> >::Instance())
{
	
	chart.items.get<FixedGridSeries>().sensorCount = App::count_sensors;
	chart.minAxesY = 1;
	chart.maxAxesY = 1 + App::count_sensors;
	chart.minAxesX = 0;
	chart.maxAxesX = App::zonesCount;
	cursor.SetMouseMoveHandler(this, &LongViewer::Draw);
	label.fontHeight = 12;
	label.top = 0;
	chart.rect.top = 17;
	mouseMove = true;
	label.fontHeight = 10;
	label.top = 0;

	chart.items.get<FixedGridSeries>().SetColorCellHandler(this, &LongViewer::GetColorBar);
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
		mouseMove = false;
	
		chart.items.get<FixedGridSeries>().OffsetToPixel(
			storedMouseMove.x
			, storedMouseMove.y
			, l.delta / 120
			, 0 == l.flags.lButton 
			);
		cursor.CrossCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));
}
//--------------------------------------------------------------------------------------
void LongViewer::operator()(TLButtonDown &l)
{
	mouseMove = false;
}
//------------------------------------------------------------------------------------------------
unsigned LongViewer::operator()(TCreate &l)
{
	storedMouseMove.hwnd = l.hwnd;
	storedMouseMove.x = 0;	
	storedMouseMove.y = WORD(chart.rect.top + 1);
	return 0;
}
//------------------------------------------------------------------------------------------
//CONTEXT_MENU(DetailedView)
void LongViewer::operator()(TRButtonDown &l)
{
	//PopupMenu<ViewerMenu::items_list>::Do(l.hwnd, l.hwnd);
	//PopupMenu<DetailedViewContextMenu::items_list>::Do(l.hwnd, l.hwnd);
}
//--------------------------------------------------------------------------------
