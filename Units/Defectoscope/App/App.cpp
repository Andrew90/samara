#include "stdafx.h"
#include "App.h"
#include "Config.h"
#include "DebugMess.h"
#include "MainWindow.h"
#include "WindowsPosition.h"
#include "EmptyWindow.h"
#include "Device1730.h"
#include "AppBase.h"
#include "Automat.h"
#include "USPCData.h"
#include "ConstData.h"
#include "Compute.h"
#include "ut_files.h"
#include "Automat.h"
#include "HookKey.h"
#include "LogUSPC.h"

HANDLE App::ProgrammExitEvent;
HANDLE App::ProgrammContinueEvent;
HANDLE App::ProgrammStopEvent;
bool App::measurementOfRunning = false;

//---------------------------------------------------------------
App::App()	
	: mainWindow(Singleton<MainWindow>::Instance())
{}

void App::Init()
{
	AppBase().Init();
	ConstData::Init();
	App::ProgrammExitEvent		= CreateEvent(NULL, TRUE, FALSE, NULL);
	App::ProgrammContinueEvent	= CreateEvent(NULL, TRUE, FALSE, NULL);
	App::ProgrammStopEvent		= CreateEvent(NULL, FALSE, FALSE, NULL);
	LogUSPC::Clear();
	RECT r;
	WindowPosition::Get<MainWindow>(r);
	HWND h = WindowTemplate(&mainWindow, L"ЛБТ \"Буран-5000\"", r.left, r.top, r.right, r.bottom);
	ShowWindow(h, SW_SHOWNORMAL);
	StartKeyHook(h);
	if(!device1730.Init(Singleton<Descriptor1730Table>::Instance().items.get<Descriptor1730>().value))
	{
		MessageBox(h, L"Не могу инициировать плату 1730", L"Ошибка !!!", MB_ICONERROR);
#ifndef DEBUG_ITEMS
		return;
#endif
	}
	
	automat.Init();
/////--------------------------test
//	zprint("---------------------\n");
//	unsigned start = GetTickCount();
//	 TestUSPC test;
//	 test.Init(Singleton<ItemData<Long> >::Instance());
//	 test.Init(Singleton<ItemData<Cross> >::Instance());
//	 test.InitThickness(Singleton<ItemData<Thickness> >::Instance());
//	 zprint("  Stop time %d\n", GetTickCount() - start);
/////-------------------------------test------------------------------
}

void App::Destroy()
{
	SetEvent(ProgrammExitEvent);
	Sleep(1000);
}

void App::MainWindowTopLabel(wchar_t *txt)
{
	mainWindow.topLabelViewer.SetMessage(txt);
}

void App::MainWindowBottomLabel(int n, wchar_t *text)
{
	SendMessage(mainWindow.hStatusWindow, SB_SETTEXT, n, (LONG)text);
}

void App::MainWindowUpdate()
{
	RepaintWindow(mainWindow.hWnd);
}

Device1730 device1730;

App app;

Automat automat;





