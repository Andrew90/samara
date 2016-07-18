#include "stdafx.h"
#include "MainWindowToolBar.h"
#include "Config.h"
#include "DebugMess.h"
#include "../Resource.h"
#include "InitToolBar.hpp"
#include "message.h"
#include "Emptywindow.h"
#include "MainWindow.h"
#include "Common.h"
#include "PacketWindow.h"
#include <gdiplus.h>
#include "AppKeyHandler.h"
#include "Stored.h"
#include "LogUSPCWindow.h"
using namespace Gdiplus;
namespace 
{
	template<int N>struct Key;

#define KEY(ID, text)template<>struct Key<ID> \
	{\
	static void Click(HWND);\
	static wchar_t *Text(){return text;}\
	};
	KEY(IDB_CycleBtn   , L"F4 Цикл")
	KEY(IDB_Reset      , L"F9 Стоп")
	KEY(IDB_QueryBtn   , L"Тест")
	KEY(IDB_arrow_down , L"F5 Вниз")
	KEY(IDB_arrow_up   , L"F6 Вверх")
	KEY(IDB_arrow_left , L"F7 Влево")
	KEY(IDB_arrow_right, L"F8 Вправо")
#undef KEY
#define BUTTON_KEY(ID)ButtonToolbar<ID, Key<ID> > 
		typedef TL::MkTlst<
		SeparatorToolbar<0>
		, BUTTON_KEY(IDB_CycleBtn)
		, BUTTON_KEY(IDB_Reset)
		, BUTTON_KEY(IDB_QueryBtn) 		
		, SeparatorToolbar<1>
		, BUTTON_KEY(IDB_arrow_down) 
		, BUTTON_KEY(IDB_arrow_up)
		, BUTTON_KEY(IDB_arrow_left) 
		, BUTTON_KEY(IDB_arrow_right)
		, SeparatorToolbar<2>
		>::Result tool_button_list;
#undef BUTTON_KEY
//----------------------------------------------------------------------------------
	static bool closed_packet_dialog = true;
	void Key<IDB_CycleBtn>::Click(HWND h)
	{
		if(closed_packet_dialog)
		{
			closed_packet_dialog = false;
			if(PacketWindowDlg(h)) 
			{
				CloseAllWindows();
				SetEvent(App::ProgrammContinueEvent);
				AppKeyHandler::Run();
			}
			closed_packet_dialog = true;
		}
	}
//-------------------------------------------------------------------------------
	void Key<IDB_QueryBtn>::Click(HWND h)
	{
		zprint("\n");
		//SetEvent(App::ProgrammTestRunEvent);
		AppKeyHandler::Run();
	}
//------------------------------------------------------------------------------
	void Key<IDB_arrow_down>::Click(HWND h)
	{
		zprint("\n");	
		//-test
	//	RECT r;
	//WindowPosition::Get<LogUSPCWindow>(r);
	//HWND hh = WindowTemplate(&Singleton<LogUSPCWindow>::Instance(), L"События USPC", r.left, r.top, r.right, r.bottom);
	//ShowWindow(hh, SW_SHOWNORMAL);
		LogUSPCWindow::Open();
		//test
	}
//------------------------------------------------------------------------------
	void Key<IDB_arrow_up>::Click(HWND h)
	{
		zprint("\n");
//test
		Stored::Do();
//test
	}
//------------------------------------------------------------------------------
	void Key<IDB_arrow_left>::Click(HWND h)
	{
		zprint("\n");	
		LogUSPCWindow::Open();
	}
//------------------------------------------------------------------------------
	void Key<IDB_arrow_right>::Click(HWND h)
	{
		zprint("\n");	
	}
//----------------------------------------------------------------------------
	void Key<IDB_Reset>::Click(HWND h)
	{
		SetEvent(App::ProgrammStopEvent);
		AppKeyHandler::Stop();
	}
}
//--------------------------------------------------------------------------------------------
HWND MainWindowToolBar::Init(HWND h)
{
	return hWnd = InitToolbar<tool_button_list>()(h);
}
//-------------------------------------------------------------------
void MainWindowToolBar::Size()
{
	MoveWindow(hWnd, 0, 0, 0, 0, false);
}
//----------------------------------------------------------------------------
const int MainWindowToolBar::Width()
{
	return __tool_bar_width__<tool_button_list>::value;
}
//------------------------------------------------------------------------


