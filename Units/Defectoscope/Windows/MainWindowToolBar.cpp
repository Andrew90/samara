#include "stdafx.h"
#include "MainWindowToolBar.h"
#include "Config.h"
#include "DebugMess.h"
#include "../Resource.h"
#include "InitToolBar.hpp"
#include "message.h"
//#include "DetailedWindow.h"
//#include "ThicknessViewer.h"
#include "Emptywindow.h"
//#include "Automat.h"
#include "MainWindow.h"
//#include "USPCWindow.h"
#include "Common.h"
#include "PacketWindow.h"
#include <gdiplus.h>
#include "AppKeyHandler.h"
//#include "SaveScreenToJpeg.h"
using namespace Gdiplus;
namespace 
{

	template<int N>struct Key;
#define KEY(ID, text)template<>struct Key<ID> \
	{\
		static void Click(HWND); \
		static wchar_t *Text(){return text;}\
	};

//void SycleMeashurement(HWND);//обработчик для кнопки "Циклическое измерение"
//
//void StopMeashurement(HWND);//обработчик для кнопки "Остановка измерения"
//
//void TresholdsViewBtn(HWND);
//
//void TestBtn(HWND);
//
//#define TOOL_LIP_TEXT(name, text)struct name{static wchar_t *Text(){return text;}};
//
//TOOL_LIP_TEXT(ToolLipCycleBtn     , L"F4 Цикл")
//TOOL_LIP_TEXT(ToolLipReset  	  , L"F9 Стоп")
//TOOL_LIP_TEXT(ToolTresholdsViewBtn  , L"Просмотр")
//TOOL_LIP_TEXT(ToolTestBtn  , L"Тест")
//  
//#undef TOOL_LIP_TEXT

	KEY(IDB_CycleBtn, L"F4 Цикл")

#define BUTTON_KEY(ID)ButtonToolbar<ID, Key<ID>::Click, Key<ID> > 

typedef TL::MkTlst<
  SeparatorToolbar<0>
   , BUTTON_KEY(IDB_CycleBtn)
  //, ButtonToolbar<IDB_CycleBtn, SycleMeashurement, ToolLipCycleBtn>  
  //, ButtonToolbar<IDB_Reset, StopMeashurement    , ToolLipReset>  
  //, ButtonToolbar<IDB_QueryBtn, TestBtn             , ToolTestBtn>  
  //, ButtonToolbar<IDB_MashBtn, TresholdsViewBtn  , ToolTresholdsViewBtn>
  , SeparatorToolbar<1>
>::Result tool_button_list;
//----------------------------------------------------------------------------------
//void SycleMeashurement(HWND h)
	template<>void Key<IDB_CycleBtn>::Click(HWND h)
{
   if(AppKeyHandler::IsEnabledRun() && PacketWindowDlg(h)) 
   {
	   SetEvent(App::ProgrammContinueEvent);
	   AppKeyHandler::Run();
   }
}
//-------------------------------------------------------------------------------
void TresholdsViewBtn(HWND h)
{
	zprint("");
	//DetailedWindow::Open();
	//app.mainWindow.viewers.get<ThicknessViewer>().openDetailedWindow = true;
	//RepaintWindow(DetailedWindow::Instance().hWnd);
}
//-----------------------------------------------------------------------------
void TestBtn(HWND)
{
	//SaveBitmap(L"tmp.jpg", app.mainWindow.viewers.get<CrossViewer>().backScreen);
}
//----------------------------------------------------------------------------
void StopMeashurement(HWND h)
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


