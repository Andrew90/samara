#pragma once
#include "AppBase.h"
#include "ScanWindow.h"
#include "ItemIni.h"

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
	wchar_t *operator()(USPC7100_ASCANDATAHEADER *d, ScanWindow &s)
	{
		return s.label.buffer;
	}
};

template<>struct __for_label__<Thickness>
{
	wchar_t buffer[128];
	wchar_t *operator()(USPC7100_ASCANDATAHEADER *d, ScanWindow &s)
	{
		wsprintf(buffer, L"<ff>�������� %s  ��������� %d"
			, Wchar_from<double>(0.005 * d->hdr.G1Tof)()
			, d->hdr.G1Amp
			);
		return buffer;
	}
};
#define USPC(name) double _##name = uspc.param[d->Channel].get<name>().value;
template<class T>struct __gates__
{
	void operator()(ScanWindow &s, USPC7100_ASCANDATAHEADER *d)
	{
		s.chart.items.get<ScanWindow::Gate1>().visible = true;
		s.chart.items.get<ScanWindow::GateIF>().visible = false;
		s.chart.items.get<ScanWindow::GateIFBorder>().visible = false;
		s.chart.items.get<ScanWindow::Gate1Border>().visible = false;
// todo  ������ ������ ��� ���������
		ItemData<T> &uspc = Singleton<ItemData<T>>::Instance();
		USPC(scope_range);
		dprint("scope_range %f\n", _scope_range);
		USPC(scope_offset);
		dprint("scope_offset %f\n", _scope_offset);
		//_scope_offset = 0.5 * int(_scope_offset / 0.5);
		s.chart.minAxesX = _scope_offset;
		s.chart.maxAxesX = _scope_offset + _scope_range;

		ScanWindow::Gate1 &g1 = s.chart.items.get<ScanWindow::Gate1>();

		USPC(gate1_width);
		dprint("gate1_width %f\n", _gate1_width);

		USPC(gate1_position);
		dprint("gate1_position %f\n", _gate1_position);

		USPC(gate1_level);
		dprint("gate1_level %f\n", _gate1_level);

		g1.x = (_gate1_position + _scope_offset);
		g1.width = _gate1_width;
		double offs = 0.005 * d->hdr.G1Tof;
		dprint("gate1_position %f  offs %f  %d %d\n", _gate1_position, offs, d->TimeEqu, d->hdr.G1Tof);
		g1.y = _gate1_level;

		
		int count = d->DataSize;
		if(0 == count) count = 500;
		double mash = s.chart.items.get<ScanWindow::Line>().mash = 0.001 * d->TimeEqu / count;
		s.chart.items.get<ScanWindow::Line>().count = count;
        s.chart.items.get<ScanWindow::Line>().offset = 0;

		s.chart.items.get<ScanWindow::Gate1Border>().value = 0.005 * d->hdr.G1Tof - _scope_offset;
		int gate1Amp = d->hdr.G1Amp;

		int end = int((_gate1_position + _gate1_width) / mash);
		int beg = int(_gate1_position / mash);
		if(end > count)	end = count;
		if(beg < 0) beg = 0;
		wchar_t buf[128];
		buf[0] = 0;
		int amp = 0;
		for(int i = beg; i < end; ++i)
		{
			if(d->Point[i] > amp)
			{
				amp	= d->Point[i];	
			}
		}
		
		wsprintf(buf, L"<ff>��������� %d", amp);
		s.label = buf;
		
	}
};

template<>struct __gates__<Thickness>
{
	void operator()(ScanWindow &s, USPC7100_ASCANDATAHEADER *d)
	{
		ItemData<Thickness> &uspc = Singleton<ItemData<Thickness>>::Instance();
		
		s.chart.items.get<ScanWindow::GateIFBorder>().visible = false;
		s.chart.items.get<ScanWindow::Gate1Border>().visible = false;
		s.chart.items.get<ScanWindow::GateIF>().visible = false;
		s.chart.items.get<ScanWindow::Gate1>().visible = false;

		ScanWindow::GateIF &gif = s.chart.items.get<ScanWindow::GateIF>();
		gif.visible = true;

        dprint("Channel %d\n", d->Channel);
// todo  ������ ������ ��� ���������
		USPC(scope_range);
		dprint("scope_range %f\n", _scope_range);
		USPC(scope_offset);
		dprint("scope_offset %f\n", _scope_offset);
		//_scope_offset = 0.5 * int(_scope_offset / 0.5);
		s.chart.minAxesX = _scope_offset;
		s.chart.maxAxesX = _scope_offset + _scope_range;

		USPC(gateIF_position);
		dprint("gateIF_position %f\n", _gateIF_position);
		USPC(gateIF_width);
		dprint("gateIF_width %f\n", _gateIF_width);

		USPC(gateIF_level);
		dprint("gateIF_level %f\n", _gateIF_level);

		gif.x = _gateIF_position;
		gif.width = _gateIF_width;
		gif.y = _gateIF_level;

		ScanWindow::Gate1 &g1 = s.chart.items.get<ScanWindow::Gate1>();

		USPC(gate1_width);
		dprint("gate1_width %f\n", _gate1_width);

		USPC(gate1_position);
		dprint("gate1_position %f\n", _gate1_position);

		USPC(gate1_level);
		dprint("gate1_level %f\n", _gate1_level);

		int count = d->DataSize;
		if(0 == count) count = 500;
		double mash = s.chart.items.get<ScanWindow::Line>().mash = 0.001 * d->TimeEqu / count;
		s.chart.items.get<ScanWindow::Line>().count = count;
        s.chart.items.get<ScanWindow::Line>().offset = 0;

		s.chart.items.get<ScanWindow::GateIFBorder>().value = 0.005 * d->hdr.GIFTof;
		s.chart.items.get<ScanWindow::Gate1Border>().value = 0.005 * (d->hdr.G1Tof + d->hdr.GIFTof);
		dprint("gateif %f gate1 %f\n", 0.005 * d->hdr.GIFTof, 0.005 * (d->hdr.G1Tof + d->hdr.GIFTof)) ;
		if(0 != d->hdr.GIFTof)
		{
			int i = 0;
			dprint("gif position %f\n", 0.005 *  d->hdr.GIFTof);
			for(; i < count; ++i)
			{
				if(d->Point[i] > _gateIF_level)
				{
					s.chart.items.get<ScanWindow::GateIFBorder>().visible = true;
					s.chart.items.get<ScanWindow::GateIF>().visible = true;
					s.chart.items.get<ScanWindow::Line>().offset = 
					  int(i - (0.005 * d->hdr.GIFTof - _scope_offset) / mash);
					break;
				}
			}
			if(0 != d->hdr.G1Tof)
			{
				s.chart.items.get<ScanWindow::Gate1Border>().visible = true;
				s.chart.items.get<ScanWindow::Gate1>().visible = true;
				double x = 5e-3 * d->hdr.GIFTof;
				g1.x = _gate1_position + x - s.chart.items.get<ScanWindow::Line>().offset * mash;
				g1.width = _gate1_width;
				g1.y = _gate1_level;
				dprint("offs gate1 %f\n", g1.x);
				s.chart.items.get<ScanWindow::Gate1Border>().value = 0.005 * (d->hdr.G1Tof + d->hdr.GIFTof);
			}
		}
	}
};
#undef USPC

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
			, __for_label__<Ascan>()(d.scan, s)
			, d.scan
			, o
			, ptr
			);
	}
};
}

