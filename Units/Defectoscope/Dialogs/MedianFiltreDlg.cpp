#include "stdafx.h"
#include "Dialogs.h"
#include "ParamDlg.h"
#include "ParamDlg.hpp"
#include "AppBase.h"
#include "App.h"

MIN_EQUAL_VALUE(MedianFiltreWidth, 3)
MAX_EQUAL_VALUE(MedianFiltreWidth, 15)
PARAM_TITLE(MedianFiltreWidth, L"������ ���������� �������")

PARAM_TITLE(MedianFiltreOn, L"����������� ���������� �������")

void MedianFiltreDlg::Do(HWND h)
{
	if(TemplDialog<MedianFiltreTable>(Singleton<MedianFiltreTable>::Instance()).Do(h, L"��������� ������"))
	{
	}
}