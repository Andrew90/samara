#pragma once
#include "AppBase.h"
#include "ScanWindow.h"
#include "ItemIni.h"
#include "ut_files.h"

namespace
{
	template<class T>struct Title;
	template<>struct Title<Cross>{wchar_t *operator()(){return L"����������";}};
	template<>struct Title<Long>{wchar_t *operator()(){return L"����������";}};
	template<>struct Title<Thickness>{wchar_t *operator()(){return L"�������";}};
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
				wsprintf(label.buffer, L"<ff>���� <ff0000>%d <ff>������ <ff0000>%d <ff>�������� %d  �������� %S   %S     "
					//, 1 + owner->lastZone, 1 + N, offsetX, Wchar_from<double, 5>(valY)(), s);
					, 1 + owner->lastZone, 1 + N, 1 + offsetX, val, s);
			}
			else
			{
				wsprintf(label.buffer, L"<ff>���� <ff0000>%d <ff>������ <ff0000>%d <ff>�������� %d   %S     "
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
		wsprintf(buffer, L"<ff>��������� %d", d->hdr.G1Amp);
		return buffer;
	}
};

template<>struct __for_label__<Thickness>
{
	wchar_t buffer[128];
	wchar_t *operator()(USPC7100_ASCANDATAHEADER *d)
	{
		wsprintf(buffer, L"<ff>�������� %d  ��������� %d", d->hdr.G1Tof, d->hdr.G1Amp);
		return buffer;
	}
};

template<class>struct __gates__
{
	void operator()(ScanWindow &s, int)
	{
		s.chart.items.get<ScanWindow::GateIF>().visible = false;
		s.chart.items.get<ScanWindow::Gate1>().visible = false;
	}
};

template<>struct __gates__<Thickness>
{
	void operator()(ScanWindow &s, int g1Tof)
	{
		wchar_t path[256];
		if(!ExistCurrentUSPCFile(path)) return;
		ScanWindow::GateIF &gif = s.chart.items.get<ScanWindow::GateIF>();
		gif.visible = true;

		double scope_offset = 0;
		scope_offset = ItemIni::Get(L"Test 0", L"scope_offset", scope_offset, path); 
		double gateIF_position = 0;
		gateIF_position = ItemIni::Get(L"Test 0", L"gateIF_position", gateIF_position, path); 
		double gateIF_width = 0;
		gateIF_width = ItemIni::Get(L"Test 0", L"gateIF_width", gateIF_width, path); 

		double gateIF_level = 0;
		gateIF_level = ItemIni::Get(L"Test 0", L"gateIF_level", gateIF_level, path); 

		gif.x = (gateIF_position - scope_offset) * 25;
		gif.width = gateIF_width;
		gif.y = gateIF_level;

		ScanWindow::Gate1 &g1 = s.chart.items.get<ScanWindow::Gate1>();
		g1.visible = true;

		double gate1_width = 0;
		gate1_width = ItemIni::Get(L"Test 0", L"gate1_width", gate1_width, path); 

		double gate1_position = 0;
		gate1_position = ItemIni::Get(L"Test 0", L"gate1_position", gate1_position, path); 

		double gate1_level = 0;
		gate1_level = ItemIni::Get(L"Test 0", L"gate1_level", gate1_level, path); 

		g1.x = gate1_position + g1Tof;
		g1.width = gate1_width ;
		g1.y = gate1_level;
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
		ScanWindow &s = Singleton<ScanWindow>::Instance();
		__gates__<Ascan>()(s,  d.scan->hdr.G1Amp);
		s.Open(
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

