#pragma once
#include "TablesDefine.h"
#include "typelist.hpp"

#define ITEM(item, type, def)struct item\
{\
  typedef type type_value;\
  type_value value;\
  const type_value default_value;\
  item() : value(def), default_value(def){}\
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
	typedef TL::MkTlst<position, width, level>::Result items_list;
	TL::Factory<items_list> items;
};
struct gate1
{
	typedef TL::MkTlst<position, width, level, nb_alarm_level>::Result items_list;
	TL::Factory<items_list> items;
};
struct gate2
{
	typedef TL::MkTlst<position, width, level, nb_alarm_level>::Result items_list;
	TL::Factory<items_list> items;
};
struct scope
{
	typedef TL::MkTlst<offset, range, velocity>::Result items_list;
	TL::Factory<items_list> items;
};



struct LongParam
{
	typedef TL::MkTlst<gateIF, gate1, gate2, scope>::Result items_list;
	TL::Factory<items_list> items;
};
struct CrossParam
{
	typedef TL::MkTlst<gateIF, gate1, gate2, scope>::Result items_list;
	TL::Factory<items_list> items;
};
struct ThicknessParam
{
	typedef TL::MkTlst<gateIF, gate1, gate2, scope>::Result items_list;
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
	 
	 TL::Factory<items_list> items;
	 void Init();
	 void Save();
	 void Update();
 };
