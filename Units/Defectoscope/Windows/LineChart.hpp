#pragma once
#include "AppBase.h"
#include "ScanWindow.h"

namespace
{
	template<class T>struct Title;
	template<>struct Title<Cross>{wchar_t *operator()(){return L"Поперечный";}};
	template<>struct Title<Long>{wchar_t *operator()(){return L"Продольный";}};
	template<>struct Title<Thickness>{wchar_t *operator()(){return L"Толщина";}};
};

template<class T>struct DefVal
{
	double operator()(double d, int){return d;};
};

template<>struct DefVal<Thickness>
{
	double operator()(double d, int zone)
	{
		if(0.0 == d)
		{
			return Singleton<ThresholdsTable>::Instance().items.get<BorderNominal<Thickness> >().value[zone];
		}
		return d;
	}
};

template<class T, int N>struct Line: LineTresholdsViewer<typename TL::SelectT<ThresholdsTable::items_list, typename T::sub_type>::Result>
{
	typedef LineTresholdsViewer<typename TL::SelectT<ThresholdsTable::items_list, typename T::sub_type>::Result> Parent;
	T *owner;		
	DataViewer<typename T::sub_type> dataViewer;
	Line()
	{
		((Parent::TChart *)chart)->items.get<BarSeries>().SetColorBarHandler(this, &Line::GetColorBar);
		cursor->SetMouseMoveHandler(this, &Line<T, N>::CursorDraw);
	}	
	bool GetColorBar(int offs, double &data, unsigned &color)
	{
		if(NULL != dataViewer.data && offs < dataViewer.count)
		{
			data = DefVal<typename T::sub_type>()(dataViewer.data[offs], offs);
			color = ConstData::ZoneColor(dataViewer.status[offs]);
			return true;
		}
		return false;
	}

	bool CursorDraw(TMouseMove &l, VGraphics &g)	  
	{	
		if(owner->drawZones)
		{
			double valY = ((Parent::TChart *)chart)->items.get<BarSeries>().ValueY(offsetX);
			int color;
			bool b;
			char *s = StatusText()(dataViewer.status[offsetX], color, b);
			if(TL::IndexOf<ColorTable::items_list, Clr<Undefined>>::value != dataViewer.status[offsetX])
			{
				wsprintf(label.buffer, L"<ff>Зона <ff0000>%d <ff>датчик <ff0000>%d <ff>смещение %d  величина %s   %S     "
					, 1 + owner->lastZone, 1 + N, offsetX, Wchar_from<double, 5>(valY)(), s);
			}
			else
			{
				wsprintf(label.buffer, L"<ff>Зона <ff0000>%d <ff>датчик <ff0000>%d <ff>смещение %d   %S     "
					, 1 + owner->lastZone, 1 + N, offsetX, s);
			}
		}
		else
		{
			label.buffer[0] = 0;
		}
		label.Draw(g());
		return true;
	}

	void operator()(TRButtonDown &l)
	{
		Scan<T>::Do(owner->lastZone, N, offsetX, owner, (void(*)())Scan<T>::Do);//, dataViewer.scan[offsetX]);
	}
};

//Set(zone, start, stop, channel, offs, maxOffs, s);

//template<class T, int N>struct Line
namespace
{
//template<template<class, int>class Wapper, class T, int start, int max>struct CreateNumList
//{
//	typedef Tlst<Wapper<T, start>, typename CreateNumList<Wapper, T, 1 + start, max>::Result> Result;
//};
//template<template<class, int>class Wapper, class T, int max>struct CreateNumList<Wapper, T, max, max>
//{
//	typedef Tlst<Wapper<T, max>, NullType> Result;
//};

template<class O, class P>struct __scan__
{
	bool operator()(O *o, P *p)
	{
		return true;
	}
};

struct __scan_data__
{
	int channel;
	int zone;
	int offs;
	 USPC7100_ASCANDATAHEADER *scan;
};

template<template<class, int>class L, class T, int N, class P>struct __scan__<L<T, N>, P>
{
	typedef typename L<T, N> O;
	bool operator()(O *o, P *p)
	{
		if(N == p->channel)
		{
			o->dataViewer.Do(p->zone, p->channel);
			p->scan = o->dataViewer.scan[p->offs];
			return false;
		}
		return true;
	}
};


template<class T> struct Scan
{
	//typedef typename CreateNumList<Line, T, 0, App::count_sensors - 1>::Result line_list;
	static void Do(int zone, int sens, int offs, void *o, void(*ptr)())//, USPC7100_ASCANDATAHEADER *scan)
	{
		typedef typename T::sub_type Ascan;
		ItemData<Ascan> &data = Singleton<ItemData<Ascan>>::Instance();
		int of = (data.offsets[zone + 1] - data.offsets[zone]) / App::count_sensors - 1;
		if(of < offs)
		{
			++zone;
			offs = 0;

		}
		if(offs < 0)
		{
			--zone;
			if(zone >= 0)
			{
				 offs = (data.offsets[zone + 1] - data.offsets[zone]) / App::count_sensors - 1;
			}
			else
			{
				offs = zone = 0;
			}
		}
		__scan_data__ d = {sens, zone, offs, NULL};
		TL::foreach<typename T::viewers_list, __scan__>()(&((T *)o)->viewers, &d);
		//int x = data.offsets[zone];
		////dprint("1offs %d first zone %d last zone %d\n", x
		//	, data.offsets[zone] +  data.offsSensor[sens]
		//, data.offsets[zone + 1] +  data.offsSensor[sens]);
		//x += data.offsSensor[sens];
		//dprint("2offs %d sensor %d, offset sens %d\n", x, sens, data.offsSensor[sens]);
		//x += offs * App::count_sensors + sens;
		//dprint("G1Amp %d  G1Tof %d\n", data.ascanBuffer[x].hdr.G1Amp,  data.ascanBuffer[x].hdr.G1Tof);
		//dprint("3offs %d offsetX %d channel %d  ----\n", x, offs, data.ascanBuffer[x].Channel);
		
		//if(x < 0) return;

		Singleton<ScanWindow>::Instance().Open(
			zone
			, sens
			, offs
			, Title<Ascan>()()
			, d.scan
			, o
			, ptr
			);
	}
};
}

