#include "StdAfx.h"
#include "SelectMessage.h"
#include "App/AppBase.h"
#include "MessageText\LabelMessage.h"
#include "debug_tools/DebugMess.h"
#include "App/AppBase.h"

namespace SelectMesageN
{
	template<class >struct __selectItem__
	{
		bool value;
		__selectItem__() : value(false){}
	};
	template<>struct __selectItem__<Undefined>
	{
		bool value;
		__selectItem__() : value(true){}
	};
	typedef	TL::MkTlst<
		/*0*/	Undefined   
		/*1*/	, DeathZone
		/*2*/	, Nominal	
		/*3*/	, BorderAbove<Thickness> 
		/*4*/	, BorderLower<Thickness> 
		/*5*/	, BorderKlass2<Long>  
		/*6*/	, BorderDefect<Long> 
		/*7*/	, BorderKlass2<Cross>
		/*8*/	, BorderDefect<Cross>
		/*9*/   , BrakStrobe2<Thickness>
		/*10*/, Cancel<Projectionist>
	>::Result  __selected_list__;

	template<class O, class P>struct __set__
	{
		void operator()(P &p)
		{
			p.get<__selectItem__<O>>().value = true;
		}
	};

	template<class O, class P>struct __skip__
	{
		void operator()(P &p)
		{
			if(p.get<__selectItem__<O>>().value)
			{
				p.get<__selectItem__<Undefined>>().value = false;
				p.get<__selectItem__<Nominal>>().value = false;
			}
		}
	};
	template<class P>struct __skip__<Nominal, P>
	{
		void operator()(P &p)
		{
			if(p.get<__selectItem__<Nominal>>().value)
			{
				p.get<__selectItem__<Undefined>>().value = false;
			}
		}
	};
	template<class P>struct __skip__<Undefined, P>
	{
		void operator()(P &p)
		{
		}
	};
	template<class X, class P>struct __skip__<BorderDefect<X>, P>
	{
		void operator()(P &p)
		{
			if(p.get<__selectItem__<BorderDefect<X>>>().value)
			{
				p.get<__selectItem__<Undefined>>().value = false;
				p.get<__selectItem__<Nominal>>().value = false;
				p.get<__selectItem__<BorderKlass2<X>>>().value = false;
			}
		}
	};

	template<class O, class P>struct __select__
	{
		bool operator()(P &p)
		{
			if(TL::IndexOf<label_message_list, O>::value == p.id)
			{
				TL::foreach<typename O::items_list, __set__>()(p.items);
				return false;
			}
			return true;
		}
	};

	template<class List, class T>struct __in_list__;

	template<class Head, class Tail, class T>struct __in_list__<Tlst<Head, Tail>, T>
	{
		static const bool value = __in_list__<Tail, T>::value;
	};

	template<class Tail, class T>struct __in_list__<Tlst<T, Tail>, T>
	{
		static const bool value = true;
	};

	template<class T>struct __in_list__<NullType, T>
	{
		static const bool value = false;
	};

	template<class List, class T>struct __filtre__;

	template<class Head, class Tail, class T>struct __filtre__<Tlst<Head, Tail>, T>
	{
		typedef typename TL::_if<__in_list__<typename Head::items_list, T>::value
			, Tlst<Head, typename __filtre__<Tail, T>::Result>
			, typename __filtre__<Tail, T>::Result
		>::Result Result;
	};

	template<class T>struct __filtre__<NullType, T>
	{
		typedef NullType Result;
	};

	template<class List, class T>struct __filtre_off__;

	template<class Head, class Tail, class T>struct __filtre_off__<Tlst<Head, Tail>, T>
	{
		typedef typename TL::_if<!__in_list__<typename Head::items_list, T>::value
			, Tlst<Head, typename __filtre_off__<Tail, T>::Result>
			, typename __filtre_off__<Tail, T>::Result
		>::Result Result;
	};

	template<class T>struct __filtre_off__<NullType, T>
	{
		typedef NullType Result;
	};

	template<class T>struct __data__
	{
		typedef T items_list;
		int &res;
		__data__(int &res): res(res){}
	};

	template<class List>struct __get_id__;
	template<class Head, class Tail>struct __get_id__<Tlst<Head, Tail>>
	{
		template<class O, class P>void operator()(O &o, P &p)
		{
			if(o.get<__selectItem__<Head>>().value)
			{
				typedef __filtre__<typename P::items_list, Head>::Result list;
				__get_id__<Tail>()(o, (__data__<list> &)p);
			}
			else
			{
				typedef __filtre_off__<typename P::items_list, Head>::Result list;
				__get_id__<Tail>()(o, (__data__<list> &)p);
			}
		}
	};

	template<class List>struct __sel_first__;
	template<class Head, class Tail>struct __sel_first__<Tlst<Head, Tail>>
	{
		static const int value = TL::IndexOf<label_message_list, Head>::value;
	};
	template<>struct __sel_first__<NullType>
	{
		static const int value = -1;
	};

	template<>struct __get_id__<NullType>
	{
		template<class O, class P>void operator()(O &o, P &p)
		{
			p.res = __sel_first__<typename P::items_list>::value;
		}
	};

	struct __result_data__
	{
		int id;
		typedef TL::ListToWapperList<__selected_list__, __selectItem__>::Result items_list;
		TL::Factory<items_list> items;
	};

	template<class List, int start, int stop>struct SubList
	{
		typedef Tlst<typename TL::TypeAt<List, start>::Result, typename SubList<List, 1 + start, stop>::Result> Result;
	};

	template<class List, int stop>struct SubList<List, stop, stop>
	{
		typedef Tlst<typename TL::TypeAt<List, stop>::Result, NullType> Result;
	};		

	int Result(int *x)
	{
		__result_data__ data;

		while(-1 != *x)
		{
			data.id = *x;
			TL::find<label_message_list, __select__>()(data);
			x = &x[1];
		}
		int res = 0;
		TL::foreach<__selected_list__, __skip__>()(data.items);
		__get_id__<__selected_list__>()(data.items, __data__<label_message_list>(res));
		return res;
	}
}

char *SelectMessage(int *x, int &res)
{
	res = SelectMesageN::Result(x);
	return NULL;
}

