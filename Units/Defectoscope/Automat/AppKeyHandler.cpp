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
		>::Result list;
		__btn__<list>()();
	}

	void Run()
	{
		typedef TL::MkTlst<
			Off<IDB_CycleBtn>
			, On<IDB_Reset>
			, Off<IDB_QueryBtn>
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
		>::Result list;
		__btn__<list>()();
	}

	void VK_(unsigned id)
	{
		//SendMessage(app.mainWindow.hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
		//SetForegroundWindow(app.mainWindow.hWnd);
		TCommand c = {app.mainWindow.hWnd, WM_COMMAND, id, 0, app.mainWindow.toolBar.hWnd};
		SendMessage(MESSAGE(c));
	}

	void KeyPressed(unsigned key)
	{
		switch(key)
		{		
		case VK_F4:	VK_(IDB_CycleBtn); break;
		case VK_F9: VK_(IDB_Reset);break;
		}
	}
}