#include "stdafx.h"
#include "USPCWindowToolBar.h"
#include "Config.h"
#include "DebugMess.h"
#include "../Resource.h"
#include "InitToolBar.hpp"
#include "message.h"
#include "Emptywindow.h"
#include "USPCWindow.h"

namespace
{
void SycleMeashurement(HWND);//обработчик для кнопки "Циклическое измерение"

void StopMeashurement(HWND);//обработчик для кнопки "Остановка измерения"

void TresholdsViewBtn(HWND);

void TestBtn(HWND);

#define TOOL_LIP_TEXT(name, text)struct name{static wchar_t *Do(){return text;}};

TOOL_LIP_TEXT(ToolLipCycleBtn     , L"Старт")
TOOL_LIP_TEXT(ToolLipReset  	  , L"Стоп")
TOOL_LIP_TEXT(ToolTresholdsViewBtn  , L"Просмотр")
TOOL_LIP_TEXT(ToolTestBtn  , L"Тест")
  
#undef TOOL_LIP_TEXT

typedef TL::MkTlst<
  SeparatorToolbar<0>
  , ButtonToolbar<IDB_CycleBtn, SycleMeashurement, ToolLipCycleBtn>  
  , ButtonToolbar<IDB_Reset, StopMeashurement    , ToolLipReset>  
  //, ButtonToolbar<IDB_QueryBtn, TestBtn             , ToolTestBtn>  
  //, ButtonToolbar<IDB_MashBtn, TresholdsViewBtn  , ToolTresholdsViewBtn>
  , SeparatorToolbar<1>
>::Result tool_button_list;
//----------------------------------------------------------------------------------
void SycleMeashurement(HWND h)
{
  zprint("\n");
}
//-------------------------------------------------------------------------------
void TresholdsViewBtn(HWND h)
{
	zprint("\n");
	//DetailedWindow::Open();
	//app.mainWindow.viewers.get<ThicknessViewer>().openDetailedWindow = true;
	//RepaintWindow(DetailedWindow::Instance().hWnd);
}
//-----------------------------------------------------------------------------
void TestBtn(HWND)
{
	zprint("\n");
	//if(automat.TestMode())
	//{
	//	app.mainWindow.viewers.get<ThicknessViewer>().Repaint();
	//}
}
//----------------------------------------------------------------------------
void StopMeashurement(HWND h)
{
	 zprint("\n");
}
}
//--------------------------------------------------------------------------------------------
HWND USPCWindowToolBar::Init(HWND h)
{
	 return hWnd = InitToolbar<tool_button_list>()(h);
}
//-------------------------------------------------------------------
void USPCWindowToolBar::Size()
{
	MoveWindow(hWnd, 0, 0, 0, 0, false);
}
//----------------------------------------------------------------------------
const int USPCWindowToolBar::Width()
{
	return __tool_bar_width__<tool_button_list>::value;
}
//------------------------------------------------------------------------


