#pragma once
#include <vector>
#include "TableData.h"

class CustomerData: public TableData
{
	template<class, class>struct __load__;
	std::vector<std::wstring> data;
public:		
	CustomerData();
	void Init();
	void Add(wchar_t *);
	void Del(wchar_t *);
	const wchar_t *Cell(int, int);
	unsigned Count();
	void Header(HWND);
};