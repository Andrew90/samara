#include "stdafx.h"
#include "ThresholdWindow.h"
#include "EmptyWindow.h"
#include "DebugMess.h"
#include "ConstData.h"
#include "MenuApi.h"
#include "ViewersMenu.hpp"
#include "CrossWindow.h"
#include "Common.h"

using namespace Gdiplus;
//-----------------------------------------------------------------------------------------
TresholdWindow::TresholdWindow()
	: backScreen(NULL)
	, chart(backScreen)
	, cursor(chart)
{
	chart.minAxesY = 0;
	chart.maxAxesY = 255;
	chart.minAxesX = 0;
	chart.maxAxesX = App::zonesCount;
	chart.rect.top = 17;
	mouseMove = true;
	label.fontHeight = 12;
	label.top = 0;
	cursor.SetMouseMoveHandler(this, &TresholdWindow::Draw);	
	chart.items.get<StepSeries>().SetData(border[0], App::zonesCount, 0, 255);
}
//--------------------------------------------------------------------------
bool TresholdWindow::Draw(TMouseMove &l, VGraphics &g)
{
	int x, y;
	chart.CoordCell(l.x, l.y, x, y);
	
	wsprintf(label.buffer, L"<ff>зона %d  датчик %s        ", 1 + x, Wchar_from<double, 5>(border[0][x])());
	label.Draw(g());

	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void TresholdWindow::operator()(TSize &l)
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
void TresholdWindow::operator()(TPaint &l)
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
void TresholdWindow::operator()(TMouseMove &l)
{
	if(mouseMove)
	{
		if(cursor.VerticalCursor(l, HDCGraphics(l.hwnd, backScreen)))
		{
			storedMouseMove = l;
		}
	}
}
//------------------------------------------------------------------------------
void TresholdWindow::operator()(TLButtonDbClk &l)
{
	 mouseMove = true;
	if(cursor.VerticalCursor(*(TMouseMove *)&l, HDCGraphics(l.hwnd, backScreen)))
	{
		storedMouseMove.x = l.x;
	}
}
//--------------------------------------------------------------------------------
void TresholdWindow::operator()(TMouseWell &l)
{
		mouseMove = false;
		int d = l.delta / 120;
		bool m = 0 == (l.flags.lButton || l.flags.rButton);
		double incr = 0 == l.flags.rButton  ? 0.1 : 5;
		OffsetToPixel(
			chart
			, storedMouseMove.x
			, storedMouseMove.y
			, d
			, m 
			);
		if(!m)
		{
			int x, y;
	        chart.CoordCell( storedMouseMove.x, storedMouseMove.y, x, y);
			border[0][x] += d > 0 ? -incr : incr;
			RECT r;
			GetClientRect(l.hwnd, &r);
			TSize size = {l.hwnd, WM_SIZE, 0, (WORD)r.right, (WORD)r.bottom};
			(*this)(size);
		}
		cursor.VerticalCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));	
}
//--------------------------------------------------------------------------------------
void TresholdWindow::operator()(TLButtonDown &)
{
		mouseMove = false;
}
//------------------------------------------------------------------
unsigned TresholdWindow::operator()(TCreate &l)
{
	storedMouseMove.hwnd = l.hwnd;
	storedMouseMove.x = 0;	
	storedMouseMove.y = WORD(chart.rect.top + 1);
	for(int i = 0; i < 60; ++i) border[0][i] = 20 + i;
	return 0;
}
//------------------------------------------------------------------------------------------
void TresholdWindow::operator()(TKeyDown &l)
{
	TMouseWell mouseWheel = {l.hwnd, WM_MOUSEWHEEL, 0, 0, 0, 0};
	switch(l.VirtKey)
	{
	case VK_LEFT:
		mouseWheel.delta = 120;
		break;
	case VK_RIGHT:
		mouseWheel.delta = -120;
		break;
	case VK_UP:
		mouseWheel.delta = -120;
		mouseWheel.flags.lButton = 1;
		break;
	case VK_DOWN:
		mouseWheel.delta = 120;
		mouseWheel.flags.lButton = 1;
		break;
	default: return;
	}
	mouseWheel.flags.rButton = 0 > GetKeyState(VK_SHIFT);
	(*this)(mouseWheel);
}
//--------------------------------------------------------------------------------------------
wchar_t *TresholdWindow::Title()
{
	return L"Ntcnm";
}