#include "stdafx.h"
#include "Dialogs.h"
#include "ParamDlg.h"
#include "ParamDlg.hpp"
#include "AppBase.h"
#include "App.h"

MIN_EQUAL_VALUE(DeadAreaMM0, 0)
MAX_EQUAL_VALUE(DeadAreaMM0, 500)
PARAM_TITLE(DeadAreaMM0, L"Начало трубы")

MIN_EQUAL_VALUE(DeadAreaMM1, 0)
MAX_EQUAL_VALUE(DeadAreaMM1, 500)
PARAM_TITLE(DeadAreaMM1, L"Конец трубы")

void DeadZonesDlg::Do(HWND h)
{
	if(TemplDialog<DeadAreaTable>(Singleton<DeadAreaTable>::Instance()).Do(h, L"Мёртвые зоны"))
	{
	}
}