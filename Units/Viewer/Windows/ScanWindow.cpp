#include "stdafx.h"
#include "ScanWindow.h"
#include "EmptyWindow.h"
#include "ViewersMenu.hpp"

using namespace Gdiplus;

ScanWindow::ScanWindow()
	: chart(backScreen)
{
	chart.minAxesY = 0;
	chart.maxAxesY = 255;
	chart.minAxesX = 0;
	chart.maxAxesX = 512;

	chart.items.get<LineSeries>().data = data;

	label.fontHeight = 12;
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
		return 0;
	}
	
	void ScanWindow::operator()(TLButtonDown &l)
	{
		mouseMove = false;
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
		 maxY = 100;
		 wchar_t buf[1024];
		 wchar_t alarmBuff[128];
		 alarmBuff[0] = 0;
		 if(uspc->CycleAlarm) wcscpy(alarmBuff, L"авария Cycle, ");
		 if(uspc->PrfAlarm) wcscat(alarmBuff, L"авария PRF, ");
		  if(uspc->PowerAlarm) wcscat(alarmBuff, L"авария Power, ");
		 wsprintf(buf, L"%s зона %d датчик %d %s смещение %d номер скана %d", mess, 1 + zone_, 1 + sensor_, alarmBuff, 1 + offset_, uspc->hdr.ScanCounter);
		 
		 label = mess1;
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

	bool ScanWindow::CursorDraw(TMouseMove &l, VGraphics &g)	  
	{			
		RepaintWindow(l.hwnd);
		return true;
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

