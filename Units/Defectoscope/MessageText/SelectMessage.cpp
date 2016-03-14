#include "StdAfx.h"
#include "SelectMessage.h"
#include "AppBase.h"
#include "LabelMessage.h"
#include "DebugMess.h"

namespace
{
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

	template<class List, class T>struct __sel__;
	template<class Head, class Tail, class T>struct __sel__<Tlst<Head, Tail>, T>
	{
		typedef typename TL::_if<
			__in_list__<typename Head::items_list, T>::value
			, Tlst<Head, typename __sel__<Tail, T>::Result>
			, typename __sel__<Tail, T>::Result
		>::Result Result;
	};
	template<class T>struct __sel__<NullType, T>
	{
		typedef NullType Result;
	};

	template<class List, class SulList>struct __sel_list__;
	template<class List, class Head, class Tail>struct __sel_list__<List, Tlst<Head, Tail> >
	{
		typedef typename __sel_list__<
			typename __sel__<List, Head>::Result
			, Tail>::Result Result;
	};
	template<class List>struct __sel_list__<List, NullType>
	{
		typedef List Result;
	};

	template<class T>struct __data_id__
	{
		typedef T items_list;
		int *id;
		char *str;
		int res;
	};

	template<class List, class tmp = NullType, int len = 100>struct __min_list__;
	template<class Head, class Tail, class tmp, int len>struct __min_list__<Tlst<Head, Tail>, tmp, len>
	{
		static const int tmp_len = TL::Length<typename Head::items_list>::value;
		typedef typename __min_list__ <
			Tail
			, typename TL::_if < tmp_len < len, Head, tmp>::Result
			, tmp_len < len ? tmp_len : len
			>::Result Result;
	};

	template<class tmp, int len>struct __min_list__<NullType, tmp, len>
	{
		typedef tmp Result;
	};

	template<class O>struct __id_skip__
	{
		template<class P>bool operator()(P *p)
		{
			return false;
		}
	};
	template<>struct __id_skip__<Clr<Undefined>>
	{
		template<class P>bool operator()(P *p)
		{
			if (-1 == *p->id)
			{
				if (NULL == p->str) 
				{
						p->str = __status_label__<Clr<Undefined>>::text();
						p->res = __status_label__<Clr<Undefined>>::ID;
				}
				return true;
			}
			return false;
		}
	};
	template<>struct __id_skip__<Clr<DeathZone>>
	{
		template<class P>bool operator()(P *p)
		{
			*p->id = -1;
			p->str = __status_label__<Clr<DeathZone>>::text();
			p->res = __status_label__<Clr<DeathZone>>::ID;
			return true;
		}
	};
	template<>struct __id_skip__<Clr<Nominal>>
	{
		template<class P>bool operator()(P *p)
		{
			if (NULL == p->str)
			{
				p->str = __status_label__<Clr<Nominal>>::text();
				p->res = __status_label__<Clr<Nominal>>::ID;
			}
			return false;
		}
	};

	template<class O, class P>struct __type_skip__
	{
		typedef typename __sel_list__<typename P::items_list, typename O::items_list>::Result Result;
	};

	template<class P>struct __type_skip__<Clr<Nominal>, P>
	{
		typedef typename P::items_list Result;
	};

	template<class O, class P>struct __id_XXX__;
	template<class O, class P>struct __id__
	{
		bool operator()(O *, P *p)
		{
			if (TL::IndexOf<ColorTable::items_list, O>::value == *p->id)
			{
				p->id = &p->id[1];
				if (__id_skip__<O>()(p)) return false;

				typedef typename __type_skip__<O, P>::Result list;

				if (-1 == *p->id)
				{
					typedef __status_label__<typename __min_list__<list>::Result> type;
					p->str = type::text();
					p->res = type::ID;
					//dprint("O---%s\nP---%s\nL---%s\n"
					//	, typeid(typename O::items_list).name()
					//	, typeid(typename P::items_list).name()
					//	, typeid(typename list)			.name()
					//	);
					return false;
				}
				TL::find<ColorTable::items_list, __id_XXX__>()((TL::Factory<ColorTable::items_list> *)0, (__data_id__<list> *)p);
				return false;
			}
			return true;
		}
	};
	template<class O, class P>struct __id_XXX__: __id__<O, P>{};
}

char *SelectMessage(int *x, int &res)
{
	__data_id__<ColorTable::items_list> d = { x, NULL, 0 };
	TL::find<ColorTable::items_list, __id__>()((TL::Factory<ColorTable::items_list> *)0, &d);
	res = d.res;
	return d.str;
}

//template<class O, class P>struct Print
//{
//	void operator()(O *, P *)
//	{
//		printf("%s\n", typeid(O).name());
//	}
//};
//
//void Test()
//{
//	typedef __sel_list__<ColorTable::items_list, Clr<BorderAbove<Thickness>>::items_list>::Result list;
//	typedef __sel_list__<list, TL::MkTlst<BorderDefect<Cross>, BorderLower<Thickness>>::Result>::Result list0;
//	TL::foreach<list0, Print>()((TL::Factory<list0> *)0, (int *)0);
//
//	typedef __min_list__<list0>::Result x;
//
//	printf("\n\n%s\n", typeid(x).name());
//}