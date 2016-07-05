#pragma once
#include <Windows.h>

class RegistryPathLogFile
{
	HKEY hKey;
	wchar_t buffer[1024];
public:
	RegistryPathLogFile();
	~RegistryPathLogFile();
	wchar_t *operator()();
};