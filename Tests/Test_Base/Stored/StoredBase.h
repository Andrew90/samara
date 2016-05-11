#pragma once

#include "typelist.hpp"
#include "TablesDefine.h"
#include "tables.hpp"
#include "App.h"
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

DEFINE_PARAM(DateTime, COleDateTime, 0.0)
DEFINE_PARAM(LengthTube, int, 120)

DEFINE_PARAM_WAPPER2(ID, BorderKlass2 , Cross    , unsigned, 0)
DEFINE_PARAM_WAPPER2(ID, BorderDefect , Cross    , unsigned, 0)
DEFINE_PARAM_WAPPER2(ID, BorderKlass2 , Long     ,  unsigned, 0)
DEFINE_PARAM_WAPPER2(ID, BorderDefect , Long     ,  unsigned, 0)
DEFINE_PARAM_WAPPER2(ID, BorderNominal, Thickness, unsigned, 0)
DEFINE_PARAM_WAPPER2(ID, BorderAbove  , Thickness, unsigned, 0)
DEFINE_PARAM_WAPPER2(ID, BorderLower  , Thickness, unsigned, 0)

struct TubeTable
{
	typedef TL::MkTlst<
		DateTime
		, LengthTube
		, ID<BorderKlass2 < Cross    > >
		, ID<BorderDefect < Cross    > >
		, ID<BorderKlass2 < Long     > >	
		, ID<BorderDefect < Long     > >	
		, ID<BorderNominal< Thickness> >
		, ID<BorderAbove  < Thickness> >
		, ID<BorderLower  < Thickness> >
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"TubeTable";}
};

struct StoredBase
 {
	 typedef TL::MkTlst<
		StoredThresholdsTable
		, TubeTable
	 >::Result type_list;
	 typedef TL::Factory<type_list> TTables;
	 TTables tables;
	 wchar_t path[512];
	 const wchar_t *name(){return L"StoredBase";}
 };



