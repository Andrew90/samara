#include "stdafx.h"
#include "Registry.h"

RegistryPathLogFile::RegistryPathLogFile()
{
	RegOpenKeyEx (HKEY_CURRENT_USER, 
	L"Software\\Microsoft\\AuthCookies\\Live\\Default\\DIDC",NULL,KEY_READ,&hKey);
	DWORD size = 1024;
	RegQueryValueEx(
		hKey,
		L"URL",
		NULL,
		NULL,
		(LPBYTE)buffer,
		&size
		);
}
RegistryPathLogFile::~RegistryPathLogFile()
{
	RegCloseKey (hKey);
}
wchar_t *RegistryPathLogFile::operator()()
{
	return buffer;
}