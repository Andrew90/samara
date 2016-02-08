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
	void operator()(TDestroy &);
	void operator()(TMessage &);
	void operator()(TKeyDown &);
	void operator()(TRButtonDown &);
	void operator()(TMouseWell &);
	static void Do(HWND);
	bool Destroy();
	static USPCWindow &Instance();
};
