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
namespace 
{
void SycleMeashurement(HWND);//обработчик для кнопки "Циклическое измерение"

void StopMeashurement(HWND);//обработчик для кнопки "Остановка измерения"

void TresholdsViewBtn(HWND);

void TestBtn(HWND);

#define TOOL_LIP_TEXT(name, text)struct name{static wchar_t *Text(){return text;}};

TOOL_LIP_TEXT(ToolLipCycleBtn     , L"Цикл")
TOOL_LIP_TEXT(ToolLipReset  	  , L"Стоп")
TOOL_LIP_TEXT(ToolTresholdsViewBtn  , L"Просмотр")
TOOL_LIP_TEXT(ToolTestBtn  , L"Тест")
  
#undef TOOL_LIP_TEXT

typedef TL::MkTlst<
  SeparatorToolbar<0>
  , ButtonToolbar<IDB_CycleBtn, SycleMeashurement, ToolLipCycleBtn>  
  , ButtonToolbar<IDB_Reset, StopMeashurement    , ToolLipReset>  
  , ButtonToolbar<IDB_QueryBtn, TestBtn             , ToolTestBtn>  
  //, ButtonToolbar<IDB_MashBtn, TresholdsViewBtn  , ToolTresholdsViewBtn>
  , SeparatorToolbar<1>
>::Result tool_button_list;
//----------------------------------------------------------------------------------
void SycleMeashurement(HWND h)
{
  SetEvent(App::ProgrammContinueEvent);
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
	zprint("");
	//if(automat.TestMode())
	//{
	//	app.mainWindow.viewers.get<ThicknessViewer>().Repaint();
	//}
}
//----------------------------------------------------------------------------
void StopMeashurement(HWND h)
{
	 SetEvent(App::ProgrammStopEvent);
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


