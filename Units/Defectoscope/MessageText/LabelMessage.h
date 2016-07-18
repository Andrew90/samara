#pragma once
#include "AppBase.h"
//-----------------------------------------------------------------
	typedef TL::MkTlst<	
/*0*/	Clr<Undefined   >
/*1*/	, Clr<DeathZone>
/*2*/	, Clr<Nominal	>
/*3*/	, Clr<BorderAbove<Thickness> >
/*4*/	, Clr<BorderLower<Thickness> >
/*5*/	, Clr<BorderLower<Thickness>, BorderAbove<Thickness>>
/*6*/	, Clr<BorderKlass2<Long> > 
/*7*/	, Clr<BorderDefect<Long> >
/*8*/	, Clr<BorderKlass2<Cross>>
/*9*/	, Clr<BorderDefect<Cross>>

/*10*/	, Clr<BorderDefect<Cross>, BorderAbove<Thickness>>
/*11*/	, Clr<BorderDefect<Cross>, BorderLower<Thickness>>
/*12*/	, Clr<BorderDefect<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>>
/*13*/	, Clr<BorderDefect<Long>, BorderAbove<Thickness>>
/*14*/	, Clr<BorderDefect<Long>, BorderLower<Thickness>>
/*15*/	, Clr<BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
/*16*/	, Clr<BorderDefect<Cross>, BorderDefect<Long> >
/*17*/  , Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderAbove<Thickness>>   
/*18*/  , Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>> 
/*19*/  , Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>

/*20*/  , Clr<BorderKlass2<Cross>, BorderAbove<Thickness>>
/*21*/	, Clr<BorderLower<Thickness>, BorderKlass2<Cross>>
/*22*/	, Clr<BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>>
/*23*/	, Clr<BorderKlass2<Long>, BorderAbove<Thickness>>
/*24*/	, Clr<BorderLower<Thickness>, BorderKlass2<Long>>
/*25*/	, Clr<BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Long>>
/*26*/	, Clr<BorderKlass2<Cross>, BorderKlass2<Long> >
/*27*/  , Clr<BorderKlass2<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>>   
/*28*/  , Clr<BorderLower<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>> 
/*29*/  , Clr<BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>>

/*30*/  , Clr<BorderDefect<Cross>, BorderKlass2<Long> >
/*31*/  , Clr<BorderDefect<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>>
/*32*/  , Clr<BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>>    
/*33*/  , Clr<BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
/*34*/  , Clr<BorderDefect<Long>, BorderKlass2<Cross> >
/*35*/  , Clr<BorderDefect<Long>, BorderKlass2<Cross>, BorderAbove<Thickness>>   
/*36*/  , Clr<BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>>      
/*37*/  , Clr<BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>> 
//------------------------------------------------------------------------------
	>::Result label_message_list;
//-------------------------------------------------------------------
#define	JOIN2(a, b) a##,##b
#define	JOIN3(a, b, c) a##,##b##,##c
#define	JOIN4(a, b, c, d) a##,##b##,##c,##d

//DEFINE_WAPPER(JOIN2(Clr<BorderLower<Thickness>, BorderAbove<Thickness>>											), int, 0xffff0000)
DEFINE_WAPPER(JOIN2(Clr<BorderDefect<Cross>, BorderAbove<Thickness>>											), int, 0xffff0000)
DEFINE_WAPPER(JOIN2(Clr<BorderDefect<Cross>, BorderLower<Thickness>>											), int, 0xffff0000)
DEFINE_WAPPER(JOIN3(Clr<BorderDefect<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>>					), int, 0xffff0000)
DEFINE_WAPPER(JOIN2(Clr<BorderDefect<Long>, BorderAbove<Thickness>>												), int, 0xffff0000)
DEFINE_WAPPER(JOIN2(Clr<BorderDefect<Long>, BorderLower<Thickness>>												), int, 0xffff0000)
DEFINE_WAPPER(JOIN3(Clr<BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>						), int, 0xffff0000)
DEFINE_WAPPER(JOIN2(Clr<BorderDefect<Cross>, BorderDefect<Long> >												), int, 0xffff0000)
DEFINE_WAPPER(JOIN3(Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderAbove<Thickness>>   						), int, 0xffff0000)
DEFINE_WAPPER(JOIN3(Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>> 						), int, 0xffff0000)
DEFINE_WAPPER(JOIN4(Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>), int, 0xffff0000)
DEFINE_WAPPER(JOIN2(Clr<BorderKlass2<Cross>, BorderAbove<Thickness>>											), int, 0xffffff00)

