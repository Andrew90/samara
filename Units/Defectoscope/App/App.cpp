#include "stdafx.h"
#include "App.h"
#include "DebugMess.h"
#include "MainWindow.h"
#include "WindowsPosition.h"
#include "EmptyWindow.h"

extern HINSTANCE hInstance;

//---------------------------------------------------------------
App::App()	
	: mainWindow(Singleton<MainWindow>::Instance())
{}

void App::Init()
{
	RECT r;
	WindowPosition::Get<MainWindow>(r);
	HWND h = WindowTemplate(&mainWindow, L"Дефектоскоп \"Буран 9000\"", r.left, r.top, r.right, r.bottom);
	ShowWindow(h, SW_SHOWNORMAL);
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





