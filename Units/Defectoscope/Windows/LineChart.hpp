#pragma once
#include "AppBase.h"
#include "ScanWindow.h"
#include "FFT.h"

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
			double dX = (chart->rect.right - chart->rect.left - chart->offsetAxesLeft - chart->offsetAxesRight)
				/(chart->maxAxesX - chart->minAxesX);
			storedMouseMove.x = (WORD)(chart->rect.left + chart->offsetAxesLeft + dX * offsetX + dX / 2);

			double valY = ((Parent::TChart *)chart)->items.get<BarSeries>().ValueY(offsetX);
			int color;
			bool b;
			char *s = StatusText()(dataViewer.status[offsetX], color, b);
			if(TL::IndexOf<ColorTable::items_list, Clr<Undefined>>::value != dataViewer.status[offsetX])
			{
				char val[128];
				sprintf(val, "%.1f", valY);
				wsprintf(label.buffer, L"<ff>Зона <ff0000>%d <ff>датчик <ff0000>%d <ff>смещение %d  величина %S   %S     "
					//, 1 + owner->lastZone, 1 + N, offsetX, Wchar_from<double, 5>(valY)(), s);
					, 1 + owner->lastZone, 1 + N, 1 + offsetX, val, s);
			}
			else
			{
				wsprintf(label.buffer, L"<ff>Зона <ff0000>%d <ff>датчик <ff0000>%d <ff>смещение %d   %S     "
					, 1 + owner->lastZone, 1 + N, 1 + offsetX, s);
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
			typedef typename O::Parent::Parent TWhell;
			
			o->offsetX = p->offs;
			TMouseWell w = {o->hWnd, WM_MOUSEWHEEL};
			(*(TWhell *)o)(w);
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

template<class O>struct MinY
{
	static const int value = 0;
};
template<class O>struct MaxY
{
	static const int value = 100;
};

template<>struct MinY<Thickness>
{
	static const int value = -128;
};
template<>struct MaxY<Thickness>
{
	static const int value = 127;
};

template<class T>struct ComputeFFT
{
	void operator()(unsigned char(&c)[512], double(&d)[512])
	{
		memset(d, 0, dimention_of(d));
	}
};

template<>struct ComputeFFT<Thickness>
{
	void operator()(unsigned char(&c)[512], double(&d)[512])
	{
		static const int count = dimention_of(d);
		for(int i = 0; i < count; ++i)
		{
			d[i] = c[i] - 128;
		}
		
		
		CFFT fft;
		
		fft.Init(count - 1);
		fft.Direct(d);
	    fft.Spectrum(d);
		//for(int i = 0; i < 120; ++i)
		//{
		//	d[i] = 0;
		//}
		//for(int i = 180; i < count; ++i)
		//{
		//	d[i] = 0;
		//}
		fft.Direct(d);		
        fft.Spectrum(d);	
		//
		for(int i = 0; i < count; ++i)
		{
			d[i] *= 200;
		}
	}
};

template<class T> struct Scan
{
	static void Do(int zone, int sens, int offs, void *o, void(*ptr)())
	{
        if(NULL == FindWindow(WindowClass<T>()(), 0))return;
		typedef typename T::sub_type Ascan;
		ItemData<Ascan> &data = Singleton<ItemData<Ascan>>::Instance();
		if(data.currentOffsetZones <= zone) return;
		int of = (data.offsets[zone + 1] - data.offsets[zone]) / App::count_sensors - 1;
		if(of < offs)
		{
			offs = of;
		}
		if(offs < 0)
		{
			offs = 0;
		}
		__scan_data__ d = {sens, zone, offs, NULL};
		TL::find<typename T::viewers_list, __scan__>()(&((T *)o)->viewers, &d);
		ScanWindow &sc = Singleton<ScanWindow>::Instance();
		sc.minY = MinY<Ascan>::value;
		sc.maxY = MaxY<Ascan>::value;
		ComputeFFT<Thickness>()(
			  d.scan->Point
		    , sc.chart.items.get<RedLineSeries>().buf
		);
		sc.Open(
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

