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
		unsigned bits;
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

	typedef TL::MkTlst<
		Clr<BorderDefect<Cross>>, Clr<BorderDefect<Long>>
		, Clr<BorderKlass2<Cross>>, Clr<BorderKlass2<Long>>
	>::Result skip_list;

	template<class X>struct __id_skip__<Clr<BorderDefect<X>>>
	{
		template<class P>bool operator()(P *p)
		{
			p->bits |= 1 << (TL::IndexOf<skip_list, Clr<BorderDefect<X>>>::value);
			return false;
		}
	};
	template<class X>struct __id_skip__<Clr<BorderKlass2<X>>>
	{
		template<class P>bool operator()(P *p)
		{
			p->bits |= 1 << (TL::IndexOf<skip_list, Clr<BorderKlass2<X>>>::value);
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

	template<class P>struct __type_skip__<Clr<Undefined>, P>
	{
		typedef typename P::items_list Result;
	};

	template<class X, class P>struct __type_skip__<Clr<BorderDefect<X>>, P>
	{
		typedef typename P::items_list Result;
	};

	template<class X, class P>struct __type_skip__<Clr<BorderKlass2<X>>, P>
	{
		typedef typename P::items_list Result;
	};

	template<class List>struct __result__;
	template<class List>struct __result_XXX__;
	template<class X, class Tail>struct __result__<Tlst<Clr<BorderDefect<X>>, Tail>>
	{
		typedef Clr<BorderDefect<X>> O;
		template<class P>void operator()(P *p)
		{
			if(p->bits & (1 << TL::IndexOf<skip_list, O>::value))
			{
				p->bits &= ~(1 << TL::IndexOf<skip_list, Clr<BorderKlass2<X>>>::value);
				typedef typename __sel_list__<typename P::items_list, typename O::items_list>::Result list;
				//dprint(" %s\n  %s\n  %s\n"
				//	, typeid(typename P::items_list).name()
				//	, typeid(typename O::items_list).name()
				//	, typeid(list).name()
				//	);
				__result_XXX__<Tail>()((__data_id__<list> *)p);
				return;
			}
			__result_XXX__<Tail>()(p);
		}
	};

	template<class X, class Tail>struct __result__<Tlst<Clr<BorderKlass2<X>>, Tail>>
	{
		typedef Clr<BorderKlass2<X>> O;
		template<class P>void operator()(P *p)
		{
			if(p->bits & (1 << TL::IndexOf<skip_list, O>::value))
			{
				typedef typename __sel_list__<typename P::items_list, typename O::items_list>::Result list;
				__result_XXX__<Tail>()((__data_id__<list> *)p);
				return;
			}
			__result_XXX__<Tail>()(p);
		}
	};

	template<class O>struct __test_undifined__
	{
		template<class P>void operator()(P *p)
		{
		   p->str = O::text();
		   p->res = O::ID;
		}
	};

	template<>struct __test_undifined__<__status_label__<Clr<Undefined>>>
	{
		typedef __status_label__<Clr<Undefined>> O;
		template<class P>void operator()(P *p)
		{
			if(0 == p->res)
			{
				p->str = O::text();
				p->res = O::ID;
			}
		}
	};

	template<>struct __result__<NullType>
	{
		template<class P>void operator()(P *p)
		{
			typedef __status_label__<typename __min_list__<typename P::items_list>::Result> type;
		   __test_undifined__<type>()(p);
		}
	};

	template<class List>struct __result_XXX__: __result__<List>{}; 

	template<class O, class P>struct __id_XXX__;
	template<class O, class P>struct __id__
	{
		bool operator()(O *, P *p)
		{
			if (TL::IndexOf<label_message_list, O>::value == *p->id)
			{
				p->id = &p->id[1];
				if (__id_skip__<O>()(p)) return false;

				typedef typename __type_skip__<O, P>::Result list;

				if (-1 == *p->id)
				{
					__result__<skip_list>()((__data_id__<list> *)p);
					return false;
				}
				TL::find<label_message_list, __id_XXX__>()((TL::Factory<label_message_list> *)0, (__data_id__<list> *)p);
				return false;
			}
			return true;
		}
	};
	template<class O, class P>struct __id_XXX__: __id__<O, P>{};
}

char *SelectMessage(int *x, int &res)
{
	__data_id__<label_message_list> d = { x, NULL, 0, 0 };
	TL::find<label_message_list, __id__>()((TL::Factory<label_message_list> *)0, &d);
	res = d.res;
	return d.str;
}

