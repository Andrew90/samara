#include "stdafx.h"
#include "ut_files.h"
#include <Shlwapi.h>
#include <direct.h>
#include "App.h"
#include "AppBase.h"

namespace
{
	const wchar_t *ut_files = L"ut_files";
	struct CurrentDir
	{
		int len;
		wchar_t path[1024];
		CurrentDir()
		{
			DWORD length = GetModuleFileName( NULL, path, 1024);
			PathRemoveFileSpec(path);
			len = wcslen(path);
		}
	};
}

void MkDir(const wchar_t *dir)
{
	CurrentDir d;
	wsprintf(&d.path[d.len], L"\\%s", dir);
	_wmkdir(d.path);
}

bool NewUSPCFile(HWND h, wchar_t *file)
{
	MkDir(ut_files);
	CurrentDir src, dest;
	wsprintf(
		&src.path[src.len]
	, L"\\%s\\default.us"
		, ut_files
		);
	wsprintf(
		&dest.path[dest.len]
	, L"\\%s\\%s.us"
		, ut_files
		, file
		);
	bool b = true;
	if(0 == CopyFile(src.path, dest.path, true))
	{
		b = false;
		int err = GetLastError();		
		wchar_t buffer[1024];
		if(2 == err)
		{
			wsprintf(buffer, L"Отсутствует файл %s", src.path);
			MessageBox(h, buffer, L"Ошибка!!!", MB_ICONHAND);
		}
		else if(80 == err)
		{
			wsprintf(buffer, L"Файл существует %s", dest.path);
			MessageBox(h, buffer, L"Ошибка!!!", MB_ICONHAND);
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

bool ExistCurrentUSPCFile(wchar_t (&file)[1024])
{
	CurrentDir src;
	wchar_t *s = Singleton<ParametersTable>::Instance().items.get<NameParam>().value;
	wsprintf(&src.path[src.len], L"\\%s\\%s"
		, ut_files
		, s
		);
	bool b = 0 != PathFileExists(src.path);
	if(b)
	{
		wcscpy(file, src.path);
	}
	return b;
}