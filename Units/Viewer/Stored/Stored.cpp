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
#include "ProtocolTable.h"
#include "PacketBase.h"
#include "..\..\Defectoscope\App\VersionDiff.h"

namespace Stored
{
	int &lengthTube = compute.lengthTube;
	MainWindow &mainWindow = app.mainWindow;

	ItemData<Cross> &crossData = Singleton<ItemData<Cross>>::Instance();
	ItemData<Long> &longData = Singleton<ItemData<Long>>::Instance();
	ItemData<Thickness> &thicknessData = Singleton<ItemData<Thickness>>::Instance();

	unsigned StoreThreshold(CBase &base, double(&data)[App::count_zones])
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
	void __Store__(USPC7100_ASCANDATAHEADER *d, int count, FILE *f)
	{
		for(int i = 0; i < count; ++i)
	    	fwrite(&d[i], sizeof(USPC7100_ASCANDATAHEADER), 1, f);
	}
	void DataToFile(wchar_t *path)
	{
		 FILE *f = _wfopen(path, L"wb");
		 if(NULL != f)
		 {
			 fwrite(&crossData.currentOffsetFrames, sizeof(int), 1, f);
			 fwrite(&longData.currentOffsetFrames, sizeof(int), 1, f);
		     fwrite(&thicknessData.currentOffsetFrames, sizeof(int), 1, f);
			 fwrite(&compute.lengthTube, sizeof(int), 1, f);
			 __Store__(crossData.ascanBuffer    , crossData.currentOffsetFrames, f);
			 __Store__(longData.ascanBuffer     , longData.currentOffsetFrames, f);
			 __Store__(thicknessData.ascanBuffer, thicknessData.currentOffsetFrames, f);

			 fclose(f);
			 int t = sizeof(USPC7100_ASCANDATAHEADER) * (
				 crossData.currentOffsetFrames
				 + longData.currentOffsetFrames
				 + thicknessData.currentOffsetFrames
				 );
		 }
	}

	void __Load__(USPC7100_ASCANDATAHEADER *d, int count, FILE *f)
	{
		for(int i = 0; i < count; ++i)
	    	fread(&d[i], sizeof(USPC7100_ASCANDATAHEADER), 1, f);
	}

