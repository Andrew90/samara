#include "stdafx.h"
#include "Dialogs.h"
#include "ParamDlg.h"
#include "ParamDlg.hpp"
#include "AppBase.h"
#include "App.h"

MIN_EQUAL_VALUE(MedianFiltreWidth<Cross>, 3)
MAX_EQUAL_VALUE(MedianFiltreWidth<Cross>, 15)
PARAM_TITLE(MedianFiltreWidth<Cross>, L"������ ���������� ������ ���������� ��������")
PARAM_TITLE(MedianFiltreOn<Cross>, L"����������� ���������� ������� ���������� ��������")

MIN_EQUAL_VALUE(MedianFiltreWidth<Long>, 3)
MAX_EQUAL_VALUE(MedianFiltreWidth<Long>, 15)
PARAM_TITLE(MedianFiltreWidth<Long>, L"������ ���������� ������� ���������� ��������")
PARAM_TITLE(MedianFiltreOn<Long>, L"����������� ���������� ������� ���������� ��������")

MIN_EQUAL_VALUE(MedianFiltreWidth<Thickness>, 3)
MAX_EQUAL_VALUE(MedianFiltreWidth<Thickness>, 15)
PARAM_TITLE(MedianFiltreWidth<Thickness>, L"������ ���������� ������� �������")
PARAM_TITLE(MedianFiltreOn<Thickness>, L"����������� ���������� ������� �������")

void MedianFiltreDlg::Do(HWND h)
{
	if(TemplDialog<MedianFiltreTable>(Singleton<MedianFiltreTable>::Instance()).Do(h, L"��������� ������"))
	{
	}
}