#include "stdafx.h"
#include "ut_files.h"
#include <Shlwapi.h>
#include <direct.h>
#include "App.h"
#include "AppBase.h"
#include "Registry.h"

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
	//MkDir(ut_files);
	//CurrentDir src, dest;
	//wsprintf(
	//	&src.path[src.len]
	//, L"\\%s\\default.us"
	//	, ut_files
	//	);
	//wsprintf(
	//	&dest.path[dest.len]
	//, L"\\%s\\%s.us"
	//	, ut_files
	//	, file
	//	);
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
			wsprintf(buffer, L"����������� ���� %s", src);
			MessageBox(h, buffer, L"������!!!", MB_ICONHAND);
		}
		else if(80 == err)
		{
			wsprintf(buffer, L"���� ���������� %s", dest);
			MessageBox(h, buffer, L"������!!!", MB_ICONHAND);
		}
		else if(FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
                       NULL, 
                       err,
                       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                       buffer, 
                       1024, 
                       NULL))
		{
			 MessageBox(h, buffer, L"������!!!", MB_ICONHAND);
		}		
	}
	return b;
}

bool ExistCurrentUSPCFile(wchar_t (&file)[256])
{
	//CurrentDir src;
	//wchar_t *s = Singleton<ParametersTable>::Instance().items.get<NameParam>().value;
	//wsprintf(&src.path[src.len], L"\\%s\\%s.us"
	//	, ut_files
	//	, s
	//	);
	RegistryPathUTFile appPath;
	wchar_t src[256];
	wsprintf(src, L"%s\\%s.us", appPath(), file);
	bool b = 0 != PathFileExists(src);
	if(b)
	{
		wcscpy(file, src);
	}
	return b;
}
