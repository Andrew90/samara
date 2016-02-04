#pragma once
#include "TablesDefine.h"
#include "typelist.hpp"

#define ITEM(name, type, def)template<class T>struct name\
	{\
	typedef type type_value;\
	type_value value;\
	const type_value default_value;\
	name() : value(def), default_value(def){}\
	};

ITEM(position		, double, 10)
ITEM(width			, double, 10)
ITEM(level			, double, 10)
ITEM(nb_alarm_level , double, 10)
ITEM(offset			, double, 10)
ITEM(range			, double, 10)
ITEM(velocity		, double, 10)

#undef ITEM

struct gateIF
{
	typedef  TL::MkTlst<position<gateIF>, width<gateIF>, level<gateIF> >::Result items_list;
	TL::Factory<items_list> items;
};
struct gate1
{
	typedef  TL::MkTlst<position<gate1>, width<gate1>, level<gate1>, nb_alarm_level<gate1>>::Result items_list;
	TL::Factory<items_list> items;
};
struct gate2
{
	typedef  TL::MkTlst<position<gate2>, width<gate2>, level<gate2>, nb_alarm_level<gate2> >::Result items_list;
	TL::Factory<items_list> items;
};
struct scope
{
	typedef  TL::MkTlst<offset<scope>, range<scope>, velocity<scope>>::Result items_list;
	TL::Factory<items_list> items;
};

template<class T, int NUM>struct Unit
{
	typedef typename TL::MkTlst<gateIF, gate1, gate2, scope>::Result items_list;
	TL::Factory<items_list> items;
};

struct LongParam
{
	typedef TL::CreateWapperNumList<Unit, LongParam, 0, 7>::Result items_list;
	TL::Factory<items_list> items;
};
struct CrossParam
{
	typedef TL::CreateWapperNumList<Unit, CrossParam, 0, 7>::Result items_list;
	TL::Factory<items_list> items;
};
struct ThicknessParam
{
	typedef TL::CreateWapperNumList<Unit, ThicknessParam, 0, 7>::Result items_list;
	TL::Factory<items_list> items;
};

 class USPCParam
 {
 public:
	 typedef TL::MkTlst<
		 LongParam
		 , CrossParam
		 , ThicknessParam
	 >::Result items_list;
	 
	 typedef TL::Factory<items_list> TItems;
	 TItems items;
	 void Init();
	 void Save();
	 void Update();
 };
