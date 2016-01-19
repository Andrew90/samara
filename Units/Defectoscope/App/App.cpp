#include "stdafx.h"
#include "App.h"
#include "DebugMess.h"
#include "MainWindow.h"
#include "WindowsPosition.h"
#include "EmptyWindow.h"
#include "Device1730.h"
#include "AppBase.h"

extern HINSTANCE hInstance;

HANDLE App::ProgrammExitEvent;
HANDLE App::ProgrammContinueEvent;
HANDLE App::ProgrammStopEvent;

//---------------------------------------------------------------
App::App()	
	: mainWindow(Singleton<MainWindow>::Instance())
{
}

void App::Init()
{
	App::ProgrammExitEvent		= CreateEvent(NULL, FALSE, TRUE, NULL);
	App::ProgrammContinueEvent	= CreateEvent(NULL, FALSE, TRUE, NULL);
	App::ProgrammStopEvent		= CreateEvent(NULL, FALSE, FALSE, NULL);
	RECT r;
	WindowPosition::Get<MainWindow>(r);
	HWND h = WindowTemplate(&mainWindow, L"Дефектоскоп \"Буран 9000\"", r.left, r.top, r.right, r.bottom);
	ShowWindow(h, SW_SHOWNORMAL);
	if(!device1730.Init(Singleton<Handle1730Table>::Instance().items.get<Handle1730>().value))
	{
		MessageBox(h, L"Не могу инициировать плату 1730", L"Ошибка !!!", MB_ICONERROR);
		return;
	}	
}

void App::Destroy()
{
	
}

void App::MainWindowTopLabel(wchar_t *txt)
{
}

void App::MainWindowBottomLabel(int n, wchar_t *text)
{
}

Device1730 device1730;





