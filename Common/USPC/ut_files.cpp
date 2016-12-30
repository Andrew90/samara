#include "stdafx.h"
#include "USPC/ut_files.h"
#include <Shlwapi.h>
#include <direct.h>
#include "DiffApp/App.h"
#include "App/AppBase.h"
#include "Registry.h"
#include "App/Config.h"

namespace
{
	//const wchar_t *ut_files = L"ut_files";
	//struct CurrentDir
	//{
	//	int len;
	//	wchar_t path[1024];
	//	CurrentDir()
	//	{
	//		DWORD length = GetModuleFileName( NULL, path, 1024);
	//		PathRemoveFileSpec(path);
	//		len = wcslen(path);
	//	}
	//};
}

//void MkDir(const wchar_t *dir)
//{
//	CurrentDir d;
//	wsprintf(&d.path[d.len], L"\\%s", dir);
//	_wmkdir(d.path);
//}

bool NewUSPCFile(HWND h, wchar_t *file)
{
	RegistryPathUTFile appPath;
	wchar_t src[256];
	wchar_t dest[256];

	wsprintf(src, L"%s\\default.us", appPath());
	wsprintf(dest, L"%s\\%s.us", appPath(), file);

	bool b = true;
	if(0 == CopyFile(src, dest, true))
	{
		b = false;
		int err = GetLastError();		
		wchar_t buffer[1024];
		if(2 == err)
		{
			wsprintf(buffer, L"Отсутствует файл %s", src);
			MessageBox(h, buffer, L"Ошибка!!!", MB_ICONHAND);
		}
		else if(80 == err)
		{
			wsprintf(buffer, L"Файл существует %s", dest);
			MessageBox(h, buffer, L"Предупреждение!!!", MB_ICONHAND);
			return true;
		}
		else if(FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
                       NULL, 
                       err,
                       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                       buffer, 
                       1024, 
                       NULL))
		{
			 MessageBox(h, buffer, L"Ошибка!!!", MB_ICONHAND);
		}		
	}
	return b;
}

bool ExistCurrentUSPCFile(wchar_t (&file)[256])
{
	wchar_t *s = Singleton<ParametersTable>::Instance().items.get<NameParam>().value;
	RegistryPathUTFile appPath;
	wchar_t src[512];
	wsprintf(src, L"%s\\%s.us", appPath(), s);
#ifndef DEBUG_ITEMS
	bool b = 0 != PathFileExists(src);
#else
	bool b = true;
#endif
	if(b)
	{
		wcscpy(file, src);
	}
	else
	{
		wcscpy(file, L"Файл инициализации платы \"USPC7100\" отсутствует");
		wchar_t buf[1024];
		wsprintf(buf, L"Файл %s\nинициализации платы \"USPC7100\" отсутствует", src);
		MessageBox(App::MainWindowHWND(), buf, L"Ошибка!!!", MB_ICONERROR);
	}
	return b;
}
