// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <clocale>
#include "SelectMessage.h"
#include <Windows.h>
#include "typelist.hpp"
#include "DebugMess.h"


int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_CTYPE, "rus");
	dprint("\n\n\n\n");
	//Test();
	int x[][4] = {
		{ 9, 2, 3, -1 }
		, { 0, 0, 4, -1 }
		, { 0, 12, 1, -1 }
		, { 0, 3, 4, -1 }
		, { 5, 2, 4, -1 }
	};
	char *s;
	unsigned start = GetTickCount();
//	for (int i = 0; i < dimention_of(x); ++i)
	{
		int t = 10000;
		s = SelectMessage(x[0], t);
		printf("%s  %d\n", s, t);
	}

	
	getchar();
	return 0;
}

