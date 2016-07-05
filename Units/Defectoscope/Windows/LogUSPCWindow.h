#pragma once
#include "message.h"
#include "LogUSPCWindowGrid.h"
#include "LogUSPC.h"

class LogUSPCWindow
{
	int lastIndex;
	LogUSPCWindowGrid mainWindowGrid;
public:
	LogUSPC data;
public:
	static const int IDT_TIMER1 = 999;
	HWND hWnd;
	LogUSPCWindow();
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	unsigned operator()(TNotify &);
	static void Open();
};
