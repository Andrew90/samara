#pragma once
#include "tables.hpp"
#include "TablesDefine.h"
#include "App.h"
//-------------------------------------------------------------------------------------------
DEFINE_PARAM(CurrentID, int, 1)

struct CurrentParametersTable
{
	typedef TL::MkTlst<
		CurrentID
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"CurrentParametersTable";}
};
//------------------------------------------------------------------------------------------
template<class>struct Border2Class;
template<class>struct BorderDefect;

template<class>struct AboveBorder;
template<class>struct LowerBorder;
template<class>struct NominalBorder;

DEFINE_ARRAY_PARAM_WAPPER(Border2Class, Long, double, App::zonesCount, 20)
DEFINE_ARRAY_PARAM_WAPPER(BorderDefect, Long, double, App::zonesCount, 30)

DEFINE_ARRAY_PARAM_WAPPER(Border2Class, Cross, double, App::zonesCount, 40)
DEFINE_ARRAY_PARAM_WAPPER(BorderDefect, Cross, double, App::zonesCount, 60)

DEFINE_ARRAY_PARAM_WAPPER(AboveBorder  , Thickness, double, App::zonesCount, 3.0)
DEFINE_ARRAY_PARAM_WAPPER(LowerBorder  , Thickness, double, App::zonesCount, 2.0)
DEFINE_ARRAY_PARAM_WAPPER(NominalBorder, Thickness, double, App::zonesCount, 12.0)

struct ThresholdsTable
{
	typedef TL::MkTlst<
		Border2Class<Long>
		, BorderDefect<Long>
		, Border2Class<Cross>
		, BorderDefect<Cross>
		 , AboveBorder  <Thickness>
		 , LowerBorder  <Thickness>
		 , NominalBorder<Thickness>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ThresholdsTable";}
};

//----------------------------------------------------------------------------------------
DEFINE_PARAM(CommunicationRemoveUnit, int, 0)
DEFINE_PARAM(CounterTubesStored, int, 0)

struct DifferentOptionsTable
{
	typedef TL::MkTlst<
		CommunicationRemoveUnit
		, CounterTubesStored
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"DifferentOptionsTable";}
};
DEFINE_PARAM(OffsetAxesX, int, 100)
DEFINE_PARAM(FrameWidth, int, 400)
DEFINE_PARAM(MaxAxesY, double, 8191)
DEFINE_PARAM(MinAxesY, double, -8192)
struct GraphicSignalOptionsTable
{
	typedef TL::MkTlst<
		OffsetAxesX
		, FrameWidth
		, MaxAxesY
		, MinAxesY
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"GraphicSignalOptionsTable";}
};
struct PointsOptionsTable
{
	typedef TL::MkTlst<
		MaxAxesY
		, MinAxesY
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"PointsOptionsTable";}
};
//----------------------------------------------------------------------------------
DEFINE_PARAM(Undefined, int, 0xff555555)

DEFINE_PARAM(Nominal       , int, 0xff00ff00)
DEFINE_PARAM(Treshold2Class, int, 0xffffff00)
DEFINE_PARAM(Defect        , int, 0xffff0000)

DEFINE_PARAM(AboveNorm, int, 0xff0000ff)
DEFINE_PARAM(BelowNorm, int, 0xffff0000)
DEFINE_PARAM(BelowAboveNorm, int, 0xffff00ff)

DEFINE_PARAM(DefectAboveNorm     , int, 0xffff0000)
DEFINE_PARAM(DefectBelowNorm     , int, 0xffff0000)
DEFINE_PARAM(DefectBelowAboveNorm, int, 0xffff0000)

DEFINE_PARAM(Treshold2ClassAboveNorm     , int, 0xffffff00)
DEFINE_PARAM(Treshold2ClassBelowNorm     , int, 0xffff0000)
DEFINE_PARAM(Treshold2ClassBelowAboveNorm, int, 0xffff0000)


 
DEFINE_PARAM(DeathZone, int, 0xff333333)  

struct ColorTable
{
	typedef TL::MkTlst<	
		 Undefined		
		, AboveNorm
		, BelowNorm
		, BelowAboveNorm
		, Nominal
		, Defect
		, Treshold2Class
		, DeathZone
		, DefectAboveNorm     
		, DefectBelowNorm     
		, DefectBelowAboveNorm
		, Treshold2ClassAboveNorm     
		, Treshold2ClassBelowNorm     
		, Treshold2ClassBelowAboveNorm
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ColorTable";}
};

