// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <clocale>
#include "SelectMessage.h"
#include <Windows.h>
#include "typelist.hpp"


int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_CTYPE, "rus");
	//Test();
	int x[][4] = {
		{ 0, 12, 4, -1 }
		, { 0, 0, 4, -1 }
		, { 0, 12, 1, -1 }
		, { 0, 3, 4, -1 }
		, { 5, 2, 4, -1 }
	};
	char *s;
	unsigned start = GetTickCount();
	for (int i = 0; i < dimention_of(x); ++i)
	{
		s = SelectMessage(x[i]);
		printf("%s  \n", s);
	}

	
	getchar();
	return 0;
}

