#include "stdafx.h"
#include "Resource.h"
#include "App.h"
#include "typelist.hpp"
#include "MainWindow.h"
#include "DebugMess.h"

namespace AppKeyHandler
{
	template<int ID>struct On {static const int ID = ID; static const bool state = true;};
	template<int ID>struct Off{static const int ID = ID; static const bool state = false;};
	template<class List>struct __btn__;
	template<class Head, class Tail>struct __btn__<Tlst<Head, Tail>>
	{
		void operator()()
		{
			SendMessage(app.mainWindow.toolBar.hWnd, TB_ENABLEBUTTON, Head::ID, MAKELONG(Head::state, 0));
			__btn__<Tail>()();
		}
	};

	template<>struct __btn__<NullType>{void operator()(){}};

	bool IsEnabled(int id)
	{
		return 0 != SendMessage(app.mainWindow.toolBar.hWnd, TB_ISBUTTONENABLED, id, 0);//MAKELONG(enable, 0));
	}

	void Init()
	{
		typedef TL::MkTlst<
			On<IDB_CycleBtn>
			, On<IDB_Reset>
			, On<IDB_QueryBtn>
			, On<IDB_arrow_down>
			, On<IDB_arrow_up>
			, On<IDB_arrow_left>
			, On<IDB_arrow_right>
		>::Result list;
		__btn__<list>()();
	}

	void Run()
	{
		typedef TL::MkTlst<
			Off<IDB_CycleBtn>
			, On<IDB_Reset>
			, Off<IDB_QueryBtn>
			, Off<IDB_arrow_down>
			, Off<IDB_arrow_up>
			, Off<IDB_arrow_left>
			, Off<IDB_arrow_right>
		>::Result list;
		__btn__<list>()();
	}
	bool IsEnabledRun()
	{		
		return IsEnabled(IDB_CycleBtn);
	}
	void Continue()
	{
		typedef TL::MkTlst<
			On<IDB_CycleBtn>
			, On<IDB_Reset>
		>::Result list;
		__btn__<list>()();
	}
	void Stop()
	{
		typedef TL::MkTlst<
			On<IDB_CycleBtn>
			, On<IDB_Reset>
			, On<IDB_QueryBtn>
			, On<IDB_arrow_down>
			, On<IDB_arrow_up>
			, On<IDB_arrow_left>
			, On<IDB_arrow_right>
		>::Result list;
		__btn__<list>()();
	}

	bool VK_(unsigned id)
	{
		TCommand c = {app.mainWindow.hWnd, WM_COMMAND, id, 0, app.mainWindow.toolBar.hWnd};
		SendMessage(MESSAGE(c));
		return true;
	}

	void KeyPressed(unsigned key)
	{
		switch(key)
		{		
		case VK_F4:	IsEnabled(IDB_CycleBtn) && VK_(IDB_CycleBtn); break;
		case VK_F9: VK_(IDB_Reset);break;
		case VK_F5: IsEnabled(IDB_arrow_down) && VK_(IDB_arrow_down);break;
		case VK_F6: IsEnabled(IDB_arrow_up) && VK_(IDB_arrow_up);break;
		case VK_F7: IsEnabled(IDB_arrow_left) && VK_(IDB_arrow_left);break;
		case VK_F8: IsEnabled(IDB_arrow_right) && VK_(IDB_arrow_right);break;
		}
	}
}