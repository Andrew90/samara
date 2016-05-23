#pragma once

#include "typelist.hpp"
#include "TablesDefine.h"
#include "tables.hpp"
#include "AppBase.h"

DEFINE_PARAM(Hash, unsigned, -1)

struct Treshold
{
	typedef double type_value[App::zonesCount];
	type_value value;	
	const wchar_t *name(){return L"Treshold";}
};

struct StoredThresholdsTable
{
	typedef TL::MkTlst<
		Hash
		, Treshold
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"StoredThresholdsTable";}
};

DEFINE_PARAM(Unit, unsigned, -1)
DEFINE_PARAM(Tube, unsigned, -1)
DEFINE_PARAM(Sensor, unsigned, -1)
struct Data
{
	typedef double type_value[App::zonesCount];
	type_value value;	
	const wchar_t *name(){return L"Data";}
};
struct Status
{
	typedef unsigned char type_value[App::zonesCount];
	type_value value;	
	const wchar_t *name(){return L"Status";}
};
struct StoredMeshureTable
{
	typedef TL::MkTlst<
		Unit
		, Tube
		, Sensor
		, Data
		, Status
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"StoredMeshureTable";}
};

DEFINE_PARAM(Date_Time, COleDateTime, 0.0)
DEFINE_PARAM(LengthTube, int, 120)

DEFINE_PARAM_WAPPER2(ID, BorderKlass2 , Cross    , unsigned, 0)
DEFINE_PARAM_WAPPER2(ID, BorderDefect , Cross    , unsigned, 0)
DEFINE_PARAM_WAPPER2(ID, BorderKlass2 , Long     ,  unsigned, 0)
DEFINE_PARAM_WAPPER2(ID, BorderDefect , Long     ,  unsigned, 0)
DEFINE_PARAM_WAPPER2(ID, BorderNominal, Thickness, unsigned, 0)
DEFINE_PARAM_WAPPER2(ID, BorderAbove  , Thickness, unsigned, 0)
DEFINE_PARAM_WAPPER2(ID, BorderLower  , Thickness, unsigned, 0)

STR_PARAM(Operator, 128, L"someone");
DEFINE_PARAM_WAPPER(ID, Operator, unsigned, 0)
STR_PARAM(Customer, 128, L"something");
DEFINE_PARAM_WAPPER(ID, Customer, unsigned, 0)

STR_PARAM(NumberPacket, 128, L"00000000");
DEFINE_PARAM_WAPPER(ID, NumberPacket, unsigned, 0)

struct TubesTable
{
	typedef TL::MkTlst<
		Date_Time
		, LengthTube
		, ID<BorderKlass2 < Cross    > >
		, ID<BorderDefect < Cross    > >
		, ID<BorderKlass2 < Long     > >	
		, ID<BorderDefect < Long     > >	
		, ID<BorderNominal< Thickness> >
		, ID<BorderAbove  < Thickness> >
		, ID<BorderLower  < Thickness> >
		, ID<Operator>
		, ID<Customer>
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"TubeTable";}
};

struct OperatorsTable
{
	typedef TL::MkTlst<
		Operator
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"OperatorsTable";}
};

struct CustomersTable
{
	typedef TL::MkTlst<
		Customer
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"CustomersTable";}
};

struct NumberPacketsTable
{
	typedef TL::MkTlst<
		NumberPacket
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"NumberPacketsTable";}
};

struct StoredBase
 {
	 typedef TL::MkTlst<		
		 TubesTable
		 , OperatorsTable
		 , CustomersTable
		 , StoredThresholdsTable
		 , StoredMeshureTable
		// , NumberPacketsTable не удалять
	 >::Result type_list;
	 typedef TL::Factory<type_list> TTables;
	 TTables tables;
	 const wchar_t *name(){return L"StoredBase";}
 };



