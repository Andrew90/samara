#include "stdafx.h"
#include "LabelMessage.h"
#include "AppBase.h"
//------------------------------------------------------------------
#define	__(a, b) a##,##b
#define	___(a, b, c) a##,##b##,##c
//-----------------------------------------------------------------
template<class O>struct __status_label__
{
	typedef typename O::__template_must_be_overridded__ noused;
};

#define STATUS_LABEL(O, txt)template<>struct __status_label__<O>\
{\
	static char *text;\
};\
char *__status_label__<O>::text = txt;

#define WAPPER_STATUS_LABEL(O, txt)template<class X>struct __status_label__<Clr<O<X>>>\
{\
	static char *text;\
};\
template<class X>char *__status_label__<Clr<O<X>>>::text = txt;

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
				p->text = __status_label__<O>::text;
				p->color = o->value;
				p->visibleVal = ValueVisible<O>::value;
				return false;
			}
			return true;
		}
	};
}

char *StatusText(int id, int &color, bool &visible)
{
	__data_text__ data = {id, NULL};
	TL::find<ColorTable::items_list, __select__>()
		(//(TL::Factory<ColorTable::items_list> *)0
		&Singleton<ColorTable>::Instance().items
		, &data
		);
	color = data.color;
	visible = data.visibleVal;
	return data.text;
}


//STATUS_LABEL(AboveNorm, "\"Толщина выше нормы\"")
//STATUS_LABEL(GreaterMaximumEnergy, "Энергия сигнала больше порогового значения")
//STATUS_LABEL(BelowNorm, "\"Толщина ниже нормы\"")
//STATUS_LABEL(BelowAboveNorm, "\"Толщина ниже и выше нормы\"")
//STATUS_LABEL(ThicknessLessThreshold, "Вычисленная толщина меньше минимального значения")
//STATUS_LABEL(ThicknessGreaterThreshold, "Вычисленная толщина больше максимального значения")
STATUS_LABEL(Undefined, "\"Результат не определён\"")
STATUS_LABEL(DeathZone, "\"Мёртвая зона\"")

STATUS_LABEL(Nominal, "\"Норма\"")
STATUS_LABEL(Defect, "\"Брак\"")
//STATUS_LABEL(Treshold2Class, "<ffff00>Класс 2")

//STATUS_LABEL(DefectAboveNorm, "xxxxxxxxxxxxxxxxxxxxxxxxxxx")
///////////////////STATUS_LABEL(DefectMinMax, "<ffff00>Брак плюс-минус")
///////////////////STATUS_LABEL(DefectDifferentWall, "<ffff>Брак разностенность")
/////////////////
///////////////////STATUS_LABEL(DefectDifferentWallMin   , "<993333>Брак разностенность минус")
///////////////////STATUS_LABEL(DefectDifferentWallMax   , "<7749ff>Брак разностенность плюс")
///////////////////STATUS_LABEL(DefectDifferentWallMinMax, "<663300>Брак разностенность плюс-минус")

WAPPER_STATUS_LABEL(BorderKlass2, "\"2 класс\"")
WAPPER_STATUS_LABEL(BorderDefect, "\"Брак\"")

STATUS_LABEL(Clr<BorderAbove<Thickness>>, "\"Толщина выше нормы\"") 
STATUS_LABEL(Clr<BorderLower<Thickness>>, "\"Толщина меньше нормы\"")
STATUS_LABEL(Clr<BorderNominal<Thickness>>, "\"Норма\"")
//STATUS_LABEL(Clr<BorderDefect<Long>>, "ClrBorderDefectLong")
//STATUS_LABEL(Clr<BorderKlass2<Cross>>, "2 класс")
//STATUS_LABEL(Clr<BorderDefect<Cross>>, "\"Брак\"")

//template<class X>struct __status_label__<Clr<BorderKlass2<X>>>
//{
//	static char *text;
//};
//template<class X>char *__status_label__<Clr<BorderKlass2<X>>>::text = "2 класс ";

//---------------------------------------------------------------------------
template<class O, class P>struct __set_color_bar__;
//{
//	typedef typename O::__template_must_be_overridded__ noused;
//};

