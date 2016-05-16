#include "stdafx.h"
#include "Stored.h"
#include "Hash.h"
#include "ExpressBase.hpp"
#include "tables.hpp"
#include "StoredBase.h"
//#include "typelist.hpp"

namespace
{
	///////todo заменить на реальный тип	начало
	class Compute
{
public:
	int lengthTube;
	Compute(){}
	void LengthTube(unsigned startTime, unsigned baseTime, unsigned stopTime){}
	void Recalculation(){}
	//void Test();
}  compute;

	struct MainWindow
	{
		HWND hWnd;
	} mainWindow;

	////todo заменить на реальный тип  конец

   int &lengthTube = compute.lengthTube;
}

namespace Stored
{
	unsigned StoreThreshold(CBase &base, double(&data)[App::zonesCount])
	{
		unsigned hash = HashFAQ6((unsigned char *)data, sizeof(data));
		unsigned id = Select<StoredThresholdsTable>(base).eq<Hash>(hash).Execute();
		if(0 == id)
		{
			StoredThresholdsTable table;
			table.items.get<Hash>().value = hash;
			memmove(table.items.get<Treshold>().value, data, sizeof(data));
			Insert_Into<StoredThresholdsTable>(table, base).Execute();
			id = Select<StoredThresholdsTable>(base).eq<Hash>(hash).Execute();
		}
		return id;
	}

	void StoredDataToFile(wchar_t *path)
	{

	}

	template<class O, class P>struct __stored__{void operator()(O *, P *){}};

	template<template<class>class W, class X, class P>struct __stored__<ID<W<X>>, P>
	{
		typedef ID<W<X>> O;
		void operator()(O *o, P *p)
		{
			o->value = StoreThreshold(*p, Singleton<W<X>>::Instance().value);
		}
	};

	void Do()
	{
		COleDateTime tme = COleDateTime::GetCurrentTime();
		TubesTable tt;

		tt.items.get<Date_Time>().value = tme;
		tt.items.get<LengthTube>().value = lengthTube;

		wchar_t path[1024];
		GetModuleFileName(0, path, dimention_of(path));
		PathRemoveFileSpec(path);
		int len = wcslen(path);

		StoredBase parameters;
		
		CExpressBase base(
		//CBase base(
			parameters.name()
			, CreateDataBase<StoredBase::type_list, SetDefault<StoredBase::type_list>, MSsql>()
			, parameters.tables
			);

		if(base.IsOpen())
		{
			TL::foreach<TubesTable::items_list, __stored__>()(&tt.items, &base);
		}
		else
		{
			MessageBox(mainWindow.hWnd, L"Не могу открыть базу", L"Ошибка !!!", MB_ICONERROR);
		}
	}
}