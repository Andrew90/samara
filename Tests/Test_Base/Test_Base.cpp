// Test_Base.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Stored.h"
#include "Base.hpp"

int _tmain(int argc, _TCHAR* argv[])
{
	Initialize initialize;

	//StoredBase parameters;
	//
	//CExpressBase base(
	//	parameters.name()
	//	, CreateDataBase<StoredBase::type_list, SetDefault<StoredBase::type_list> >()
	//	, parameters.tables
	//	);

	Stored::Do();
	
	getchar();
	return 0;
}

