#include "stdafx.h"
#include <ShellAPI.h>
#include "App.h"
#include "Config.h"
#include "DebugMess.h"
#include "MainWindow.h"
#include "WindowsPosition.h"
#include "EmptyWindow.h"
#include "AppBase.h"
#include "USPCData.h"
#include "ConstData.h"
#include "Compute.h"
#include "ut_files.h"
#include "HookKey.h"
#include "LogUSPC.h"
#include "LoadFromBase.h"

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
	InitCaretka();
	RECT r;
	WindowPosition::Get<MainWindow>(r);	
	HWND h = WindowTemplate(&mainWindow, (wchar_t *)App::TitleApp(), r.left, r.top, r.right, r.bottom);
	ShowWindow(h, SW_SHOWNORMAL);
////tartKeyHook(h);
	//int num = 0;
	//wchar_t *buf = CommandLineToArgvW(GetCommandLineW(), &num)[1];
////romBase::Load(h, arg[num - 1]);
	////FromBase::Load(h, buf);
	//wchar_t txt[512];
	//wchar_t *protocol = &buf[1 + wcslen(buf)];
	//wchar_t *party = &protocol[1 + wcslen(protocol)];
	//wchar_t *number = &party[1 + wcslen(party)];
	//wsprintf(txt, L"�������� %s ������ %s ����� ����� %s", protocol, party, number);
	//SetWindowText(h, txt);
}

void App::WindowUp(HWND h, wchar_t *buf)
{	
	app.InitCaretka();
	FromBase::Load(h, buf);
	wchar_t txt[512];
	wchar_t *protocol = &buf[1 + wcslen(buf)];
	wchar_t *party = &protocol[1 + wcslen(protocol)];
	wchar_t *number = &party[1 + wcslen(party)];
	wsprintf(txt, L"�������� %s ������ %s ����� ����� %s", protocol, party, number);
	SetWindowText(h, txt);
}

void App::Destroy()
{
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

App app;





