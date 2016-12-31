#pragma once
#include "App/AppBase.h"
#include "debug_tools/DebugMess.h"
//-----------------------------------------------------------------
	typedef TL::MkTlst<	
/*0*/		Clr<Undefined   >
/*1*/		, Clr<DeathZone>
/*2*/		, Clr<Nominal	>
/*3*/	    , Clr<BorderAbove<Thickness> >
/*4*/	    , Clr<BorderLower<Thickness> >
/*5*/	    , Clr<BorderKlass2<Long> > 
/*6*/	    , Clr<BorderDefect<Long> >
/*7*/	    , Clr<BorderKlass2<Cross>>
/*8*/	    , Clr<BorderDefect<Cross>>
/*9*/       , Clr<BrakStrobe2<Thickness>>

/*10*/	, Clr<BorderLower<Thickness>, BorderAbove<Thickness>>

/*11*/	, Clr<BorderDefect<Cross>, BorderAbove<Thickness>>
/*12*/	, Clr<BorderDefect<Cross>, BorderLower<Thickness>>
/*13*/	, Clr<BorderDefect<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>>
/*14*/	, Clr<BorderDefect<Long>, BorderAbove<Thickness>>
/*15*/	, Clr<BorderDefect<Long>, BorderLower<Thickness>>
/*16*/	, Clr<BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
/*17*/	, Clr<BorderDefect<Cross>, BorderDefect<Long> >
/*18*/  , Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderAbove<Thickness>>   
/*19*/  , Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>> 
/*20*/  , Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>

/*21*/  , Clr<BorderKlass2<Cross>, BorderAbove<Thickness>>
/*22*/	, Clr<BorderLower<Thickness>, BorderKlass2<Cross>>
/*23*/	, Clr<BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>>
/*24*/	, Clr<BorderKlass2<Long>, BorderAbove<Thickness>>
/*25*/	, Clr<BorderLower<Thickness>, BorderKlass2<Long>>
/*26*/	, Clr<BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Long>>
/*27*/	, Clr<BorderKlass2<Cross>, BorderKlass2<Long> >
/*28*/  , Clr<BorderKlass2<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>>   
/*29*/  , Clr<BorderLower<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>> 
/*30*/  , Clr<BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>>

/*31*/  , Clr<BorderDefect<Cross>, BorderKlass2<Long> >
/*32*/  , Clr<BorderDefect<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>>
/*33*/  , Clr<BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>>    
/*34*/  , Clr<BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
/*35*/  , Clr<BorderDefect<Long>, BorderKlass2<Cross> >
/*36*/  , Clr<BorderDefect<Long>, BorderKlass2<Cross>, BorderAbove<Thickness>>   
/*37*/  , Clr<BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>>      
/*38*/  , Clr<BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>> 

/*39*/	, Clr<BrakStrobe2<Thickness>, BorderAbove<Thickness> >

/*40*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness> >
/*41*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderAbove<Thickness>>
/*42*/	, Clr<BrakStrobe2<Thickness>, BorderKlass2<Long> > 
/*43*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Long> >
/*44*/	, Clr<BrakStrobe2<Thickness>, BorderKlass2<Cross>>
/*45*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>>
//			
/*46*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderAbove<Thickness>>
/*47*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderLower<Thickness>>
/*48*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>>

/*49*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderAbove<Thickness>>
/*50*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderLower<Thickness>>
/*51*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
/*52*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderDefect<Long> >
/*53*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderDefect<Long>, BorderAbove<Thickness>>   
/*54*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>> 
/*55*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
			
/*56*/  , Clr<BrakStrobe2<Thickness>, BorderKlass2<Cross>, BorderAbove<Thickness>>
/*57*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderKlass2<Cross>>
/*58*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>>
/*59*/	, Clr<BrakStrobe2<Thickness>, BorderKlass2<Long>, BorderAbove<Thickness>>
/*60*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderKlass2<Long>>

/*61*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Long>>
/*62*/	, Clr<BrakStrobe2<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long> >
/*63*/  , Clr<BrakStrobe2<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>>   
/*64*/  , Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>> 
/*65*/  , Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>>
			
