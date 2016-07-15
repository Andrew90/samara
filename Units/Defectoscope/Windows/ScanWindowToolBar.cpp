#include "stdafx.h"
#include "ScanWindowToolBar.h"
#include "../Resource.h"
#include "InitToolBar.hpp"
#include "DebugMess.h"

namespace
{
	template<int N>struct Key;

#define KEY(ID, text)template<>struct Key<ID> \
	{\
	static void Click(HWND);\
	static wchar_t *Text(){return text;}\
	};
	KEY(IDB_arrow_down , L"+ Датчик")
	KEY(IDB_arrow_up   , L"- Датчик")
	KEY(IDB_arrow_left , L"- Зона")
	KEY(IDB_arrow_right, L"+ Зона")
#undef KEY
#define BUTTON_KEY(ID)ButtonToolbar<ID, Key<ID> > 
		typedef TL::MkTlst<
		SeparatorToolbar<0>
		, BUTTON_KEY(IDB_arrow_down) 
		, BUTTON_KEY(IDB_arrow_up)
		, BUTTON_KEY(IDB_arrow_left) 
		, BUTTON_KEY(IDB_arrow_right)
		, SeparatorToolbar<2>
		>::Result tool_button_list;
#undef BUTTON_KEY
	void Key<IDB_arrow_down>::Click(HWND h)
	{
		zprint("\n");	
	}
//------------------------------------------------------------------------------
	void Key<IDB_arrow_up>::Click(HWND h)
	{
		zprint("\n");
	}
//------------------------------------------------------------------------------
	void Key<IDB_arrow_left>::Click(HWND h)
	{
		zprint("\n");	
	}
//------------------------------------------------------------------------------
	void Key<IDB_arrow_right>::Click(HWND h)
	{
		zprint("\n");	
	}
}

HWND ScanWindowToolBar::Init(HWND h)
{
	return hWnd = InitToolbar<tool_button_list>()(h);
}
void ScanWindowToolBar::Size()
{
	MoveWindow(hWnd, 0, 0, 0, 0, false);
}
