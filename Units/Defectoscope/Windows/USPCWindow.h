#pragma once
#include "SelectTypeSizeList.h"
#include <windows.h>
#include "message.h"
#include "USPCWindowToolBar.h"
#include "DropDownItem.h"
#include "USPCChartViewer.h"
#include "USPCBottomPanel.h"

class USPCWindow
{
public:
	HWND hWnd;
	HWND hStatusWindow;
	USPCWindowToolBar toolBar;
	DropDown dropDownUnit;
	DropDown dropDownSensors;
	USPCChartViewer uspcChartViewer;
	USPCBottomPanel panel;
	USPCWindow();
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TMessage &);
	void operator()(TMouseWell &);
	static wchar_t *Title();
	//static void Do(HWND);
	//bool Destroy();
};

template<>struct Singleton<USPCWindow>
{
	static USPCWindow &Instance();
	//{
	//	  HWND hh = FindWindow(WindowClass<USPCWindow>()(), 0);
	//	  if(NULL == hh) throw L"Singleton<USPCWindow> error";
	//	  return *(USPCWindow *)GetWindowLong(hh, GWL_USERDATA);
	//}
};
