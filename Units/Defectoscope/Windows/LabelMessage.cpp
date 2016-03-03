#include "stdafx.h"
#include "LabelMessage.h"
#include "AppBase.h"
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

namespace
{
	struct __data_text__
	{
		int id;
		char *text;
	};
	template<class O, class P>struct __select__
	{
		bool operator()(O *, P *p)
		{
			if(TL::IndexOf<ColorTable::items_list, O>::value == p->id)
			{
				p->text = __status_label__<O>::text;
				return false;
			}
			return true;
		}
	};
}

char *StatusText(int id)
{
	__data_text__ data = {id, NULL};
	TL::find<ColorTable::items_list, __select__>()((TL::Factory<ColorTable::items_list> *)0, &data);
	return data.text;
}


STATUS_LABEL(LessMinimumEnergy, "Энергия сигнала меньше порогового значения")
//STATUS_LABEL(GreaterMaximumEnergy, "Энергия сигнала больше порогового значения")
STATUS_LABEL(ValueLessThreshold, "Сигнал меньше порогового значения")
STATUS_LABEL(ThicknessLessThreshold, "Вычисленная толщина меньше минимального значения")
//STATUS_LABEL(ThicknessGreaterThreshold, "Вычисленная толщина больше максимального значения")
STATUS_LABEL(Undefined, "Результат не определён")
STATUS_LABEL(DeathZone, "Мёртвая зона")

STATUS_LABEL(Nominal, "<ff00>Норма")
STATUS_LABEL(Defect, "<ff0000>Брак")
STATUS_LABEL(Treshold2Class, "<ff>Класс 2")
//STATUS_LABEL(DefectMinMax, "<ffff00>Брак плюс-минус")
//STATUS_LABEL(DefectDifferentWall, "<ffff>Брак разностенность")

//STATUS_LABEL(DefectDifferentWallMin   , "<993333>Брак разностенность минус")
//STATUS_LABEL(DefectDifferentWallMax   , "<7749ff>Брак разностенность плюс")
//STATUS_LABEL(DefectDifferentWallMinMax, "<663300>Брак разностенность плюс-минус")

#undef STATUS_LABEL
//---------------------------------------------------------------------------
template<class O, class P>struct __set_color_bar__
{
	typedef typename O::__template_must_be_overridded__ noused;
};

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

COLOR_DATA(LessMinimumEnergy)
//COLOR_DATA(GreaterMaximumEnergy)
COLOR_DATA(ValueLessThreshold)
COLOR_DATA(ThicknessLessThreshold)
//COLOR_DATA(ThicknessGreaterThreshold)
COLOR_DATA(Undefined)
COLOR_DATA(DeathZone)


COLOR(Nominal)
COLOR(Defect)
COLOR(Treshold2Class)
//COLOR(DefectMinMax)
//COLOR(DefectDifferentWall)
//
//COLOR(DefectDifferentWallMin)
//COLOR(DefectDifferentWallMax   )
//COLOR(DefectDifferentWallMinMax)
#undef COLOR
#undef COLOR_DATA
//------------------------------------------------------------------------------------------------