template<class T>inline int StatusId()
{
	return TL::IndexOf<ColorTable::items_list, T>::value;
}

template<class T>struct Stat
	{
		static const unsigned value = TL::IndexOf<ColorTable::items_list, T>::value;
	};
//-------------------------------------------------------------------------------------- 
DEFINE_PARAM(SupplySensorDelay, int, 150)
DEFINE_PARAM(RemoveSensorDelay, int,  150)
DEFINE_PARAM(FrequencyFrames, int,  100)

DEFINE_PARAM(ReferenceOffset1, unsigned, 1750)
DEFINE_PARAM(ReferenceOffset2, unsigned, 4600)

DEFINE_PARAM(ReferenceOffset3, unsigned, 1260)
DEFINE_PARAM(ReferenceOffset4, unsigned, 3030)
struct AdditionalSettingsTable
{
	typedef TL::MkTlst<	
		ReferenceOffset1
		, ReferenceOffset2
		, ReferenceOffset3
		, ReferenceOffset4
		, SupplySensorDelay
		, RemoveSensorDelay
		, FrequencyFrames
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"AdditionalSettingsTable";}
};
//--------------------------------------------------------------------------------------
DEFINE_PARAM(ACFBorderLeft, int, 80)
DEFINE_PARAM(ACFBorderRight, int, 160)
struct ACFBorderTable
{
	typedef TL::MkTlst<
		ACFBorderLeft
		, ACFBorderRight
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ACFBorderTable";}
};
//-----------------------------------------------------------------------------------------------------
 DEFINE_PARAM(DeadAreaMM0, int, 200)
 DEFINE_PARAM(DeadAreaMM1, int, 200)
 struct DeadAreaTable
 {
	typedef TL::MkTlst<
		DeadAreaMM0
		, DeadAreaMM1
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"DeadAreaTable";}
 };
//----------------------------------------------------------------------------------------------------
 template<class>struct MedianFiltreWidth;
 template<class>struct MedianFiltreOn;

 DEFINE_PARAM_WAPPER(MedianFiltreWidth, Cross, int, 5)
 DEFINE_PARAM_WAPPER(MedianFiltreOn, Cross, bool, true)

 DEFINE_PARAM_WAPPER(MedianFiltreWidth, Long, int, 5)
 DEFINE_PARAM_WAPPER(MedianFiltreOn, Long, bool, true)

 DEFINE_PARAM_WAPPER(MedianFiltreWidth, Thickness, int, 5)
 DEFINE_PARAM_WAPPER(MedianFiltreOn, Thickness, bool, true)
 
 struct MedianFiltreTable
 {
	 typedef TL::MkTlst<
		 MedianFiltreWidth<Cross>
		 , MedianFiltreOn<Cross> 
		 , MedianFiltreWidth<Long>
		 , MedianFiltreOn<Long> 
		 , MedianFiltreWidth<Thickness>
		 , MedianFiltreOn<Thickness> 
	 >::Result items_list;
	 typedef TL::Factory<items_list> TItems;
	 TItems items;
	 const wchar_t *name(){return L"MedianFiltreTable";}
 };
 //---------------------------------------------------------------------------------------------------------
 DEFINE_PARAM(NamePlate1730, int, 3)
 struct NamePlate1730ParametersTable
 {
	 typedef TL::MkTlst<
		 NamePlate1730
	 >::Result items_list;
	 typedef TL::Factory<items_list> TItems;
	 TItems items;
	 const wchar_t *name(){return L"NamePlate1730ParametersTable";}
 };
//-------------------------------------------------------------------------------------------------------
struct AxesTable;
DEFINE_PARAM_ID(ThresholdsTable            , int, 1)
DEFINE_PARAM_ID(DeadAreaTable			   , int, 1)
DEFINE_PARAM_ID(AxesTable	   , int, 1)
DEFINE_PARAM_ID(MedianFiltreTable, int, 1)
STR_PARAM(NameParam, 128, L"NONAME")
 struct ParametersTable
 {
	typedef TL::MkTlst<
		ID<ThresholdsTable>
		, ID<DeadAreaTable			   	>
		, ID<AxesTable	   	>
		, ID<MedianFiltreTable>
		, NameParam
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ParametersTable";}
 };
