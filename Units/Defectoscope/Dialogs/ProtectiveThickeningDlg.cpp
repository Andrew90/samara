#include "stdafx.h"
#include "Dialogs.h"
#include "ParamDlg.h"
#include "ParamDlg.hpp"
#include "AppBase.h"
#include "DiffApp/App.h"

DO_NOT_CHECK(N0)
PARAM_TITLE(N0, L"������ �������")

DO_NOT_CHECK(N1)
PARAM_TITLE(N1, L"������ �������")

DO_NOT_CHECK(N2)
PARAM_TITLE(N2, L"������ �������")
							
DO_NOT_CHECK(N3)
PARAM_TITLE(N3, L"�������� �������")

void ProtectiveThickeningDlg::Do(HWND h)
{
	if(TemplDialog<ParametersBase, ProtectiveThickeningTable>(Singleton<ProtectiveThickeningTable>::Instance())
		.Do(h, L"������� ���� �����"))
	{
	}
}