//ColorBar::ColorBar(double &data, unsigned &color, char status)
//: data(data)
//, color(color)
//, status(status)
//{
//	//TL::find<PrimaryData::message_list, __set_color_bar__>()((TL::Factory<PrimaryData::message_list> *)0, this);
//}
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

void ColorBar(double &data, unsigned &color, int id, double defData)
{
	__data_color__ d(id, color, data, defData);
	TL::find<ColorTable::items_list, __set_color_bar__>()((TL::Factory<ColorTable::items_list> *)0, &d);
}

/*
#define COLOR(O)template<class P>struct __set_color_bar__<O, P>\
{\
	int &color;\
	__set_color_bar__() : color(Singleton<ColorTable>::Instance().items.get<O>().value){}\
	bool operator()(O *, P *p)\
    {\
        if(TL::IndexOf<ColorTable::items_list, O>::value == p->id)\
		{\
		     p->color = color;\
             return false;\
		}\
		return true;\
    }\
};
*/

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

////////////////COLOR_DATA(LessMinimumEnergy)
////////////////COLOR_DATA(GreaterMaximumEnergy)
////////////////COLOR_DATA(ValueLessThreshold)
////////////////COLOR_DATA(ThicknessLessThreshold)
////////////////COLOR_DATA(ThicknessGreaterThreshold)
COLOR_DATA(Undefined)
COLOR_DATA(DeathZone)


//COLOR(Nominal)
//COLOR(Defect)
//COLOR(Treshold2Class)
//COLOR(AboveNorm)
//COLOR(BelowNorm)
//COLOR(BelowAboveNorm)

//COLOR(DefectAboveNorm)


//COLOR(Clr<BorderKlass2<Long>>)
//COLOR(Clr<BorderAbove<Thickness>>)
//COLOR(Clr<BorderLower<Thickness>>)
//COLOR(Clr<BorderNominal<Thickness>>)
//COLOR(Clr<BorderDefect<Long>>)
//COLOR(Clr<BorderKlass2<Cross>>)
//COLOR(Clr<BorderDefect<Cross>>)


//COLOR(__(Clr<BorderDefect<Cross>, BorderAbove<Thickness>>))

//#define xxx_struct_1(a) a
//#define xxx_struct_2(a, b) a, b
//#define xxx_struct_3(a, b, c) a, d, c
//#define xxx_struct_4(a, b, c, d) a, b, c, d
//
//#define MK(a) mk##a
//
//#define xxx_txt_1(a) MK(a)
//#define xxx_txt_2(a, b) MK(a)##MK(b)
//#define xxx_txt_3(a, b, c) MK(a)##MK(b)##MK(c)
//#define xxx_txt_4(a, b, c, d) MK(a)##MK(b)MK(c)##MK(d)

#define TXT(a, b) a##_##b
#define STR(a, b) a<b>
#define PARAM_STR(a, b) STR(a, b)
#define PARAM_TXT(a, b) TXT(a, b)

//#define STATUS_LABEL_1(a)template<>struct __status_label__<Clr<a>>\
//{\
//	static char *text;\
//};\
//char *__status_label__<O>::text = MK(a);

#define STATUS_LABEL_2(a, b)template<>struct __status_label__<Clr<PARAM_STR##a, PARAM_STR##b>>\
{\
	static char *text;\
};\
char *__status_label__<Clr<PARAM_STR##a, PARAM_STR##b>>::text = PARAM_TXT##a##PARAM_TXT##b;




#define BorderDefect_Cross "\"поперечный дефект\", "
#define BorderLower_Thickness "\"толщина меньше нормы\", "
#define BorderAbove_Thickness "\"толщина выше нормы\", "

STATUS_LABEL_2((BorderDefect, Cross), (BorderAbove, Thickness))

STATUS_LABEL_2((BorderDefect, Cross), (BorderLower, Thickness))
STATUS_LABEL(___(Clr<BorderDefect<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>>), "")



////////////////////COLOR(DefectMinMax)
////////////////////COLOR(DefectDifferentWall)
////////////////////
////////////////////COLOR(DefectDifferentWallMin)
////////////////////COLOR(DefectDifferentWallMax   )
////////////////////COLOR(DefectDifferentWallMinMax)
#undef	__
#undef	___
#undef COLOR
#undef COLOR_DATA
#undef STATUS_LABEL
//------------------------------------------------------------------------------------------------
