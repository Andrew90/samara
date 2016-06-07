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

#define BUTTON_KEY(ID)ButtonToolbar<ID, Key<ID> > 

		typedef TL::MkTlst<
		SeparatorToolbar<0>
		, BUTTON_KEY(IDB_CycleBtn)
		, BUTTON_KEY(IDB_Reset)
		, BUTTON_KEY(IDB_QueryBtn) 
		, SeparatorToolbar<1>
		>::Result tool_button_list;
#undef BUTTON_KEY
//----------------------------------------------------------------------------------
	void Key<IDB_CycleBtn>::Click(HWND h)
	{
		if(AppKeyHandler::IsEnabledRun() && PacketWindowDlg(h)) 
		{
			SetEvent(App::ProgrammContinueEvent);
			AppKeyHandler::Run();
		}
	}
//-------------------------------------------------------------------------------
	void Key<IDB_QueryBtn>::Click(HWND h)
	{
		zprint("");	
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


