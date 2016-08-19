#include "stdafx.h"
#include <Windows.h>
#include "Lib.h"

void Lib::Do(wchar_t *path)
{
	STARTUPINFO si = {sizeof(si)};
	PROCESS_INFORMATION pi;
	CreateProcess(NULL, L"ViewerData", NULL, NULL, FALSE, 0, NULL, NULL,&si, &pi);
}
