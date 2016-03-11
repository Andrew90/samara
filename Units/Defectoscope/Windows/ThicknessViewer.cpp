#include "stdafx.h"
#include "ThicknessViewer.h"
#include <stdio.h>
#include "EmptyWindow.h"
#include "LabelMessage.h"
#include "DebugMess.h"
#include "AppBase.h"
#include "App.h"
#include "ThicknessWindow.h"
#include "ViewersMenu.hpp"
//------------------------------------------------------------------------------------------------------
using namespace Gdiplus;

bool ThicknessViewer::Draw(TMouseMove &l, VGraphics &g)
{
	int x, y;
	chart.CoordCell(l.x, l.y, x, y);
	int color;
	bool b;
	char *s = StatusText(viewerData.commonStatus[x], color, b);
	wchar_t buf[128];
	if(b)
	{
		wsprintf(buf, L"<ff>мин.толщина <ff0000>%s <ff>мах.толщина <ff0000>%s", 
			Wchar_from<double>(viewerData.bufferMin[x])()
			, Wchar_from<double>(viewerData.bufferMax[x])()
			);
	}
	else
	{
		buf[0] = 0;
	}
	wsprintf(label.buffer, L"<ff>Толщина зона %d  <%6x>%S %s"
		, 1 + x
		, color
		, s
		, buf
		);
	label.Draw(g());

	return x < viewerData.currentOffsetZones;
}

bool ThicknessViewer::GetColorBar(int zone, double &data_, unsigned &color, double &data_1, unsigned &color1)
{
	data_1 = viewerData.bufferMin[zone];
	data_ =  viewerData.bufferMax[zone];
	ColorBar(
		data_
		, color1
		, viewerData.commonStatus[zone]
		, Singleton<ThresholdsTable>::Instance().items.get<NominalBorder<Thickness> >().value[zone]
	);

	color = color1;
	unsigned char *x = (unsigned char *) &color;
	x[0] = unsigned char(3.0 * x[0] / 4);
	x[1] = unsigned char(3.0 * x[1] / 4);
	x[2] = unsigned char(3.0 * x[2] / 4);

	return zone < viewerData.currentOffsetZones;	  
}
//-----------------------------------------------------------------------------
ThicknessViewer::ThicknessViewer()
	: backScreen(NULL)
	, chart(backScreen)
	, cursor(chart)
	, openDetailedWindow(false)
	, painting(true)
	, mouseMove(true)
	, viewerData(Singleton<ItemData<Thickness> >::Instance())
{
	chart.rect.top = 17;
	
	chart.minAxesX = 0;
	chart.maxAxesX = App::zonesCount;
	chart.minAxesY = 0;

	label.fontHeight = 12;
	label.top = 0;	

	cursor.SetMouseMoveHandler(this, &ThicknessViewer::Draw);
	chart.items.get<BarSeriesDouble>().SetColorBarHandler(this, &ThicknessViewer::GetColorBar);
}
//----------------------------------------------------------------------------------------------------
#pragma warning(disable : 4996)
void ThicknessViewer::operator()(TSize &l)
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

	chart.rect.right = l.Width;
	chart.rect.bottom = l.Height;
	chart.minAxesY = Singleton<AxesTable>::Instance().items.get<AxesYMin<Thickness> >().value;
	chart.maxAxesY = Singleton<AxesTable>::Instance().items.get<AxesYMax<Thickness> >().value;
	chart.Draw(g);
}
//----------------------------------------------------------------------------------------------------
void ThicknessViewer::operator()(TPaint &l)
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
void ThicknessViewer::operator()(TMouseMove &l)
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
void ThicknessViewer::operator()(TLButtonDown &l)
{
	mouseMove = false;
}
//--------------------------------------------------------------------------
void ThicknessViewer::operator()(TLButtonDbClk &l)
{
	mouseMove = true;
	if(cursor.VerticalCursor(*(TMouseMove *)&l, HDCGraphics(l.hwnd, backScreen)))
	{
		storedMouseMove.x = l.x;
	}
}
void ThicknessViewer::operator()(TMouseWell &l)
{	
		mouseMove = false;

		OffsetToPixel(
			chart
			, storedMouseMove.x
			, storedMouseMove.y
			, l.delta / 120
			, true 
			);
		cursor.VerticalCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));
}
//------------------------------------------------------------------------------------------------------
unsigned ThicknessViewer::operator()(TCreate &l)
{
	storedMouseMove.hwnd = l.hwnd;
	storedMouseMove.x = 0;	
	storedMouseMove.y = WORD(chart.rect.top + 1);
	return 0;
}
//------------------------------------------------------------------------------------------
CONTEXT_MENU(ThicknessWindow)
void ThicknessViewer::operator()(TRButtonDown &l)
{
	PopupMenu<ContextMenuThicknessWindow::items_list>::Do(l.hwnd, l.hwnd);
}
//--------------------------------------------------------------------------------
/*
void ThicknessViewer::operator()(TDestroy &m)
{
	delete backScreen;
    backScreen = NULL;
	SetWindowLongPtr(m.hwnd, GWLP_USERDATA, NULL);
}
*/
