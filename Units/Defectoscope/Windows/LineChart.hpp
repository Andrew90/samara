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
		Scan<T>::Do(owner->lastZone, N, offsetX, owner, (void(*)())Scan<T>::Do);
	}
};

namespace
{
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

template<class T>struct __for_label__
{
	wchar_t buffer[128];
	wchar_t *operator()(USPC7100_ASCANDATAHEADER *d)
	{
		wsprintf(buffer, L"<ff>амплитуда %d", d->hdr.G1Amp);
		return buffer;
	}
};

template<>struct __for_label__<Thickness>
{
	wchar_t buffer[128];
	wchar_t *operator()(USPC7100_ASCANDATAHEADER *d)
	{
		wsprintf(buffer, L"<ff>смещение %d  амплитуда %d", d->hdr.G1Tof, d->hdr.G1Amp);
		return buffer;
	}
};

template<class T> struct Scan
{
	static void Do(int zone, int sens, int offs, void *o, void(*ptr)())
	{
        if(NULL == FindWindow(WindowClass<T>()(), 0))return;
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

		//g1Tof = uspc->hdr.G1Tof;
		 //g1Amp = uspc->hdr.G1Amp;
		 //wsprintf(label.buffer, L"<ff>смещение %d  амплитуда %d", g1Tof, g1Amp);
		
		Singleton<ScanWindow>::Instance().Open(
			zone
			, sens
			, offs
			, Title<Ascan>()()
			, __for_label__<Ascan>()(d.scan)
			, d.scan
			, o
			, ptr
			);
	}
};
}

