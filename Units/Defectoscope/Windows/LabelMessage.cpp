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


STATUS_LABEL(LessMinimumEnergy, "������� ������� ������ ���������� ��������")
//STATUS_LABEL(GreaterMaximumEnergy, "������� ������� ������ ���������� ��������")
STATUS_LABEL(ValueLessThreshold, "������ ������ ���������� ��������")
STATUS_LABEL(ThicknessLessThreshold, "����������� ������� ������ ������������ ��������")
//STATUS_LABEL(ThicknessGreaterThreshold, "����������� ������� ������ ������������� ��������")
STATUS_LABEL(Undefined, "��������� �� ��������")
STATUS_LABEL(DeathZone, "̸����� ����")

STATUS_LABEL(Nominal, "<ff00>�����")
STATUS_LABEL(Defect, "<ff0000>����")
STATUS_LABEL(Treshold2Class, "<ff>����� 2")
//STATUS_LABEL(DefectMinMax, "<ffff00>���� ����-�����")
//STATUS_LABEL(DefectDifferentWall, "<ffff>���� ��������������")

//STATUS_LABEL(DefectDifferentWallMin   , "<993333>���� �������������� �����")
//STATUS_LABEL(DefectDifferentWallMax   , "<7749ff>���� �������������� ����")
//STATUS_LABEL(DefectDifferentWallMinMax, "<663300>���� �������������� ����-�����")

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
