#include "stdafx.h"
#include "Dialogs.h"
#include "ParamDlg.h"
#include "ParamDlg.hpp"
#include "AppBase.h"
#include "App.h"
//#include "Process.h"
//#include "ut_files.h"


PARAM_TITLE(PathUSPC, L"")
DO_NOT_CHECK(PathUSPC)
//
//MIN_EQUAL_VALUE(DeadAreaMM1, 0)
//MAX_EQUAL_VALUE(DeadAreaMM1, 500)
//PARAM_TITLE(DeadAreaMM1, L"����� �����")

void AnalogPlateDlg::Do(HWND h)
{
	//wchar_t *appName = Singleton<PathUSPCTable>::Instance().items.get<PathUSPC>().value;
	//if(IsProcessRun(appName))
	//{
	//	MessageBox(h, L"��� ����������� �������� ���������� ������� ��������� \"USPC\"", L"��������������", MB_ICONEXCLAMATION);
	//	return;
	//}
	//wchar_t path[1024];
	//if(!ExistCurrentUSPCFile(path))
	//{
	//	MessageBox(h, L"����� ��� �������� �������� ����������� ���", L"������!!!", MB_ICONERROR);
	//	return;
	//}
	//if(ProcessRun(appName, path))
	//{
	//   return;
	//}
	//
	//MessageBox(h, L"��������� \"USPC\" �� ��������, �������� ���������� ��������� ���� ������� ���������", L"������!!!", MB_ICONERROR);

	//if(TemplDialog<PathUSPC>(Singleton<PathUSPC>::Instance()).Do(h, L"���� ������� ��������� \"USPC\""))
	//{
	//}
}