DEFINE_WAPPER(JOIN2(Clr<BorderLower<Thickness>, BorderKlass2<Cross>>											), int, 0xffff0000)

DEFINE_WAPPER(JOIN3(Clr<BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>>					), int, 0xffff0000)

DEFINE_WAPPER(JOIN2(Clr<BorderKlass2<Long>, BorderAbove<Thickness>>												), int, 0xffffff00)

DEFINE_WAPPER(JOIN2(Clr<BorderLower<Thickness>, BorderKlass2<Long>>												), int, 0xffff0000)
DEFINE_WAPPER(JOIN3(Clr<BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Long>>						), int, 0xffff0000)

DEFINE_WAPPER(JOIN2(Clr<BorderKlass2<Cross>, BorderKlass2<Long> >												), int, 0xffffff00)

DEFINE_WAPPER(JOIN3(Clr<BorderKlass2<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>>   						), int, 0xffffff00)
DEFINE_WAPPER(JOIN3(Clr<BorderLower<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>> 						), int, 0xffff0000)
DEFINE_WAPPER(JOIN4(Clr<BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>>), int, 0xffff0000)
DEFINE_WAPPER(JOIN2(Clr<BorderDefect<Cross>, BorderKlass2<Long> >												), int, 0xffff0000)
DEFINE_WAPPER(JOIN3(Clr<BorderDefect<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>>						), int, 0xffff0000)
DEFINE_WAPPER(JOIN3(Clr<BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>>    					), int, 0xffff0000)
DEFINE_WAPPER(JOIN4(Clr<BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>), int, 0xffff0000)
DEFINE_WAPPER(JOIN2(Clr<BorderDefect<Long>, BorderKlass2<Cross> >												), int, 0xffff0000)
DEFINE_WAPPER(JOIN3(Clr<BorderDefect<Long>, BorderKlass2<Cross>, BorderAbove<Thickness>>   						), int, 0xffff0000)
DEFINE_WAPPER(JOIN3(Clr<BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>>      					), int, 0xffff0000)
DEFINE_WAPPER(JOIN4(Clr<BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>>), int, 0xffff0000) 

#undef	JOIN2
#undef	JOIN3
#undef	JOIN4
//---------------------------------------------------------------------
template<class O>struct __status_label__
{
	typedef typename O::__template_must_be_overridded__ noused;
};

template<>struct __status_label__<NullType>
{
	static const int ID = 0;
	static char *text(){return "NO_MESSAGE";}
};

#define STATUS_LABEL(O, txt)template<>struct __status_label__<O>\
{\
	static const int ID = TL::IndexOf<label_message_list, O>::value;\
	static char *text(){return txt;};\
};

namespace
{
	struct __data_text__
	{
		int id;
		char *text;
		int color;
		bool visibleVal;
	};
	template<class T>struct ValueVisible
	{
		static const bool value = true;
	};
	template<>struct ValueVisible<Clr<Undefined>>
	{
		static const bool value = false;
	};
	template<>struct ValueVisible<Clr<DeathZone>>
	{
		static const bool value = false;
	};
	template<class T>struct __first_color__;
	template<class  _0, class _1, class _2, class _3>struct __first_color__<Clr<_0, _1, _2, _3>>
	{
		typedef Clr<_0, _1, _2, _3> O;
		typedef typename TL::_if<(TL::Length<ColorTable::items_list>::value > TL::IndexOf<label_message_list, O>::value), O, Clr<_0>>::Result Result;
	};
	template<class O, class P>struct __select__
	{
		bool operator()(P *p)
		{
			if(TL::IndexOf<label_message_list, O>::value == p->id)
			{
				p->text = __status_label__<O>::text();
				p->color = Singleton<ColorTable>::Instance().items.get<typename __first_color__<O>::Result>().value;
				p->visibleVal = ValueVisible<O>::value;
				return false;
			}
			return true;
		}
	};
}

STATUS_LABEL(Clr<Undefined>, "\"Результат не определён\"")
STATUS_LABEL(Clr<DeathZone>, "\"Мёртвая зона\"")

STATUS_LABEL(Clr<Nominal>, "\"Норма\"")

STATUS_LABEL(Clr<BorderAbove<Thickness>>, "\"Толщина выше нормы\"") 
STATUS_LABEL(Clr<BorderLower<Thickness>>, "\"Толщина меньше нормы\"")
//---------------------------------------------------------------------------
template<class O, class P>struct __set_color_bar__;

