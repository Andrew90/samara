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
	void operator()(ScanWindow &s, USPC7100_ASCANDATAHEADER *)
	{
		s.chart.items.get<ScanWindow::GateIF>().visible = false;
		s.chart.items.get<ScanWindow::Gate1>().visible = false;
	}
};

template<>struct __gates__<Thickness>
{
	void operator()(ScanWindow &s, USPC7100_ASCANDATAHEADER *d)
	{
		double k = 10000 == d->TimeEqu ? 1.5 : 1.0;
		wchar_t path[256];
		if(!ExistCurrentUSPCFile(path)) return;
		ScanWindow::GateIF &gif = s.chart.items.get<ScanWindow::GateIF>();
		gif.visible = true;

		wchar_t section[16];
		wsprintf(section, L"Test %d", d->Channel);
        dprint("Channel %d\n", d->Channel);
// todo  ������ ������ ��� ���������
		double scope_range = 0;
        scope_range = ItemIni::Get(section, L"scope_range", scope_range, path); 
		dprint("scope_range %f\n", scope_range);
		double dX = scope_range / d->DataSize;
		double scope_offset = 0;
		scope_offset = ItemIni::Get(section, L"scope_offset", scope_offset, path); 
		dprint("scope_offset %f\n", scope_offset);
		double gateIF_position = 0;
		gateIF_position = ItemIni::Get(section, L"gateIF_position", gateIF_position, path);
		dprint("gateIF_position %f\n", gateIF_position);
		double gateIF_width = 0;
		gateIF_width = ItemIni::Get(section, L"gateIF_width", gateIF_width, path); 
		dprint("gateIF_width %f\n", gateIF_width);

		double gateIF_level = 0;
		gateIF_level = ItemIni::Get(section, L"gateIF_level", gateIF_level, path); 
		dprint("gateIF_level %f\n", gateIF_level);

		gif.x = k * (gateIF_position - scope_offset) / dX;
		gif.width = k * gateIF_width / dX;
		gif.y = gateIF_level;

		ScanWindow::Gate1 &g1 = s.chart.items.get<ScanWindow::Gate1>();
		g1.visible = true;

		double gate1_width = 0;
		gate1_width = ItemIni::Get(section, L"gate1_width", gate1_width, path); 
		dprint("gate1_width %f\n", gate1_width);

		double gate1_position = 0;
		gate1_position = ItemIni::Get(section, L"gate1_position", gate1_position, path);
		dprint("gate1_position %f\n", gate1_position);

		double gate1_level = 0;
		gate1_level = ItemIni::Get(section, L"gate1_level", gate1_level, path);
		dprint("gate1_level %f\n", gate1_level);

		double x = 5e-3 * d->hdr.GIFTof - scope_offset;
		g1.x = k * (gate1_position + x) / dX;
		g1.width = k * gate1_width / dX;
		dprint("x %f  gate1_position %f  d->hdr.GIFTof %d  %d %d\n", x, gate1_position, d->hdr.GIFTof, d->TimeEqu, d->hdr.G1Tof);
		g1.y = gate1_level;

		double thick =  (k * (0.005 * d->hdr.GIFTof/* + 2.5e-3 * d->hdr.G1Tof*/) - scope_offset) / dX;
		s.chart.items.get<ScanWindow::ThickBorder>().value = thick;
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
		__gates__<Ascan>()(s,  d.scan);
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

