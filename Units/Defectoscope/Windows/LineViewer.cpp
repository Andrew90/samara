#include "stdafx.h"
#include "LineViewer.h"
#include "FixedGridSeries.h"
#include "DebugMess.h"

using namespace Gdiplus;
LineViewer::LineViewer()
	: backScreen(NULL)
	, chart(backScreen)
	, cursor(chart)
{
	chart.minAxesY = 0;
	chart.maxAxesY = 256;
	chart.minAxesX = 0;
	chart.maxAxesX = 512;
	label.fontHeight = 11;
	label.top = 0;

	chart.rect.top = 17;

	cursor.SetMouseMoveHandler(this, &LineViewer::CursorDraw);
}
void LineViewer::operator()(TSize &l)		   
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
void LineViewer::operator()(TPaint &l)
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
unsigned LineViewer::operator()(TCreate &l)	   
{
	storedMouseMove.hwnd = l.hwnd;
	storedMouseMove.x = 0;	
	storedMouseMove.y = WORD(chart.rect.top + 1);
	mouseMove = true;
	return 0;
}
//----------------------------------------------------------------
void LineViewer::operator()(TMouseWell &l)
{
	mouseMove = false;
	OffsetToPixel(chart, storedMouseMove.x, storedMouseMove.y, l.delta / 120, 0 == l.flags.lButton);
	cursor.CrossCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));
}
//--------------------------------------------------------------
 bool LineViewer::CursorDraw(TMouseMove &l, VGraphics &g)
 {
	int x, y;
	chart.CoordCell(l.x, l.y, x, y);	
	wsprintf(label.buffer, L"<ff>смещение %d  величина %d        ", 1 + x, 1 + y);
	label.Draw(g());
	return true;
 }
 //--------------------------------------------------------------------------
 void LineViewer::operator()(TMouseMove &l)
 {	 
	 if(mouseMove)
	 {
		 if(cursor.CrossCursor(l, HDCGraphics(l.hwnd, backScreen)))
		 {
			 storedMouseMove = l;
		 }
	 }
 }
 //--------------------------------------------------------------
 void LineViewer::operator()(TLButtonDown &l)
{
	mouseMove = false;
}
 //-----------------------------------------------------------------
 void LineViewer::operator()(TLButtonDbClk &l)
{
	 mouseMove = true;
	if(cursor.CrossCursor(*(TMouseMove *)&l, HDCGraphics(l.hwnd, backScreen)))
	{
		storedMouseMove.x = l.x;
	}
}
 //--------------------------------------------------------------------------