#pragma once
#include "stdafx.h"
#include "Dialogs.h"
#include "IOportsWindow.h"
#include "EmptyWindow.h"
#include "AppBase.h"
#include "mainwindow.h"
#include "Pass.h"
#include <CommCtrl.h>
#include <WindowsX.h>
#include "WindowsPosition.h"

void IOportsDlg::Do(HWND h)
{
	HWND hh = FindWindow(L"IOportsWindow", 0);
	if(NULL != hh)
	{
		SendMessage(hh, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(hh);
	}
	else
	{
		RECT r;
		WindowPosition::Get<IOportsWindow>(r);
		h = WindowTemplate(
			&IOportsWindow::Instance()
			, L""
			, r.left, r.top
			, IOportsWindow::width, IOportsWindow::height
			);
		ShowWindow(h, SW_SHOWNORMAL);
	}
	//SendTitle();
}
/*
void IOportsDlg::SendTitle()
{
	HWND hh = FindWindow(L"ManagementIOportsDlg", 0);
	if(NULL != hh)
	{
		wchar_t *mess = L"Просмотр дискретных входов-выходов";
		if(SendMessage(mainWindow.hToolBar, TB_ISBUTTONENABLED, IDB_SensorsUnit, 0))
		{
			mess = L"Управление дискретными выходами";
		}
		SetWindowText(hh, mess);
		SendMessage(hh, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(hh);
	}
}
*/
