#include "stdafx.h"
#include "ScanWindow.h"
#include "EmptyWindow.h"
#include "ViewersMenu.hpp"
#include "FixedGridSeries.h"

using namespace Gdiplus;

RedLineSeries::RedLineSeries(Chart &c)
	: line(c)
{
	line.color = 0xffff0000;
	static const int count = dimention_of(buf);
	line.SetData(buf, count, 0, count - 1);
}

void RedLineSeries::Draw()
{
	line.Draw();
}

ScanWindow::ScanWindow()
	: chart(backScreen)
	, cursor(chart)
{
	chart.minAxesY = 0;
	chart.maxAxesY = 255;
	chart.minAxesX = 0;
	chart.maxAxesX = 256;

	chart.items.get<LineSeries>().data = data;

	label.fontHeight = 12;

	cursor.SetMouseMoveHandler(this, &ScanWindow::CursorDraw);
}
void ScanWindow::operator()(TSize &l)
{
	storedMouseMove.hwnd = l.hwnd;
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
		toolBar.Size();
		RECT r = {};
		GetClientRect(toolBar.hWnd, &r);
	    if(0 == r.bottom)  return;

		Graphics g(backScreen);
		SolidBrush solidBrush(Color(0xffaaaaaa));
		g.FillRectangle(&solidBrush, 0, r.bottom, l.Width, 20);
		label.top = r.bottom;
		chart.rect.top = r.bottom + 20;
		chart.rect.right = l.Width;
		chart.rect.bottom = l.Height;
		chart.minAxesY = minY;
		chart.maxAxesY = maxY;
		chart.items.get<LineSeries>().SetData(data, maxX, 0, maxX - 1);
		chart.Draw(g);
		label.Draw(g);
	}
	void ScanWindow::operator()(TPaint &l)
	{
		if(NULL == backScreen) return;
		PAINTSTRUCT p;
		HDC hdc = BeginPaint(l.hwnd, &p);
		{		
			Graphics g(hdc);		
			g.DrawCachedBitmap(&CachedBitmap(backScreen, &g), 0, 0);
			cursor.CrossCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));
		}
		EndPaint(l.hwnd, &p);
	}
	void ScanWindow::operator()(TCommand &l)
	{
		EventDo(l);
	}
	void ScanWindow::operator()(TGetMinMaxInfo &l)
	{
		if(NULL != l.pMinMaxInfo)
		{
			l.pMinMaxInfo->ptMinTrackSize.x = 300;
			l.pMinMaxInfo->ptMinTrackSize.y = 200;
		}	
	}
	VIEWERS_MENU(ScanWindow)
	unsigned ScanWindow::operator()(TCreate &l)
	{
		mouseMove = false;
		offset = 0;
		Menu<ViewersMenuScanWindow::MainMenu>().Init(l.hwnd);
		toolBar.Init(l.hwnd);

		storedMouseMove.hwnd = l.hwnd;
		storedMouseMove.x = WORD((chart.rect.right - chart.rect.left) / 2);	
		storedMouseMove.y = WORD((chart.rect.bottom - chart.rect.top) / 2);

        chart.CoordCell(storedMouseMove.x, storedMouseMove.y, currentX, currentY);
		return 0;
	}
	
	void ScanWindow::operator()(TLButtonDown &l)
	{
		mouseMove = false;
	}

	void ScanWindow::operator()(TMouseMove &l)
{
	if(mouseMove)
	{
		storedMouseMove = l;
		chart.CoordCell(storedMouseMove.x, storedMouseMove.y, currentX, currentY);
		cursor.CrossCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));
	}
}
//------------------------------------------------------------------------------
void ScanWindow::operator()(TLButtonDbClk &l)
{
	 mouseMove = true;
	if(cursor.CrossCursor(*(TMouseMove *)&l, HDCGraphics(l.hwnd, backScreen)))
	{
		storedMouseMove.x = l.x;
	}
}
//--------------------------------------------------------------------------------
void ScanWindow::operator()(TMouseWell &l)
{
		mouseMove = false;
	
		OffsetToPixel(
			chart
			, storedMouseMove.x
			, storedMouseMove.y
			, l.delta / 120
			, 0 == l.flags.lButton 
			);
		chart.CoordCell(storedMouseMove.x, storedMouseMove.y, currentX, currentY);
		cursor.CrossCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));		
}

bool ScanWindow::CursorDraw(TMouseMove &l, VGraphics &g)
{
	wsprintf(&label.buffer[lengthMess], L"   X %d  Y %d", currentX + 1, currentY - 128);
	label.Draw(g());
	return true;
}

	void ScanWindow::Open(int zone_, int sensor_, int offset_, wchar_t *mess, wchar_t *mess1, USPC7100_ASCANDATAHEADER *uspc, void *o, void(*ptr)())
	{
		owner = o;
		zone = zone_;
		sensor = sensor_;
		offsetInZone = offset_;
		ptrScan = (void(*)(int, int, int, void *, void(*)()))ptr;
		for(int i = 0; i < 512; ++i)  data[i] = uspc->Point[i];
		maxX = uspc->DataSize > 0 ?  uspc->DataSize : dimention_of(uspc->Point);
		
		 wchar_t buf[1024];
		 wchar_t alarmBuff[128];
		 alarmBuff[0] = 0;
		 if(uspc->CycleAlarm) wcscpy(alarmBuff, L"авария Cycle, ");
		 if(uspc->PrfAlarm) wcscat(alarmBuff, L"авария PRF, ");
		  if(uspc->PowerAlarm) wcscat(alarmBuff, L"авария Power, ");
		 wsprintf(buf, L"%s зона %d датчик %d %s смещение %d номер скана %d", mess, 1 + zone_, 1 + sensor_, alarmBuff, 1 + offset_, uspc->hdr.ScanCounter);
		 
		 label = mess1;
		 lengthMess = wcslen(mess1);
		 HWND h = FindWindow(WindowClass<ScanWindow>()(), 0);
		 if(NULL != h)
		 {			
			 RepaintWindow(h);
			 SetWindowText(h, buf);
			 SendMessage(h, WM_SYSCOMMAND, SC_RESTORE, 0);
			 SetForegroundWindow(h);
		 }
		 else
		 {
			 RECT r;
			 WindowPosition::Get<ScanWindow>(r);
			 HWND h = WindowTemplate(this, buf, r.left, r.top, r.right, r.bottom);
			 ShowWindow(h, SW_SHOWNORMAL);
		 }		 
	}

	void ScanWindow::SensPlus() 
	{
		++sensor;
		sensor %= App::count_sensors;
		(*ptrScan)(zone, sensor, offsetInZone, owner, (void(*)())ptrScan);
	}
	void ScanWindow::SensMinus()
	{
		--sensor;
		sensor = (unsigned)sensor % App::count_sensors;
		(*ptrScan)(zone, sensor, offsetInZone, owner, (void(*)())ptrScan);
	}
	void ScanWindow::OffsPlus()	
	{
       ++offsetInZone;
	   (*ptrScan)(zone, sensor, offsetInZone, owner, (void(*)())ptrScan);
	}
	void ScanWindow::OffsMinus()
	{
		--offsetInZone;
	   (*ptrScan)(zone, sensor, offsetInZone, owner, (void(*)())ptrScan);
	}

