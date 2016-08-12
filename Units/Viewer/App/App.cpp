#include "stdafx.h"
#include "App.h"
#include "Config.h"
#include "DebugMess.h"
#include "MainWindow.h"
#include "WindowsPosition.h"
#include "EmptyWindow.h"
#include "Device1730.h"
#include "AppBase.h"
#include "USPCData.h"
#include "ConstData.h"
#include "Compute.h"
#include "ut_files.h"
#include "HookKey.h"
#include "LogUSPC.h"

HANDLE App::ProgrammExitEvent;
HANDLE App::ProgrammContinueEvent;
HANDLE App::ProgrammStopEvent;
HANDLE App::ProgrammRunEvent;
bool App::measurementOfRunning = false;
int __lengthCaretka = 0;
const int &App::lengthCaretka = __lengthCaretka;
//---------------------------------------------------------------
App::App()	
	: mainWindow(Singleton<MainWindow>::Instance())
{}

namespace
{
	template<class O, class P>struct __caretka__
	{
		void operator()(O *o, P *p)
		{
			if(o->value > *p) *p = o->value;
		}
	};
}

void App::InitCaretka()
{
	__lengthCaretka = 0;
	TL::foreach<OffsetsTable::items_list, __caretka__>()(&Singleton<OffsetsTable>::Instance().items, &__lengthCaretka);
}

void App::Init()
{
	AppBase().Init();
	ConstData::Init();
	App::ProgrammExitEvent		= CreateEvent(NULL, TRUE, FALSE, NULL);
	App::ProgrammContinueEvent	= CreateEvent(NULL, TRUE, FALSE, NULL);
	App::ProgrammStopEvent		= CreateEvent(NULL, FALSE, FALSE, NULL);
	App::ProgrammRunEvent   = CreateEvent(NULL, FALSE, FALSE, NULL);
	InitCaretka();
	LogUSPC::Clear();
	RECT r;
	WindowPosition::Get<MainWindow>(r);	
	HWND h = WindowTemplate(&mainWindow, (wchar_t *)App::TitleApp(), r.left, r.top, r.right, r.bottom);
	ShowWindow(h, SW_SHOWNORMAL);
	StartKeyHook(h);
}

void App::Destroy()
{
	device1730.Write(0);
	SetEvent(ProgrammExitEvent);
	Sleep(2000);
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
HWND App::MainWindowHWND()
{
	return app.mainWindow.hWnd;
}

Device1730 device1730;

App app;






