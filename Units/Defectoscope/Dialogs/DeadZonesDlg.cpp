#include "stdafx.h"
#include "Dialogs.h"
#include "ParamDlg.h"
#include "ParamDlg.hpp"
#include "AppBase.h"
#include "App.h"

MIN_EQUAL_VALUE(DeadAreaMM0<Cross>, 0)
MAX_EQUAL_VALUE(DeadAreaMM0<Cross>, 500)
PARAM_TITLE(    DeadAreaMM0<Cross>, L"Поперечник начало трубы")

MIN_EQUAL_VALUE(DeadAreaMM1<Cross>, 0)
MAX_EQUAL_VALUE(DeadAreaMM1<Cross>, 500)
PARAM_TITLE(    DeadAreaMM1<Cross>, L"Поперечник конец трубы")

MIN_EQUAL_VALUE(DeadAreaMM0<Long>, 0)
MAX_EQUAL_VALUE(DeadAreaMM0<Long>, 500)
PARAM_TITLE(    DeadAreaMM0<Long>, L"Продольник начало трубы")
							
MIN_EQUAL_VALUE(DeadAreaMM1<Long>, 0)
MAX_EQUAL_VALUE(DeadAreaMM1<Long>, 500)
PARAM_TITLE(    DeadAreaMM1<Long>, L"Продольник конец трубы")

MIN_EQUAL_VALUE(DeadAreaMM0<Thickness>, 0)
MAX_EQUAL_VALUE(DeadAreaMM0<Thickness>, 500)
PARAM_TITLE(    DeadAreaMM0<Thickness>, L"Толщиномер начало трубы")
							
MIN_EQUAL_VALUE(DeadAreaMM1<Thickness>, 0)
MAX_EQUAL_VALUE(DeadAreaMM1<Thickness>, 500)
PARAM_TITLE(    DeadAreaMM1<Thickness>, L"Толщиномер конец трубы")

void DeadZonesDlg::Do(HWND h)
{
	if(TemplDialog<ParametersBase, DeadAreaTable>(Singleton<DeadAreaTable>::Instance()).Do(h, L"Мёртвые зоны"))
	{
	}
}