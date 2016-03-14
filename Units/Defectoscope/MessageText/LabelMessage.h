#pragma once
#include "AppBase.h"
//-----------------------------------------------------------------
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
	static const int ID = TL::IndexOf<ColorTable::items_list, O>::value;\
	static char *text(){return txt;};\
};

#define WAPPER_STATUS_LABEL(O, txt)template<class X>struct __status_label__<Clr<O<X>>>\
{\
	static const int ID = TL::IndexOf<ColorTable::items_list, Clr<O<X>>>::value;\
	static char *text(){return txt;}\
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
	template<>struct ValueVisible<Undefined>
	{
		static const bool value = false;
	};
	template<>struct ValueVisible<DeathZone>
	{
		static const bool value = false;
	};
	template<class O, class P>struct __select__
	{
		bool operator()(O *o, P *p)
		{
			if(TL::IndexOf<ColorTable::items_list, O>::value == p->id)
			{
				p->text = __status_label__<O>::text();
				//p->res = __status_label__<O>::text();
				p->color = o->value;
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

//WAPPER_STATUS_LABEL(BorderKlass2, "\"2 класс\"")
//WAPPER_STATUS_LABEL(BorderDefect, "\"Брак\"")

STATUS_LABEL(Clr<BorderAbove<Thickness>>, "\"Толщина выше нормы\"") 
STATUS_LABEL(Clr<BorderLower<Thickness>>, "\"Толщина меньше нормы\"")
STATUS_LABEL(Clr<BorderNominal<Thickness>>, "\"Норма\"")
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
	bool operator()(O *, P *p)
    {
        if(TL::IndexOf<ColorTable::items_list, O>::value == p->id)
		{
		     p->color = color;
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
	bool operator()(O *, P *p)\
    {\
        if(TL::IndexOf<ColorTable::items_list, O>::value == p->id)\
		{\
		     p->color = color;\
			 p->data = p->defData;\
             return false;\
		}\
		return true;\
    }\
};

COLOR_DATA(Undefined)
COLOR_DATA(DeathZone)

#define TXT(a, b) a##_##b
#define STR(a, b) a<b>
#define PARAM_STR(a, b) STR(a, b)
#define PARAM_TXT(a, b) TXT(a, b)

#define STATUS_LABEL_1(a)template<>struct __status_label__<Clr<PARAM_STR##a>>\
{\
	static const int ID = TL::IndexOf<ColorTable::items_list, Clr<PARAM_STR##a>>::value;\
	static char *text(){return PARAM_TXT##a;}\
};

#define STATUS_LABEL_2(a, b)template<>struct __status_label__<Clr<PARAM_STR##a, PARAM_STR##b>>\
{\
	static const int ID = TL::IndexOf<ColorTable::items_list, Clr<PARAM_STR##a, PARAM_STR##b>>::value;\
	static char *text(){return PARAM_TXT##a##PARAM_TXT##b;}\
};

#define STATUS_LABEL_3(a, b, c)template<>struct __status_label__<Clr<PARAM_STR##a, PARAM_STR##b, PARAM_STR##c>>\
{\
	static const int ID = TL::IndexOf<ColorTable::items_list, Clr<PARAM_STR##a, PARAM_STR##b, PARAM_STR##c>>::value;\
	static char *text(){return PARAM_TXT##a##PARAM_TXT##b##PARAM_TXT##c;};\
};

#define STATUS_LABEL_4(a, b, c, d)template<>struct __status_label__<Clr<PARAM_STR##a, PARAM_STR##b, PARAM_STR##c, PARAM_STR##d>>\
{\
	static const int ID = TL::IndexOf<ColorTable::items_list, Clr<PARAM_STR##a, PARAM_STR##b, PARAM_STR##c, PARAM_STR##d>>::value;\
	static char *text(){return PARAM_TXT##a##PARAM_TXT##b##PARAM_TXT##c##PARAM_TXT##d;}\
};

#define BorderDefect_Cross "\"поперечный дефект\", "
#define BorderDefect_Long "\"продольный дефект\", "

#define BorderKlass2_Cross "\"поперечный 2 класс\", "
#define BorderKlass2_Long "\"продольный 2 класс\", "

#define BorderLower_Thickness "\"толщина меньше нормы\", "
#define BorderAbove_Thickness "\"толщина выше нормы\", "

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
STATUS_LABEL_2((BorderKlass2, Cross), (BorderLower, Thickness))
STATUS_LABEL_3((BorderKlass2, Cross), (BorderLower, Thickness), (BorderAbove, Thickness))
				
STATUS_LABEL_2((BorderKlass2, Long), (BorderAbove, Thickness))
STATUS_LABEL_2((BorderKlass2, Long), (BorderLower, Thickness))
STATUS_LABEL_3((BorderKlass2, Long), (BorderLower, Thickness), (BorderAbove, Thickness))
			
STATUS_LABEL_2((BorderKlass2, Cross), (BorderKlass2, Long))
				
STATUS_LABEL_3((BorderKlass2, Cross), (BorderKlass2, Long), (BorderAbove, Thickness))
STATUS_LABEL_3((BorderKlass2, Cross), (BorderKlass2, Long), (BorderLower, Thickness))
STATUS_LABEL_4((BorderKlass2, Cross), (BorderKlass2, Long), (BorderLower, Thickness), (BorderAbove, Thickness))
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
		TL::find<ColorTable::items_list, __select__>()(		
			&Singleton<ColorTable>::Instance().items
			, &data
			);
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
		TL::find<ColorTable::items_list, __set_color_bar__>()((TL::Factory<ColorTable::items_list> *)0, &d);
	}
};
