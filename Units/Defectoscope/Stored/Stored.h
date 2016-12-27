#pragma once
#include "App.h"

class CBase;
namespace Stored
{
	bool Do();
	void DataToFile(wchar_t *path);
	bool DataFromFile(wchar_t *path);
	void RemoveNULLTables(CBase &);
}