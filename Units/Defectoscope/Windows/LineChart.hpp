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
			data = dataViewer.data[offs];
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
			wsprintf(label.buffer, L"<ff>Зона <ff0000>%d <ff>датчик <ff0000>%d <ff>смещение %d  величина %s   %S     "
				, 1 + owner->lastZone, 1 + N, offsetX, Wchar_from<double, 5>(valY)(), s);
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
		//ItemData<typename T::sub_type> &data = Singleton<ItemData<typename T::sub_type>>::Instance();
		//int x = data.offsets[owner->lastZone];
		//dprint("1offs %d first zone %d last zone %d\n", x
		//	, data.offsets[owner->lastZone] -  data.offsSensor[N]
		//, data.offsets[owner->lastZone + 1] -  data.offsSensor[N]);
		//x -= data.offsSensor[N];
		//dprint("2offs %d sensor %d, offset sens %d\n", x, N, data.offsSensor[N]);
		//x += offsetX * App::count_sensors + N;
		//dprint("G1Amp %d  G1Tof %d\n", data.ascanBuffer[x].hdr.G1Amp,  data.ascanBuffer[x].hdr.G1Tof);
		//dprint("3offs %d offsetX %d channel %d  ----\n", x, offsetX, data.ascanBuffer[x].Channel);
		//
		//if(x < 0) return;
		//Singleton<ScanWindow>::Instance().Open(
		//	1 + owner->lastZone
		//	, 1 + N
		//	, offsetX
		//	, Title<typename T::sub_type>()()
		//	, data.ascanBuffer[x].Point
		//	, data.ascanBuffer[x].DataSize
		//	, 100
		//	);
		Scan<typename T::sub_type>::Do(owner->lastZone, N, offsetX, (void(*)())Scan<typename T::sub_type>::Do);
	}
};

//USPCData
template<class T> struct Scan
{
	static void Do(int zone, int sens, int offs, void(*ptr)())
	{
		ItemData<T> &data = Singleton<ItemData<T>>::Instance();
		int x = data.offsets[zone];
		dprint("1offs %d first zone %d last zone %d\n", x
			, data.offsets[zone] +  data.offsSensor[sens]
		, data.offsets[zone + 1] +  data.offsSensor[sens]);
		x += data.offsSensor[sens];
		dprint("2offs %d sensor %d, offset sens %d\n", x, sens, data.offsSensor[sens]);
		x += offs * App::count_sensors + sens;
		dprint("G1Amp %d  G1Tof %d\n", data.ascanBuffer[x].hdr.G1Amp,  data.ascanBuffer[x].hdr.G1Tof);
		dprint("3offs %d offsetX %d channel %d  ----\n", x, offs, data.ascanBuffer[x].Channel);
		
		if(x < 0) return;
		Singleton<ScanWindow>::Instance().Open(
			zone
			, sens
			, offs
			, Title<T>()()
			, &data.ascanBuffer[x]
			//, data.ascanBuffer[x].Point
			//, data.ascanBuffer[x].DataSize
			//, 100
			, ptr
			);
	}
};