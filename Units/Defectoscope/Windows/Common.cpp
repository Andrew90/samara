#include "stdafx.h"
#include "Common.h"

#include "ThicknessWindow.h"
#include "LongWindow.h"
#include "CrossWindow.h"

namespace
{
	typedef TL::MkTlst<
		ThicknessWindow
		, LongWindow
		, CrossWindow
		, ScanWindow
	>::Result list;

	template<class List>struct __destroy_window__;
	template<class Head, class Tail>struct __destroy_window__<Tlst<Head, Tail>>
	{
		void operator()()
		{
			Common::DestroyWindow<Head>();
			__destroy_window__<Tail>()();
		}
	};

	template<>struct __destroy_window__<NullType>{void operator()(){}};
}

void CloseAllWindows()
{
	__destroy_window__<list>()();
}