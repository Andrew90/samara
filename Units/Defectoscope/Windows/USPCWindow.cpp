#include "stdafx.h"
#include "USPCWindow.h"
#include "MenuApi.h"
#include <CommCtrl.h>
#include "ViewersMenu.hpp"
#include "Emptywindow.h"
#include "USPCmessages.h"
#include "DebugMess.h"

//------------------------------------------------------------------------
USPCWindow::USPCWindow()
	: panel(dropDownUnit.current, dropDownSensors.current) 
{}
// 	 ---------------------------------------------------------------------
void USPCWindow::operator()(TSize &m)
{
	if(m.resizing == SIZE_MINIMIZED || 0 == m.Width || 0 == m.Height) return;
	MoveWindow(hStatusWindow, 0, 0, 0, 0, false);
	toolBar.Size();
	static const int offs = toolBar.Width();
	static const int dropDownUnitOffs = 220;
	dropDownUnit.Size(offs, 5, dropDownUnitOffs);
	static const int dropDownSensorsOffs = 190;
	dropDownSensors.Size(offs + dropDownUnitOffs + 10, 5, dropDownSensorsOffs);

	RECT rt;
	GetClientRect(toolBar.hWnd, &rt);
    RECT rs;
	GetClientRect(hStatusWindow, &rs);
	RECT r;
	GetClientRect(m.hwnd, &r);	

	static const int bottomPanelHigth = 200;

	int y = rt.bottom;
	int t = r.bottom - rs.bottom - rt.bottom + 2 - bottomPanelHigth;

	MoveWindow(uspcChartViewer.hWnd , 0, y, r.right, t, true);
	y += t;
	MoveWindow(panel.hWnd , 0, y, r.right, bottomPanelHigth, true);
}
//------------------------------------------------------------------------
void USPCWindow::operator()(TCommand &m)
{
	EventDo(m);
}
//------------------------------------------------------------------------
void USPCWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 600;
		m.pMinMaxInfo->ptMinTrackSize.y = 600;
	}		
}
//------------------------------------------------------------------------
VIEWERS_MENU(USPCWindow)
unsigned USPCWindow::operator()(TCreate &m)
{
	Menu<ViewersMenuUSPCWindow::MainMenu>().Init(m.hwnd);
	toolBar.Init(m.hwnd);

	const wchar_t *items[] = {
		L"Продольный"
		, L"Поперечный"
		, L"Толщина"
	};
	dropDownUnit.Init(toolBar.hWnd, 40, items, dimention_of(items));

	const wchar_t *sens[] = {
		L"Датчик 0"
		, L"Датчик 1"
		, L"Датчик 2"
		, L"Датчик 3"
		, L"Датчик 4"
		, L"Датчик 5"
		, L"Датчик 6"
		, L"Датчик 7"
	};
	dropDownSensors.Init(toolBar.hWnd, 40, sens, dimention_of(sens));

	uspcChartViewer.hWnd = CreateChildWindow(m.hwnd, (WNDPROC)&Viewer<USPCChartViewer>::Proc, L"USPCChartViewer", &uspcChartViewer);

	panel.hWnd = CreateChildWindowBackground(m.hwnd, (WNDPROC)&Viewer<USPCBottomPanel>::Proc, L"Panel", &panel);

	hStatusWindow = CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL, m.hwnd, 0);
	int pParts[] = {550,900, 3000};
	SendMessage(hStatusWindow, SB_SETPARTS, 3, (LPARAM)pParts);
	dropDownUnit.SetCommandHandler(&panel, &USPCBottomPanel::UpdatePage);
	dropDownSensors.SetCommandHandler(&panel, &USPCBottomPanel::UpdatePage);
	return 0;
}
//-------------------------------------------------------------------------
typedef void(*TptrMess)(void *);
void USPCWindow::operator()(TMessage &m)
{
	if(m.wParam)((TptrMess )(m.wParam))((void *)m.lParam);
}
//-----------------------------------------------------------------------------
void USPCWindow::operator()(TMouseWell &l)
{
	l.hwnd = uspcChartViewer.hWnd;
	SendMessage(MESSAGE(l));
}
//--------------------------------------------------------------------------------
/*
void USPCWindow::Do(HWND)
{
	HWND hh = FindWindow(WindowClass<USPCWindow>()(), 0);
	if(NULL != hh)
	{
		SendMessage(hh, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(hh);
	}
	else
	{
		RECT r;
		WindowPosition::Get<USPCWindow>(r);
		HWND h = WindowTemplate(&Singleton<USPCWindow>::Instance(), L"Настройка USPC", r.left, r.top, r.right, r.bottom);
		ShowWindow(h, SW_SHOWNORMAL);
		SetParamToGateItem();
	}
}
*/
 /*
bool USPCWindow::Destroy()
{
	HWND hh = FindWindow(WindowClass<USPCWindow>()(), 0);
	if(NULL != hh)
	{
		DestroyWindow(hh);
		return true;
	}
	return false;
}
*/
 wchar_t *USPCWindow::Title()
 {
	 return L"Настройка USPC";
 }