namespace
{
	struct __data_color__
	{
		int id;
		unsigned &color;
		double &data;
		double defData;
		__data_color__(int id, unsigned &color, double &data, double defData)
			: id(id)
		    , color(color)
		    , data(data)
			, defData(defData)
		{}
	};
}

template<class O, class P>struct __set_color_bar__
{
	int &color;
	__set_color_bar__() : color(Singleton<ColorTable>::Instance().items.get<O>().value){}
	bool operator()(P *p)
    {
		if(TL::IndexOf<ColorTable::items_list, O>::value == p->id)
		{
		     p->color = color;
             return false;
		}
		return true;
    }
};

template<class T>struct GetFirst;
template<template<class, class, class, class>class X, class A, class B, class C, class D>struct GetFirst<X<A, B, C, D> >
{
	typedef X<A, NullType, NullType, NullType> Result;
};
template<class O, class P>struct __set_color_bar_next__
{
	bool operator()(P *p)
    {
		if(TL::IndexOf<label_message_list, O>::value == p->id)
		{
			p->color = Singleton<ColorTable>::Instance().items.get<typename GetFirst<O>::Result>().value;
             return false;
		}
		return true;
    }
};

#define COLOR_DATA(O)template<class P>struct __set_color_bar__<O, P>\
{\
	int &color;\
	__set_color_bar__() \
        : color(Singleton<ColorTable>::Instance().items.get<O>().value)\
        {}\
	bool operator()(P *p)\
    {\
        if(TL::IndexOf<label_message_list, O>::value == p->id)\
		{\
		     p->color = color;\
			 p->data = p->defData;\
             return false;\
		}\
		return true;\
    }\
};

COLOR_DATA(Clr<Undefined>)
COLOR_DATA(Clr<DeathZone>)

#define TXT(a, b) a##_##b
#define STR(a, b) a<b>
#define PARAM_STR(a, b) STR(a, b)
#define PARAM_TXT(a, b) TXT(a, b)

