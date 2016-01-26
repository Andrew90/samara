#include "stdafx.h"
#include "Dialogs.h"
#include "AppBase.h"
#include "TabWidgets.h"
#include "App.h"

namespace
{
	struct Items
	{
		template<class T> T &get(){return Singleton<T>::Instance();}
	};
    struct Tabs
	{
		typedef TL::MkTlst<
		InputBitTable
		 , OutputBitTable
		 , Descriptor1730Table
		 , DeadAreaTable
		>::Result items_list;
		Items items;
	};
}

PARAM_TITLE(InputBitTable, L"������� �����")
PARAM_TITLE(OutputBitTable, L"�������� �����")
PARAM_TITLE(Descriptor1730Table, L"���������� �����")
PARAM_TITLE(DeadAreaTable, L"̸����� ����")

void TestTabsDlg::Do(HWND h)
{
	Tabs t;
	 TabControl<Tabs>(t, 400, 230).Do(NULL, L"Tab");
}