	bool DataFromFile(wchar_t *path)
	{
		 FILE *f = _wfopen(path, L"rb");
		 if(NULL != f)
		 {
			 fread(&crossData.currentOffsetFrames, sizeof(int), 1, f);
			 if(Version::LoadFromFile(crossData.currentOffsetFrames, f))
			 {
				 fread(&crossData.currentOffsetFrames, sizeof(int), 1, f);
			 }
			 fread(&longData.currentOffsetFrames, sizeof(int), 1, f);
		     fread(&thicknessData.currentOffsetFrames, sizeof(int), 1, f);
			 if((App::count_frames < crossData.currentOffsetFrames
				 || App::count_frames < longData.currentOffsetFrames
				 || App::count_frames < thicknessData.currentOffsetFrames
				 )
				 && crossData.currentOffsetFrames > 0
				 &&	longData.currentOffsetFrames > 0
				 &&	thicknessData.currentOffsetFrames > 0
				 )
			 {
				 crossData.currentOffsetFrames = 0;
				 longData.currentOffsetFrames = 0;
				 thicknessData.currentOffsetFrames = 0;
				 compute.lengthTube = 0;

				 MessageBox(App::MainWindowHWND(), L"Файл имеет неизвесный формат", L"Ошибка !!!", MB_ICONERROR);
				 fclose(f);
				 return false;
			 }
			 fread(&compute.lengthTube, sizeof(int), 1, f);
			 __Load__(crossData.ascanBuffer, crossData.currentOffsetFrames, f);
			 __Load__(longData.ascanBuffer, longData.currentOffsetFrames, f);
			 __Load__(thicknessData.ascanBuffer, thicknessData.currentOffsetFrames, f);

			 fclose(f);
			 return true;
		 }
		 return false;
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

	template<class Table, class Data>struct __get_id__
	{
		unsigned operator()(CBase &base, Data &data)
		{
			unsigned id = Select<Table>(base).eq<Data>(data.value).Execute();
			if(0 == id)
			{
				Table t;
				t.items.get<Data>().value = data.value;
				Insert_Into<Table>(t, base).Execute();
				id = Select<Table>(base).eq<Data>(data.value).Execute();
			}
			return id;
		}
	};

	unsigned StoredStatus(CBase &base)
	{
		StoredMeshureTable t;
		int len = lengthTube / App::zone_length;
		len *= App::zone_length;
		t.items.get<LengthTube>().value = len;

		TL::foreach<StoredMeshureTable::items_list, __stored__>()(&t.items, &base);

		ResultViewerData &unit = Singleton<ResultViewerData>::Instance();
		memmove(t.items.get<Status>().value, unit.commonStatus, sizeof(t.items.get<Status>().value));

		Insert_Into<StoredMeshureTable>(t, base).Execute();
        unsigned id = 0;
		CMD(base).CommandText(
			L"SELECT max([ID]) as ID_LAST FROM [StoredBase].[dbo].[StoredMeshureTable]"
			).GetValue(L"ID_LAST", id);
		return id;
	}

	struct DeleteLast
	{
		unsigned count;
		wchar_t *path;
		wchar_t *offsPath;
		CBase *base;
		DeleteLast()
			: count(0)
			, path(path)
			, offsPath(offsPath)
		{}
		void operator()(CBase &, wchar_t *path, wchar_t *offsPath);
	} deleteLast;

	void Path(wchar_t *path, COleDateTime tme)
	{
		wsprintf(path, L"%02d%02d%02d%02d%02d%02d.dat"
			, tme.GetYear()	- 2000
			, tme.GetMonth()
			, tme.GetDay()
			, tme.GetHour()
			, tme.GetMinute()
			, tme.GetSecond()
			);
	}
	struct __delete_data__
	{
		unsigned id;
		CExpressBase &base;
		__delete_data__(CExpressBase &base) : id(0), base(base){}
	};
	template<class T, class D>struct __delete__
	{
		bool operator()(unsigned id, T &t, D &d)
		{
			d.id = id;
			Delete<StoredMeshureTable>(d.base).ID(t.items.get<ID<StoredMeshureTable> >().value).Execute();
			return true;
		}
	};

	bool Do()
	{
		COleDateTime tme = COleDateTime::GetCurrentTime();
		
		wchar_t path[1024];
		GetModuleFileName(0, path, dimention_of(path));
		PathRemoveFileSpec(path);
		int len = wcslen(path);

		StoredBase parameters;
		
		CExpressBase base(
			parameters.name()
			, CreateDataBase<StoredBase::type_list, NullType, MSsql>()
			, parameters.tables
			);

		if(base.IsOpen())
		{
			TubesTable tt;
			tt.items.get<Date_Time>().value = tme;
			tt.items.get<ID<Operator>>().value = __get_id__<OperatorsTable, Operator>()(base, Singleton<Operator>::Instance());
			unsigned protocolID = GetProtocolID(base);
			tt.items.get<ID<ProtocolsTable>>().value = protocolID;

			tt.items.get<ID<StoredMeshureTable>>().value = StoredStatus(base);
			wchar_t *num = Singleton<NumberTube>::Instance().value;
			tt.items.get<NumberTube>().value = Singleton<NumberTube>::Instance().value;

			__delete_data__ data(base);
			int id = Select<TubesTable>(base)
				.eq<NumberTube>(Singleton<NumberTube>::Instance().value)
				.eq<ID<ProtocolsTable>>(protocolID)
				.ExecuteLoop<__delete__>(data);

			if(0 == data.id)
			{
				Insert_Into<TubesTable>(tt, base).Execute();
			}
			else
			{
				UpdateWhere<TubesTable>(tt, base).ID(data.id).Execute();
			}
			dprint("database stored\n");
		}
		else
		{
			MessageBox(mainWindow.hWnd, L"Не могу открыть базу", L"Ошибка !!!", MB_ICONERROR);
			return false;
		}

		wchar_t *c = &path[len];

		wcscpy(c, L"\\..\\Stored\\");

		CreateDirectory(path, NULL);

		c = &c[wcslen(c)];
		Path(c, tme);
		DataToFile(path);

		deleteLast(base, path, c);
		return true;
	}

	struct __list_data__
	{
		wchar_t path[1024];
	};
template<class Table, class Data>struct __list__
{
	bool operator()(unsigned id, Table &t, Data &d)
	{		
		Path(d.offsPath, t.items.get<Date_Time>().value);
		DeleteFile(d.path);
		return false;
	}
};
#if 0
void DeleteLast::operator()(CBase &b, wchar_t *path_, wchar_t *offsPath_)
{
	path = path_;
	offsPath= offsPath_;
	base = &b;
	if(count > 10000)
	{
		COleDateTime tme;
		CMD(b).CommandText(
			L"SELECT max([Date_Time]) as TME FROM [StoredBase].[dbo].[TubeTable]"\
			L"WHERE [Date_Time] "\
			L"IN (SELECT TOP(100)[Date_Time] FROM [StoredBase].[dbo].[TubeTable] ORDER BY [Date_Time] ASC)"
			).GetValue(L"TME", tme);
		Select<TubesTable>(b).op<Date_Time>(L"<", tme).ExecuteLoop<__list__>(*this);

		//unsigned id = 0;
		//CMD(b).CommandText(
		//	L"SELECT ID FROM [StoredBase].[dbo].[TubeTable] WHERE Date_Time=?"
		//	).Param(tme).GetValue(L"ID", id);
		//
		//CMD(b).CommandText(
		//	L"DELETE FROM [StoredBase].[dbo].[StoredMeshureTable] WHERE Tube<?"
		//	).Param(id).Execute();

		CMD(b).CommandText(
			L"DELETE FROM [StoredBase].[dbo].[StoredMeshureTable] WHERE Tube IN(SELECT ID FROM [StoredBase].[dbo].[TubeTable] WHERE Date_Time<?)"
			).Param(tme).Execute();

		CMD(b).CommandText(
			L"DELETE FROM [StoredBase].[dbo].[ProtocolsTable] WHERE ID IN(SELECT IDProtocolsTable FROM [StoredBase].[dbo].[TubeTable] WHERE Date_Time<?)"
			).Param(tme).Execute();

		CMD(b).CommandText(
			L"DELETE FROM [StoredBase].[dbo].[TubeTable] WHERE Date_Time<?"
			).Param(tme).Execute();

		CMD(b).CommandText(L"SELECT count([Date_Time]) as C FROM [StoredBase].[dbo].[TubeTable]").GetValue(L"C", count);
		return;
	}
	if(count > 0)
	{
		++count;
	}
	else
	{
		CMD(b).CommandText(L"SELECT count([Date_Time]) as C FROM [StoredBase].[dbo].[TubeTable]").GetValue(L"C", count);
	}
}
#else	
#pragma message("Проверить удаление при заполнении базы данных")
void DeleteLast::operator()(CBase &b, wchar_t *path_, wchar_t *offsPath_)
{
	path = path_;
	offsPath= offsPath_;
	base = &b;
	if(count > 10000)
	{
		COleDateTime tme;
		CMD(b).CommandText(
			L"SELECT max([Date_Time]) as TME FROM [StoredBase].[dbo].[TubeTable]"\
			L"WHERE [Date_Time] "\
			L"IN (SELECT TOP(100)[Date_Time] FROM [StoredBase].[dbo].[TubeTable] ORDER BY [Date_Time] ASC)"
			).GetValue(L"TME", tme);
		Select<TubesTable>(b).op<Date_Time>(L"<", tme).ExecuteLoop<__list__>(*this);

		CMD(b).CommandText(
			L"DELETE FROM [StoredBase].[dbo].[TubeTable] WHERE Date_Time<?"
			).Param(tme).Execute();

		CMD(b).CommandText(
			L"DELETE a"\
			L" FROM [StoredBase].[dbo].[StoredMeshureTable] AS a"\
			L" LEFT JOIN [StoredBase].[dbo].[TubeTable] AS b"\
			L" ON a.ID = b.IDStoredMeshureTable"\
			L" WHERE b.ID IS NULL"
			).Execute();

		CMD(b).CommandText(
			L"DELETE a"\
			L" FROM [StoredBase].[dbo].[ProtocolsTable] AS a"\
			L" LEFT JOIN [StoredBase].[dbo].[TubesTable] AS b"\
			L" ON a.ID = b.IDProtocolsTable"\
			L" WHERE b.IDProtocolsTable IS NULL"
			).Execute();

		CMD(b).CommandText(L"SELECT count([Date_Time]) as C FROM [StoredBase].[dbo].[TubesTable]").GetValue(L"C", count);
		return;
	}
	if(count > 0)
	{
		++count;
	}
	else
	{
		CMD(b).CommandText(L"SELECT count([Date_Time]) as C FROM [StoredBase].[dbo].[TubesTable]").GetValue(L"C", count);
	}
}
#endif
}