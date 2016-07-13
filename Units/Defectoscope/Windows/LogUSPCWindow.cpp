#include "stdafx.h"
#include "LogUSPCWindow.h"
#include "MenuApi.h"
#include "EmptyWindow.h"
#include "LogBuffer.h"
#include "LogUSPCWindowMenu.hpp"
#include "Emptywindow.h"
#include "App.h"

#include "DebugMess.h"
#pragma warning(disable : 4355)
//------------------------------------------------------------------------
LogUSPCWindow::LogUSPCWindow()
{
	lastIndex = -1;
}
//---------------------------------------------------------------------------
void LogUSPCWindow::operator()(TSize &m)
{
	if(m.resizing == SIZE_MINIMIZED || 0 == m.Width || 0 == m.Height) return;
	RECT r;
	GetClientRect(m.hwnd, &r);
	MoveWindow(mainWindowGrid.hWnd, 0,  0, r.right, r.bottom, TRUE);
}
//------------------------------------------------------------------------
void LogUSPCWindow::operator()(TCommand &m)
{
	EventDo(m);
}
//-------------------------------------------------------------------------------------------
unsigned LogUSPCWindow::operator()(TNotify &m)
{
	NotifyHandler *x = (NotifyHandler *)GetWindowLongPtr(m.pnmh->hwndFrom, GWLP_USERDATA);
	if(0 == IsBadReadPtr(x, sizeof(x)))return (*x)(m);
	return 0;
}
//------------------------------------------------------------------------
void LogUSPCWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 300;
		m.pMinMaxInfo->ptMinTrackSize.y = 200;
		m.pMinMaxInfo->ptMaxTrackSize.x = 2000;
		m.pMinMaxInfo->ptMaxTrackSize.y = 2000;		
	}		
}
//------------------------------------------------------------------------
unsigned LogUSPCWindow::operator()(TCreate &m)
{
	Menu<LogUSPCWindowMenu::MainMenu>().Init(m.hwnd);
	data.Open();
	mainWindowGrid.Init(m.hwnd);
	return 0;
}
//------------------------------------------------------------------------
void LogUSPCWindow::Open()
{
	RECT r;
	WindowPosition::Get<LogUSPCWindow>(r);
	HWND hh = WindowTemplate(&Singleton<LogUSPCWindow>::Instance(), L"������� USPC", r.left, r.top, r.right, r.bottom);
	SetWindowPos(hh, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	DWORD dwStyle = GetWindowLong(hh, GWL_STYLE);
	dwStyle &= ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
	SetWindowLong(hh, GWL_STYLE, dwStyle);
	ShowWindow(hh, SW_SHOWNORMAL);
}
