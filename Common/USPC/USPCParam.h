#pragma once
#include "TablesDefine.h"
#include "typelist.hpp"

#define LOOP(n, val)\
  DEFINE_PARAM_NUM(n, 0, double, val)\
  DEFINE_PARAM_NUM(n, 1, double, val)\
  DEFINE_PARAM_NUM(n, 2, double, val)\
  DEFINE_PARAM_NUM(n, 3, double, val)\
  DEFINE_PARAM_NUM(n, 4, double, val)\
  DEFINE_PARAM_NUM(n, 5, double, val)\
  DEFINE_PARAM_NUM(n, 6, double, val)\
  DEFINE_PARAM_NUM(n, 7, double, val)

template<int N>struct gateIF_position     ;
template<int N>struct gateIF_width		  ;
template<int N>struct gate1_position      ; 
template<int N>struct gate1_width		  ;
template<int N>struct gate2_position  	  ;
template<int N>struct gate2_width		  ;
template<int N>struct gateIF_level 		  ;
template<int N>struct gate1_level		  ;
template<int N>struct gate2_level		  ;
template<int N>struct gate1_nb_alarm_level;
template<int N>struct gate2_nb_alarm_level;
template<int N>struct scope_offset		  ;
template<int N>struct scope_range		  ;
template<int N>struct scope_velocity      ; 

LOOP(gateIF_position     , 10)
LOOP(gateIF_width		 , 10)
LOOP(gate1_position		 , 10)
LOOP(gate1_width		 , 10)
LOOP(gate2_position  	 , 10)
LOOP(gate2_width		 , 10)
LOOP(gateIF_level 		 , 10)
LOOP(gate1_level		 , 10)
LOOP(gate2_level		 , 10)
LOOP(gate1_nb_alarm_level, 10)
LOOP(gate2_nb_alarm_level, 10)
LOOP(scope_offset		 , 10)
LOOP(scope_range		 , 10)
LOOP(scope_velocity		 , 10)

#undef LOOP

struct LongParamTable
{
	typedef TL::MultyListToList<TL::MkTlst<
	      TL::CreateNumList<gateIF_position     , 0, 7>::Result
		, TL::CreateNumList<gateIF_width		, 0, 7>::Result
		, TL::CreateNumList<gate1_position		, 0, 7>::Result
		, TL::CreateNumList<gate1_width		 	, 0, 7>::Result
		, TL::CreateNumList<gate2_position  	, 0, 7>::Result
		, TL::CreateNumList<gate2_width		 	, 0, 7>::Result
		, TL::CreateNumList<gateIF_level 		, 0, 7>::Result
		, TL::CreateNumList<gate1_level		 	, 0, 7>::Result
		, TL::CreateNumList<gate2_level		 	, 0, 7>::Result
		, TL::CreateNumList<gate1_nb_alarm_level, 0, 7>::Result
		, TL::CreateNumList<gate2_nb_alarm_level, 0, 7>::Result
		, TL::CreateNumList<scope_offset		, 0, 7>::Result
		, TL::CreateNumList<scope_range		 	, 0, 7>::Result
		, TL::CreateNumList<scope_velocity		, 0, 7>::Result
	>::Result>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"LongParamTable";}
};

struct CrossParamTable
{
	typedef TL::MultyListToList<TL::MkTlst<
	      TL::CreateNumList<gateIF_position     , 0, 7>::Result
		, TL::CreateNumList<gateIF_width		, 0, 7>::Result
		, TL::CreateNumList<gate1_position		, 0, 7>::Result
		, TL::CreateNumList<gate1_width		 	, 0, 7>::Result
		, TL::CreateNumList<gate2_position  	, 0, 7>::Result
		, TL::CreateNumList<gate2_width		 	, 0, 7>::Result
		, TL::CreateNumList<gateIF_level 		, 0, 7>::Result
		, TL::CreateNumList<gate1_level		 	, 0, 7>::Result
		, TL::CreateNumList<gate2_level		 	, 0, 7>::Result
		, TL::CreateNumList<gate1_nb_alarm_level, 0, 7>::Result
		, TL::CreateNumList<gate2_nb_alarm_level, 0, 7>::Result
		, TL::CreateNumList<scope_offset		, 0, 7>::Result
		, TL::CreateNumList<scope_range		 	, 0, 7>::Result
		, TL::CreateNumList<scope_velocity		, 0, 7>::Result
	>::Result>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"CrossParamTable";}
};

 struct ThicknessParamTable
{
	typedef TL::MultyListToList<TL::MkTlst<
	      TL::CreateNumList<gateIF_position     , 0, 7>::Result
		, TL::CreateNumList<gateIF_width		, 0, 7>::Result
		, TL::CreateNumList<gate1_position		, 0, 7>::Result
		, TL::CreateNumList<gate1_width		 	, 0, 7>::Result
		, TL::CreateNumList<gate2_position  	, 0, 7>::Result
		, TL::CreateNumList<gate2_width		 	, 0, 7>::Result
		, TL::CreateNumList<gateIF_level 		, 0, 7>::Result
		, TL::CreateNumList<gate1_level		 	, 0, 7>::Result
		, TL::CreateNumList<gate2_level		 	, 0, 7>::Result
		, TL::CreateNumList<gate1_nb_alarm_level, 0, 7>::Result
		, TL::CreateNumList<gate2_nb_alarm_level, 0, 7>::Result
		, TL::CreateNumList<scope_offset		, 0, 7>::Result
		, TL::CreateNumList<scope_range		 	, 0, 7>::Result
		, TL::CreateNumList<scope_velocity		, 0, 7>::Result
	>::Result>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ThicknessParamTable";}
};
