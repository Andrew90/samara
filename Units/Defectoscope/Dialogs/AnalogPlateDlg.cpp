#include "stdafx.h"
#include "Dialogs.h"
#include "ParamDlg.h"
#include "ParamDlg.hpp"
#include "AppBase.h"
#include "App.h"
#include "ProcessRun.h"
#include "ut_files.h"


PARAM_TITLE(PathUSPC, L"")
DO_NOT_CHECK(PathUSPC)
template<int N>struct DlgSubItems<PathUSPC, Holder<N> >: EditItems<PathUSPC, 420>{};

void AnalogPlateDlg::Do(HWND h)
{
	wchar_t *appName = Singleton<PathUSPCTable>::Instance().items.get<PathUSPC>().value;
	if(IsProcessRun(appName))
	{
		MessageBox(h, L"��� ����������� �������� ���������� ������� ��������� \"USPC\"", L"��������������", MB_ICONEXCLAMATION);
		return;
	}
	wchar_t path[256];
	if(!ExistCurrentUSPCFile(path))
	{
		MessageBox(h, L"����� ��� �������� �������� ����������� ���", L"������!!!", MB_ICONERROR);
		return;
	}
	if(ProcessRun(appName, path))
	{
	   return;
	}
	
	MessageBox(h, L"��������� \"USPC\" �� ��������, �������� ���������� ��������� ���� ������� ���������", L"������!!!", MB_ICONERROR);

	if(TemplDialog<PathUSPCTable>(Singleton<PathUSPCTable>::Instance()).Do(h, L"���� ������� ��������� \"USPC\""))
	{
	}
}