//--------------------------------------------------------------------------------------------------------
DEFINE_PARAM(SQ0            , unsigned, 1 << 0)
DEFINE_PARAM(SQ1            , unsigned, 1 << 1)
DEFINE_PARAM(SQ2            , unsigned, 1 << 2)
DEFINE_PARAM(SQ3            , unsigned, 1 << 3)
DEFINE_PARAM(SQ4            , unsigned, 1 << 4)

struct InputBitTable
 {
	typedef TL::MkTlst<
		SQ0, SQ1, SQ2, SQ3, SQ4
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"InputBitTable";}
 };

DEFINE_PARAM(Y0            , unsigned, 1 << 0)
DEFINE_PARAM(Y1            , unsigned, 1 << 1)
DEFINE_PARAM(Y2            , unsigned, 1 << 2)
DEFINE_PARAM(Y3            , unsigned, 1 << 3)
DEFINE_PARAM(Y4            , unsigned, 1 << 4)

struct OutputBitTable
 {
	typedef TL::MkTlst<
		Y0, Y1, Y2, Y3, Y4
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"OutputBitTable";}
 };

 DEFINE_PARAM(Descriptor1730, unsigned, 1)
struct Descriptor1730Table
 {
	typedef TL::MkTlst<
		Descriptor1730
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"Descriptor1730Table";}
 };
 //--------------------------------------------------------------------------------------------------------
///\brief �������� ������������� ��������
template<class T, int N>struct Offset;
DEFINE_PARAM_WAPPER_NUM(Offset, Long, 0, int, 0)
DEFINE_PARAM_WAPPER_NUM(Offset, Long, 1, int, 0)
DEFINE_PARAM_WAPPER_NUM(Offset, Long, 2, int, 0)
DEFINE_PARAM_WAPPER_NUM(Offset, Long, 3, int, 0)
DEFINE_PARAM_WAPPER_NUM(Offset, Long, 4, int, 0)
DEFINE_PARAM_WAPPER_NUM(Offset, Long, 5, int, 0)
DEFINE_PARAM_WAPPER_NUM(Offset, Long, 6, int, 0)
DEFINE_PARAM_WAPPER_NUM(Offset, Long, 7, int, 0)

DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 0, int, 0)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 1, int, 0)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 2, int, 0)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 3, int, 0)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 4, int, 0)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 5, int, 0)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 6, int, 0)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 7, int, 0)

DEFINE_PARAM_WAPPER_NUM(Offset, Thickness, 0, int, 0)
DEFINE_PARAM_WAPPER_NUM(Offset, Thickness, 1, int, 0)
DEFINE_PARAM_WAPPER_NUM(Offset, Thickness, 2, int, 0)
DEFINE_PARAM_WAPPER_NUM(Offset, Thickness, 3, int, 0)
DEFINE_PARAM_WAPPER_NUM(Offset, Thickness, 4, int, 0)
DEFINE_PARAM_WAPPER_NUM(Offset, Thickness, 5, int, 0)
DEFINE_PARAM_WAPPER_NUM(Offset, Thickness, 6, int, 0)
DEFINE_PARAM_WAPPER_NUM(Offset, Thickness, 7, int, 0)

struct OffsetsTable
{
	typedef TL::MultyListToList<TL::MkTlst<
		TL::CreateWapperNumList<Offset, Long, 0, 7>::Result
		, TL::CreateWapperNumList<Offset, Cross, 0, 7>::Result
		, TL::CreateWapperNumList<Offset, Thickness, 0, 7>::Result
	>::Result>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"OffsetsTable";}
};
//-----------------------------------------------------------------------------------------------------------
template<class T>struct AxesYMin;
template<class T>struct AxesYMax;
DEFINE_PARAM_WAPPER(AxesYMin, Long, double, 0)
DEFINE_PARAM_WAPPER(AxesYMax, Long, double, 150)
DEFINE_PARAM_WAPPER(AxesYMin, Cross, double, 0)
DEFINE_PARAM_WAPPER(AxesYMax, Cross, double, 150)
DEFINE_PARAM_WAPPER(AxesYMin, Thickness, double, 0)
DEFINE_PARAM_WAPPER(AxesYMax, Thickness, double, 15)

