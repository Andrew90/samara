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
//PARAM_TITLE(DeadAreaMM1, L"Конец трубы")

void AnalogPlateDlg::Do(HWND h)
{
	//wchar_t *appName = Singleton<PathUSPCTable>::Instance().items.get<PathUSPC>().value;
	//if(IsProcessRun(appName))
	//{
	//	MessageBox(h, L"Для продолжения настроек необходимо закрыть программу \"USPC\"", L"Предупреждение", MB_ICONEXCLAMATION);
	//	return;
	//}
	//wchar_t path[1024];
	//if(!ExistCurrentUSPCFile(path))
	//{
	//	MessageBox(h, L"Файла для настроек текущего типоразмера нет", L"Ошибка!!!", MB_ICONERROR);
	//	return;
	//}
	//if(ProcessRun(appName, path))
	//{
	//   return;
	//}
	//
	//MessageBox(h, L"Программа \"USPC\" не запущена, возможно необходимо настроить путь запуска программы", L"Ошибка!!!", MB_ICONERROR);

	//if(TemplDialog<PathUSPC>(Singleton<PathUSPC>::Instance()).Do(h, L"Путь запуска программы \"USPC\""))
	//{
	//}
}