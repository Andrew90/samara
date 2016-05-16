#include "stdafx.h"
#include "Stored.h"
#include "Hash.h"
#include "ExpressBase.hpp"
#include "tables.hpp"
#include "StoredBase.h"
#include "Compute.h"
#include "App.h"
#include "MainWindow.h"
#include "USPCData.h"

//namespace
//{
//	///////todo заменить на реальный тип	начало
//	//class Compute
//	//{
//	//public:
//	//	int lengthTube;
//	//	Compute(){}
//	//	void LengthTube(unsigned startTime, unsigned baseTime, unsigned stopTime){}
//	//	void Recalculation(){}
//	//	//void Test();
//	//}  compute;
//
//	//struct MainWindow
//	//{
//	//	HWND hWnd;
//	//} mainWindow;
//
//	////todo заменить на реальный тип  конец
//}

namespace Stored
{
	int &lengthTube = compute.lengthTube;
	MainWindow &mainWindow = app.mainWindow;

	ItemData<Cross> &crossData = Singleton<ItemData<Cross>>::Instance();
	ItemData<Long> &longData = Singleton<ItemData<Long>>::Instance();
	ItemData<Thickness> &thicknessData = Singleton<ItemData<Thickness>>::Instance();

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

	void DataToFile(wchar_t *path)
	{
		 FILE *f = _wfopen(path, L"w");
		 if(NULL != f)
		 {
			 fwrite(&crossData.currentOffsetFrames, sizeof(int), 1, f);
			 fwrite(&longData.currentOffsetFrames, sizeof(int), 1, f);
		     fwrite(&thicknessData.currentOffsetFrames, sizeof(int), 1, f);
			 fwrite(&compute.lengthTube, sizeof(int), 1, f);

			 fwrite(crossData.ascanBuffer, sizeof(USPC7100_ASCANDATAHEADER), crossData.currentOffsetFrames, f);
			 fwrite(longData.ascanBuffer, sizeof(USPC7100_ASCANDATAHEADER), longData.currentOffsetFrames, f);
			 fwrite(thicknessData.ascanBuffer, sizeof(USPC7100_ASCANDATAHEADER), thicknessData.currentOffsetFrames, f);

			 fclose(f);
		 }
	}

	void DataFromFile(wchar_t *path)
	{
		 FILE *f = _wfopen(path, L"r");
		 if(NULL != f)
		 {
			 fread(&crossData.currentOffsetFrames, sizeof(int), 1, f);
			 fread(&longData.currentOffsetFrames, sizeof(int), 1, f);
		     fread(&thicknessData.currentOffsetFrames, sizeof(int), 1, f);
			 fread(&compute.lengthTube, sizeof(int), 1, f);

			 fread(crossData.ascanBuffer, sizeof(USPC7100_ASCANDATAHEADER), crossData.currentOffsetFrames, f);
			 fread(longData.ascanBuffer, sizeof(USPC7100_ASCANDATAHEADER), longData.currentOffsetFrames, f);
			 fread(thicknessData.ascanBuffer, sizeof(USPC7100_ASCANDATAHEADER), thicknessData.currentOffsetFrames, f);

			 fclose(f);
		 }
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
			return;
		}

		wchar_t *c = &path[len];

		wcscpy(c, L"..\\Stored\\");

		CreateDirectory(path, NULL);

		c = &c[wcslen(c)];

		wsprintf(c, L"%02d%02d%02d%02d%02d%02d.uspc"
			, tme.GetYear()	- 2000
			, tme.GetMonth()
			, tme.GetDay()
			, tme.GetHour()
			, tme.GetMinute()
			, tme.GetSecond()
			);

		DataToFile(path);
	}
}