#include "stdafx.h"
#include "LineViewer.h"
#include "FixedGridSeries.h"
#include "DebugMess.h"
#include "EmptyWindow.h"
#include "templates.hpp"

using namespace Gdiplus;
LineViewer::LineViewer()
	: backScreen(NULL)
{
	label.fontHeight = 9;
	label.top = 0;
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

	chart->rect.right = l.Width;
	chart->rect.bottom = l.Height;
	
	chart->Draw(g);
}
void LineViewer::operator()(TPaint &l)
{
	if(NULL == backScreen) return;
	PAINTSTRUCT p;
	HDC hdc = BeginPaint(l.hwnd, &p);
	{		
		Graphics g(hdc);		
		g.DrawCachedBitmap(&CachedBitmap(backScreen, &g), 0, 0);
		cursor->VerticalCursor(storedMouseMove, PaintGraphics(g));
	}
	EndPaint(l.hwnd, &p);
}
unsigned LineViewer::operator()(TCreate &l)	   
{	
	chart->minAxesX = 0;
	chart->maxAxesX = 512;
	chart->rect.top = 17;	
	storedMouseMove.hwnd = l.hwnd;
	storedMouseMove.x = 0;	
	storedMouseMove.y = WORD(chart->rect.top + 1);
	mouseMove = true;
	mouseMove = false;
	offsetX = 0;
	return 0;
}
//----------------------------------------------------------------
void LineViewer::operator()(TMouseWell &l)
{
	mouseMove = false;
	if(l.delta > 0) offsetX -= 1; else if(l.delta < 0)offsetX += 1;
	double dX = (chart->rect.right - chart->rect.left - chart->offsetAxesLeft - chart->offsetAxesRight)
		/(chart->maxAxesX - chart->minAxesX);
	storedMouseMove.x = (WORD)(chart->rect.left + chart->offsetAxesLeft + dX * offsetX);
	cursor->VerticalCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));
}
//--------------------------------------------------------------------------
 void LineViewer::operator()(TMouseMove &l)
 {	 
	 if(mouseMove)
	 {
		 if(cursor->VerticalCursor(l, HDCGraphics(l.hwnd, backScreen)))
		 {
			 storedMouseMove = l;
			 int y;
			 chart->CoordCell(l.x, l.y, offsetX, y);	 
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
	if(cursor->VerticalCursor(*(TMouseMove *)&l, HDCGraphics(l.hwnd, backScreen)))
	{
		int y;
	    chart->CoordCell(l.x, l.y, offsetX, y);	
	}
}
 //-----------------------------------------------------------------------------
 /*
 void LineViewer::operator()(TDestroy &)
 {
	 delete backScreen;
	 backScreen = NULL;
	 SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
 }
 */
