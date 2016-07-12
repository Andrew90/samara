#include "stdafx.h"
#include "Registry.h"

#pragma warning(disable: 4996)
RegistryPathLogFile::RegistryPathLogFile()
{
	RegOpenKeyEx (HKEY_LOCAL_MACHINE, 
	L"SOFTWARE\\Wow6432Node\\USPC7100\\Paths",NULL,KEY_READ,&hKey);
	DWORD size = 1024;
	RegQueryValueEx(
		hKey,
		L"Applications",
		NULL,
		NULL,
		(LPBYTE)buffer,
		&size
		);
	wcscat(buffer, L"USPC7100log.txt");
}
RegistryPathLogFile::~RegistryPathLogFile()
{
	RegCloseKey (hKey);
}
wchar_t *RegistryPathLogFile::operator()()
{
	return buffer;
}