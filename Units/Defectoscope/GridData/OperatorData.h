#pragma once
#include <vector>
#include "TableData.h"

class OperatorData: public TableData
{
	template<class, class>struct __load__;
	std::vector<std::wstring> data;
public:		
	OperatorData();
	void Init();
	void Add(wchar_t *);
	void Del(wchar_t *);
	const wchar_t *Cell(int, int);
	unsigned Count();
	void Header(HWND);
};