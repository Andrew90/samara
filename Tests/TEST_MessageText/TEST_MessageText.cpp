// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <clocale>
#include <Windows.h>
#include <typeinfo>
#include "D:\work\tmp\x\Samara170119\USPCSamara\Common\templates\typelist.hpp"
#include "GenTlst.h"
//#include "D:\work\tmp\x\Samara170119\USPCSamara\Tests\TEST_MessageText\MessageText\LabelMessage.h"

#define XX(n) struct _##n{};


XX(00)
XX(01)
XX(02)
XX(03)




#define TList1(_00)Tlst<_00,NullType>
#define TList2(_00,_01)Tlst<_00, Tlst<_01,NullType>>
#define TList3(_00,_01,_02)Tlst<_00,Tlst<_01,Tlst<_02,NullType>>>

#define dprint

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_CTYPE, "rus");
	dprint("\n\n\n\n");
	//Test();
	int x[] = { 0, 5, -1 };
		
	//char *s;
	unsigned start = GetTickCount();
//	for (int i = 0; i < dimention_of(x); ++i)
	{
		int t = 10000;
	//	printf("%s  %d\n", s, t);
	}

	typedef TList3(_00,_01,_02) list0;

	printf("%s\n", typeid(list0).name());

	GenTlst gen;
	gen(3);

	printf("%s\n", gen.buf);
	
	getchar();
	return 0;
}

