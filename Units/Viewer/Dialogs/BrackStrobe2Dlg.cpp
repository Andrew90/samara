#include "stdafx.h"
#include "Dialogs.h"
#include "ParamDlg.h"
#include "ParamDlg.hpp"
#include "AppBase.h"
#include "App.h"

namespace
{
DO_NOT_CHECK(BrackStrobe2)
PARAM_TITLE(BrackStrobe2, L"Брак по стробу(mm)")
}

void BrackStrobe2Dlg::Do(HWND h)
{
	if(TemplDialog<ParametersBase, BrackStrobe2Table>(Singleton<BrackStrobe2Table>::Instance()).Do(h, L"Брак по стробу 2"))
	{
	}
}