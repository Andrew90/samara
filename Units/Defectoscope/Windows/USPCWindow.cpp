#include "stdafx.h"
#include "USPCWindow.h"
#include "MenuApi.h"
#include <CommCtrl.h>
#include "USPCWindowMenu.hpp"
#include "Emptywindow.h"
#include "DebugMess.h"

//------------------------------------------------------------------------
USPCWindow::USPCWindow()
{}
// 	 ---------------------------------------------------------------------
void USPCWindow::operator()(TSize &m)
{
	if(m.resizing == SIZE_MINIMIZED || 0 == m.Width || 0 == m.Height) return;
	MoveWindow(hStatusWindow, 0, 0, 0, 0, false);
	
}
//------------------------------------------------------------------------
void USPCWindow::operator()(TCommand &m)
{
	GetMenuToolBarEvent(m);
}
//------------------------------------------------------------------------
void USPCWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 600;
		m.pMinMaxInfo->ptMinTrackSize.y = 600;
	//	m.pMinMaxInfo->ptMaxTrackSize.x = 2000;
	//	m.pMinMaxInfo->ptMaxTrackSize.y = 700;		
	}		
}
//------------------------------------------------------------------------
unsigned USPCWindow::operator()(TCreate &m)
{
	Menu<USPCWindowMenu::MainMenu>().Init(m.hwnd);
//
	toolBar.Init(m.hwnd);
	//select.Create(toolBar.hWnd);
//
	hStatusWindow = CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL, m.hwnd, 0);
	int pParts[] = {550,900, 3000};
	SendMessage(hStatusWindow, SB_SETPARTS, 3, (LPARAM)pParts);
	return 0;
}
//-------------------------------------------------------------------------
void USPCWindow::operator()(TKeyDown &l)
{
	dprint(__FUNCTION__);
}
//-------------------------------------------------------------------------
void USPCWindow::operator()(TRButtonDown &l)
{
  dprint(__FUNCTION__);
}
//------------------------------------------------------------------------
void USPCWindow::operator()(TDestroy &)
{

}
//---------------------------------------------------------------------------
typedef void(*TptrMess)(void *);
void USPCWindow::operator()(TMessage &m)
{
	if(m.wParam)((TptrMess )(m.wParam))((void *)m.lParam);
	dprint(__FUNCTION__);
}
//-----------------------------------------------------------------------------
void USPCWindow::operator()(TMouseWell &l)
{
}
//--------------------------------------------------------------------------------
void USPCWindow::Do(HWND)
{
	static USPCWindow x;

	HWND hh = FindWindow(L"USPCWindow", 0);
	if(NULL != hh)
	{
		SendMessage(hh, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(hh);
	}
	else
	{
		RECT r;
		WindowPosition::Get<USPCWindow>(r);
		HWND h = WindowTemplate(&x, L"Настройка USPC", r.left, r.top, r.right, r.bottom);
		ShowWindow(h, SW_SHOWNORMAL);
	}
}

