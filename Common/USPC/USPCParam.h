#pragma once
#include "TablesDefine.h"
#include "typelist.hpp"

template<class T>struct position		;
template<class T>struct width			;
template<class T>struct level			;
template<class T>struct nb_alarm_level	;
template<class T>struct offset			;
template<class T>struct range			;
template<class T>struct velocity		;

#define ITEM(name, sub, type, def)template<>struct name<sub>\
	{\
	typedef type type_value;\
	type_value value;\
	const type_value default_value;\
	name() : value(def), default_value(def){}\
	};

struct gateIF;
struct gate1 ;
struct gate2 ;
struct scope ;

ITEM(position      , gateIF, double, 5)
ITEM(width         , gateIF, double, 50)
ITEM(level         , gateIF, double, 100)
			  	    		  
ITEM(position      , gate1, double, 190)
ITEM(width         , gate1, double, 50)
ITEM(level         , gate1, double, 80)
ITEM(nb_alarm_level, gate1, double, 100)

ITEM(position      , gate2, double, 390)
ITEM(width         , gate2, double, 50)
ITEM(level         , gate2, double, 60)
ITEM(nb_alarm_level, gate2, double, 100)

ITEM(offset        , scope, double, 50)
ITEM(range         , scope, double, 80)
ITEM(velocity      , scope, double, 2000)

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
