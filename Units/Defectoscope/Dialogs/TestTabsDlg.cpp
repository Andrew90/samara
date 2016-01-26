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

PARAM_TITLE(InputBitTable, L"Входные порты")
PARAM_TITLE(OutputBitTable, L"Выходные порты")
PARAM_TITLE(Descriptor1730Table, L"Дескриптор платы")
PARAM_TITLE(DeadAreaTable, L"Мёртвые зоны")

PARAM_TITLE(SQ0, L"SQ0")
PARAM_TITLE(SQ1, L"SQ1")
PARAM_TITLE(SQ2, L"SQ2")
PARAM_TITLE(SQ3, L"SQ3")

void TestTabsDlg::Do(HWND h)
{
	Tabs t;
	 TabControl<Tabs>(t, 400, 230).Do(NULL, L"Tab");
}