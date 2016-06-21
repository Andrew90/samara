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

PARAM_TITLE(InputBitTable, L"������� �����")
PARAM_TITLE(OutputBitTable, L"�������� �����")
PARAM_TITLE(Descriptor1730Table, L"���������� �����")
PARAM_TITLE(DeadAreaTable, L"̸����� ����")

PARAM_TITLE(i�ontrol�ircuits, L"���� ����������")
PARAM_TITLE(iCycle          , L"����")
PARAM_TITLE(iReady          , L"����������")
PARAM_TITLE(iControl        , L"��������")
PARAM_TITLE(iBase           , L"����")
PARAM_TITLE(iReserve        , L"������")       

PARAM_TITLE(oWork      , L"������")
PARAM_TITLE(oToShiftThe, L"����������")
PARAM_TITLE(oResult1   , L"���������1")
PARAM_TITLE(oResult2   , L"���������2")
PARAM_TITLE(oPowerBM   , L"������� ��")
PARAM_TITLE(oReserve   , L"������")


PARAM_TITLE(Descriptor1730, L"���������� ����� 1730")
PARAM_TITLE(DeadAreaMM0, L"̸����� ���� ������ �����")
PARAM_TITLE(DeadAreaMM1, L"̸����� ���� ����� �����")

MIN_EQUAL_VALUE(i�ontrol�ircuits, 0)
MIN_EQUAL_VALUE(iCycle          , 0)
MIN_EQUAL_VALUE(iReady          , 0)
MIN_EQUAL_VALUE(iControl        , 0)
MIN_EQUAL_VALUE(iBase           , 0)
MIN_EQUAL_VALUE(iReserve        , 0)

MAX_EQUAL_VALUE(i�ontrol�ircuits, 31)
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