struct AxesTable
{
	typedef TL::MkTlst<
		AxesYMin<Long>
		, AxesYMax<Long>
		, AxesYMin<Cross>
		, AxesYMax<Cross>
		, AxesYMin<Thickness>
		, AxesYMax<Thickness>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"AxesTable";}
};
//-----------------------------------------------------------------------------------------------------------
template<class T>struct OnTheJob;
DEFINE_PARAM_WAPPER(OnTheJob, Cross    , bool, true)
DEFINE_PARAM_WAPPER(OnTheJob, Long     , bool, true)
DEFINE_PARAM_WAPPER(OnTheJob, Thickness, bool, true)

struct OnTheJobTable
{
	typedef TL::MkTlst<
		OnTheJob<Cross    >
		, OnTheJob<Long     >
		, OnTheJob<Thickness>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"OnTheJobTable";}
};
//-----------------------------------------------------------------------------------------------------------
 struct ParametersBase
 {
	 typedef TL::MkTlst<
		  NamePlate1730ParametersTable	  		
		 , PointsOptionsTable			
		 , GraphicSignalOptionsTable	   
		 , AdditionalSettingsTable
		 , ColorTable
		 , ACFBorderTable
		 , InputBitTable
		 , OutputBitTable
		 , Descriptor1730Table
		 , OffsetsTable
		 , OnTheJobTable
	 >::Result one_row_table_list;

	 typedef TL::MkTlst<
		  CurrentParametersTable		 
		 , ParametersTable			   
		 , ThresholdsTable			  
		 , DeadAreaTable	
		 , MedianFiltreTable
		 , AxesTable
	 >::Result multy_row_table_list;

	 typedef TL::MkTlst<
		  multy_row_table_list
		 , one_row_table_list
	 >::Result multy_type_list; 

	 typedef TL::MultyListToList<multy_type_list>::Result type_list;
	 typedef TL::Factory<type_list> TTables;
	 TTables tables;
	 wchar_t path[512];
	 const wchar_t *name();
 };

struct AppBase
{
	void Init();
	static void InitTypeSizeTables(CBase &);
};

template<class T>int CurrentId()
{
	 // CurrentParametersTable &current = Singleton<CurrentParametersTable>::Instance();
	 // Select<CurrentParametersTable>(base).ID(1).Execute(current);
	 // ParametersTable param;
	 // Select<ParametersTable>(base).ID(current.items.get<CurrentID>().value).Execute(param);
	  return Singleton<ParametersTable>::Instance().items.get<T>().value;
}

/*
template<class T>int CountId(CBase &base, int num)
{
	wchar_t buf[128];
	wsprintf(buf, L"SELECT COUNT(*) FROM ParametersTable WHERE %s=%d", T().name(), num);
	int t = base.ConnectionSQL(buf);
	return t;
}
*/
template<class T>void UpdateId(CBase &base, int num)
{
   CurrentParametersTable &current = Singleton<CurrentParametersTable>::Instance();
   Select<CurrentParametersTable>(base).ID(1).Execute(current);
   ParametersTable &t = Singleton<ParametersTable>::Instance();
   t.items.get<T>().value = num;
   UpdateWhere<ParametersTable>(t, base).ID(current.items.get<CurrentID>().value).Execute();
}
/*
template<class T>int CurrentId(CBase &base)
{
	  CurrentParametersTable &current = Singleton<CurrentParametersTable>::Instance();
	  Select<CurrentParametersTable>(base).ID(1).Execute(current);
	  ParametersTable param;
	  Select<ParametersTable>(base).ID(current.items.get<CurrentID>().value).Execute(param);
	  return param.items.get<T>().value;
}
*/
template<class T>int CountId(CBase &base, int num)
{
	ADODB::_RecordsetPtr rec;
	Select<ParametersTable>(base).eq<T>(num).Execute(rec);
	int i = 0;
	while (!rec->EndOfFile) 
	{			
		++i;
		rec->MoveNext(); 
	}
	return i;
}
/*
template<class T>void UpdateId(CBase &base, int num)
{
   CurrentParametersTable &current = Singleton<CurrentParametersTable>::Instance();
   Select<CurrentParametersTable>(base).ID(1).Execute(current);
   Update<ParametersTable>(base).set<T>(num).Where().ID(current.items.get<CurrentID>().value).Execute();
}
*/