/*66*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderKlass2<Long> >
/*67*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>>
/*68*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>>    
/*69*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
/*70*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderKlass2<Cross> >
/*71*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderKlass2<Cross>, BorderAbove<Thickness>>   
/*72*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>>      
/*73*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>> 
//------------------------------------------------------------------------------
>::Result label_message_list;
//-------------------------------------------------------------------
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

	template<class List>struct __xel__;
	template<class A, class B, class C, class D, class Tail>struct __xel__<Tlst<Clr<A, B, C, D>, Tail>>
	{
		typedef Tlst<Clr<A, B, C, D>, typename __xel__<Tail>::Result> Result;
	};
	template<class Head, class Tail>struct __xel__<Tlst<Head, Tail>>
	{
		typedef typename __xel__<Tail>::Result Result;
	};
	template<>struct __xel__<NullType>
	{
		typedef NullType Result;
	};

	template<class  _0, class _1, class _2, class _3, class _4>struct __first_color__<Clr<_0, _1, _2, _3, _4>>
	{
		typedef Clr<_0, _1, _2, _3, _4> O;
		typedef typename TL::_if<(TL::Length<typename __xel__<ColorTable::items_list>::Result>::value > TL::IndexOf<label_message_list, O>::value), O, Clr<_0>>::Result Result;
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

STATUS_LABEL(Clr<Undefined>, "\"результат не определён\"")
STATUS_LABEL(Clr<DeathZone>, "\"мёртвая зона\"")

STATUS_LABEL(Clr<Nominal>, "\"норма\"")
STATUS_LABEL(Clr<BrakStrobe2<Thickness>>, "\"брак по стробу\"")

STATUS_LABEL(Clr<BorderAbove<Thickness>>, "\"толщина больше нормы\"") 
STATUS_LABEL(Clr<BorderLower<Thickness>>, "\"толщина меньше нормы\"")
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
template<template<class, class, class, class, class>class X, class A, class B, class C, class D, class E>struct GetFirst<X<A, B, C, D, E> >
{
	typedef X<A, NullType, NullType, NullType, NullType> Result;
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

#define STATUS_LABEL_5(a, b, c, d, e)template<>struct __status_label__<Clr<PARAM_STR##a, PARAM_STR##b, PARAM_STR##c, PARAM_STR##d, PARAM_STR##e>>\
{\
	static const int ID = TL::IndexOf<label_message_list, Clr<PARAM_STR##a, PARAM_STR##b, PARAM_STR##c, PARAM_STR##d, PARAM_STR##e>>::value;\
	static char *text(){return PARAM_TXT##a##PARAM_TXT##b##PARAM_TXT##c##PARAM_TXT##d##PARAM_TXT##e;}\
};

//-------------------------------
#define ASTATUS_LABEL_2(a, b)template<>struct __status_label__<Clr<PARAM_STR##a, PARAM_STR##b>>\
{\
	static const int ID = TL::IndexOf<brack_strobe_list, Clr<PARAM_STR##a, PARAM_STR##b>>::value;\
	static char *text(){return PARAM_TXT##a##PARAM_TXT##b;}\
};

#define ASTATUS_LABEL_3(a, b, c)template<>struct __status_label__<Clr<PARAM_STR##a, PARAM_STR##b, PARAM_STR##c>>\
{\
	static const int ID = TL::IndexOf<brack_strobe_list, Clr<PARAM_STR##a, PARAM_STR##b, PARAM_STR##c>>::value;\
	static char *text(){return PARAM_TXT##a##PARAM_TXT##b##PARAM_TXT##c;};\
};

#define ASTATUS_LABEL_4(a, b, c, d)template<>struct __status_label__<Clr<PARAM_STR##a, PARAM_STR##b, PARAM_STR##c, PARAM_STR##d>>\
{\
	static const int ID = TL::IndexOf<brack_strobe_list, Clr<PARAM_STR##a, PARAM_STR##b, PARAM_STR##c, PARAM_STR##d>>::value;\
	static char *text(){return PARAM_TXT##a##PARAM_TXT##b##PARAM_TXT##c##PARAM_TXT##d;}\
};

#define ASTATUS_LABEL_5(a, b, c, d, e)template<>struct __status_label__<Clr<PARAM_STR##a, PARAM_STR##b, PARAM_STR##c, PARAM_STR##d, PARAM_STR##e>>\
{\
	static const int ID = TL::IndexOf<brack_strobe_list, Clr<PARAM_STR##a, PARAM_STR##b, PARAM_STR##c, PARAM_STR##d, PARAM_STR##e>>::value;\
	static char *text(){return PARAM_TXT##a##PARAM_TXT##b##PARAM_TXT##c##PARAM_TXT##d##PARAM_TXT##e;}\
};

#define BorderDefect_Cross "\"поперечный дефект\""
#define BorderDefect_Long "\"продольный дефект\""

#define BorderKlass2_Cross "\"поперечный 2 класс\""
#define BorderKlass2_Long "\"продольный 2 класс\""

#define BorderLower_Thickness "\"толщина меньше нормы\""
#define BorderAbove_Thickness "\"толщина больше нормы\""

#define BrakStrobe2_Thickness "\"брак по стробу\""

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
STATUS_LABEL_2((BorderDefect, Long), (BorderKlass2, Cross))												
STATUS_LABEL_3((BorderDefect, Long), (BorderKlass2, Cross), (BorderAbove, Thickness))
STATUS_LABEL_3((BorderDefect, Long), (BorderKlass2, Cross), (BorderLower, Thickness))
STATUS_LABEL_4((BorderDefect, Long), (BorderKlass2, Cross), (BorderLower, Thickness), (BorderAbove, Thickness))
STATUS_LABEL_2((BrakStrobe2, Thickness), (BorderAbove, Thickness))
STATUS_LABEL_2((BrakStrobe2, Thickness), (BorderLower , Thickness))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderLower , Thickness), (BorderAbove, Thickness))
STATUS_LABEL_2((BrakStrobe2, Thickness), (BorderKlass2, Long)) 
STATUS_LABEL_2((BrakStrobe2, Thickness), (BorderDefect, Long))
STATUS_LABEL_2((BrakStrobe2, Thickness), (BorderKlass2, Cross))
STATUS_LABEL_2((BrakStrobe2, Thickness), (BorderDefect, Cross))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderAbove, Thickness))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderLower, Thickness))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderLower, Thickness), (BorderAbove, Thickness))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderDefect, Long), (BorderAbove, Thickness))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderDefect, Long), (BorderLower, Thickness))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderDefect, Long), (BorderLower, Thickness), (BorderAbove, Thickness))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderDefect, Long))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderDefect, Long), (BorderAbove, Thickness))   
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderDefect, Long), (BorderLower, Thickness)) 
STATUS_LABEL_5((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderDefect, Long), (BorderLower, Thickness), (BorderAbove, Thickness))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderKlass2, Cross), (BorderAbove, Thickness))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderLower , Thickness), (BorderKlass2, Cross))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderLower , Thickness), (BorderAbove, Thickness), (BorderKlass2, Cross))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderKlass2, Long), (BorderAbove, Thickness))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderLower , Thickness), (BorderKlass2, Long))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderLower , Thickness), (BorderAbove, Thickness), (BorderKlass2, Long))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderKlass2, Cross), (BorderKlass2, Long))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderKlass2, Cross), (BorderKlass2, Long), (BorderAbove, Thickness))   
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderLower , Thickness), (BorderKlass2, Cross), (BorderKlass2, Long))
STATUS_LABEL_5((BrakStrobe2, Thickness), (BorderLower , Thickness), (BorderAbove, Thickness), (BorderKlass2, Cross), (BorderKlass2, Long))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderKlass2, Long))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderKlass2, Long), (BorderAbove, Thickness))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderKlass2, Long), (BorderLower, Thickness))
STATUS_LABEL_5((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderKlass2, Long), (BorderLower, Thickness), (BorderAbove, Thickness))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderDefect, Long), (BorderKlass2, Cross))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderDefect, Long), (BorderKlass2, Cross), (BorderAbove, Thickness))   
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderDefect, Long), (BorderKlass2, Cross), (BorderLower, Thickness))      
STATUS_LABEL_5((BrakStrobe2, Thickness), (BorderDefect, Long), (BorderKlass2, Cross), (BorderLower, Thickness), (BorderAbove, Thickness))

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