#define STATUS_LABEL_1(a)template<>struct __status_label__<Clr<PARAM_STR##a>>\
{\
	static const int ID = TL::IndexOf<label_message_list, Clr<PARAM_STR##a>>::value;\
	static char *text(){return PARAM_TXT##a;}\
};

#define STATUS_LABEL_2(a, b)template<>struct __status_label__<Clr<PARAM_STR##a, PARAM_STR##b>>\
{\
	static const int ID = TL::IndexOf<label_message_list, Clr<PARAM_STR##a, PARAM_STR##b>>::value;\
	static char *text(){return PARAM_TXT##a##PARAM_TXT##b;}\
};

#define STATUS_LABEL_3(a, b, c)template<>struct __status_label__<Clr<PARAM_STR##a, PARAM_STR##b, PARAM_STR##c>>\
{\
	static const int ID = TL::IndexOf<label_message_list, Clr<PARAM_STR##a, PARAM_STR##b, PARAM_STR##c>>::value;\
	static char *text(){return PARAM_TXT##a##PARAM_TXT##b##PARAM_TXT##c;};\
};

#define STATUS_LABEL_4(a, b, c, d)template<>struct __status_label__<Clr<PARAM_STR##a, PARAM_STR##b, PARAM_STR##c, PARAM_STR##d>>\
{\
	static const int ID = TL::IndexOf<label_message_list, Clr<PARAM_STR##a, PARAM_STR##b, PARAM_STR##c, PARAM_STR##d>>::value;\
	static char *text(){return PARAM_TXT##a##PARAM_TXT##b##PARAM_TXT##c##PARAM_TXT##d;}\
};

#define BorderDefect_Cross "\"поперечный дефект\", "
#define BorderDefect_Long "\"продольный дефект\", "

#define BorderKlass2_Cross "\"поперечный 2 класс\", "
#define BorderKlass2_Long "\"продольный 2 класс\", "

#define BorderLower_Thickness "\"толщина меньше нормы\", "
#define BorderAbove_Thickness "\"толщина больше нормы\", "

STATUS_LABEL_1((BorderDefect, Cross))
STATUS_LABEL_1((BorderDefect, Long))

STATUS_LABEL_1((BorderKlass2, Cross))
STATUS_LABEL_1((BorderKlass2, Long))

STATUS_LABEL_2((BorderLower, Thickness), (BorderAbove, Thickness))

STATUS_LABEL_2((BorderDefect, Cross), (BorderAbove, Thickness))
STATUS_LABEL_2((BorderDefect, Cross), (BorderLower, Thickness))
STATUS_LABEL_3((BorderDefect, Cross), (BorderLower, Thickness), (BorderAbove, Thickness))

STATUS_LABEL_2((BorderDefect, Long), (BorderAbove, Thickness))
STATUS_LABEL_2((BorderDefect, Long), (BorderLower, Thickness))
STATUS_LABEL_3((BorderDefect, Long), (BorderLower, Thickness), (BorderAbove, Thickness))

STATUS_LABEL_2((BorderDefect, Cross), (BorderDefect, Long))

STATUS_LABEL_3((BorderDefect, Cross), (BorderDefect, Long), (BorderAbove, Thickness))
STATUS_LABEL_3((BorderDefect, Cross), (BorderDefect, Long), (BorderLower, Thickness))
STATUS_LABEL_4((BorderDefect, Cross), (BorderDefect, Long), (BorderLower, Thickness), (BorderAbove, Thickness))
//---------------------------------------------------------------------------
STATUS_LABEL_2((BorderKlass2, Cross), (BorderAbove, Thickness))
STATUS_LABEL_2((BorderLower, Thickness), (BorderKlass2, Cross))
STATUS_LABEL_3( (BorderLower, Thickness), (BorderAbove, Thickness),(BorderKlass2, Cross))
				
STATUS_LABEL_2((BorderKlass2, Long), (BorderAbove, Thickness))
STATUS_LABEL_2((BorderLower, Thickness), (BorderKlass2, Long))
STATUS_LABEL_3((BorderLower, Thickness), (BorderAbove, Thickness), (BorderKlass2, Long))
			
STATUS_LABEL_2((BorderKlass2, Cross), (BorderKlass2, Long))
				
STATUS_LABEL_3((BorderKlass2, Cross), (BorderKlass2, Long), (BorderAbove, Thickness))
STATUS_LABEL_3((BorderLower, Thickness), (BorderKlass2, Cross), (BorderKlass2, Long))
STATUS_LABEL_4((BorderLower, Thickness), (BorderAbove, Thickness), (BorderKlass2, Cross), (BorderKlass2, Long))
//--------------------
STATUS_LABEL_2((BorderDefect, Cross), (BorderKlass2, Long))
				
STATUS_LABEL_3((BorderDefect, Cross), (BorderKlass2, Long), (BorderAbove, Thickness))
STATUS_LABEL_3((BorderDefect, Cross), (BorderKlass2, Long), (BorderLower, Thickness))
STATUS_LABEL_4((BorderDefect, Cross), (BorderKlass2, Long), (BorderLower, Thickness), (BorderAbove, Thickness))
//---------------------------------------
STATUS_LABEL_2((BorderDefect, Long), (BorderKlass2, Cross))
													
STATUS_LABEL_3((BorderDefect, Long), (BorderKlass2, Cross), (BorderAbove, Thickness))
STATUS_LABEL_3((BorderDefect, Long), (BorderKlass2, Cross), (BorderLower, Thickness))
STATUS_LABEL_4((BorderDefect, Long), (BorderKlass2, Cross), (BorderLower, Thickness), (BorderAbove, Thickness))

#undef COLOR_DATA
#undef STATUS_LABEL
#undef STATUS_LABEL_2
#undef STATUS_LABEL_3
#undef STATUS_LABEL_4
#undef TXT
#undef STR
#undef PARAM_STR
#undef PARAM_TXT

#undef BorderDefect_Cross
#undef BorderDefect_Long 
#undef BorderKlass2_Cross 
#undef BorderKlass2_Long 
#undef BorderLower_Thickness
#undef BorderAbove_Thickness
//------------------------------------------------------------------------------------------------
struct StatusText
{
	char *operator()(int id, int &color, bool &visible)
	{
		__data_text__ data = {id, NULL};
		TL::find<label_message_list, __select__>()(&data);
		color = data.color;
		visible = data.visibleVal;
		return data.text;
	}
};

struct ColorBar
{
	void operator()(double &data, unsigned &color, int id, double defData)
	{
		__data_color__ d(id, color, data, defData);
		if(TL::find<ColorTable::items_list, __set_color_bar__>()(&d))
		{
			TL::find<label_message_list, __set_color_bar_next__>()(&d);
		}
	}
};
