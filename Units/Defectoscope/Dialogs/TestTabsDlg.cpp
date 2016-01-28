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

PARAM_TITLE(InputBitTable, L"Входные порты")
PARAM_TITLE(OutputBitTable, L"Выходные порты")
PARAM_TITLE(Descriptor1730Table, L"Дескриптор платы")
PARAM_TITLE(DeadAreaTable, L"Мёртвые зоны")

PARAM_TITLE(SQ0, L"SQ0")
PARAM_TITLE(SQ1, L"SQ1")
PARAM_TITLE(SQ2, L"SQ2")
PARAM_TITLE(SQ3, L"SQ3")
PARAM_TITLE(SQ4, L"SQ4")

PARAM_TITLE(Y0, L"Y0")
PARAM_TITLE(Y1, L"Y1")
PARAM_TITLE(Y2, L"Y2")
PARAM_TITLE(Y3, L"Y3")
PARAM_TITLE(Y4, L"Y4")

PARAM_TITLE(Descriptor1730, L"Дескриптор платы 1730")
PARAM_TITLE(DeadAreaMM0, L"Мёртвая зона начало трубы")
PARAM_TITLE(DeadAreaMM1, L"Мёртвая зона конец трубы")

MIN_EQUAL_VALUE(SQ0, 0 )
MAX_EQUAL_VALUE(SQ0, 31)
MIN_EQUAL_VALUE(SQ1, 0 )
MAX_EQUAL_VALUE(SQ1, 31)
MIN_EQUAL_VALUE(SQ2, 0 )
MAX_EQUAL_VALUE(SQ2, 31)
MIN_EQUAL_VALUE(SQ3, 0 )
MAX_EQUAL_VALUE(SQ3, 31)
MIN_EQUAL_VALUE(SQ4, 0 )
MAX_EQUAL_VALUE(SQ4, 31)

MIN_EQUAL_VALUE(Y0, 0 )
MAX_EQUAL_VALUE(Y0, 31)
MIN_EQUAL_VALUE(Y1, 0 )
MAX_EQUAL_VALUE(Y1, 31)
MIN_EQUAL_VALUE(Y2, 0 )
MAX_EQUAL_VALUE(Y2, 31)
MIN_EQUAL_VALUE(Y3, 0 )
MAX_EQUAL_VALUE(Y3, 31)
MIN_EQUAL_VALUE(Y4, 0 )
MAX_EQUAL_VALUE(Y4, 31)

DO_NOT_CHECK(Descriptor1730)

MIN_EQUAL_VALUE(DeadAreaMM0, 0 )
MAX_EQUAL_VALUE(DeadAreaMM0, 500)
MIN_EQUAL_VALUE(DeadAreaMM1, 0 )
MAX_EQUAL_VALUE(DeadAreaMM1, 500)
}

void TestTabsDlg::Do(HWND h)
{
	Tabs t;
	TabControl<Tabs>(h, t, 400, 230).Do(L"Tab");
}