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

PARAM_TITLE(iСontrolСircuits, L"Цепи управления")
PARAM_TITLE(iCycle          , L"Цикл")
PARAM_TITLE(iReady          , L"Готовность")
PARAM_TITLE(iControl        , L"Контроль")
PARAM_TITLE(iBase           , L"База")
PARAM_TITLE(iReserve        , L"Резерв")       

PARAM_TITLE(oWork      , L"Работа")
PARAM_TITLE(oToShiftThe, L"Перекладка")
PARAM_TITLE(oResult1   , L"Результат1")
PARAM_TITLE(oResult2   , L"Результат2")
PARAM_TITLE(oPowerBM   , L"Питание БМ")
PARAM_TITLE(oReserve   , L"Резерв")


PARAM_TITLE(Descriptor1730, L"Дескриптор платы 1730")
PARAM_TITLE(DeadAreaMM0, L"Мёртвая зона начало трубы")
PARAM_TITLE(DeadAreaMM1, L"Мёртвая зона конец трубы")

MIN_EQUAL_VALUE(iСontrolСircuits, 0)
MIN_EQUAL_VALUE(iCycle          , 0)
MIN_EQUAL_VALUE(iReady          , 0)
MIN_EQUAL_VALUE(iControl        , 0)
MIN_EQUAL_VALUE(iBase           , 0)
MIN_EQUAL_VALUE(iReserve        , 0)

MAX_EQUAL_VALUE(iСontrolСircuits, 31)
MAX_EQUAL_VALUE(iCycle          , 31)
MAX_EQUAL_VALUE(iReady          , 31)
MAX_EQUAL_VALUE(iControl        , 31)
MAX_EQUAL_VALUE(iBase           , 31)
MAX_EQUAL_VALUE(iReserve        , 31)


MIN_EQUAL_VALUE(oWork      , 0)
MIN_EQUAL_VALUE(oToShiftThe, 0)
MIN_EQUAL_VALUE(oResult1   , 0)
MIN_EQUAL_VALUE(oResult2   , 0)
MIN_EQUAL_VALUE(oPowerBM   , 0)
MIN_EQUAL_VALUE(oReserve   , 0)

MAX_EQUAL_VALUE(oWork      , 31)
MAX_EQUAL_VALUE(oToShiftThe, 31)
MAX_EQUAL_VALUE(oResult1   , 31)
MAX_EQUAL_VALUE(oResult2   , 31)
MAX_EQUAL_VALUE(oPowerBM   , 31)
MAX_EQUAL_VALUE(oReserve   , 31)

DO_NOT_CHECK(Descriptor1730)

MIN_EQUAL_VALUE(DeadAreaMM0, 0 )
MAX_EQUAL_VALUE(DeadAreaMM0, 500)
MIN_EQUAL_VALUE(DeadAreaMM1, 0 )
MAX_EQUAL_VALUE(DeadAreaMM1, 500)
}

void TestTabsDlg::Do(HWND h)
{
	//Tabs t;
	//TabControl<ParametersBase, Tabs>(h, t, 400, 230).Do(L"Tab");
}