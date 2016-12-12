#pragma once
#include "AppBase.h"
#include "ScanWindow.h"
#include "ItemIni.h"
#include "ut_files.h"

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
		wsprintf(buffer, L"<ff>смещение %s  амплитуда %d"
			, Wchar_from<double>(0.005 * d->hdr.G1Tof)()
			, d->hdr.G1Amp
			);
		return buffer;
	}
};

template<class>struct __gates__
{
	void operator()(ScanWindow &s, USPC7100_ASCANDATAHEADER *d)
	{
		wchar_t path[256];
		if(!ExistCurrentUSPCFile(path)) return;

		wchar_t section[16];
		wsprintf(section, L"Test %d", d->Channel);
        dprint("Channel %d\n", d->Channel);
// todo  расчёт гайтов для отрисовки
		double scope_range = 0;
        scope_range = ItemIni::Get(section, L"scope_range", scope_range, path); 
		dprint("scope_range %f\n", scope_range);
		double scope_offset = 0;
		scope_offset = ItemIni::Get(section, L"scope_offset", scope_offset, path); 
		dprint("scope_offset %f\n", scope_offset);
		scope_offset = 0.5 * int(scope_offset / 0.5);
		s.chart.minAxesX = scope_offset;
		s.chart.maxAxesX = scope_offset + scope_range;

		ScanWindow::Gate1 &g1 = s.chart.items.get<ScanWindow::Gate1>();

		double gate1_width = 0;
		gate1_width = ItemIni::Get(section, L"gate1_width", gate1_width, path); 
		dprint("gate1_width %f\n", gate1_width);

		double gate1_position = 0;
		gate1_position = ItemIni::Get(section, L"gate1_position", gate1_position, path);
		dprint("gate1_position %f\n", gate1_position);

		double gate1_level = 0;
		gate1_level = ItemIni::Get(section, L"gate1_level", gate1_level, path);
		dprint("gate1_level %f\n", gate1_level);

		g1.x = (gate1_position + scope_offset);
		g1.width = gate1_width;
		double offs = 0.005 * d->hdr.G1Tof;
		dprint("gate1_position %f  offs %f  %d %d\n", gate1_position, offs, d->TimeEqu, d->hdr.G1Tof);
		g1.y = gate1_level;

		
		int count = d->DataSize;
		if(0 == count) count = 500;
		double mash = s.chart.items.get<ScanWindow::Line>().mash = 0.001 * d->TimeEqu / count;
		s.chart.items.get<ScanWindow::Line>().count = count;
        s.chart.items.get<ScanWindow::Line>().offset = 0;

		s.chart.items.get<ScanWindow::Gate1Border>().value = 0.005 * d->hdr.G1Tof - scope_offset;
		int gate1Amp = d->hdr.G1Amp;

		int end = int((gate1_position + gate1_width) / mash) + 1;
		int beg = int(gate1_position / mash) - 1;
		if(end > count)	end = count;
		if(beg < 0) beg = 0;
		wchar_t buf[128];
		buf[0] = 0;
		int amp = (int)gate1_level;
		bool b = false;
		for(int i = beg; i < end; ++i)
		{
			if(d->Point[i] > amp)
			{
				double gate1AmpOffs = i * mash + scope_offset;
				int toffs =  int((gate1AmpOffs -  0.005 * d->hdr.G1Tof) / mash);
				s.chart.items.get<ScanWindow::Line>().offset = 	toffs;
				amp	= d->Point[i];	
				b = true;
			}
		}
		if(b) wsprintf(buf, L"<ff>Амплитуда %d", amp);
		s.label = buf;
		
	}
};

template<>struct __gates__<Thickness>
{
	void operator()(ScanWindow &s, USPC7100_ASCANDATAHEADER *d)
	{
		wchar_t path[256];
		if(!ExistCurrentUSPCFile(path)) return;

		s.chart.items.get<ScanWindow::GateIFBorder>().visible = true;

		ScanWindow::GateIF &gif = s.chart.items.get<ScanWindow::GateIF>();
		gif.visible = true;

		wchar_t section[16];
		wsprintf(section, L"Test %d", d->Channel);
        dprint("Channel %d\n", d->Channel);
// todo  расчёт гайтов для отрисовки
		double scope_range = 0;
        scope_range = ItemIni::Get(section, L"scope_range", scope_range, path); 
		dprint("scope_range %f\n", scope_range);
		double scope_offset = 0;
		scope_offset = ItemIni::Get(section, L"scope_offset", scope_offset, path); 
		dprint("scope_offset %f\n", scope_offset);
		scope_offset = 0.5 * int(scope_offset / 0.5);
		s.chart.minAxesX = scope_offset;
		s.chart.maxAxesX = scope_offset + scope_range;

		double gateIF_position = 0;
		gateIF_position = ItemIni::Get(section, L"gateIF_position", gateIF_position, path);
		dprint("gateIF_position %f\n", gateIF_position);
		double gateIF_width = 0;
		gateIF_width = ItemIni::Get(section, L"gateIF_width", gateIF_width, path); 
		dprint("gateIF_width %f\n", gateIF_width);

		double gateIF_level = 0;
		gateIF_level = ItemIni::Get(section, L"gateIF_level", gateIF_level, path); 
		dprint("gateIF_level %f\n", gateIF_level);

		gif.x = gateIF_position;
		gif.width = gateIF_width;
		gif.y = gateIF_level;

		ScanWindow::Gate1 &g1 = s.chart.items.get<ScanWindow::Gate1>();

		double gate1_width = 0;
		gate1_width = ItemIni::Get(section, L"gate1_width", gate1_width, path); 
		dprint("gate1_width %f\n", gate1_width);

		double gate1_position = 0;
		gate1_position = ItemIni::Get(section, L"gate1_position", gate1_position, path);
		dprint("gate1_position %f\n", gate1_position);

		double gate1_level = 0;
		gate1_level = ItemIni::Get(section, L"gate1_level", gate1_level, path);
		dprint("gate1_level %f\n", gate1_level);

		double x = 5e-3 * d->hdr.GIFTof;
		g1.x = (gate1_position + x);
		g1.width = gate1_width;
		dprint("x %f  gate1_position %f  d->hdr.GIFTof %d  %d %d\n", x, gate1_position, d->hdr.GIFTof, d->TimeEqu, d->hdr.G1Tof);
		g1.y = gate1_level;

		
		int count = d->DataSize;
		if(0 == count) count = 500;
		double mash = s.chart.items.get<ScanWindow::Line>().mash = 0.001 * d->TimeEqu / count;
		s.chart.items.get<ScanWindow::Line>().count = count;
        s.chart.items.get<ScanWindow::Line>().offset = 0;

		s.chart.items.get<ScanWindow::GateIFBorder>().value = 0.005 * d->hdr.GIFTof;
		s.chart.items.get<ScanWindow::Gate1Border>().value = 0.005 * (d->hdr.G1Tof + d->hdr.GIFTof);
		if(0 != d->hdr.GIFTof)
		{
			int i = 0;
			for(; i < count; ++i)
			{
				if(d->Point[i] > gateIF_level)
				{
					break;
				}
			}
			if(0 != d->hdr.G1Tof)
			{
				int k = 1 + int(0.005 * d->hdr.G1Tof / mash);
				i += k;
				for(; i < count; ++i)
				{
					if(d->Point[i] > gate1_level)
					{
						break;
					}
				}
			}
			s.chart.items.get<ScanWindow::Line>().offset = 
					int(i - (0.005 * (d->hdr.G1Tof + d->hdr.GIFTof) - scope_offset) / mash);
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

